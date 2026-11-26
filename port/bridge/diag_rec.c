#ifndef _WIN32
#define _DEFAULT_SOURCE 1 /* usleep, localtime_r under -std=c11 */
#endif
/* Record diagnostics: the recording core (port/bridge/diag_rec.h says what it records and the file's layout; tools/diagrec.py reads
 * it). Everything is kept in memory while it records (the hooks copy a few numbers; a keyframe copies its registers, its changed VRAM
 * pages and its command stream, on the bridge's worker) and written by a thread of its own at the end. Shared code: it runs only when
 * a platform's button or the cfgscript's diagrec=S starts it (Windows, Linux and Android have no button: only the cfgscript there). */
#include "diag_rec.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

/* libntr (sim_main.cpp, sim_gui.cpp) */
unsigned long long SIM_PaceNowNs(void);
typedef void (*SIM_PresentTapFn)(int kind, unsigned long long now, unsigned long long tick, int r, int mid, int why, unsigned long long slot,
                                 unsigned long long call, long long game, unsigned long long due, int early);
void SIM_SetPresentTap(SIM_PresentTapFn fn);
float SIM_DisplayShown(int *waiting, float *hz);
void SIM_DiagSettings(char *buf, int n);
#ifdef __APPLE__
#include <TargetConditionals.h>
void bwport_diagrec_platform(char *buf, int n, const char *prefix); /* port/src/apple_thermal.m: device, system, heat, Low Power Mode */
#if TARGET_OS_IOS
extern const char bwport_build_info[]; /* tools/ios_buildinfo.py */
#endif
#endif

enum { DR_PAGE = 4096, DR_NPAGES = (int)(sizeof(((VCoreState *)0)->vram) / DR_PAGE), DR_DELAY_NS = 1000000000 };
enum { DR_KEYBYTES_MAX = 400 << 20 }; /* keyframes past this many bytes are counted, not kept */

typedef struct DrTick {
  DrTickInfo i;
  long long t;
  int key, old, recUs, keyBytes;
} DrTick;
typedef struct DrPresent {
  long long t, slot, call, game, due;
  unsigned long long tick;
  int kind, r, mid, why, early;
  unsigned picId, picSerial, picId3d;
  int picIdx, picR3;
} DrPresent;
/* a KEY chunk: this, then u16 page numbers (npages, padded to 4 bytes), io9, io9b, pltt, oam, the pages, the stream's words */
typedef struct DrKeyHead {
  unsigned seq, id, frame, swapFrame, swaps, gxWords, npages;
  int old; /* the keyframe it is paired with (-1: none, IpGather started again or before the recording) */
} DrKeyHead;

volatile int bwport_diagrec_state; /* 0 idle, 1 waiting for the settle, 2 recording, 3 writing */
static struct {
  int busy; /* hooks inside (the writer waits for 0) */
  double seconds;
  void (*done)(const char *path);
  long long armAt, t0, tEnd;
  time_t wall;
  DrTick *tick;
  DrPresent *pres;
  DrKeyHead **key;
  int ntick, cticks, npres, cpres, nkey, ckey, lostTick, lostPres, lostKey, keySeq, curKey, curOld;
  long long keyBytes, recNs, recMax, recLast;
  unsigned lastId; /* the renderer call of the last keyframe (its time and bytes go on that call's T line) */
  int lastBytes;
  u8 *vram; /* the last kept keyframe's VRAM (the next one's pages are compared with it) */
  int vramOk;
  /* the picture the GL thread drew last (bwport_diagrec_shows) */
  unsigned picId, picSerial, picId3d;
  int picIdx, picR3;
  char settings[6144], plat0[512], plat1[512];
  pthread_t writer;
  int writerOn;
} D;
static char sMsg[512];

static int DrEnter(void) {
  if (bwport_diagrec_state != 2) return 0;
  __atomic_add_fetch(&D.busy, 1, __ATOMIC_ACQ_REL);
  if (bwport_diagrec_state == 2) return 1;
  __atomic_sub_fetch(&D.busy, 1, __ATOMIC_ACQ_REL);
  return 0;
}
static void DrLeave(void) { __atomic_sub_fetch(&D.busy, 1, __ATOMIC_ACQ_REL); }

