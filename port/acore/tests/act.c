/* port/acore's unit-test runner and stream builders. OWNED BY THE SKELETON.
 *   acore_tests [-q] [-v] [FILTER ...]   FILTER: a substring of the test or file name (p07, t12_, pcm16 ...) */
#include "act.h"
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  const char *name, *file;
  act_fn fn;
} Test;
static Test tests[4096];
static int ntests;
static int cur_failed, cur_todo;
static const char *cur_name;

void act_register(const char *name, const char *file, act_fn fn) {
  if (ntests < (int)(sizeof tests / sizeof *tests)) tests[ntests++] = (Test){name, file, fn};
}

void act_fail(const char *file, int line, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  printf("FAIL %s  %s:%d: ", cur_name, file, line);
  vprintf(fmt, ap);
  printf("\n");
  va_end(ap);
  cur_failed = 1;
}

static int g_quiet;
void act_todo(const char *msg) {
  cur_todo = 1;
  if (!g_quiet) printf("TODO %s  %s\n", cur_name, msg);
}

/* ---- the recorder: one per state, allocated with it ---- */
typedef struct {
  u8 *buf;
  size_t n, cap;
  u32 pend[AC_MAX_LIST][5];
  u32 npend;
  int hw;
} Rec;
#define MAX_STATES 64 /* per test; a table-driven test makes one per row (main session) */
static AcoreState *states[MAX_STATES];
static Rec recs[MAX_STATES];
static int nstates;

static Rec *rec_of(const AcoreState *s) {
  for (int i = 0; i < nstates; i++)
    if (states[i] == s) return &recs[i];
  return NULL;
}
static void put(Rec *r, const void *d, size_t n) {
  if (r->n + n > r->cap) {
    while (r->n + n > r->cap) r->cap = r->cap ? r->cap * 2 : 4096;
    r->buf = realloc(r->buf, r->cap);
  }
  memcpy(r->buf + r->n, d, n);
  r->n += n;
}
static void put32(Rec *r, u32 v) {
  u8 b[4] = {(u8)v, (u8)(v >> 8), (u8)(v >> 16), (u8)(v >> 24)};
  put(r, b, 4);
}
static void chunk(Rec *r, const char *tag, u32 size) {
  put(r, tag, 4);
  put32(r, size);
}
static void pad(Rec *r, u32 size) {
  static const u8 z[4];
  if (size & 3) put(r, z, 4 - (size & 3));
}
static void put_time(Rec *r, u64 t) {
  put32(r, (u32)t);
  put32(r, (u32)(t >> 32));
}

static AcoreState *new_state(int hw) {
  if (nstates == MAX_STATES) {
    printf("act: more than %d states in one test\n", MAX_STATES);
    exit(2);
  }
  AcoreState *s = acore_create();
  if (hw) s->drv.on = 0;
  states[nstates] = s;
  memset(&recs[nstates], 0, sizeof recs[nstates]);
  recs[nstates].hw = hw;
  nstates++;
  return s;
}
AcoreState *act_state(void) { return new_state(0); }
AcoreState *act_state_hw(void) { return new_state(1); }

void act_mem(AcoreState *s, u32 addr, const void *data, u32 n) {
  Rec *r = rec_of(s);
  for (u32 i = 0; i < n; i++) ac_wr8(s, addr + i, ((const u8 *)data)[i]);
  if (!r) return;
  chunk(r, "MEM ", 12 + n), put_time(r, s->now), put32(r, addr), put(r, data, n), pad(r, n);
}

void act_reg(AcoreState *s, u32 addr, u32 value, int size) {
  Rec *r = rec_of(s);
  ac_hw_write(s, addr, value, size);
  if (!r) return;
  chunk(r, "REGS", 24), put_time(r, s->now), put32(r, 1), put32(r, addr), put32(r, value), put32(r, (u32)size);
}

void act_cmd(AcoreState *s, u32 id, u32 a0, u32 a1, u32 a2, u32 a3) {
  Rec *r = rec_of(s);
  if (r && r->npend < AC_MAX_LIST) {
    u32 *c = r->pend[r->npend++];
    c[0] = id, c[1] = a0, c[2] = a1, c[3] = a2, c[4] = a3;
  }
}

