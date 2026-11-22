/* port/gpu piece G12: the texture cache's directory (g12_texcache.h). OWNED BY G12. */
#include "g12_texcache.h"
#include "g12_tcmips.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ---- xxHash64 (from the algorithm's specification) ---- */
static const u64 P1 = 0x9E3779B185EBCA87ull, P2 = 0xC2B2AE3D27D4EB4Full, P3 = 0x165667B19E3779F9ull,
                 P4 = 0x85EBCA77C2B2AE63ull, P5 = 0x27D4EB2F165667C5ull;
static u64 rotl(u64 x, int r) { return (x << r) | (x >> (64 - r)); }
static u64 rd64(const u8 *p) {
  u64 v;
  memcpy(&v, p, 8);
  return v;
}
static u32 rd32(const u8 *p) {
  u32 v;
  memcpy(&v, p, 4);
  return v;
}
static u64 xround(u64 acc, u64 in) { return rotl(acc + in * P2, 31) * P1; }
static u64 xmerge(u64 acc, u64 v) { return (acc ^ xround(0, v)) * P1 + P4; }

u64 g12_xxh64(const void *data, size_t n, u64 seed) {
  const u8 *p = data, *end = p + n;
  u64 h;
  if (n >= 32) {
    u64 v1 = seed + P1 + P2, v2 = seed + P2, v3 = seed, v4 = seed - P1;
    for (; p + 32 <= end; p += 32) {
      v1 = xround(v1, rd64(p));
      v2 = xround(v2, rd64(p + 8));
      v3 = xround(v3, rd64(p + 16));
      v4 = xround(v4, rd64(p + 24));
    }
    h = rotl(v1, 1) + rotl(v2, 7) + rotl(v3, 12) + rotl(v4, 18);
    h = xmerge(xmerge(xmerge(xmerge(h, v1), v2), v3), v4);
  } else
    h = seed + P5;
  h += (u64)n;
  for (; p + 8 <= end; p += 8) h = rotl(h ^ xround(0, rd64(p)), 27) * P1 + P4;
  if (p + 4 <= end) {
    h = rotl(h ^ (u64)rd32(p) * P1, 23) * P2 + P3;
    p += 4;
  }
  for (; p < end; p++) h = rotl(h ^ *p * P5, 11) * P1;
  h ^= h >> 33;
  h *= P2;
  h ^= h >> 29;
  h *= P3;
  return h ^ (h >> 32);
}

/* ---- the directory ---- */
enum { ST_PENDING, ST_READY, ST_VOLATILE, ST_BIG };
enum { NOLAYER = 0xFFFF, GC_EVERY = 60, GC_IDLE = 600 };
static const u32 init_cap[G12_NBUCKET] = {64, 32, 16, 4, 1};

typedef struct Entry {
  u64 key, hash, img; /* img: the decoded image's hash (TCCHECK) */
  u32 tp, pltt;
  s32 next;           /* hash chain, or the free list */
  u8 state, bucket, live;
  u16 layer, S, pal_n;
  G12Ranges r;
  u32 last_used, decoded, vol_until, hist_frame, redecodes;
  u16 hist; /* bit k: re-decoded (changed) k frames ago */
  u8 pal[G12_PAL_COPY];
} Entry;

typedef struct Bucket {
  u32 cap, top;  /* layers in the array; layers handed out so far (0..top-1) */
  u16 *freel;    /* layers given back */
  u32 nfree;
  s32 *owner;    /* cap: the entry holding each layer, -1 */
} Bucket;

struct G12Cache {
  G12Options o;
  u32 frame;
  int primed;
  u8 tex[G12_TEX_BYTES], pal[G12_PAL_BYTES]; /* last frame's bytes */
  u8 dt[G12_TEX_CHUNKS], dp[G12_PAL_CHUNKS];  /* this frame's dirty chunks */
  Entry *e;
  u32 ne, cape;
  s32 freee;
  s32 *head;
  u32 nhead;
  u32 nlive;
  Bucket b[G12_NBUCKET];
  G12Use use[VC_MAX_POLYS];
  G12Upload *up;
  u32 nup, capup;
  u8 *stage;
  size_t nstage, capstage;
  u64 budget_left;
  u8 img[512 * 512 * 4], lay[512 * 512 * 4];
  const u8 *ftex, *fpal; /* this frame's bytes */
  G12Frame *out;
};