static void DrFree(void) {
  for (int i = 0; i < D.nkey; i++) free(D.key[i]);
  free(D.key), free(D.tick), free(D.pres), free(D.vram);
  D.key = NULL, D.tick = NULL, D.pres = NULL, D.vram = NULL;
}

static void Chunk(FILE *f, const char *tag, const void *data, unsigned size) {
  static const unsigned char pad[4] = {0, 0, 0, 0};
  fwrite(tag, 1, 4, f);
  fwrite(&size, 4, 1, f);
  if (size) fwrite(data, 1, size, f);
  if (size & 3) fwrite(pad, 1, 4 - (size & 3), f);
}

static void Platform(char *buf, int n, const char *prefix) {
  buf[0] = 0;
#ifdef __APPLE__
  bwport_diagrec_platform(buf, n, prefix);
#else
  (void)n, (void)prefix;
#endif
}
static const char *Build(void) {
#if defined(__APPLE__) && TARGET_OS_IOS
  return bwport_build_info;
#else
  return "";
#endif
}
static void SleepMs(void) {
#ifdef _WIN32
  Sleep(1);
#else
  usleep(1000);
#endif
}

/* the file, then done(path); on the writer's thread (or flush's) */
static void *Write(void *arg) {
  (void)arg;
  while (__atomic_load_n(&D.busy, __ATOMIC_ACQUIRE) != 0) SleepMs();
  char dir[1024], path[1200], name[64];
  const char *d = getenv("BWPORT_DIAGREC_DIR"); /* else iOS: the app's Documents (Files shows it); elsewhere the working directory */
#if defined(__APPLE__) && TARGET_OS_IOS
  if ((d == NULL || !*d) && getenv("HOME") != NULL) snprintf(dir, sizeof dir, "%s/Documents", getenv("HOME"));
  else
#endif
    snprintf(dir, sizeof dir, "%s", d != NULL && *d ? d : ".");
  struct tm lt;
#ifdef _WIN32
  const struct tm *lp = localtime(&D.wall);
  if (lp != NULL) lt = *lp;
  else memset(&lt, 0, sizeof lt);
#else
  localtime_r(&D.wall, &lt);
#endif
  strftime(name, sizeof name, "bwdiag-%Y%m%d-%H%M%S.bwdiag", &lt);
  snprintf(path, sizeof path, "%s/%s", dir, name);
  FILE *f = fopen(path, "wb");
  int ok = f != NULL;
  long long total = 0;
  if (ok) {
    static const unsigned head[2] = {0x47445742u /* "BWDG" */, 1};
    fwrite(head, 4, 2, f);
    /* the manifest */
    size_t cap = 16384, k = 0;
    char *m = (char *)malloc(cap);
    if (m != NULL) {
      char when[64];
      strftime(when, sizeof when, "%Y-%m-%d %H:%M:%S", &lt);
      int waiting = 0;
      float hz = 0.0f;
      SIM_DisplayShown(&waiting, &hz);
      k += (size_t)snprintf(m + k, cap - k,
                            "format=bwdiag 1 (port/bridge/diag_rec.h; tools/diagrec.py)\nbuild=%s\nstart=%s\nseconds=%g\nt0_ns=%lld\n"
                            "panel_hz=%.2f\nticks=%d\npresents=%d\nkeyframes=%d\nkeyframes_lost=%d\nticks_lost=%d\npresents_lost=%d\n"
                            "key_bytes=%lld\nrec_ms=%.3f\nrec_max_us=%.1f\nio9=%u\nio9b=%u\npltt=%u\noam=%u\nvram=%u\npage=%d\n",
                            Build(), when, D.seconds, D.t0, hz, D.ntick,
                            D.npres, D.nkey, D.lostKey, D.lostTick, D.lostPres, D.keyBytes, D.recNs / 1e6, D.recMax / 1e3,
                            (unsigned)sizeof(((VCoreState *)0)->io9), (unsigned)sizeof(((VCoreState *)0)->io9b),
                            (unsigned)sizeof(((VCoreState *)0)->pltt), (unsigned)sizeof(((VCoreState *)0)->oam),
                            (unsigned)sizeof(((VCoreState *)0)->vram), DR_PAGE);
      if (k < cap) k += (size_t)snprintf(m + k, cap - k, "%s%s%s", D.plat0, D.settings, D.plat1);
#ifndef _WIN32
      extern char **environ;
      for (char **e = environ; e != NULL && *e != NULL && k < cap - 512; e++)
        if (!strncmp(*e, "BWPORT_", 7)) k += (size_t)snprintf(m + k, cap - k, "env %.400s\n", *e);
#endif
      if (k > cap) k = cap;
      Chunk(f, "MANI", m, (unsigned)k);
      free(m);
    }
    /* the T and P lines (times in us from t0) */
    size_t lcap = (size_t)(D.ntick + D.npres) * 200 + 4096;
    char *l = (char *)malloc(lcap);
    k = 0;
    if (l != NULL) {
      k += (size_t)snprintf(l + k, lcap - k,
                            "# T id t nominal serial frame swaps swapframe ipN why valid key old nl nv gap work_us rec_us key_bytes blend f1..fN\n"
                            "# P kind t tick r mid why slot call game due early pic_id pic_idx pic_serial pic_r3 pic_id3d\n");
      for (int i = 0; i < D.ntick && k < lcap - 400; i++) {
        const DrTick *t = &D.tick[i];
        k += (size_t)snprintf(l + k, lcap - k, "T %u %lld %lld %lld %u %u %u %d %d %d %d %d %d %d %lld %lld %d %d %x", t->i.id,
                              (t->t - D.t0) / 1000, (t->i.nominal - D.t0) / 1000, t->i.serial, t->i.frame, t->i.swaps, t->i.swapFrame,
                              t->i.ipN, t->i.why, t->i.valid, t->key, t->old, t->i.nl, t->i.nv, t->i.gap / 1000, t->i.workNs / 1000,
                              t->recUs, t->keyBytes, t->i.blend);
        for (int j = 1; j <= t->i.ipN && j < 9; j++) k += (size_t)snprintf(l + k, lcap - k, " %.4f", t->i.f[j]);
        k += (size_t)snprintf(l + k, lcap - k, "\n");
      }
      for (int i = 0; i < D.npres && k < lcap - 300; i++) {
        const DrPresent *p = &D.pres[i];
        k += (size_t)snprintf(l + k, lcap - k, "P %d %lld %llu %d %d %d %lld %lld %lld %lld %d %u %d %u %d %u\n", p->kind,
                              (p->t - D.t0) / 1000, p->tick, p->r, p->mid, p->why, p->slot ? (p->slot - D.t0) / 1000 : 0,
                              p->call ? (p->call - D.t0) / 1000 : 0, p->game >= 0 ? (p->game - D.t0) / 1000 : -1,
                              p->due ? (p->due - D.t0) / 1000 : 0, p->early, p->picId, p->picIdx, p->picSerial, p->picR3, p->picId3d);
      }
      if (k > lcap) k = lcap;
      Chunk(f, "LOG ", l, (unsigned)k);
      free(l);
    }
    for (int i = 0; i < D.nkey; i++) {
      const DrKeyHead *h = D.key[i];
      size_t n = sizeof *h + (((size_t)h->npages * 2 + 3) & ~(size_t)3) + sizeof(((VCoreState *)0)->io9) +
                 sizeof(((VCoreState *)0)->io9b) + sizeof(((VCoreState *)0)->pltt) + sizeof(((VCoreState *)0)->oam) +
                 (size_t)h->npages * DR_PAGE + (size_t)h->gxWords * 4;
      Chunk(f, "KEY ", h, (unsigned)n);
    }
    total = ftell(f);
    ok = ferror(f) == 0;
    ok &= fclose(f) == 0;
  }
  if (ok)
    snprintf(sMsg, sizeof sMsg, "%s: %.1f MB, %d keyframes, %d presents", name, total / 1048576.0, D.nkey, D.npres);
  else
    snprintf(sMsg, sizeof sMsg, "%s could not be written", path);
  fprintf(stderr, "bwport: diagrec: %s (%s)\n", sMsg, dir);
  DrFree();
  void (*done)(const char *) = D.done;
  __atomic_store_n(&bwport_diagrec_state, 0, __ATOMIC_RELEASE);
  if (done != NULL) done(ok ? path : NULL);
  return NULL;
}

