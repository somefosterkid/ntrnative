/* port/bridge/vcore_bridge.c: port/vcore draws the game's picture (the bridge; docs/vcore.md, docs/port-runtime.md).
 *
 * libntr calls the frame renderer registered here when it draws a frame (SIM_Render, at the game's V-blank wait) and
 * sim_config.ini's Renderer is vcore (libntr-vbridge.patch; BWPORT_RENDERER=vcore|libntr overrides the file): it
 * fills a VCoreState with the frame's whole video state straight from libntr (SIM_VCapGather: the registers, VRAM
 * banks A-I, palettes, OAM, and the shown 3D frame's commands after a prologue that rebuilds the geometry engine's
 * state, the same data a .vcs file holds, <ref>/VCS_FORMAT.md), renders it with vcore and returns both screens
 * as one 256x384 RGBA8 picture, top screen first, which libntr shows in its window in place of its own engines'.
 * A display capture vcore made (DISPCAPCNT) is stored back into the bank in libntr's VRAM, where the game and the
 * next frame read it.
 *
 * The pipeline: the frame is gathered here, on the game's thread, and drawn on a worker thread while the
 * game runs its next frame; the call returns the previous frame's picture. The pictures are the same, in the same
 * order, one frame later than without the pipeline (BWPORT_VCORE_PIPE=0 draws each frame before the call returns).
 * A frame that captures (DISPCAPCNT) is drawn before the call returns, as the game reads the capture's bank next.
 *
 * N x: with a Resolution above 1 (SIM_RenderScale; BWPORT_VCORE_SCALE=N overrides it) vcore draws its 3D
 * at N times the DS resolution on the CPU (p21) and the picture is 256 N x 384 N: libntr's sized hook (libntr-vscale.patch,
 * SIM_SetFrameRendererSized) takes pixels and size; each slot keeps its own picture, so the one a call returns stays valid
 * until the call after next. A frame that captures runs the 1x 3D too (p21), the capture is stored as at 1x.
 *
 * Antialiasing: with Antialiasing on, 4 samples (SIM_AntialiasWanted k = 2), vcore draws the 3D at Resolution x k
 * and the picture it hands over is the k x k box average, Resolution x again (vcore_set_aa; p21): the averaging runs in the
 * picture stage on the worker, nothing larger is uploaded. Resolution x k is capped at 8 (VC_MAX_SCALE): above Resolution 4 k
 * is 1 and SIM_RendererAAUsed says so, for the status line. (The gpu renderer multisamples instead: gpu_bridge.c.)
 *
 * Built apart from the game's C (port/meson.build: C11 with vcore's headers, not the game's types) and linked whole,
 * so that the constructor below registers the renderer before main.
 */
#define _POSIX_C_SOURCE 200809L
#include "vcore.h"
#include "vc_shared.h"

#include <pthread.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* libntr (libntr-vbridge.patch) */
typedef const unsigned char *(*SIM_FrameRendererFn)(void);
void SIM_SetFrameRenderer(SIM_FrameRendererFn fn);
uint32_t SIM_VCapGather(uint8_t *io9, uint8_t *io9b, uint8_t *vram, uint8_t *pltt, uint8_t *oam, uint32_t **gx,
                        uint32_t *cap);