static u64 layer_bytes(const G12Cache *c, int b) {
  u64 s = (u64)(32u << b) * (32u << b) * 4;
  return c->o.mips ? s + s / 3 : s;
}
static u64 total_bytes(const G12Cache *c) {
  u64 t = 0;
  for (int b = 0; b < G12_NBUCKET; b++) t += c->b[b].cap * layer_bytes(c, b);
  return t;
}

G12Cache *g12_create(const G12Options *o) {
  G12Cache *c = calloc(1, sizeof *c);
  if (!c) return NULL;
  if (o) c->o = *o;
  if (!c->o.budget_mb) c->o.budget_mb = 128;
  if (!c->o.decode_texels) c->o.decode_texels = 16 * 128 * 128;
  c->freee = -1;
  return c;
}

static void free_all(G12Cache *c) {
  free(c->e);
  free(c->head);
  for (int b = 0; b < G12_NBUCKET; b++) {
    free(c->b[b].freel);
    free(c->b[b].owner);
  }
  free(c->up);
  free(c->stage);
}

void g12_destroy(G12Cache *c) {
  if (!c) return;
  free_all(c);
  free(c);
}

void g12_reset(G12Cache *c) {
  G12Options o = c->o;
  free_all(c);
  memset(c, 0, sizeof *c);
  c->o = o;
  c->freee = -1;
}

u64 g12_key(u32 tp, u32 pltt) {
  int fmt = (tp >> 26) & 7;
  if (fmt == 0) return 0;
  tp &= 0x3FFFFFFFu;
  if (fmt == 1 || fmt == 5 || fmt == 6 || fmt == 7) tp &= ~(1u << 29);
  u64 k = tp;
  if (fmt != 7) k |= (u64)(pltt & 0x1FFF) << 32;
  return k;
}

static u32 hslot(const G12Cache *c, u64 k) { return (u32)((k * P1) >> 40) & (c->nhead - 1); }

static void rehash(G12Cache *c, u32 n) {
  free(c->head);
  c->nhead = n;
  c->head = malloc(n * sizeof *c->head);
  for (u32 i = 0; i < n; i++) c->head[i] = -1;
  for (u32 i = 0; i < c->ne; i++) {
    Entry *e = &c->e[i];
    if (!e->live) continue;
    u32 h = hslot(c, e->key);
    e->next = c->head[h];
    c->head[h] = (s32)i;
  }
}

static Entry *find(G12Cache *c, u64 k) {
  if (!c->nhead) return NULL;
  for (s32 i = c->head[hslot(c, k)]; i >= 0; i = c->e[i].next)
    if (c->e[i].key == k) return &c->e[i];
  return NULL;
}

static Entry *create(G12Cache *c, u64 k, u32 tp, u32 pltt) {
  s32 i;
  if (c->freee >= 0) {
    i = c->freee;
    c->freee = c->e[i].next;
  } else {
    if (c->ne == c->cape) {
      c->cape = c->cape ? c->cape * 2 : 256;
      c->e = realloc(c->e, c->cape * sizeof *c->e);
    }
    i = (s32)c->ne++;
  }
  Entry *e = &c->e[i];
  memset(e, 0, offsetof(Entry, pal));
  e->key = k;
  e->tp = tp;
  e->pltt = pltt & 0x1FFF;
  e->live = 1;
  e->layer = NOLAYER;
  e->bucket = 0xFF;
  int S = g12_tc_layer_size(tp);
  e->S = (u16)S;
  if (S == 0) e->state = ST_BIG;
  else {
    int b = 0;
    while ((32 << b) < S) b++;
    e->bucket = (u8)b;
    e->state = ST_PENDING;
  }
  c->nlive++;
  if (c->nlive * 2 > c->nhead) rehash(c, c->nhead ? c->nhead * 2 : 1024);
  else {
    u32 h = hslot(c, k);
    e->next = c->head[h];
    c->head[h] = i;
  }
  return e;
}

