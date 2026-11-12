/* port/vcore's unit-test runner and helpers (tests/vct.h). OWNED BY THE SKELETON.
 *   vcore_tests                 all tests
 *   vcore_tests p01 p05         the tests whose name or file contains p01 or p05
 *   vcore_tests -l              list the tests
 *   vcore_tests -q              print only failures and the summary
 * Exit 0 when no test failed (TODO tests don't fail). */
#include "vct.h"

#include <stdarg.h>
#include <stdlib.h>

typedef struct {
  const char *name, *file;
  vct_fn fn;
} Test;

static Test *sTests;
static int sNum, sCap;
static int sFailed, sTodo;
static const char *sTodoMsg;
static char sFailMsg[1024];
static VCoreState *sStates[16];
static int sNumStates;

void vct_register(const char *name, const char *file, vct_fn fn) {
  if (sNum == sCap) {
    sCap = sCap ? sCap * 2 : 64;
    sTests = (Test *)realloc(sTests, sizeof(Test) * (size_t)sCap);
  }
  sTests[sNum].name = name;
  sTests[sNum].file = file;
  sTests[sNum].fn = fn;
  sNum++;
}

void vct_fail(const char *file, int line, const char *fmt, ...) {
  if (sFailed) return;
  sFailed = 1;
  const char *base = strrchr(file, '/');
  int n = snprintf(sFailMsg, sizeof sFailMsg, "%s:%d: ", base ? base + 1 : file, line);
  va_list ap;
  va_start(ap, fmt);
  vsnprintf(sFailMsg + n, sizeof sFailMsg - (size_t)n, fmt, ap);
  va_end(ap);
}

void vct_todo(const char *msg) {
  sTodo = 1;
  sTodoMsg = msg;
}

int vct_failed(void) { return sFailed; }

VCoreState *vct_state(void) {
  VCoreState *s = vcore_create();
  if (s == NULL || sNumStates == (int)(sizeof sStates / sizeof sStates[0])) {
    fprintf(stderr, "vct_state: out of states\n");
    exit(2);
  }
  sStates[sNumStates++] = s;
  return s;
}

void vct_io8(VCoreState *s, u32 ofs, u8 v) { s->io9[ofs] = v; }
void vct_io16(VCoreState *s, u32 ofs, u16 v) {
  s->io9[ofs] = (u8)v;
  s->io9[ofs + 1] = (u8)(v >> 8);
}
void vct_io32(VCoreState *s, u32 ofs, u32 v) {
  vct_io16(s, ofs, (u16)v);
  vct_io16(s, ofs + 2, (u16)(v >> 16));
}
void vct_eng16(VCoreState *s, int e, u32 ofs, u16 v) {
  u8 *io = e ? s->io9b : s->io9;
  io[ofs] = (u8)v;
  io[ofs + 1] = (u8)(v >> 8);
}
void vct_eng32(VCoreState *s, int e, u32 ofs, u32 v) {
  vct_eng16(s, e, ofs, (u16)v);
  vct_eng16(s, e, ofs + 2, (u16)(v >> 16));
}
void vct_pal(VCoreState *s, int e, int obj, int idx, u16 c) {
  u32 a = (u32)e * 0x400 + (u32)obj * 0x200 + (u32)idx * 2;
  s->pltt[a] = (u8)c;
  s->pltt[a + 1] = (u8)(c >> 8);
}

void vct_map_flat(VCoreState *s, int r, const u8 *buf, u32 size) {
  VC_Region *g = &s->rgn[r];
  memset(g, 0, sizeof *g);
  g->npages = size <= VC_PAGE_SIZE ? 1 : size / VC_PAGE_SIZE;
  if (g->npages > VC_MAX_PAGES) g->npages = VC_MAX_PAGES;
  for (u32 i = 0; i < g->npages; i++) {
    g->page[i].src[0] = buf + i * VC_PAGE_SIZE;
    g->page[i].n = 1;
  }
}