uint8_t *SIM_VCapBankHome(int bank);
void SIM_SetFrameRendererSized(const unsigned char *(*fn)(int *w, int *h)); /* libntr-vscale.patch */
int SIM_GLRendererOption(int which); /* libntr-gpuopt.patch: 0 dot, 1 line, 2 edge, 3 filter (each 0 or 1: the N x looks) */
int SIM_RenderScale(void); /* the Resolution 1..8 (sim_config.ini Resolution=, BWPORT_RESOLUTION, the Config window) */
void SIM_SetRendererIdle(int which, void (*fn)(void)); /* libntr-switchcheck.patch: fn runs when the other renderer is wanted */
uint64_t SIM_FrameLogNow(void); /* libntr-vs70.patch: BWPORT_FRAMELOG's clock (0 when the log is off) and its extra columns */
void SIM_FrameLogExtra(int i, uint64_t ns);
int SIM_AntialiasWanted(void); /* libntr-aa70.patch, libntr-ios-msaa.patch: Antialiasing off|4 samples as k = 1, 2; 3 SMAA (libntr-ios-smaa.patch) */
void SIM_RendererAAUsed(int k); /* the k vcore draws with (lowered when Resolution x k is over the 8x cap) */
int SIM_FrameSkipping(void); /* libntr-frameskip.patch: this frame is not to be drawn (BWPORT_FRAMESKIP, sim_config.ini FrameSkip) */
/* libntr-ib78.patch: in-between pictures (docs/interp.md) */
int SIM_InterpWanted(void);
int SIM_InterpPace(long long *serial, long long *nominal, long long *tick, int max, long long *off, long long *next);
void SIM_InterpLate(void); /* libntr-ios-smoothdrop.patch: this slot's in-between was not ready in time */
void SIM_SetInterpPicture(const unsigned char *(*fn)(int slot, int *w, int *h));
/* libntr-pf82.patch: the per-stage timing readout (the Metrics window's Breakdown, Debug > Save performance log): time the
 * stages when SIM_PerfOn; SIM_PerfStage(name, kind, ms) on the frame thread, kind 1 inside this call, 2 the worker's */
int SIM_PerfOn(void);
void SIM_PerfStage(const char *name, int kind, double ms);

#define IP_MAXSLOT 8
/* an in-between picture for one present slot of a tick: drawn by the worker, shown by libntr's cap loop at the slot */
typedef struct IpMid {
  uint8_t *buf;
  size_t cap;
  int w, h;
  int state; /* 0 pending, 1 ready, 2 none (the slot shows the picture before it again) */
} IpMid;

/* a frame's state and its picture; two, so that one is drawn while the next is gathered */
typedef struct Slot {
  VCoreState *s;
  uint32_t gxcap; /* s->gx's size in words (SIM_VCapGather grows it) */
  uint8_t rgba[VC_W * VC_H * 2 * 4];
  const uint8_t *pic; /* the picture of the frame drawn into this slot: rgba, or vcore's N x one */
  int pw, ph;
  uint64_t drawNs; /* BWPORT_FRAMELOG: the worker's time to draw this slot */
  int pfOn;        /* the readout: SIM_PerfOn when the slot was launched; its draw by vcore's stages (DrawSlotTimed) */
  int drawn;       /* the state's last render is complete (engine B's picture in s->eng_out[1]) */
  const u32 (*chkB)[VC_W]; /* BWPORT_2D_SKIPCHECK: the picture the skip would have kept */
  double pfStage[VC_NSTAGE], pfWall[VC_NWALL];
  /* in-between pictures (ib78): the tick's present slots as libntr's SIM_InterpPace gave them; ipN = 0: none */
  int ipN;
  long long ipSerial, ipNominal, ipTick, ipOff[IP_MAXSLOT], ipNext;
  IpMid mid[IP_MAXSLOT];
} Slot;
static Slot sSlot[2];
static int sCur;             /* the slot this call gathers into */
static int sPipe = -1;       /* the pipeline is on */
static int sInflight;        /* a worker draws slot sCur ^ 1 */
static VCTask sThread;
static Slot *sHeld;           /* the previous frame's slot when it is already drawn and not shown yet */
static Slot *sShown;          /* the slot whose picture the last drawn call handed to libntr (frame skip shows it again) */
static int sForce;            /* bwport_render_now: draw this call whatever the frame skip says */

/* the last picture and how many were drawn: port/src/lstep_dump.c (the lockstep dump) reads the screens here */
const unsigned char *bwport_last_rgba;
unsigned bwport_last_rgba_count;

/* GBATEK 28 (DISPCAPCNT): the capture's width and height by bits 20-21 */
static const u16 kCapW[4] = {128, 256, 256, 256};
static const u16 kCapH[4] = {128, 64, 128, 192};

/* vcore wrote a capture into its copy of the bank (p11_capture.c: the destination bank A-D in LCDC mode, from the
 * write offset, wrapping at the bank's 128 KB): the same bytes into libntr's bank */