static void release_layer(G12Cache *c, Entry *e) {
  if (e->layer == NOLAYER) return;
  Bucket *B = &c->b[e->bucket];
  B->owner[e->layer] = -1;
  B->freel[B->nfree++] = e->layer;
  e->layer = NOLAYER;
}

static void drop(G12Cache *c, s32 i) {
  Entry *e = &c->e[i];
  release_layer(c, e);
  s32 *pp = &c->head[hslot(c, e->key)];
  while (*pp != i) pp = &c->e[*pp].next;
  *pp = e->next;
  e->live = 0;
  e->next = c->freee;
  c->freee = i;
  c->nlive--;
}

/* a layer of bucket b for entry i: a free one, a new one (growing the array while the budget allows), or the least recently used
 * idle entry's; -1 when none */
static int alloc_layer(G12Cache *c, int b, s32 i) {
  Bucket *B = &c->b[b];
  int L = -1;
  if (B->nfree) L = B->freel[--B->nfree];
  else if (B->top < B->cap) L = (int)B->top++;
  else {
    u64 lb = layer_bytes(c, b), budget = (u64)c->o.budget_mb << 20, other = total_bytes(c) - B->cap * lb;
    u32 want = B->cap ? B->cap * 2 : init_cap[b];
    if (want > NOLAYER) want = NOLAYER;
    if (want > (c->o.max_layers ? c->o.max_layers : 256u)) want = c->o.max_layers ? c->o.max_layers : 256u;
    while (want > B->cap && other + want * lb > budget) want--;
    if (want > B->cap) { /* grow: new storage, the old layers' entries decode again */
      B->owner = realloc(B->owner, want * sizeof *B->owner);
      B->freel = realloc(B->freel, want * sizeof *B->freel);
      for (u32 k = B->cap; k < want; k++) B->owner[k] = -1;
      for (u32 k = 0; k < B->cap; k++) {
        s32 o = B->owner[k];
        if (o >= 0 && c->e[o].state == ST_READY && c->e[o].decoded != c->frame) c->e[o].state = ST_PENDING;
      }
      B->cap = want;
      c->out->grow_cap[b] = want;
      c->out->st.grows++;
      L = (int)B->top++;
    } else { /* evict the least recently used idle entry */
      s32 best = -1;
      for (u32 k = 0; k < B->cap; k++) {
        s32 o = B->owner[k];
        if (o < 0 || c->e[o].last_used + G12_IDLE_FRAMES > c->frame) continue;
        if (best < 0 || c->e[o].last_used < c->e[best].last_used) best = o;
      }
      if (best < 0) return -1;
      Entry *v = &c->e[best];
      L = v->layer;
      B->owner[L] = -1;
      v->layer = NOLAYER;
      if (v->state == ST_READY) v->state = ST_PENDING;
      c->out->st.evicted++;
    }
  }
  B->owner[L] = i;
  return L;
}

/* clip a palette range to the bytes that exist (slots 6-7 read 0: never change) */
static void pal_clip(const G12Ranges *r, u32 *lo, u32 *hi) {
  *lo = r->pal_lo;
  *hi = r->pal_hi > G12_PAL_BYTES ? G12_PAL_BYTES : r->pal_hi;
  if (*hi < *lo) *hi = *lo;
}

static u64 hash_entry(const G12Cache *c, const Entry *e) {
  const G12Ranges *r = &e->r;
  u64 h = 0;
  if (r->tex_hi > r->tex_lo) h = g12_xxh64(c->ftex + r->tex_lo, r->tex_hi - r->tex_lo, h);
  if (r->idx_hi > r->idx_lo) h = g12_xxh64(c->ftex + r->idx_lo, r->idx_hi - r->idx_lo, h ^ 1);
  if (!e->pal_n) {
    u32 lo, hi;
    pal_clip(r, &lo, &hi);
    if (hi > lo) h = g12_xxh64(c->fpal + lo, hi - lo, h ^ 2);
  }
  return h;
}

static int any(const u8 *d, u32 lo, u32 hi) { /* bytes [lo, hi) touch a dirty chunk */
  if (hi <= lo) return 0;
  for (u32 k = lo / G12_CHUNK; k <= (hi - 1) / G12_CHUNK; k++)
    if (d[k]) return 1;
  return 0;
}

