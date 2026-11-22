/* port/gpu piece G12: the texture cache's CPU half as include/gpu.h gives it (g12_tcframe.h). OWNED BY G12. No GL. */
#define _POSIX_C_SOURCE 200809L
#include "g12_tcframe.h"
#include "g04_texture.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static u32 slot(u64 k) { return (u32)((k * 0x9E3779B97F4A7C15ull) >> 40) & (G12_MAP_SLOTS - 1); }

u32 g12_tcf_find(const GpuTcFrame *f, u64 key) {
  if (key == 0 || f->nready == 0) return 0;
  for (u32 i = slot(key);; i = (i + 1) & (G12_MAP_SLOTS - 1)) {
    if (f->key[i] == key) return f->val[i];
    if (f->key[i] == 0) return 0;
  }
}

static void map_put(GpuTcFrame *f, u64 key, u32 val) {
  for (u32 i = slot(key);; i = (i + 1) & (G12_MAP_SLOTS - 1)) {
    if (f->key[i] == key) return; /* (one entry per key: the same value) */
    if (f->key[i] == 0) {
      f->key[i] = key, f->val[i] = val, f->nready++;
      return;
    }
  }
}

GpuTexCache *gpu_texcache_create(const GpuOptions *o) {
  GpuOptions env;
  if (o == NULL) gpu_options_from_env(&env), o = &env;
  GpuTexCache *c = calloc(1, sizeof *c);
  if (c == NULL) return NULL;
  G12Options go = {0};
  go.budget_mb = o->tcmb > 0 ? (u32)o->tcmb : 0;
  go.mips = o->mips != 0;
  c->mips = go.mips;
  const char *e = getenv("BWPORT_GPU_TCCHECK");
  go.check = e != NULL && e[0] && strcmp(e, "0") != 0;
  e = getenv("BWPORT_GPU_TCLOG");
  c->log_every = e != NULL ? (u32)atoi(e) : 0;
  if ((c->c = g12_create(&go)) == NULL) {
    free(c);
    return NULL;
  }
  pthread_mutex_init(&c->mu, NULL);
  c->gen = 1;
  return c;
}

void g12_queue_clear(GpuTexCache *c) {
  for (G12Batch *b = c->head, *n; b != NULL; b = n) {
    n = b->next;
    free(b->up);
    free(b->bytes);
    free(b);
  }
  c->head = c->tail = NULL;
  c->nbatch = 0;
}

void g12_restart(GpuTexCache *c) {
  g12_reset(c->c);
  g12_queue_clear(c);
  c->gen++;
  c->owner = 0;
}

void gpu_texcache_destroy(GpuTexCache *c) {
  if (c == NULL) return;
  g12_queue_clear(c);
  g12_destroy(c->c);
  pthread_mutex_destroy(&c->mu);
  free(c);
}

GpuTcFrame *gpu_tcframe_create(void) { return calloc(1, sizeof(GpuTcFrame)); }
void gpu_tcframe_destroy(GpuTcFrame *f) { free(f); }

/* out's uploads and grows as a batch at the queue's end (NULL when there are none) */
static int push_batch(GpuTexCache *c, const G12Frame *out) {
  int grows = 0;
  for (int b = 0; b < G12_NBUCKET; b++) grows |= out->grow_cap[b] != 0;
  if (out->nup == 0 && !grows) return 0;
  G12Batch *bt = calloc(1, sizeof *bt);
  if (bt == NULL) return -1;
  bt->frame = out->frame;
  memcpy(bt->grow_cap, out->grow_cap, sizeof bt->grow_cap);
  size_t bytes = 0;
  for (u32 i = 0; i < out->nup; i++) {
    size_t end = out->up[i].offset + G12_UP_BYTES(out->up[i]);
    if (end > bytes) bytes = end;
  }
  if (out->nup) {
    bt->up = malloc(out->nup * sizeof *bt->up);
    bt->bytes = malloc(bytes);
    if (bt->up == NULL || bt->bytes == NULL) {
      free(bt->up), free(bt->bytes), free(bt);
      return -1;
    }
    memcpy(bt->up, out->up, out->nup * sizeof *bt->up);
    memcpy(bt->bytes, out->staging, bytes);
    bt->nup = out->nup;
  }
  if (c->tail) c->tail->next = bt;
  else c->head = bt;
  c->tail = bt;
  c->nbatch++;
  return 0;
}

void gpu_texcache_frame(GpuTexCache *c, const GpuTexData *td, const VCoreState *s, GpuTcFrame *f) {
  const void *tex, *pal;
  int ntex, npal;
  unsigned serial;
  gpu_texdata_bytes(td, &tex, &ntex, &pal, &npal, &serial);
  memset(f->key, 0, sizeof f->key);
  f->nready = 0;
  pthread_mutex_lock(&c->mu);
  if (c->nbatch >= G12_QUEUE_MAX) g12_restart(c); /* no GL side took the uploads for this long */
  G12Frame out;
  g12_frame(c->c, (const u8 *)tex, (const u8 *)pal, &s->geo, 0, &out);
  const VC_GeomOut *geo = &s->geo;
  for (u32 i = 0; i < out.npoly; i++) {
    const G12Use *u = &out.use[i];
    if (u->bucket < 0) continue;
    const VC_Poly *p = &geo->poly[geo->order[i]];
    map_put(f, g12_key(p->texparam, p->pltt), 1u | (u32)u->bucket << 1 | (u32)u->layer << 16);
  }
  if (push_batch(c, &out) != 0) g12_restart(c), f->nready = 0; /* out of memory: start again, this frame without */
  f->frame = out.frame;
  f->gen = c->gen;
  if (c->log_every && ++c->nframe % c->log_every == 0) {
    char line[512];
    g12_log_line(&out, line, sizeof line);
    fprintf(stderr, "%s\n", line);
  }
  pthread_mutex_unlock(&c->mu);
}