static void StoreCapture(VCoreState *s, u32 cnt) {
  int bank = (cnt >> 16) & 3;
  u8 vramcnt = s->io9[VC_REG_VRAMCNT_A + bank];
  if (!(vramcnt & 0x80) || (vramcnt & 7) != 0) return; /* not LCDC: vcore captured nothing */
  uint8_t *home = SIM_VCapBankHome(bank);
  if (home == NULL) return;
  const u8 *src = vc_bank(s, bank);
  u32 at = ((cnt >> 18) & 3) * 0x8000u;
  u32 n = (u32)kCapW[(cnt >> 20) & 3] * kCapH[(cnt >> 20) & 3] * 2;
  while (n > 0) {
    u32 run = 0x20000 - at < n ? 0x20000 - at : n;
    memcpy(home + at, src + at, run);
    n -= run;
    at = (at + run) & 0x1FFFF;
  }
}

/* BWPORT_INTERP_STATS=FILE (docs/interp.md, p23): one line per drawn frame, the 3D structure against the previous frame */
static FILE *sIpLog;
static int sIpInit;
static unsigned sIpFrame;

/* ---- in-between pictures ----
 * Every call with Interpolation on knows its tick's present slots. The worker records the frame's pre-clip vertices (a
 * geometry run, p23), tells a KEYFRAME (the 3D differs from the last keyframe's: the field's odd frames repeat it) from
 * a repeat, pairs a new keyframe's lists with the previous keyframe's, and draws the pictures of the slots after the
 * first (mids, handed to libntr's cap loop at the slot) and of the next tick's first slot (the picture the next call
 * returns, as without interpolation): each is this call's state with its vertices blended toward the previous keyframe
 * by 1 - f, f = (the picture's time - the keyframe's time) / the gap between the two keyframes, so the keyframe itself
 * shows at f = 1, one gap after it was gathered. f >= 1, or a pairing that failed, draws the plain frame / shows the
 * picture before again. No extrapolation. */
#include "ip_common.h"
static Slot *sIpTick; /* the slot this call launched, when it has in-between pictures */


static const uint8_t *InterpPicture(int slot, int *w, int *h) {
  Slot *sl = sIpTick;
  if (sl == NULL || slot < 1 || slot >= sl->ipN) return NULL;
  IpMid *m = &sl->mid[slot];
  int st = __atomic_load_n(&m->state, __ATOMIC_ACQUIRE);
  if (st == 0) {
    static int grace = -1;
    if (grace < 0) {
      const char *e = getenv("BWPORT_INTERP_GRACE_US");
      grace = e != NULL ? atoi(e) : 0;
    }
    long long end = NowNs() + (long long)grace * 1000;
    while (st == 0 && NowNs() < end) st = __atomic_load_n(&m->state, __ATOMIC_ACQUIRE);
  }
  if (st == 1) {
    IC(midsShown, 1);
    *w = m->w;
    *h = m->h;
    return m->buf;
  }
  if (st == 0) { IC(midsLate, 1); SIM_InterpLate(); }
  else IC(midsNone, 1);
  return NULL;
}

/* the picture of the state's last render (vcore's N x one, or the 1x screens) into sl->pic */
static void SlotPicture(Slot *sl) {
  sl->pic = vcore_picture(sl->s, &sl->pw, &sl->ph);
  if (sl->pic == NULL) {
    vcore_frame_rgba(sl->s, sl->rgba);
    sl->pic = sl->rgba;
    sl->pw = VC_W;
    sl->ph = VC_H * 2;
  }
}