/* 1: a texel (or block word) chunk is dirty, 2: a palette chunk */
static int dirty(const G12Cache *c, const Entry *e) {
  u32 lo, hi;
  pal_clip(&e->r, &lo, &hi);
  return (any(c->dt, e->r.tex_lo, e->r.tex_hi) || any(c->dt, e->r.idx_lo, e->r.idx_hi)) | any(c->dp, lo, hi) << 1;
}

/* the palette copy and the hash of what the entry's decode read (after its ranges are known) */
static void remember(G12Cache *c, Entry *e) {
  u32 lo, hi;
  pal_clip(&e->r, &lo, &hi);
  e->pal_n = 0;
  if (hi - lo <= G12_PAL_COPY) {
    e->pal_n = (u16)(hi - lo);
    if (e->pal_n) memcpy(e->pal, c->fpal + lo, e->pal_n);
  }
  e->hash = hash_entry(c, e);
}

/* has what the entry read changed? (its ranges are dirty) */
static int changed(G12Cache *c, Entry *e, int d) {
  u32 lo, hi;
  pal_clip(&e->r, &lo, &hi);
  if ((d & 2) && e->pal_n && memcmp(c->fpal + lo, e->pal, e->pal_n)) return 1;
  if ((d & 1) || ((d & 2) && !e->pal_n)) return hash_entry(c, e) != e->hash;
  return 0;
}

static void decode(G12Cache *c, Entry *e) {
  int w = g12_tc_width(e->tp), h = g12_tc_height(e->tp), S = e->S;
  g12_tc_decode(c->ftex, c->fpal, e->tp, e->pltt, c->img, &e->r);
  remember(c, e);
  if (c->o.check) e->img = g12_xxh64(c->img, (size_t)w * h * 4, 0);
  g12_tc_layout(c->img, e->tp, c->lay);
  if (c->o.mips) g12_tc_fill(c->lay, S);
  int levels = 1;
  if (c->o.mips)
    for (int s = S; s > 1; s >>= 1) levels++;
  G12Upload u = {(u8)e->bucket, (u8)levels, e->layer, (u16)S, (u32)c->nstage};
  size_t n = G12_UP_BYTES(u);
  if (c->nstage + n > c->capstage) {
    c->capstage = (c->nstage + n) * 2;
    c->stage = realloc(c->stage, c->capstage);
  }
  g12_tc_rgba8(c->lay, S * S, c->stage + c->nstage);
  if (levels > 1) { /* the chain from level 0 (hard alpha: every format but A3I5 and A5I3 that has transparent texels) */
    u32 f = (e->tp >> 26) & 7;
    int hard = f == 5 || f == 7 || (f >= 2 && f <= 4 && ((e->tp >> 29) & 1));
    g12_tc_mips(c->stage + c->nstage, S, hard, c->stage + c->nstage + (size_t)S * S * 4);
  }
  if (c->nup == c->capup) {
    c->capup = c->capup ? c->capup * 2 : 64;
    c->up = realloc(c->up, c->capup * sizeof *c->up);
  }
  c->up[c->nup++] = u;
  c->nstage += n;
  e->decoded = c->frame;
  e->state = ST_READY;
  c->out->st.decodes++;
  c->out->st.decoded_texels += (u64)S * S;
}

static int popc16(u32 v) {
  int n = 0;
  for (; v; v &= v - 1) n++;
  return n;
}