static void DrTap(int kind, unsigned long long now, unsigned long long tick, int r, int mid, int why, unsigned long long slot,
                  unsigned long long call, long long game, unsigned long long due, int early);

/* the main thread, at each present slot: the start after the settle, the end */
static void DrPoll(long long now) {
  if (bwport_diagrec_state == 1) {
    if (D.armAt == 0) D.armAt = now + DR_DELAY_NS;
    if (now < D.armAt) return;
    D.t0 = now;
    D.tEnd = now + (long long)(D.seconds * 1e9);
    D.wall = time(NULL);
    Platform(D.plat0, sizeof D.plat0, "");
    __atomic_store_n(&bwport_diagrec_state, 2, __ATOMIC_RELEASE);
  } else if (bwport_diagrec_state == 2 && now >= D.tEnd) {
    __atomic_store_n(&bwport_diagrec_state, 3, __ATOMIC_RELEASE);
    SIM_SetPresentTap(NULL);
    Platform(D.plat1, sizeof D.plat1, "end_");
    D.writerOn = pthread_create(&D.writer, NULL, Write, NULL) == 0;
    if (D.writerOn) pthread_detach(D.writer);
    else Write(NULL);
  }
}

static void DrTap(int kind, unsigned long long now, unsigned long long tick, int r, int mid, int why, unsigned long long slot,
                  unsigned long long call, long long game, unsigned long long due, int early) {
  if (bwport_diagrec_state != 2) {
    DrPoll((long long)now);
    if (bwport_diagrec_state != 2) return;
  }
  if (DrEnter()) {
    if (D.npres < D.cpres) {
      DrPresent *p = &D.pres[D.npres++];
      p->t = (long long)now, p->slot = (long long)slot, p->call = (long long)call, p->game = game, p->due = (long long)due;
      p->tick = tick, p->kind = kind, p->r = r, p->mid = mid, p->why = why, p->early = early;
      p->picId = D.picId, p->picIdx = D.picIdx, p->picSerial = D.picSerial, p->picR3 = D.picR3, p->picId3d = D.picId3d;
    } else {
      D.lostPres++;
    }
    DrLeave();
  }
  DrPoll((long long)now);
}