/* one picture of the state with its vertices blended toward the previous keyframe by 1 - f */
static int sIpReuse = -1;   /* 1: a render after the first of a call redoes only the 3D (the keyframe's 2D is reused; BWPORT_INTERP_REUSE=1: on, default off) */
static int sIpVerify;       /* BWPORT_INTERP_VERIFY=1: also render in full and compare, count differences */
static void IpRender(Slot *sl, int first) {
  VCoreState *s = sl->s;
  if (sIpReuse < 0) {
    const char *e = getenv("BWPORT_INTERP_REUSE");
    sIpReuse = e != NULL && e[0] == '1'; /* measured slower in wall time (the full render overlaps its 2D with the 3D on other threads) */
    sIpVerify = getenv("BWPORT_INTERP_VERIFY") != NULL;
  }
  if (first || !sIpReuse) {
    vcore_forget_2d(s);
    vcore_render(s);
    return;
  }
  vcore_render_again(s);
  if (sIpVerify) { /* the full render of the same state and blend, against the one just made */
    int w, h;
    const uint8_t *p = vcore_picture(s, &w, &h);
    size_t n = p != NULL ? (size_t)w * h * 4 : (size_t)VC_W * VC_H * 2 * 4;
    uint8_t *a = (uint8_t *)malloc(n);
    if (a != NULL) {
      if (p != NULL) memcpy(a, p, n);
      else vcore_frame_rgba(s, a);
      vcore_forget_2d(s);
      vcore_render(s);
      const uint8_t *q = vcore_picture(s, &w, &h);
      uint8_t *b = (uint8_t *)malloc(n);
      if (b != NULL) {
        if (q != NULL) memcpy(b, q, n);
        else vcore_frame_rgba(s, b);
        sIpVerifyN++;
        if (memcmp(a, b, n)) sIpVerifyBad++;
        free(b);
      }
      free(a);
    }
  }
}

static void IpRenderBlend(Slot *sl, double f, int first) {
  s32 b = (s32)((1.0 - f) * 4096.0 + 0.5);
  vc_ip_apply_begin(sl->s, &sIp.old, sIp.map, sIp.texhold, b < 0 ? 0 : b > 4096 ? 4096 : b);
  IpRender(sl, first);
  vc_ip_apply_end(sl->s);
}


/* BWPORT_INTERP_DUMP=DIR: every picture IpDraw makes as DIR/NNNNNN.png (top screen, the picture number of IpMetric's P lines) */
static void IpDump(const uint8_t *p, int w, int h) {
  static const char *dir;
  static int init;
  if (!init) {
    init = 1;
    dir = getenv("BWPORT_INTERP_DUMP");
  }
  if (dir == NULL || dir[0] == 0 || p == NULL) return;
  char path[512];
  snprintf(path, sizeof path, "%s/%06llu.png", dir, sClPic - 1);
  vcore_write_png(path, p, w, h / 2);
}

static void IpDraw(Slot *sl) {
  VCoreState *s = sl->s;
  long long tA = NowNs();
  long long gap = IpGather(s, sl->ipSerial, sl->ipNominal, sl->ipTick);
  int rendered = 0; /* the first render of this call is in full, the rest redo only the 3D */
  for (int idx = 1; idx <= sl->ipN; idx++) {
    long long off = idx < sl->ipN ? sl->ipOff[idx] : sl->ipNext;
    double f = sIp.valid ? (double)(sl->ipNominal + off - sIp.keyNs) / (double)gap : 2.0;
    int blend = f > 0.0 && f < 1.0;
    IpMetric(idx, f, blend);
    long long t0 = NowNs();
    if (idx < sl->ipN) {
      IpMid *m = &sl->mid[idx];
      if (!blend) {
        __atomic_store_n(&m->state, 2, __ATOMIC_RELEASE);
        continue;
      }
      IpRenderBlend(sl, f, !rendered);
      rendered = 1;
      int w, h;
      const uint8_t *p = vcore_picture(s, &w, &h);
      if (p == NULL) {
        w = VC_W;
        h = VC_H * 2;
      }
      size_t need = (size_t)w * (size_t)h * 4;
      if (m->cap != need) { /* the picture's size changes only with the Resolution: a smaller one gives memory back */
        free(m->buf);
        m->buf = (uint8_t *)malloc(need);
        m->cap = m->buf != NULL ? need : 0;
      }
      if (m->buf == NULL) {
        __atomic_store_n(&m->state, 2, __ATOMIC_RELEASE);
        continue;
      }
      if (p != NULL) memcpy(m->buf, p, need);
      else vcore_frame_rgba(s, m->buf);
      m->w = w;
      m->h = h;
      IpDump(m->buf, w, h);
      __atomic_store_n(&m->state, 1, __ATOMIC_RELEASE);
      IC(mids, 1);
      IC(midNs, NowNs() - t0);
    } else {
      if (blend) {
        IpRenderBlend(sl, f, !rendered);
        IC(nextMid, 1);
        IC(midNs, NowNs() - t0);
      } else {
        IpRender(sl, !rendered);
        IC(nextPlain, 1);
        IC(plainNs, NowNs() - t0);
      }
      SlotPicture(sl);
      IpDump(sl->pic, sl->pw, sl->ph);
    }
  }
  IC(wholeNs, NowNs() - tA);
}