static void send(AcoreState *s, int wake) {
  Rec *r = rec_of(s);
  if (!r) return;
  if (s->drv.nqueue < AC_MAX_QUEUE) {
    AC_List *l = &s->drv.queue[s->drv.nqueue++];
    l->n = r->npend;
    memcpy(l->cmd, r->pend, sizeof r->pend[0] * r->npend);
  }
  chunk(r, "CMDS", 16 + 20 * r->npend), put_time(r, s->now), put32(r, (u32)(s->now / AC_CYCLES_PER_FRAME));
  put32(r, r->npend);
  for (u32 i = 0; i < r->npend; i++)
    for (int j = 0; j < 5; j++) put32(r, r->pend[i][j]);
  r->npend = 0;
  if (wake) {
    chunk(r, "WAKE", 8), put_time(r, s->now);
    if (s->drv.on) s->drv.wakes++, ac_driver_wake(s, 0);
  }
}
void act_flush(AcoreState *s) { send(s, 1); }
void act_send(AcoreState *s) { send(s, 0); }

void act_run(AcoreState *s, u64 cycles) { acore_run(s, s->now + cycles); }

s16 act_sample(const AcoreState *s, u64 k, int lr) { return k < s->nsamples ? s->pcm[2 * k + (lr & 1)] : 0; }

void act_emit(const AcoreState *s, const char *name) {
  const char *dir = getenv("ACT_EMIT");
  Rec *r = rec_of(s);
  if (!dir || !*dir || !r) return;
  char path[1024];
  snprintf(path, sizeof path, "%s/%s.acs", dir, name);
  FILE *f = fopen(path, "wb");
  if (!f) {
    printf("act_emit: can't write %s\n", path);
    return;
  }
  char info[512];
  int il = snprintf(info, sizeof info, "source=acore_tests %s\nmode=%s\ntick0=0\n", cur_name, r->hw ? "hw" : "driver");
  u8 hdr[8] = {'A', 'C', 'S', 'T', 1, 0, 0, 0};
  fwrite(hdr, 1, 8, f);
  u32 sz = (u32)il;
  u8 ch[8] = {'I', 'N', 'F', 'O', (u8)sz, (u8)(sz >> 8), 0, 0};
  fwrite(ch, 1, 8, f), fwrite(info, 1, (size_t)il, f);
  static const u8 z[4];
  if (il & 3) fwrite(z, 1, 4 - (il & 3), f);
  fwrite(r->buf, 1, r->n, f);
  u8 end[16] = {'E', 'N', 'D', ' ', 8, 0, 0, 0};
  for (int i = 0; i < 8; i++) end[8 + i] = (u8)(s->now >> (8 * i));
  fwrite(end, 1, 16, f);
  fclose(f);
}

const char *act_tmp(const char *name) {
  static char path[1024];
  const char *t = getenv("TMPDIR");
  snprintf(path, sizeof path, "%s/acore_%s", t && *t ? t : "/tmp", name);
  return path;
}

static void free_states(void) {
  for (int i = 0; i < nstates; i++) {
    acore_destroy(states[i]);
    free(recs[i].buf);
  }
  nstates = 0;
}

static int cmp(const void *a, const void *b) {
  const Test *x = a, *y = b;
  int c = strcmp(x->file, y->file);
  return c ? c : strcmp(x->name, y->name);
}

int main(int argc, char **argv) {
  int quiet = 0, verbose = 0, nf = 0;
  const char *filters[64];
  for (int i = 1; i < argc; i++) {
    if (!strcmp(argv[i], "-q")) quiet = 1;
    else if (!strcmp(argv[i], "-v")) verbose = 1;
    else if (nf < 64) filters[nf++] = argv[i];
  }
  qsort(tests, (size_t)ntests, sizeof *tests, cmp);
  int pass = 0, fail = 0, todo = 0;
  for (int i = 0; i < ntests; i++) {
    const char *base = strrchr(tests[i].file, '/');
    base = base ? base + 1 : tests[i].file;
    int ok = !nf;
    for (int k = 0; k < nf && !ok; k++) ok = strstr(tests[i].name, filters[k]) || strstr(base, filters[k]);
    if (!ok) continue;
    cur_name = tests[i].name, cur_failed = 0, cur_todo = 0, g_quiet = quiet;
    tests[i].fn();
    free_states();
    if (cur_failed) fail++;
    else if (cur_todo) todo++;
    else {
      pass++;
      if (verbose || !quiet) printf("PASS %s\n", tests[i].name);
    }
  }
  printf("acore_tests: %d passed, %d failed, %d todo\n", pass, fail, todo);
  return fail ? 1 : 0;
}