int bwport_diagrec_start(double seconds, void (*done)(const char *path)) {
  if (bwport_diagrec_state != 0 || seconds <= 0.0) return 0;
  if (seconds > 120.0) seconds = 120.0;
  memset(&D, 0, sizeof D);
  D.seconds = seconds;
  D.done = done;
  D.cticks = (int)(seconds * 300.0) + 1024;
  D.cpres = (int)(seconds * 600.0) + 2048;
  D.ckey = (int)(seconds * 130.0) + 256;
  D.tick = (DrTick *)malloc(sizeof *D.tick * (size_t)D.cticks);
  D.pres = (DrPresent *)malloc(sizeof *D.pres * (size_t)D.cpres);
  D.key = (DrKeyHead **)calloc((size_t)D.ckey, sizeof *D.key);
  D.vram = (u8 *)malloc(sizeof(((VCoreState *)0)->vram));
  if (D.tick == NULL || D.pres == NULL || D.key == NULL || D.vram == NULL) {
    DrFree();
    snprintf(sMsg, sizeof sMsg, "no memory to record");
    return 0;
  }
  D.curKey = D.curOld = -1;
  SIM_DiagSettings(D.settings, sizeof D.settings);
  sMsg[0] = 0;
  __atomic_store_n(&bwport_diagrec_state, 1, __ATOMIC_RELEASE);
  SIM_SetPresentTap(DrTap);
  fprintf(stderr, "bwport: diagrec: %g s from the next present (after a %d s settle)\n", seconds, DR_DELAY_NS / 1000000000);
  return 1;
}

int bwport_diagrec_left(void) {
  const int st = bwport_diagrec_state;
  if (st == 1) return -1;
  if (st == 3) return -2;
  if (st != 2) return 0;
  long long left = D.tEnd - (long long)SIM_PaceNowNs();
  return left <= 0 ? 1 : (int)((left + 999999999) / 1000000000);
}

const char *bwport_diagrec_message(void) { return sMsg; }