/* the unchanged-engine skip for engine B (docs/gpu-2d.md, option 1; gpu_bridge.c's Skip2D does both engines): the frame thread,
 * the previous frame's worker joined. Engine B's inputs equal the previous frame's (vc_2d_same): vcore copies that frame's
 * engine B picture instead of drawing it (p20's keep_b). BWPORT_2D_SKIP=0 off; BWPORT_2D_SKIPCHECK=1 draws anyway and compares */
static int sSkip = -1, sSkipCheck;
static unsigned sSkipN, sSkipB, sSkipBad;
static void Skip2D(Slot *sl, const Slot *pv) {
  if (sSkip < 0) {
    const char *e = getenv("BWPORT_2D_SKIP");
    sSkip = !(e != NULL && e[0] == '0');
    e = getenv("BWPORT_2D_SKIPCHECK");
    sSkipCheck = e != NULL && e[0] == '1';
  }
  VCoreState *s = sl->s;
  s->p20.keep_b = NULL;
  sl->chkB = NULL;
  sl->drawn = 0;
  if (sSkip && pv->s != NULL && pv->drawn) {
    vc_vram_map(s); /* (vcore_render maps again) */
    if (vc_2d_same(s, pv->s, VC_ENG_B)) {
      if (sSkipCheck) sl->chkB = (const u32(*)[VC_W])pv->s->eng_out[VC_ENG_B];
      else s->p20.keep_b = (const u32(*)[VC_W])pv->s->eng_out[VC_ENG_B];
      sSkipB++;
    }
  }
  if (++sSkipN == 600) {
    printf("bwport: Renderer=vcore: 2D skipped B %u%% of the last 600 frames%s\n", sSkipB / 6,
           sSkipCheck ? (sSkipBad ? " (SKIPCHECK: mismatches, see above)" : " (SKIPCHECK: 0 mismatches)") : "");
    fflush(stdout);
    sSkipN = sSkipB = 0;
  }
}

static void DrawPicture(Slot *sl); /* the frame's render and picture (and its in-between pictures) */
static void DrawSlot(Slot *sl) {
  uint64_t t0 = SIM_FrameLogNow();
  DrawPicture(sl);
  if (sl->chkB != NULL && memcmp(sl->s->eng_out[VC_ENG_B], sl->chkB, sizeof sl->s->eng_out[0]) != 0) {
    sSkipBad++;
    printf("bwport: Renderer=vcore: 2D SKIPCHECK mismatch, engine B: DISPCNT B %08x\n", (unsigned)vc_eng32(sl->s, VC_ENG_B, VC_REG_DISPCNT));
    fflush(stdout);
  }
  sl->drawn = 1;
  if (t0 != 0) sl->drawNs = SIM_FrameLogNow() - t0;
}

static void DrawPicture(Slot *sl) {
  if (!sIpInit) {
    sIpInit = 1;
    const char *e = getenv("BWPORT_INTERP_STATS");
    if (e != NULL && e[0] != 0) sIpLog = fopen(e, "w");
  }
  if (sl->ipN > 0 && sIpLog == NULL) {
    IpDraw(sl);
    return;
  }
  if (sIpLog != NULL) sl->s->p23.mode = VC_IP_RECORD;
  vcore_render(sl->s);
  if (sIpLog != NULL) {
    vc_ip_log_frame(sl->s, sIpLog, sIpFrame++);
    if ((sIpFrame & 63) == 0) fflush(sIpLog);
    sl->s->p23.mode = VC_IP_OFF;
  }
  sl->pic = vcore_picture(sl->s, &sl->pw, &sl->ph);
  if (sl->pic == NULL) {
    vcore_frame_rgba(sl->s, sl->rgba);
    sl->pic = sl->rgba;
    sl->pw = VC_W;
    sl->ph = VC_H * 2;
  }
}