void vct_gx(VCoreState *s, u32 op, u32 n, const u32 *p) {
  s->gx = (u32 *)realloc(s->gx, sizeof(u32) * (s->gx_words + 1 + n));
  s->gx[s->gx_words++] = op | n << 8;
  for (u32 i = 0; i < n; i++) s->gx[s->gx_words++] = p[i];
}

static void put32(FILE *f, u32 v) {
  u8 b[4] = {(u8)v, (u8)(v >> 8), (u8)(v >> 16), (u8)(v >> 24)};
  fwrite(b, 1, 4, f);
}
static void chunk(FILE *f, const char *tag, const void *data, u32 n) {
  fwrite(tag, 1, 4, f);
  put32(f, n);
  if (n) fwrite(data, 1, n, f);
  static const u8 pad[4];
  if (n & 3) fwrite(pad, 1, 4 - (n & 3), f);
}

int vct_write_vcs(const VCoreState *s, const char *path) {
  FILE *f = fopen(path, "wb");
  if (f == NULL) return -1;
  fwrite("VCST", 1, 4, f);
  put32(f, 1);
  const char *info = s->info ? s->info : "source=vct\n";
  chunk(f, "INFO", info, (u32)strlen(info));
  chunk(f, "IO9 ", s->io9, sizeof s->io9);
  chunk(f, "IO9B", s->io9b, sizeof s->io9b);
  chunk(f, "VRAM", s->vram, sizeof s->vram);
  chunk(f, "PLTT", s->pltt, sizeof s->pltt);
  chunk(f, "OAM ", s->oam, sizeof s->oam);
  fwrite("GX3D", 1, 4, f);
  put32(f, s->gx_words * 4);
  for (u32 i = 0; i < s->gx_words; i++) put32(f, s->gx[i]);
  return fclose(f) == 0 ? 0 : -1;
}

void vct_emit(const VCoreState *s, const char *name) {
  const char *d = getenv("VCT_EMIT");
  if (d == NULL || !*d) return;
  char path[1024];
  snprintf(path, sizeof path, "%s/%s.vcs", d, name);
  if (vct_write_vcs(s, path) != 0) fprintf(stderr, "vct_emit: can't write %s\n", path);
}

const char *vct_tmp(const char *name) {
  static char buf[512];
  const char *d = getenv("TMPDIR");
  snprintf(buf, sizeof buf, "%s/vct_%s", d && *d ? d : "/tmp", name);
  return buf;
}

static int cmp(const void *a, const void *b) {
  const Test *x = (const Test *)a, *y = (const Test *)b;
  return strcmp(x->name, y->name);
}

int main(int argc, char **argv) {
  int quiet = 0, list = 0, nf = 0;
  const char *filters[64];
  for (int i = 1; i < argc; i++) {
    if (!strcmp(argv[i], "-q")) quiet = 1;
    else if (!strcmp(argv[i], "-l")) list = 1;
    else if (nf < 64) filters[nf++] = argv[i];
  }
  qsort(sTests, (size_t)sNum, sizeof(Test), cmp);
  int pass = 0, fail = 0, todo = 0;
  for (int i = 0; i < sNum; i++) {
    Test *t = &sTests[i];
    int want = nf == 0;
    for (int k = 0; k < nf && !want; k++) want = strstr(t->name, filters[k]) || strstr(t->file, filters[k]);
    if (!want) continue;
    if (list) {
      printf("%s  (%s)\n", t->name, t->file);
      continue;
    }
    sFailed = sTodo = 0;
    sTodoMsg = NULL;
    t->fn();
    for (int k = 0; k < sNumStates; k++) vcore_destroy(sStates[k]);
    sNumStates = 0;
    if (sFailed) {
      fail++;
      printf("FAIL %s: %s\n", t->name, sFailMsg);
    } else if (sTodo) {
      todo++;
      if (!quiet) printf("TODO %s: %s\n", t->name, sTodoMsg);
    } else {
      pass++;
      if (!quiet) printf("PASS %s\n", t->name);
    }
  }
  if (!list) printf("vcore_tests: %d passed, %d failed, %d todo\n", pass, fail, todo);
  return fail ? 1 : 0;
}