void bwport_diagrec_flush(void) {
  const int st = bwport_diagrec_state;
  if (st == 1 || st == 2) {
    if (st == 1) D.t0 = (long long)SIM_PaceNowNs(), D.wall = time(NULL);
    __atomic_store_n(&bwport_diagrec_state, 3, __ATOMIC_RELEASE);
    SIM_SetPresentTap(NULL);
    Platform(D.plat1, sizeof D.plat1, "end_");
    Write(NULL);
    return;
  }
  for (int i = 0; i < 5000 && bwport_diagrec_state == 3; i++) SleepMs(); /* the writer's thread: wait for it (at most 5 s) */
}

void bwport_diagrec_tick(const DrTickInfo *t) {
  if (!DrEnter()) return;
  const long long now = (long long)SIM_PaceNowNs();
  if (D.ntick < D.cticks) {
    DrTick *r = &D.tick[D.ntick++];
    r->i = *t;
    r->t = now;
    r->key = t->ipN > 0 ? D.curKey : -1;
    r->old = t->ipN > 0 ? D.curOld : -1;
    r->recUs = 0;
    r->keyBytes = 0;
    if (D.lastId == t->id) { /* this call's keyframe */
      r->recUs = (int)(D.recLast / 1000);
      r->keyBytes = D.lastBytes;
    }
  } else {
    D.lostTick++;
  }
  DrLeave();
}

int bwport_diagrec_key(const VCoreState *s, unsigned id, unsigned frame, unsigned swapFrame, unsigned swaps, int haveOld) {
  if (!DrEnter()) return -1;
  const long long t0 = (long long)SIM_PaceNowNs();
  const int seq = D.keySeq++;
  D.curOld = haveOld ? D.curKey : -1;
  D.curKey = seq;
  D.lastBytes = 0;
  D.lastId = id;
  if (D.nkey >= D.ckey || D.keyBytes >= DR_KEYBYTES_MAX) {
    D.lostKey++;
    D.vramOk = 0;
  } else {
    u16 pages[DR_NPAGES];
    int np = 0;
    for (int p = 0; p < DR_NPAGES; p++)
      if (!D.vramOk || memcmp(D.vram + (size_t)p * DR_PAGE, s->vram + (size_t)p * DR_PAGE, DR_PAGE) != 0) pages[np++] = (u16)p;
    const size_t pl = ((size_t)np * 2 + 3) & ~(size_t)3;
    const size_t n = sizeof(DrKeyHead) + pl + sizeof s->io9 + sizeof s->io9b + sizeof s->pltt + sizeof s->oam + (size_t)np * DR_PAGE +
                     (size_t)s->gx_words * 4;
    DrKeyHead *h = (DrKeyHead *)malloc(n);
    if (h == NULL) {
      D.lostKey++;
      D.vramOk = 0;
    } else {
      h->seq = (unsigned)seq, h->id = id, h->frame = frame, h->swapFrame = swapFrame, h->swaps = swaps;
      h->gxWords = s->gx_words, h->npages = (unsigned)np, h->old = D.curOld;
      u8 *o = (u8 *)(h + 1);
      memset(o, 0, pl);
      memcpy(o, pages, (size_t)np * 2);
      o += pl;
      memcpy(o, s->io9, sizeof s->io9), o += sizeof s->io9;
      memcpy(o, s->io9b, sizeof s->io9b), o += sizeof s->io9b;
      memcpy(o, s->pltt, sizeof s->pltt), o += sizeof s->pltt;
      memcpy(o, s->oam, sizeof s->oam), o += sizeof s->oam;
      for (int i = 0; i < np; i++, o += DR_PAGE) {
        memcpy(o, s->vram + (size_t)pages[i] * DR_PAGE, DR_PAGE);
        memcpy(D.vram + (size_t)pages[i] * DR_PAGE, o, DR_PAGE);
      }
      if (s->gx_words) memcpy(o, s->gx, (size_t)s->gx_words * 4);
      D.vramOk = 1;
      D.key[D.nkey++] = h;
      D.keyBytes += (long long)n;
      D.lastBytes = (int)n;
    }
  }
  const long long dt = (long long)SIM_PaceNowNs() - t0;
  D.recNs += dt;
  D.recLast = dt;
  if (dt > D.recMax) D.recMax = dt;
  DrLeave();
  return seq;
}

void bwport_diagrec_shows(unsigned id, int idx, unsigned id3d, int r3) {
  if (!DrEnter()) return;
  D.picId = id, D.picIdx = idx, D.picId3d = id3d, D.picR3 = r3;
  D.picSerial++;
  DrLeave();
}