/* the readout: vcore's stage sums (p20_frame.c: vc_stage_ns, each stage's thread CPU time; vc_nwall_ns, the N x 3D thread's
 * and the picture's wall time) across this slot's draw, when the slot was launched with the readout on */
static void DrawSlotTimed(Slot *sl) {
  if (!sl->pfOn) {
    DrawSlot(sl);
    return;
  }
  double st[VC_NSTAGE], nw[VC_NWALL];
  memcpy(st, vc_stage_ns, sizeof st);
  memcpy(nw, vc_nwall_ns, sizeof nw);
  DrawSlot(sl);
  for (int k = 0; k < VC_NSTAGE; k++) sl->pfStage[k] = vc_stage_ns[k] - st[k];
  for (int k = 0; k < VC_NWALL; k++) sl->pfWall[k] = vc_nwall_ns[k] - nw[k];
}

/* a drawn slot's stages to libntr (the frame thread, the worker joined) */
static void PerfReport(const Slot *sl) {
  if (!sl->pfOn || !SIM_PerfOn()) return;
  const double *st = sl->pfStage, *nw = sl->pfWall;
  /* in the pipeline's order; clear, raster and post: the 1x 3D's thread CPU (vc_3d_frame) or the N x 3D's wall time */
  SIM_PerfStage("draw (whole)", 2, (double)sl->drawNs / 1e6);
  SIM_PerfStage("3D geometry", 2, (st[VC_ST_GEO] + st[VC_ST_MAP]) / 1e6);
  if (nw[0] > 0.0) SIM_PerfStage("3D plan (N x)", 2, nw[0] / 1e6);
  SIM_PerfStage("3D clear", 2, (st[VC_ST_CLEAR] + nw[1]) / 1e6);
  SIM_PerfStage("3D raster", 2, (st[VC_ST_RASTER] + nw[2]) / 1e6);
  SIM_PerfStage("3D post", 2, (st[VC_ST_POST] + nw[3]) / 1e6);
  SIM_PerfStage("2D engine A", 2, st[VC_ST_2DA] / 1e6);
  SIM_PerfStage("2D engine B", 2, st[VC_ST_2DB] / 1e6);
  if (nw[4] > 0.0) SIM_PerfStage("picture (N x)", 2, nw[4] / 1e6);
}

/* the scale the next frame draws at */
static int WantScale(void) {
  if (getenv("BWPORT_LSTEP") != NULL) return 1; /* the lockstep dump reads 256x384 pictures */
  const char *e = getenv("BWPORT_VCORE_SCALE");
  int n = e != NULL && atoi(e) >= 1 ? atoi(e) : SIM_RenderScale();
  return n < 1 ? 1 : n > VC_MAX_SCALE ? VC_MAX_SCALE : n;
}

static uint64_t sWorkerAsked; /* BWPORT_FRAMELOG: when the worker was asked for, and how long it took to start */
static uint64_t sWorkerLate;
static void *Worker(void *arg) {
  if (sWorkerAsked != 0) sWorkerLate = SIM_FrameLogNow() - sWorkerAsked;
  DrawSlotTimed((Slot *)arg);
  return NULL;
}