/* the entry's first use this frame: check, decode, allocate */
static void visit(G12Cache *c, Entry *e, u32 cap_pages) {
  G12Stats *st = &c->out->st;
  s32 idx = (s32)(e - c->e);
  int fresh = e->last_used == 0;
  e->last_used = c->frame;
  st->textures++;
  u32 sh = c->frame - e->hist_frame;
  e->hist = sh >= 16 ? 0 : (u16)(e->hist << sh);
  e->hist_frame = c->frame;
  if (fresh) st->news++;
  if (e->state == ST_BIG) {
    st->big++;
    return;
  }
  st->by_size[e->bucket]++;
  st->used_bytes += layer_bytes(c, e->bucket);
  if (cap_pages) { /* its texel bytes (from texparam: an entry not decoded yet has no ranges) on a captured page */
    static const u8 bits[8] = {0, 8, 2, 4, 8, 2, 8, 16};
    u32 lo = (e->tp & 0xFFFF) * 8, n = (u32)g12_tc_width(e->tp) * (u32)g12_tc_height(e->tp) * bits[(e->tp >> 26) & 7] / 8;
    for (u32 p = lo / 0x4000; n && p <= (lo + n - 1) / 0x4000; p++)
      if (cap_pages >> (p & 31) & 1) {
        st->captured++;
        break;
      }
  }
  if (!fresh && (e->state == ST_READY || e->state == ST_VOLATILE)) {
    int d = dirty(c, e);
    if (d && changed(c, e, d)) {
      e->hist |= 1;
      e->redecodes++;
      if (e->state == ST_READY) st->changed++;
      if (popc16(e->hist) >= 8 && e->state != ST_VOLATILE) {
        e->state = ST_VOLATILE;
        e->vol_until = c->frame + G12_VOLATILE_FRAMES;
        release_layer(c, e);
      }
      if (e->state == ST_VOLATILE) remember(c, e); /* keep following its changes (its ranges as last decoded) */
      else e->state = ST_PENDING;
    } else if (d) {
      if (d & 1) st->kept_hash++;
      else st->kept_pal++;
    }
    if (e->state == ST_VOLATILE) {
      if (c->frame < e->vol_until) {
        st->vol++;
        return;
      }
      if (popc16(e->hist) >= 8) {
        e->vol_until = c->frame + G12_VOLATILE_FRAMES;
        st->vol++;
        return;
      }
      e->state = ST_PENDING;
    }
    if (e->state == ST_READY) {
      st->hits++;
      if (c->o.check) { /* what the layer holds against a fresh decode */
        G12Ranges r;
        int w = g12_tc_width(e->tp), h = g12_tc_height(e->tp);
        g12_tc_decode(c->ftex, c->fpal, e->tp, e->pltt, c->img, &r);
        st->checked++;
        if (g12_xxh64(c->img, (size_t)w * h * 4, 0) != e->img) {
          if (st->stale++ < 4)
            fprintf(stderr, "TCCHECK f%u: stale texture tp %08x pltt %x (layer %d of bucket %d)\n", c->frame, e->tp, e->pltt,
                    e->layer, e->bucket);
        }
      }
      return;
    }
  }
  /* pending: a layer, then a decode when the frame's budget allows */
  if (e->layer == NOLAYER) {
    int L = alloc_layer(c, e->bucket, idx);
    if (L < 0) {
      st->noroom++;
      return;
    }
    e->layer = (u16)L;
  }
  u64 n = (u64)e->S * e->S;
  if (st->decodes && n > c->budget_left) {
    st->pending++;
    return;
  }
  c->budget_left = n > c->budget_left ? 0 : c->budget_left - n;
  decode(c, e);
}

static void dirty_map(G12Cache *c) {
  G12Stats *st = &c->out->st;
  if (!c->primed) {
    memcpy(c->tex, c->ftex, G12_TEX_BYTES);
    memcpy(c->pal, c->fpal, G12_PAL_BYTES);
    memset(c->dt, 1, sizeof c->dt);
    memset(c->dp, 1, sizeof c->dp);
    st->dirty_tex = G12_TEX_CHUNKS;
    st->dirty_pal = G12_PAL_CHUNKS;
    c->primed = 1;
    return;
  }
  for (u32 k = 0; k < G12_TEX_CHUNKS; k++) {
    size_t o = (size_t)k * G12_CHUNK;
    c->dt[k] = memcmp(c->tex + o, c->ftex + o, G12_CHUNK) != 0;
    if (c->dt[k]) {
      memcpy(c->tex + o, c->ftex + o, G12_CHUNK);
      st->dirty_tex++;
    }
  }
  for (u32 k = 0; k < G12_PAL_CHUNKS; k++) {
    size_t o = (size_t)k * G12_CHUNK;
    c->dp[k] = memcmp(c->pal + o, c->fpal + o, G12_CHUNK) != 0;
    if (c->dp[k]) {
      memcpy(c->pal + o, c->fpal + o, G12_CHUNK);
      st->dirty_pal++;
    }
  }
}