static const uint8_t *FrameSized(int *pw, int *ph) {
  if (sPipe < 0) {
    const char *e = getenv("BWPORT_VCORE_PIPE");
    sPipe = !(e != NULL && e[0] == '0');
    if (getenv("BWPORT_LSTEP") != NULL) sPipe = 0; /* a lockstep dump records each frame's own picture */
    e = getenv("BWPORT_VCORE_THREADS"); /* 0: the frame's 3D and engines run one after the other (timing it) */
    if (e != NULL && e[0] == '0') vc_threads = 0;
  }
  /* frame skip (libntr-frameskip.patch): nothing gathered, nothing drawn; a worker still running is left to the next
   * drawn call (it joins it); libntr shows the last picture again. Never when there is no picture yet. A worker that has
   * finished is joined here and its frame shown: it waited for the next drawn call, so with skip N the picture shown was
   * N + 1 ticks older than it had to be (skip 3 at 60: a mean age of 5.5 ticks, now 2.5) */
  int force = sForce;
  sForce = 0;
  sIpTick = NULL; /* no in-between pictures for this call until it is launched with them */
  if (!force && sShown != NULL && SIM_FrameSkipping()) {
    if (sInflight && vcore_task_done(&sThread)) {
      vcore_task_join(&sThread);
      sInflight = 0;
      sHeld = sShown = &sSlot[sCur ^ 1];
      PerfReport(sShown);
    }
    *pw = sShown->pw;
    *ph = sShown->ph;
    return sShown->pic;
  }
  uint64_t tA = SIM_FrameLogNow();
  Slot *sl = &sSlot[sCur];
  if (sl->s == NULL) {
    sl->s = vcore_create();
    if (sl->s == NULL) return NULL; /* libntr draws */
  }
  VCoreState *s = sl->s;
  vcore_set_scale(s, WantScale());
  vcore_set_aa(s, getenv("BWPORT_LSTEP") != NULL || SIM_AntialiasWanted() != 2 ? 1 : 2); /* SMAA: off here (the gpu renderer's) */
  SIM_RendererAAUsed(vcore_get_aa(s));
  vcore_set_options(s, SIM_GLRendererOption(0), SIM_GLRendererOption(1), SIM_GLRendererOption(2));
  vcore_set_filter(s, SIM_GLRendererOption(3));
  s->gx_words = SIM_VCapGather(s->io9, s->io9b, s->vram, s->pltt, s->oam, &s->gx, &sl->gxcap);
  /* POWCNT1 as vc_state.c's loader (and the oracle) reads it: the LCD bit on, 0 = everything on, engine A on top */
  u16 pow = (u16)(s->io9[VC_REG_POWCNT1] | s->io9[VC_REG_POWCNT1 + 1] << 8);
  pow = pow == 0 ? 0x820F : (u16)(pow | 1);
  s->io9[VC_REG_POWCNT1] = (u8)pow;
  s->io9[VC_REG_POWCNT1 + 1] = (u8)(pow >> 8);
  u32 cap = vc_io32(s, VC_REG_DISPCAPCNT);
  uint64_t tB = SIM_FrameLogNow();
  sl->ipN = 0;
  if (sPipe && !(cap & 0x80000000u) && sIpLog == NULL && SIM_InterpWanted() && getenv("BWPORT_LSTEP") == NULL) {
    long long off[IP_MAXSLOT];
    int n = SIM_InterpPace(&sl->ipSerial, &sl->ipNominal, &sl->ipTick, IP_MAXSLOT, off, &sl->ipNext);
    if (n > 0) {
      for (int i = 0; i < n; i++) {
        sl->ipOff[i] = off[i];
        sl->mid[i].state = 0;
      }
      sl->ipN = n;
      sIpTick = sl;
    }
  }

  Slot *held = sHeld; /* the previous frame's slot */
  if (sInflight) {
    vcore_task_join(&sThread);
    sInflight = 0;
    held = &sSlot[sCur ^ 1];
    SIM_FrameLogExtra(2, held->drawNs); /* the worker's own time for the frame joined */
    SIM_FrameLogExtra(6, sWorkerLate); /* and how long it took to start */
    PerfReport(held);
  }
  Skip2D(sl, &sSlot[sCur ^ 1]);
  uint64_t tC = SIM_FrameLogNow();
  sHeld = NULL;
  Slot *ret = sl;
  sl->pfOn = SIM_PerfOn();
  int inCall = 1;
  if (sPipe && !(cap & 0x80000000u)) {
    sWorkerAsked = SIM_FrameLogNow();
    sInflight = vcore_task_start(&sThread, Worker, sl);
    if (!sInflight) DrawSlotTimed(sl), PerfReport(sl);
    else inCall = 0;
  } else {
    DrawSlotTimed(sl);
    if (cap & 0x80000000u) StoreCapture(s, cap);
    SIM_FrameLogExtra(5, sl->drawNs); /* drawn here, in the call */
    PerfReport(sl);
  }
  uint64_t tD = SIM_FrameLogNow();
  if (tA != 0 && SIM_PerfOn()) {
    SIM_PerfStage("gather", 1, (double)(tB - tA) / 1e6);
    SIM_PerfStage("wait for the worker", 1, (double)(tC - tB) / 1e6);
    SIM_PerfStage(inCall ? "draw (in the call)" : "start the worker", 1, (double)(tD - tC) / 1e6);
  }
  if (tA != 0) {
    SIM_FrameLogExtra(0, tB - tA); /* gather */
    SIM_FrameLogExtra(1, tC - tB); /* waiting for the worker */
    SIM_FrameLogExtra(3, tD - tC); /* creating the worker (or drawing in the call) */
  }
  if (sPipe) {
    if (held != NULL) {
      ret = held; /* this frame is shown at the next call */
      if (!sInflight) sHeld = sl;
    } else {
      /* the first frame: nothing earlier to show; draw it now and show it twice */
      if (sInflight) {
        vcore_task_join(&sThread);
        sInflight = 0;
      }
      sHeld = sl;
    }
  }
  sCur ^= 1;
  sShown = ret;
  {
    static unsigned n;
    if ((++n % 120) == 0) IpReport();
  }
  bwport_last_rgba = ret->pic; /* lockstep (port/src/lstep_dump.c): the picture of this call (256x384: it runs at 1x) */
  bwport_last_rgba_count++;
  if (tA != 0) SIM_FrameLogExtra(4, SIM_FrameLogNow() - tA); /* the whole call */
  *pw = ret->pw;
  *ph = ret->ph;
  return ret->pic;
}