void g12_frame(G12Cache *c, const u8 *tex, const u8 *pal, const VC_GeomOut *geo, u32 cap_pages, G12Frame *out) {
  memset(out, 0, sizeof *out);
  c->out = out;
  c->frame++;
  c->ftex = tex;
  c->fpal = pal;
  c->nup = 0;
  c->nstage = 0;
  c->budget_left = c->o.decode_texels;
  G12Stats *st = &out->st;
  dirty_map(c);
  u32 np = geo->npoly > VC_MAX_POLYS ? VC_MAX_POLYS : geo->npoly;
  for (u32 i = 0; i < np; i++) {
    const VC_Poly *p = &geo->poly[geo->order[i]];
    G12Use *u = &c->use[i];
    memset(u, 0, sizeof *u);
    u->bucket = -1;
    u64 k = g12_key(p->texparam, p->pltt);
    if (!k) {
      u->why = G12_USE_NOTEX;
      continue;
    }
    st->textured++;
    Entry *e = find(c, k);
    if (!e) e = create(c, k, p->texparam, p->pltt);
    if (e->last_used != c->frame) visit(c, e, cap_pages);
    if (e->state == ST_READY && e->layer != NOLAYER) {
      u->bucket = (s16)e->bucket;
      u->layer = e->layer;
      u->size = e->S;
      u->why = G12_USE_READY;
    } else
      u->why = e->state == ST_BIG ? G12_USE_BIG : e->state == ST_VOLATILE ? G12_USE_VOLATILE
               : e->layer == NOLAYER                  ? G12_USE_NOROOM
                                                      : G12_USE_PENDING;
  }
  /* a grown array lost its old layers: an entry decoded before the growth this frame is in the upload list; the others were set
   * pending; polygons that were given such an entry before the growth take the exact path */
  for (u32 i = 0; i < np; i++) {
    G12Use *u = &c->use[i];
    if (u->bucket < 0) continue;
    Entry *e = find(c, g12_key(geo->poly[geo->order[i]].texparam, geo->poly[geo->order[i]].pltt));
    if (e->state != ST_READY) {
      u->bucket = -1;
      u->size = 0;
      u->why = G12_USE_PENDING;
    }
  }
  if (c->frame % GC_EVERY == 0) /* forget entries without a layer, unused for long */
    for (u32 i = 0; i < c->ne; i++) {
      Entry *e = &c->e[i];
      if (e->live && e->layer == NOLAYER && e->last_used + GC_IDLE < c->frame) {
        drop(c, (s32)i);
        st->dropped++;
      }
    }
  st->polys = np;
  st->entries = c->nlive;
  for (int b = 0; b < G12_NBUCKET; b++) {
    st->cap[b] = c->b[b].cap;
    st->layers[b] = c->b[b].top - c->b[b].nfree;
  }
  st->bytes = total_bytes(c);
  out->frame = c->frame;
  out->npoly = np;
  out->use = c->use;
  out->nup = c->nup;
  out->up = c->up;
  out->staging = c->stage;
  c->out = NULL;
}

int g12_log_line(const G12Frame *f, char *buf, int n) {
  const G12Stats *s = &f->st;
  return snprintf(buf, (size_t)n,
                  "TCLOG f%u: polys %u textured %u textures %u (32:%u 64:%u 128:%u 256:%u 512:%u big %u) set %.2f MB; vol %u "
                  "pend %u noroom %u cap %u; hit %u new %u chg %u kept %u/%u; dec %u (%.1fK texels) up %u grow %u evict %u "
                  "drop %u; dirty %u/%u; cache %u entries %u/%u/%u/%u/%u layers %.2f MB",
                  f->frame, s->polys, s->textured, s->textures, s->by_size[0], s->by_size[1], s->by_size[2], s->by_size[3],
                  s->by_size[4], s->big, s->used_bytes / 1048576.0, s->vol, s->pending, s->noroom, s->captured, s->hits, s->news,
                  s->changed, s->kept_pal, s->kept_hash, s->decodes, s->decoded_texels / 1024.0, f->nup, s->grows, s->evicted,
                  s->dropped, s->dirty_tex, s->dirty_pal, s->entries, s->layers[0], s->layers[1], s->layers[2], s->layers[3],
                  s->layers[4], s->bytes / 1048576.0);
}