/* libntr without the sized hook (and the old name): the 256x384 picture, NULL when this frame is bigger */
static const unsigned char *BridgeFrame(void) {
  int w, h;
  const uint8_t *p = FrameSized(&w, &h);
  return w == VC_W && h == VC_H * 2 ? p : NULL;
}

/* an extra render of the state as it is now (the lockstep dump, after the V-blank work); NULL before the first frame */
const unsigned char *bwport_render_now(void) {
  if (bwport_last_rgba_count == 0) return NULL; /* no frame yet */
  sForce = 1;
  return BridgeFrame();
}

/* libntr (libntr-switchcheck.patch): Renderer or Resolution changed to the GPU path, which draws from now on: give back
 * the N x frames (~180 MB a slot at 8x) and the states. Runs on the render thread; a worker still drawing is joined. */
static void BridgeIdle(void) {
  if (sInflight) {
    vcore_task_join(&sThread);
    sInflight = 0;
  }
  for (int i = 0; i < 2; i++) {
    vcore_destroy(sSlot[i].s);
    for (int j = 0; j < IP_MAXSLOT; j++) {
      free(sSlot[i].mid[j].buf);
      sSlot[i].mid[j].buf = NULL;
      sSlot[i].mid[j].cap = 0;
    }
    sSlot[i].ipN = 0;
    sSlot[i].s = NULL;
    sSlot[i].gxcap = 0;
    sSlot[i].pic = NULL;
    sSlot[i].drawn = 0;
  }
  sCur = 0;
  sHeld = sShown = NULL;
  sIpTick = NULL;
  sIp.have = sIp.haveOld = sIp.valid = 0;
  if (bwport_last_rgba != NULL) bwport_last_rgba = sSlot[0].rgba; /* not into a freed N x picture */
}

__attribute__((constructor)) static void BridgeInstall(void) {
  SIM_SetRendererIdle(1, BridgeIdle);
  SIM_SetFrameRenderer(BridgeFrame);
  SIM_SetFrameRendererSized(FrameSized);
  SIM_SetInterpPicture(InterpPicture);
  atexit(IpReport);
}
