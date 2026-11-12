/* port/bridge/gpu_bridge.c: port/gpu draws the game's 3D at N x the DS resolution (the bridge; docs/gpu.md, "The renderer
 * switch and the bridge (G8)").
 *
 * libntr calls the GL frame renderer registered here (SIM_SetFrameRendererGL, libntr-gpu.patch) when it draws a frame
 * and sim_config.ini's Renderer is gpu (BWPORT_RENDERER=gpu; Scale=N / BWPORT_GPU_SCALE=N), with its GL context
 * current. The frame is gathered as vcore_bridge.c does (SIM_VCapGather into a VCoreState), then split in two:
 *   the CPU half (stage 1) on a worker thread while the game runs its next frame: vcore's geometry (gpu_geometry: the
 *     clipped, lit, transformed polygons in drawing order) and engine B's 2D at 1x;
 *   the GL half (stage 2) on the calling thread, one frame later, in the hook: the span records and the passes
 *     (gpu_render_3d), then the compositing, and the GL texture handed back to libntr. The GL state the passes change
 *     is saved before and restored after (SaveGL / RestoreGL).
 * The picture is one frame late, as vcore's pipeline is (BWPORT_VCORE_PIPE=0 draws each frame before the call returns).
 * A frame that captures (DISPCAPCNT) is drawn by vcore alone at 1x, as the game reads the capture's bank next.
 *
 * Engine A is composited at every scale by G7's pass C (g07_compose.c): vcore draws the 2D layers (OBJs, BGs, windows)
 * at 1x on the worker (g07_stage_build), the shader reads them nearest-neighbour with the 3D layer, which pass P left in
 * a texture (gpu_3d_tex): nothing is read back. Engine B is vcore's 1x screen, drawn at N x by the same pass.
 *
 * Fallback: no GL 3.3, a shader that fails to compile or gpu_create failing -> the hook returns 0 and libntr's vcore
 * renderer draws (one line is logged); a GL error in a frame -> that frame's 3D is vcore's CPU raster, and after
 * three of them the GPU path is given up.
 *
 * Built apart from the game's C (port/meson.build: C11 with vcore's headers, not the game's types) and linked whole,
 * so that the constructor below registers the renderer before main.
 */
#define _POSIX_C_SOURCE 200809L
#include "g07_compose.h"
#include "gpu.h"
#include "vc_shared.h"

#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define GL_GLES_PROTOTYPES 0
#include <GLES3/gl3.h>

/* libntr (libntr-gpu.patch, libntr-vbridge.patch) */
typedef unsigned (*SIM_FrameRendererGLFn)(int *w, int *h);
void SIM_SetFrameRendererGL(SIM_FrameRendererGLFn fn);
void *SIM_GLGetProc(const char *name);
int SIM_RenderScale(void); /* libntr-onerender.patch: the Resolution, 1..8 */
unsigned SIM_RendererSerial(void); /* changes with the renderer or the resolution: start afresh */
void SIM_SetRendererIdle(int which, void (*fn)(void)); /* libntr-switchcheck.patch: fn runs when the other renderer is wanted */
void SIM_RendererFailed(const char *reason); /* the status line says why the GPU path is not drawing */
int SIM_FrameSkipping(void); /* libntr-frameskip.patch: this frame is not to be drawn */
int SIM_AntialiasWanted(void); /* libntr-aa70.patch, libntr-ios-msaa.patch: Antialiasing off|4 samples as k = 1, 2; 3 SMAA (libntr-ios-smaa.patch) */
void SIM_RendererAAUsed(int k); /* the k the GPU path draws with (k x k samples per pixel; never lowered) */
void SIM_Renderer3DFast(int ok, const char *how); /* libntr-ios-fastgl.patch: Renderer3D=fast drawn (1) or not (0), how or why */
int SIM_GLRendererOption(int which); /* libntr-gpuopt.patch: 0 dot, 1 line, 2 edge, 3 filter; 4 the 3D renderer fast (libntr-ios-fast3d.patch) */
uint32_t SIM_VCapGather(uint8_t *io9, uint8_t *io9b, uint8_t *vram, uint8_t *pltt, uint8_t *oam, uint32_t **gx,
                        uint32_t *cap);
uint8_t *SIM_VCapBankHome(int bank);
/* libntr-ib78.patch / libntr-ib79.patch: in-between pictures (docs/interp.md) */
int SIM_InterpWanted(void);
int SIM_InterpPace(long long *serial, long long *nominal, long long *tick, int max, long long *off, long long *next);
void SIM_InterpLate(void); /* libntr-ios-smoothdrop.patch: this slot's in-between was not ready in time */
void SIM_SetInterpTexture(unsigned (*fn)(int slot, int *w, int *h, long long waitNs));
/* libntr-pf82.patch: the per-stage timing readout: time the stages when SIM_PerfOn; SIM_PerfStage(name, kind, ms) on the frame
 * thread, kind 1 inside this call, 2 the worker's, 3 GPU ms; SIM_PerfGpuTimers: GL timer queries work in libntr's context */
int SIM_PerfOn(void);
void SIM_PerfStage(const char *name, int kind, double ms);
int SIM_PerfGpuTimers(void);
/* libntr-ios-passes.patch: a bench row's parts (cfgscript passes=1): glFinish after each pass, the worker's CPU half and the GPU's
 * passes in ms a frame to libntr, which prints their means after the row (the phone's GLES has no timer queries) */
int SIM_PassTiming(void);
unsigned SIM_Ablation(void); /* test-only ablations (libntr-ios-ablate.patch, cfgscript ablate=): GPU_ABL_* bits, 32 = the whole GL/Metal half */
void SIM_PassCpu(double geo, double spans, double twod);
void SIM_Pass2DSkip(int reuse); /* libntr-ios-2dskip.patch: the engines whose 2D this frame reused (bit 0 A, bit 1 B) */
void SIM_PassGpu(double up, double r, double ep, double smaa, double c); /* (smaa: libntr-ios-smaa.patch) */
void SIM_PassGpuE(double e); /* pass E of ep (libntr-ios-passe.patch) */
#include "ip_common.h"
int SIM_PresentMetal(void); /* libntr-ios-metal.patch: the window presents with Metal (decided at its creation) */
#ifdef BR_METAL
#include "gpu_bridge_mtl.h"
void SIM_SetFrameTextureMetal(void *(*fn)(unsigned name)); /* libntr-ios-gpuapi.patch */
void bwport_sys_gpu_add(double ms);                       /* port/src/sysmetrics.c */
#endif

/* ---- the GL functions the bridge itself calls (the passes' are gpu_gl.c's) ---- */
#define Log(...) (printf(__VA_ARGS__), fflush(stdout)) /* the run's log.txt is a file: no buffering */

#define BR_GL_LIST(X)                                                                                                 \
  X(PFNGLACTIVETEXTUREPROC, ActiveTexture)                                                                           \
  X(PFNGLBINDFRAMEBUFFERPROC, BindFramebuffer)                                                                       \
  X(PFNGLBINDTEXTUREPROC, BindTexture)                                                                               \
  X(PFNGLBINDVERTEXARRAYPROC, BindVertexArray)                                                                       \
  X(PFNGLDELETETEXTURESPROC, DeleteTextures)                                                                        \
  X(PFNGLDISABLEPROC, Disable)                                                                                       \
  X(PFNGLENABLEPROC, Enable)                                                                                         \
  X(PFNGLFINISHPROC, Finish)                                                                                         \
  X(PFNGLGENTEXTURESPROC, GenTextures)                                                                               \
  X(PFNGLGETBOOLEANVPROC, GetBooleanv)                                                                               \
  X(PFNGLGETERRORPROC, GetError)                                                                                     \
  X(PFNGLGETINTEGERVPROC, GetIntegerv)                                                                               \
  X(PFNGLGETSTRINGPROC, GetString)                                                                                   \
  X(PFNGLISENABLEDPROC, IsEnabled)                                                                                   \
  X(PFNGLPIXELSTOREIPROC, PixelStorei)                                                                               \
  X(PFNGLCOLORMASKPROC, ColorMask)                                                                                   \
  X(PFNGLTEXIMAGE2DPROC, TexImage2D)                                                                                 \
  X(PFNGLTEXPARAMETERIPROC, TexParameteri)                                                                           \
  X(PFNGLTEXSUBIMAGE2DPROC, TexSubImage2D)                                                                           \
  X(PFNGLUSEPROGRAMPROC, UseProgram)                                                                                 \
  X(PFNGLVIEWPORTPROC, Viewport)

static struct {
#define BR_MEMBER(type, name) type name;
  BR_GL_LIST(BR_MEMBER)
#undef BR_MEMBER
} bgl;
#define BGL(name) (bgl.name)

static int LoadGL(char *err, size_t errlen) {
#define BR_GET(type, name)                                                                                            \
  bgl.name = (type)SIM_GLGetProc("gl" #name);                                                                         \
  if (bgl.name == NULL) {                                                                                             \
    snprintf(err, errlen, "GL function gl%s missing", #name);                                                         \
    return -1;                                                                                                        \
  }
  BR_GL_LIST(BR_GET)
#undef BR_GET
  return 0;
}

static GpuProc GetProc(const char *name) { return (GpuProc)SIM_GLGetProc(name); }

/* ---- the GL state the passes change, saved and restored around the GL half ---- */
enum { MAX_UNITS = 16 };
typedef struct GLSave {
  GLint active, program, vao, drawfb, readfb, viewport[4], pack, unpack, nunits, tex[MAX_UNITS];
  GLboolean depth, blend, cull, scissor, stencil, dither, mask[4];
} GLSave;

static void SaveGL(GLSave *g) {
  BGL(GetIntegerv)(GL_ACTIVE_TEXTURE, &g->active);
  BGL(GetIntegerv)(GL_CURRENT_PROGRAM, &g->program);
  BGL(GetIntegerv)(GL_VERTEX_ARRAY_BINDING, &g->vao);
  BGL(GetIntegerv)(GL_DRAW_FRAMEBUFFER_BINDING, &g->drawfb);
  BGL(GetIntegerv)(GL_READ_FRAMEBUFFER_BINDING, &g->readfb);
  BGL(GetIntegerv)(GL_VIEWPORT, g->viewport);
  BGL(GetIntegerv)(GL_PACK_ALIGNMENT, &g->pack);
  BGL(GetIntegerv)(GL_UNPACK_ALIGNMENT, &g->unpack);
  g->depth = BGL(IsEnabled)(GL_DEPTH_TEST);
  g->blend = BGL(IsEnabled)(GL_BLEND);
  g->cull = BGL(IsEnabled)(GL_CULL_FACE);
  g->scissor = BGL(IsEnabled)(GL_SCISSOR_TEST);
  g->stencil = BGL(IsEnabled)(GL_STENCIL_TEST);
  g->dither = BGL(IsEnabled)(GL_DITHER);
  BGL(GetBooleanv)(GL_COLOR_WRITEMASK, g->mask);
  GLint n = 8;
  BGL(GetIntegerv)(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &n);
  g->nunits = n > MAX_UNITS ? MAX_UNITS : n;
  for (int i = 0; i < g->nunits; i++) {
    BGL(ActiveTexture)(GL_TEXTURE0 + i);
    BGL(GetIntegerv)(GL_TEXTURE_BINDING_2D, &g->tex[i]);
  }
}

static void RestoreGL(const GLSave *g) {
  for (int i = 0; i < g->nunits; i++) {
    BGL(ActiveTexture)(GL_TEXTURE0 + i);
    BGL(BindTexture)(GL_TEXTURE_2D, (GLuint)g->tex[i]);
  }
  BGL(ActiveTexture)((GLenum)g->active);
  BGL(BindFramebuffer)(GL_DRAW_FRAMEBUFFER, (GLuint)g->drawfb);
  BGL(BindFramebuffer)(GL_READ_FRAMEBUFFER, (GLuint)g->readfb);
  BGL(UseProgram)((GLuint)g->program);
  BGL(BindVertexArray)((GLuint)g->vao);
  BGL(Viewport)(g->viewport[0], g->viewport[1], g->viewport[2], g->viewport[3]);
  BGL(PixelStorei)(GL_PACK_ALIGNMENT, g->pack);
  BGL(PixelStorei)(GL_UNPACK_ALIGNMENT, g->unpack);
  (g->depth ? BGL(Enable) : BGL(Disable))(GL_DEPTH_TEST);
  (g->blend ? BGL(Enable) : BGL(Disable))(GL_BLEND);
  (g->cull ? BGL(Enable) : BGL(Disable))(GL_CULL_FACE);
  (g->scissor ? BGL(Enable) : BGL(Disable))(GL_SCISSOR_TEST);
  (g->stencil ? BGL(Enable) : BGL(Disable))(GL_STENCIL_TEST);
  (g->dither ? BGL(Enable) : BGL(Disable))(GL_DITHER);
  BGL(ColorMask)(g->mask[0], g->mask[1], g->mask[2], g->mask[3]);
}

/* ---- the frames ---- */

#define IP_MAXSLOT 8
/* an in-between picture of one present slot: its span records (state 0 pending, 1 ready, 2 none: the slot shows the picture before again) */
typedef struct IpMid {
  GpuPrep *prep;
  int state;
} IpMid;
/* the worker tells the GL thread a mid is settled (InterpTexture sleeps on it, it no longer spins) */
static pthread_mutex_t sMidM = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t sMidC = PTHREAD_COND_INITIALIZER;
static void MidSettle(IpMid *m, int state) {
  pthread_mutex_lock(&sMidM);
  __atomic_store_n(&m->state, state, __ATOMIC_RELEASE);
  pthread_cond_broadcast(&sMidC);
  pthread_mutex_unlock(&sMidM);
}

/* a frame's state and its pictures; two, so that one is drawn while the next is gathered */
typedef struct Slot {
  VCoreState *s;
  uint32_t gxcap;  /* s->gx's size in words (SIM_VCapGather grows it) */
  int has;         /* a frame is in this slot */
  int mode;        /* M_GPU / M_VCORE */
  int done2;       /* its GL half has run (the picture is in sPic) */
  int geo;         /* M_GPU: gpu_geometry found a frame to draw */
  int uses3d;      /* M_GPU: engine A shows the 3D layer */
  uint32_t cap;    /* DISPCAPCNT */
  VC_Line *stage;  /* M_GPU: engine A's 2D layers at 1x (g07_stage_build, on the worker) */
  G07Pack *pack;   /* M_GPU: them and the 1x lines as pass C uploads them (g07_pack, on the worker; the mids reuse the upload):
                      one of sPack, shared with the frame before when neither engine changed (Build2D) */
  int ok2d;        /* M_GPU: stage 1 left this frame's 2D in pack */
  int reuse2d;     /* the engines (bit 0 A, bit 1 B) whose 2D inputs equal the frame before's (Skip2D) */
  const G07Pack *from; /* that frame's pack (NULL: none) */
  double pfCmp;    /* the readout: Skip2D's compare, ms (in the "2D" part) */
  GpuPrep *prep;   /* M_GPU: the span records, polygon table and band lists (gpu_prepare, on the worker) */
  GpuTexData *td;  /* M_GPU: the VRAM's textures flattened (on the worker), shared by the frame and its in-between pictures */
  int tdOk;        /* td holds this frame's */
  int prepared;    /* prep holds this frame's records */
  int pfOn;        /* the readout: SIM_PerfOn when the slot was launched; stage 1's parts in ms */
  double pfGeo, pfSpans, pf2d, pfWhole;
  GpuOptions opt;  /* the N x looks (sim_config.ini Dot, Line, Edge, Filter) as they were when this frame was gathered: this slot's
                      worker (dot, line) and GL half (edge, filter) read this copy, not the next frame's settings */
  uint8_t rgba[VC_W * VC_H * 2 * 4];
  /* in-between pictures (ib79): the tick's present slots as libntr's SIM_InterpPace gave them; ipN = 0: none. The worker
   * prepares each slot's 3D (mid[i]: the blended geometry's span records) and the GL thread draws it at the slot (InterpTexture) */
  int ipN;
  long long ipSerial, ipNominal, ipTick, ipOff[IP_MAXSLOT], ipNext;
  IpMid mid[IP_MAXSLOT];
} Slot;
enum { M_GPU = 0, M_VCORE = 1 };
static Slot *sIpTick; /* the slot this call launched, when it has in-between pictures */
static G07Pack *sPack[2]; /* the slots' packs: a slot writes the one the frame before does not use */

static Slot sSlot[2];
static int sCur;       /* the slot this call gathers into */
static int sPipe = -1; /* the pipeline is on */
static int sInflight;  /* a worker runs stage 1 of slot sCur ^ 1 */
static VCTask sThread;  /* vcore's persistent task threads (p22_task.c), not a new thread a frame */

static int sInit, sDead, sLogged;
static GpuCore *sCore;
static int sScale;  /* the grid the spans and samples are on: sOut * k */
static int sOut;    /* the picture's scale (the Resolution): pass R's targets and the 3D layer are at sOut */
static int sFastOk; /* the fast 3D renderer (Renderer3D=fast, docs/gpu-hwraster.md): 1 the core draws it, 0 it cannot (a GL without
                     * framebuffer fetch or fragment interlock, g10_fast_gl.c; its pipelines
                     * failed), -1 not tried: the first frame that wants it draws exact and the render thread tries (the pipelines) */
static int sFastWant; /* a gathered frame wanted it */
static int sAA;     /* multisampling k in force: 1 or 2 (k x k samples per pixel; docs/gpu.md "Antialiasing"), or GPU_AA_SMAA (k = 1) */
static int sErrors;     /* consecutive frames with a GL error of our own */
static int sDeadTicks;  /* frames since the GPU path was given up after repeated GL errors (it is tried again) */
static int sFatal;      /* the GL functions or the passes cannot be set up: no retry until Renderer, Resolution or AA changes */

/* GL errors raised by the program's other GL users (libntr's ImGui, the window) before our passes run */
static void DrainGLErrors(void) {
  for (int i = 0; i < 16 && BGL(GetError)() != GL_NO_ERROR; i++) {}
}
static G07 *sC7;

/* the picture handed to libntr */
static struct {
  GLuint tex;
  int w, h;
} sPic;
static GLuint sTex1;                        /* 256 x 384 RGBA8: the 1x screens */

static const u16 kCapW[4] = {128, 256, 256, 256};
static const u16 kCapH[4] = {128, 64, 128, 192};

/* vcore wrote a capture into its copy of the bank (as vcore_bridge.c's StoreCapture): the same bytes into libntr's bank */
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

/* the state of the frame being gathered, as vcore_bridge.c's BridgeFrame */
static int Gather(Slot *sl) {
  if (sl->s == NULL) {
    sl->s = vcore_create();
    if (sl->s == NULL) return -1;
  }
  VCoreState *s = sl->s;
  if (sl->stage == NULL && (sl->stage = malloc(sizeof(VC_Line) * VC_H)) == NULL) return -1;
  if (sl->td == NULL && (sl->td = gpu_texdata_create()) == NULL) return -1;
  for (int i = 0; i < 2; i++)
    if (sPack[i] == NULL && (sPack[i] = g07_pack_create()) == NULL) return -1;
  sl->tdOk = 0;
  sl->ok2d = 0;
  s->gx_words = SIM_VCapGather(s->io9, s->io9b, s->vram, s->pltt, s->oam, &s->gx, &sl->gxcap);
  u16 pow = (u16)(s->io9[VC_REG_POWCNT1] | s->io9[VC_REG_POWCNT1 + 1] << 8);
  pow = pow == 0 ? 0x820F : (u16)(pow | 1);
  s->io9[VC_REG_POWCNT1] = (u8)pow;
  s->io9[VC_REG_POWCNT1 + 1] = (u8)(pow >> 8);
  sl->cap = vc_io32(s, VC_REG_DISPCAPCNT);
  return 0;
}

/* the span records of s->geo (just drawn by the geometry engine) in *pp, made or remade for the scale in force */
static void PrepareInto(Slot *sl, GpuPrep **pp) {
  if (*pp != NULL && gpu_prep_scale(*pp) != sScale) gpu_prep_destroy(*pp), *pp = NULL;
  if (*pp == NULL) *pp = gpu_prep_create(sScale);
  if (*pp != NULL) gpu_prep_set_options(*pp, &sl->opt), gpu_prepare(*pp, sl->s);
}

/* ---- the unchanged-engine skip (docs/gpu-2d.md, option 1): an engine whose 2D inputs (vc_2d_same) equal the frame before's
 * keeps that frame's half of the pack (engine B's lines, engine A's layers); neither changed: the same pack, its serial too, so
 * pass C uploads nothing. Exact by construction; BWPORT_2D_SKIPCHECK=1 draws every frame anyway and logs where the reused half
 * would have differed (a missed input). BWPORT_2D_SKIP=0: off. ---- */
static int sSkip = -1, sSkipCheck;
static unsigned sSkipN, sSkipA, sSkipB, sSkipCap, sSkipBad, sSkipFrame;

/* the frame thread, the worker of prev joined: which engines of sl can reuse prev's 2D */
static void Skip2D(Slot *sl, const Slot *prev) {
  if (sSkip < 0) {
    const char *e = getenv("BWPORT_2D_SKIP");
    sSkip = !(e != NULL && e[0] == '0');
    e = getenv("BWPORT_2D_SKIPCHECK");
    sSkipCheck = e != NULL && e[0] == '1';
    if (sSkipCheck) Log("bwport: Renderer=gpu: BWPORT_2D_SKIPCHECK: every frame drawn, the 2D skip's reuse compared\n");
  }
  long long t0 = sl->pfOn ? NowNs() : 0;
  sSkipFrame++;
  sl->reuse2d = 0;
  sl->from = prev->has && prev->mode == M_GPU && prev->ok2d ? prev->pack : NULL;
  if (sl->mode != M_GPU) {
    sSkipCap++; /* a capture frame: vcore's */
    return;
  }
  if (sl->from != NULL && sSkip) {
    vc_vram_map(sl->s); /* (Stage1 maps again after vc_reset_work: the same regions) */
    sl->reuse2d = vc_2d_same(sl->s, prev->s, VC_ENG_A) | vc_2d_same(sl->s, prev->s, VC_ENG_B) << 1;
  }
  sSkipN++;
  sSkipA += sl->reuse2d & 1;
  sSkipB += sl->reuse2d >> 1;
  if (sSkipN == 600) {
    Log("bwport: Renderer=gpu: 2D skipped A %u%%, B %u%% of the last 600 frames (and %u capture frames)%s\n", sSkipA / 6, sSkipB / 6,
        sSkipCap, sSkipCheck ? (sSkipBad ? " (SKIPCHECK: mismatches, see above)" : " (SKIPCHECK: 0 mismatches)") : "");
    sSkipN = sSkipA = sSkipB = sSkipCap = 0;
  }
  sl->pfCmp = t0 != 0 ? (double)(NowNs() - t0) / 1e6 : 0.0;
}

/* stage 1's 2D (a worker): engine B's lines, engine A's layers and the pack, each engine's half reused when Skip2D found it unchanged */
static void Build2D(Slot *sl) {
  VCoreState *s = sl->s;
  int reuse = sl->reuse2d;
  if (reuse == 3 && !sSkipCheck) {
    sl->pack = (G07Pack *)sl->from;
  } else {
    sl->pack = sl->from == sPack[0] ? sPack[1] : sPack[0];
    int r = sSkipCheck ? 0 : reuse;
    if (!(r & 2))
      for (int y = 0; y < VC_H; y++) vc_render_line(s, VC_ENG_B, y);
    if (!(r & 1)) g07_stage_build(s, sl->stage);
    g07_pack_reuse(sl->pack, s, sl->stage, sl->from, r);
    int d = sSkipCheck && reuse ? g07_pack_differs(sl->pack, sl->from, s, reuse) : 0;
    if (d) {
      sSkipBad++;
      Log("bwport: Renderer=gpu: 2D SKIPCHECK mismatch, frame %u (drawn), engine %s: DISPCNT A %08x B %08x\n", sSkipFrame,
          d == 3 ? "A and B" : d == 1 ? "A" : "B", (unsigned)vc_eng32(s, VC_ENG_A, VC_REG_DISPCNT), (unsigned)vc_eng32(s, VC_ENG_B, VC_REG_DISPCNT));
    }
  }
  sl->ok2d = 1;
}

/* a slot's geometry again after IpGather's record, for a mid or the keyframe's own picture: replayed from the record's tape
 * (vc_geo_replay: the vertices as recorded, blended and assembled again; the matrices, lighting and texcoords are not redone).
 * BWPORT_IP_REPLAYCHECK=1 also runs the whole stream again (gpu_geometry_again, the way before) and logs any difference in
 * s->geo; BWPORT_IP_REPLAY=0: the whole stream every time. */
static int sReplay = -1, sReplayCheck;
static unsigned sReplayN, sReplayBad;
static unsigned long long sReplayLists, sReplayCopied;
static VC_GeomOut *sReplayRef;
static int GeometryAgain(VCoreState *s) {
  if (sReplay < 0) {
    const char *e = getenv("BWPORT_IP_REPLAY");
    sReplay = !(e != NULL && e[0] == '0');
    e = getenv("BWPORT_IP_REPLAYCHECK");
    sReplayCheck = sReplay && e != NULL && e[0] == '1';
    if (sReplayCheck) sReplayRef = (VC_GeomOut *)malloc(sizeof *sReplayRef);
    if (sReplayCheck) Log("bwport: Renderer=gpu: BWPORT_IP_REPLAYCHECK: each in-between's geometry also from the whole stream, compared\n");
  }
  if (!sReplay) return gpu_geometry_again(s);
  if (sReplayCheck && sReplayRef != NULL) {
    gpu_geometry_again(s);
    memcpy(sReplayRef, &s->geo, sizeof *sReplayRef);
  }
  int r = vc_geo_replay(s);
  if (r < 0) return gpu_geometry_again(s);
  if (sReplayCheck && sReplayRef != NULL) {
    sReplayN++;
    sReplayLists += s->p23.tape.nl;
    sReplayCopied += s->p23.tape.copied;
    if (memcmp(sReplayRef, &s->geo, sizeof *sReplayRef) != 0) {
      sReplayBad++;
      Log("bwport: Renderer=gpu: IP_REPLAYCHECK mismatch: polygons %u / %u, vertices %u / %u (whole stream / replay)\n",
          (unsigned)sReplayRef->npoly, (unsigned)s->geo.npoly, (unsigned)sReplayRef->nvtx, (unsigned)s->geo.nvtx);
    }
    if (sReplayN % 600 == 0)
      Log("bwport: Renderer=gpu: IP_REPLAYCHECK: %u geometries compared, %u differ; lists copied %llu of %llu\n", sReplayN, sReplayBad,
          sReplayCopied, sReplayLists);
  }
  return r;
}

/* stage 1 with in-between pictures (a worker): the 2D first (engine B's lines, engine A's layers: the mids reuse them, only the
 * 3D is redone), then for each present slot after the first the 3D's geometry with its vertices blended toward the previous
 * keyframe (IpGather's pairing) and its span records, and last the picture the next tick's first slot shows (this slot's own
 * prep, which Stage2 draws) */
static void Stage1Interp(Slot *sl) {
  VCoreState *s = sl->s;
  long long p0 = sl->pfOn ? NowNs() : 0; /* the readout's parts of the keyframe (libntr-ios-passes.patch; the mids' are not in them) */
  vc_vram_map(s);
  Build2D(sl); /* before the first mid is settled: the mids read the pack */
  gpu_texdata_build(sl->td, s), sl->tdOk = 1;
  long long tA = NowNs();
  if (p0 != 0) sl->pf2d = (double)(tA - p0) / 1e6 + sl->pfCmp;
  long long gap = IpGather(s, sl->ipSerial, sl->ipNominal, sl->ipTick);
  for (int idx = 1; idx <= sl->ipN; idx++) {
    long long off = idx < sl->ipN ? sl->ipOff[idx] : sl->ipNext;
    double f = sIp.valid ? (double)(sl->ipNominal + off - sIp.keyNs) / (double)gap : 2.0;
    int blend = f > 0.0 && f < 1.0;
    IpMetric(idx, f, blend);
    long long t0 = NowNs();
    if (blend) {
      s32 b = (s32)((1.0 - f) * 4096.0 + 0.5);
      vc_ip_apply_begin(s, &sIp.old, sIp.map, sIp.texhold, b < 0 ? 0 : b > 4096 ? 4096 : b);
    }
    if (idx < sl->ipN) {
      IpMid *m = &sl->mid[idx];
      if (!blend) {
        MidSettle(m, 2);
        continue;
      }
      int ok = GeometryAgain(s);
      vc_ip_apply_end(s);
      if (ok) PrepareInto(sl, &m->prep);
      if (ok && m->prep != NULL) {
        MidSettle(m, 1);
        IC(mids, 1);
        IC(midNs, NowNs() - t0);
      } else {
        MidSettle(m, 2);
      }
    } else {
      long long g0 = sl->pfOn ? NowNs() : 0;
      sl->geo = GeometryAgain(s);
      if (blend) vc_ip_apply_end(s);
      long long g1 = g0 != 0 ? NowNs() : 0;
      if (sl->geo) PrepareInto(sl, &sl->prep), sl->prepared = sl->prep != NULL;
      if (g0 != 0) sl->pfGeo = (double)(g1 - g0) / 1e6, sl->pfSpans = (double)(NowNs() - g1) / 1e6;
      if (blend) IC(nextMid, 1), IC(midNs, NowNs() - t0);
      else IC(nextPlain, 1), IC(plainNs, NowNs() - t0);
    }
  }
  IC(wholeNs, NowNs() - tA);
}

/* stage 1 (a worker): vcore alone for a capture frame; else the geometry and engine B */
static void Stage1(Slot *sl) {
  VCoreState *s = sl->s;
  long long t0 = sl->pfOn ? NowNs() : 0;
  sl->pfGeo = sl->pfSpans = sl->pf2d = 0.0;
  if (sl->mode == M_VCORE) {
    vcore_render(s);
    vcore_frame_rgba(s, sl->rgba);
    return;
  }
  vc_reset_work(s);
  sl->uses3d = (vc_eng32(s, VC_ENG_A, VC_REG_DISPCNT) & 8) != 0;
  sl->prepared = 0;
  if (sl->ipN > 0 && sl->uses3d) {
    Stage1Interp(sl);
    return;
  }
  if (sl->uses3d) sl->geo = gpu_geometry(s);
  else vc_vram_map(s), sl->geo = 0;
  long long t1 = t0 != 0 ? NowNs() : 0;
  if (sl->geo) { /* the spans with the geometry, on this thread: the GL half then only uploads them */
    if (sl->prep != NULL && gpu_prep_scale(sl->prep) != sScale) gpu_prep_destroy(sl->prep), sl->prep = NULL;
    if (sl->prep == NULL) sl->prep = gpu_prep_create(sScale);
    if (sl->prep != NULL) gpu_prep_set_options(sl->prep, &sl->opt), gpu_prepare(sl->prep, s), sl->prepared = 1;
  }
  long long t2 = t0 != 0 ? NowNs() : 0;
  Build2D(sl);
  if (sl->geo) gpu_texdata_build(sl->td, s), sl->tdOk = 1;
  if (t0 != 0) {
    sl->pfGeo = (double)(t1 - t0) / 1e6;
    sl->pfSpans = (double)(t2 - t1) / 1e6;
    sl->pf2d = (double)(NowNs() - t2) / 1e6 + sl->pfCmp;
  }
}

/* stage 1 timed for the readout (its parts: Stage1) */
static void Stage1Timed(Slot *sl) {
  long long t0 = sl->pfOn ? NowNs() : 0;
  Stage1(sl);
  sl->pfWhole = t0 != 0 ? (double)(NowNs() - t0) / 1e6 : 0.0;
}

/* a slot's stage 1 to libntr (the frame thread, the worker joined) */
static void PerfReport1(const Slot *sl) {
  if (sl->pfOn && sl->mode == M_GPU && sl->geo && SIM_PassTiming()) SIM_PassCpu(sl->pfGeo, sl->pfSpans, sl->pf2d), SIM_Pass2DSkip(sl->reuse2d);
  if (!sl->pfOn || !SIM_PerfOn()) return;
  SIM_PerfStage("stage 1 (whole)", 2, sl->pfWhole);
  if (sl->pfGeo > 0.0) SIM_PerfStage("3D geometry", 2, sl->pfGeo);
  if (sl->pfSpans > 0.0) SIM_PerfStage("spans + bins", 2, sl->pfSpans);
  if (sl->pf2d > 0.0) SIM_PerfStage("2D (B lines, A layers)", 2, sl->pf2d);
}

static void *Worker(void *arg) {
  Stage1Timed((Slot *)arg);
  return NULL;
}

/* ---- the readout's GPU times (libntr-pf82.patch): each pass of port/gpu (gpu_set_pass_hook) bracketed by a GL_TIME_ELAPSED
 * query while the readout is on; a frame's queries (its passes and its in-between pictures') are read TQ_LAG frames later,
 * one not done by then is dropped: never waited for. Only where libntr found timer queries (GL 3.3, not GLES). ---- */
#define TQ_LAG 5
#define TQ_MAX 32
#ifndef GL_TIME_ELAPSED
#define GL_TIME_ELAPSED 0x88BF
#endif
typedef void (*TqGetU64)(GLuint id, GLenum pname, uint64_t *params);
static struct {
  PFNGLGENQUERIESPROC GenQueries;
  PFNGLDELETEQUERIESPROC DeleteQueries;
  PFNGLBEGINQUERYPROC BeginQuery;
  PFNGLENDQUERYPROC EndQuery;
  PFNGLGETQUERYOBJECTUIVPROC GetQueryObjectuiv;
  TqGetU64 GetQueryObjectui64v;
  int ok; /* 0 not tried, 1 usable, -1 not */
  GLuint q[TQ_LAG][TQ_MAX];
  signed char pass[TQ_LAG][TQ_MAX];
  int n[TQ_LAG];
  unsigned frame;
  int active;
} sTq;
static const char *const kPassName[GPU_PASS_N] = {"uploads", "R: 3D raster", "E: edge map", "P: 3D post + resolve",
                                                  "C: compose"};

static void TqHook(int pass, int begin) {
  int f = (int)(sTq.frame % TQ_LAG);
  if (begin) {
    if (sTq.active || sTq.n[f] >= TQ_MAX) return;
    GLuint *q = &sTq.q[f][sTq.n[f]];
    if (*q == 0) sTq.GenQueries(1, q);
    sTq.BeginQuery(GL_TIME_ELAPSED, *q);
    sTq.pass[f][sTq.n[f]++] = (signed char)pass;
    sTq.active = 1;
  } else if (sTq.active) {
    sTq.EndQuery(GL_TIME_ELAPSED);
    sTq.active = 0;
  }
}

/* once a GpuFrame (the GL context current): the frame TQ_LAG back read, its slot reused; the hook on while the readout is */
static void TqFrame(void) {
  int on = SIM_PerfOn() && SIM_PerfGpuTimers();
  if (on && sTq.ok == 0) {
    sTq.GenQueries = (PFNGLGENQUERIESPROC)SIM_GLGetProc("glGenQueries");
    sTq.DeleteQueries = (PFNGLDELETEQUERIESPROC)SIM_GLGetProc("glDeleteQueries");
    sTq.BeginQuery = (PFNGLBEGINQUERYPROC)SIM_GLGetProc("glBeginQuery");
    sTq.EndQuery = (PFNGLENDQUERYPROC)SIM_GLGetProc("glEndQuery");
    sTq.GetQueryObjectuiv = (PFNGLGETQUERYOBJECTUIVPROC)SIM_GLGetProc("glGetQueryObjectuiv");
    sTq.GetQueryObjectui64v = (TqGetU64)SIM_GLGetProc("glGetQueryObjectui64v");
    sTq.ok = sTq.GenQueries && sTq.DeleteQueries && sTq.BeginQuery && sTq.EndQuery && sTq.GetQueryObjectuiv &&
                     sTq.GetQueryObjectui64v
                 ? 1
                 : -1;
  }
  if (sTq.ok <= 0) {
    gpu_set_pass_hook(NULL);
    return;
  }
  sTq.frame++;
  int f = (int)(sTq.frame % TQ_LAG);
  double ms[GPU_PASS_N] = {0};
  for (int i = 0; i < sTq.n[f]; i++) {
    GLuint avail = 0;
    sTq.GetQueryObjectuiv(sTq.q[f][i], GL_QUERY_RESULT_AVAILABLE, &avail);
    if (!avail) {
      sTq.DeleteQueries(1, &sTq.q[f][i]);
      sTq.q[f][i] = 0;
      continue;
    }
    uint64_t ns = 0;
    sTq.GetQueryObjectui64v(sTq.q[f][i], GL_QUERY_RESULT, &ns);
    ms[sTq.pass[f][i]] += (double)ns / 1e6;
  }
  int any = sTq.n[f] > 0;
  sTq.n[f] = 0;
  if (on && any) /* every pass (0 when it did not run: E, A), in the passes' order */
    for (int p = 0; p < GPU_PASS_N; p++) SIM_PerfStage(kPassName[p], 3, ms[p]);
  gpu_set_pass_hook(on ? TqHook : NULL);
}

/* the queries deleted (the other renderer is wanted; the GL context current) */
static void TqRelease(void) {
  gpu_set_pass_hook(NULL);
  if (sTq.ok > 0)
    for (int f = 0; f < TQ_LAG; f++) {
      for (int i = 0; i < TQ_MAX; i++)
        if (sTq.q[f][i] != 0) sTq.DeleteQueries(1, &sTq.q[f][i]), sTq.q[f][i] = 0;
      sTq.n[f] = 0;
    }
  sTq.active = 0;
}

/* engine A's 1x lines around the 3D layer now in s->f3d.out, the screens, the RGBA picture */
static void Engine1x(VCoreState *s, uint8_t *rgba) {
  for (int y = 0; y < VC_H; y++) vc_render_line(s, VC_ENG_A, y);
  for (int y = 0; y < VC_H; y++) vc_render_line(s, VC_ENG_B, y); /* (the 2D skip may have left them undrawn) */
  int a_top = (vc_io16(s, VC_REG_POWCNT1) >> 15) & 1;
  memcpy(s->screen[a_top ? VC_SCREEN_TOP : VC_SCREEN_BOTTOM], s->eng_out[VC_ENG_A], sizeof s->eng_out[0]);
  memcpy(s->screen[a_top ? VC_SCREEN_BOTTOM : VC_SCREEN_TOP], s->eng_out[VC_ENG_B], sizeof s->eng_out[0]);
  vcore_frame_rgba(s, rgba);
}

static GLuint MakeTex(int w, int h) {
  GLuint t;
  BGL(GenTextures)(1, &t);
  BGL(BindTexture)(GL_TEXTURE_2D, t);
  BGL(TexParameteri)(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  BGL(TexParameteri)(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  BGL(TexParameteri)(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  BGL(TexParameteri)(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  BGL(TexImage2D)(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  return t;
}

static void Upload1x(const uint8_t *rgba) {
  if (sTex1 == 0) sTex1 = MakeTex(VC_W, VC_H * 2);
  BGL(ActiveTexture)(GL_TEXTURE0);
  BGL(BindTexture)(GL_TEXTURE_2D, sTex1);
  BGL(PixelStorei)(GL_UNPACK_ALIGNMENT, 4);
  BGL(TexSubImage2D)(GL_TEXTURE_2D, 0, 0, 0, VC_W, VC_H * 2, GL_RGBA, GL_UNSIGNED_BYTE, rgba);
  sPic.tex = sTex1;
  sPic.w = VC_W;
  sPic.h = VC_H * 2;
}

/* ---- the GPU API's half of stage 2: OpenGL (the GL present) or Metal (the Metal present: gpu_bridge_mtl.m). The worker,
 * Gather, Stage1, the in-betweens' CPU half, the readouts and the self-check are the same for both ---- */
typedef struct Half {
  int api;                                        /* gpu.h GPU_API_GL, GPU_API_METAL */
  void (*save)(GLSave *g);                        /* the present's state around the GPU half (GL: SaveGL / RestoreGL) */
  void (*restore)(const GLSave *g);
  void (*drain)(void);                            /* errors of the program's other users of the API, before ours */
  double (*finish)(void);                         /* timing mode: what was sent is done; its GPU ms, or -1 (not known) */
  G07 *(*c_create)(int scale, char *err, size_t errlen);
  unsigned (*picture)(unsigned tex);              /* pass C's output tex as libntr's name for it, the frame's work sent */
  void (*upload1x)(const uint8_t *rgba);          /* a 1x picture from the CPU (vcore's), into sPic */
  void (*frame)(void);                            /* once a GpuFrame: the per-pass GPU readout */
  void (*release)(void);                          /* the readout's objects (the other renderer is wanted) */
  int (*oom)(void);                               /* the setup ran out of GPU memory */
  void (*info)(void);                             /* the first frame's log line */
} Half;

static void GlInfo(void) {
  /* the limits pass S's record build depends on (docs/gpu-cost.md P1: 8 draw buffers, or transform feedback's components) */
  GLint mdb = 0, mca = 0, mtf = 0;
  BGL(GetIntegerv)(0x8824 /* GL_MAX_DRAW_BUFFERS */, &mdb);
  BGL(GetIntegerv)(0x8CDF /* GL_MAX_COLOR_ATTACHMENTS */, &mca);
  BGL(GetIntegerv)(0x8C8A /* GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS */, &mtf);
  DrainGLErrors();
  Log("bwport: Renderer=gpu: %s, %s (draw buffers %d, colour attachments %d, transform feedback components %d)\n",
      (const char *)BGL(GetString)(GL_RENDERER), (const char *)BGL(GetString)(GL_VERSION), (int)mdb, (int)mca, (int)mtf);
}
static double GlFinish(void) {
  BGL(Finish)();
  return -1.0;
}
static unsigned GlPicture(unsigned tex) { return tex; }
static int GlOom(void) { return BGL(GetError)() == GL_OUT_OF_MEMORY; }
static const Half kGL = {GPU_API_GL, SaveGL, RestoreGL, DrainGLErrors, GlFinish, g07_create, GlPicture, Upload1x, TqFrame,
                         TqRelease, GlOom, GlInfo};

#ifdef BR_METAL
static int sSplit; /* the readout times passes: one command buffer a pass */
static void MtlSave(GLSave *g) { (void)g; }
static void MtlRestore(const GLSave *g) { (void)g; }
static void MtlNothing(void) {}
static unsigned MtlPicture(unsigned tex) {
  br_mtl_commit(sSplit ? GPU_PASS_C : GPU_PASS_N); /* ahead of the present's command buffer on the same queue */
  return tex != 0 ? BR_MTL_BASE + tex : 0;
}
static void MtlUpload1x(const uint8_t *rgba) {
  br_mtl_upload(rgba, VC_W, VC_H * 2);
  sPic.tex = BR_MTL_UPLOADED;
  sPic.w = VC_W;
  sPic.h = VC_H * 2;
}
static void MtlHook(int pass, int begin) {
  if (!begin && pass != GPU_PASS_C) br_mtl_commit(pass); /* (pass C's goes with the picture) */
}
/* the command buffers' GPU times finished since the last frame: the readout's per-pass rows, else the sys line's figure */
static void MtlFrame(void) {
  double ms[GPU_PASS_N + 1];
  unsigned n = br_mtl_take(ms);
  int on = SIM_PerfOn() && SIM_PerfGpuTimers() && !SIM_PassTiming(); /* (timing mode splits and waits by itself) */
  if (n > 0 && sSplit && on)
    for (int p = 0; p < GPU_PASS_N; p++) SIM_PerfStage(kPassName[p], 3, ms[p]);
  else if (n > 0) {
    double t = 0.0;
    for (int p = 0; p <= GPU_PASS_N; p++) t += ms[p];
    bwport_sys_gpu_add(t);
  }
  sSplit = on;
  gpu_set_pass_hook(on ? MtlHook : NULL);
}
static void MtlRelease(void) {
  double ms[GPU_PASS_N + 1];
  gpu_set_pass_hook(NULL);
  sSplit = 0;
  br_mtl_take(ms);
}
static int MtlOom(void) { return 0; }
static void MtlInfo(void) { Log("bwport: Renderer=gpu: Metal on %s (the Metal present's device and queue)\n", br_mtl_device_name()); }
static const Half kMetal = {GPU_API_METAL, MtlSave, MtlRestore, MtlNothing, br_mtl_finish, g07_create_metal, MtlPicture,
                            MtlUpload1x, MtlFrame, MtlRelease, MtlOom, MtlInfo};
#endif
static const Half *sHalf = &kGL;

/* pass C's 3D input and the 1x layer for the CPU fallback. Returns the 3D layer's texture for g07_compose (0: none), with
 * *sc its scale; sets *gpu when the GPU drew it, else vcore's CPU raster did (a GL error) and *tex is g07_upload_3d's */
static unsigned Layer3D(Slot *sl, int *sc) {
  VCoreState *s = sl->s;
  sHalf->drain(); /* libntr's own, not ours */
  int rc;
  gpu_use_texdata(sCore, sl->tdOk ? sl->td : NULL); /* uploaded by this frame's in-between pictures already, if it had any */
  if (sl->prepared && gpu_prep_scale(sl->prep) == sScale) rc = gpu_render_prepared(sCore, s, sl->prep);
  else rc = gpu_render_3d(sCore, s); /* no records yet (the first frame, a scale change): spans here */
  if (rc == 0) {
    *sc = sOut;
    return gpu_3d_tex(sCore);
  }
  if (++sErrors == 1 || sErrors == 3) Log("bwport: Renderer=gpu: GL error 0x%x in a frame; vcore draws its 3D\n", -rc);
  if (sErrors >= 3) sDead = 1, sDeadTicks = 0, SIM_RendererFailed("GL errors in 3 frames in a row");
  vc_3d_frame(s); /* vcore's CPU 3D into s->f3d.out */
  *sc = 1;
  return g07_upload_3d(sC7, (const uint32_t *)s->f3d.out, 1);
}

/* ---- BWPORT_GPU_CHECK=DIR: every GPU frame is also drawn by vcore from the same state, and the pictures compared ----
 * (the port_run check of docs/gpu.md G8): at 1x the screens must match; the engine B screen (the bottom one when A is
 * on top) has no 3D and must match exactly. A line for each frame whose engine A differs (the first 40), a summary every
 * 300 frames; the worst frame as PNGs in DIR (gpu.png, vcore.png), and the first 8 differing frames (fNNNNN_gpu.png,
 * fNNNNN_vcore.png). The compared GPU picture is pass C's, read back; at N > 1 its top-left pixel of each DS pixel
 * (the DS sample point: engine A differs by design where the 3D shows: the 3D layer is drawn at N x, so its edges and
 * texture samples are not vcore's; only at 1x is the check exact, and only there are the layer counts filled in). */
static const char *sCheckDir;
static VCoreState *sRef;
static uint32_t sRefGxCap;
static uint8_t sRefRgba[VC_W * VC_H * 2 * 4];
static uint8_t *sChkBuf; /* pass C's picture read back (N > 1) */
static unsigned sChkFree, sChk3d;
static unsigned sChkFrames, sChkBad, sChkAPx, sChkBPx, sChkBBad, sChkWorst;
static unsigned long long sChkSumA;

static void CheckFrame(Slot *sl, const uint8_t *rgba) {
  VCoreState *s = sl->s;
  if (sRef == NULL && (sRef = vcore_create()) == NULL) return;
  memcpy(sRef->io9, s->io9, sizeof s->io9);
  memcpy(sRef->io9b, s->io9b, sizeof s->io9b);
  memcpy(sRef->vram, s->vram, sizeof s->vram);
  memcpy(sRef->pltt, s->pltt, sizeof s->pltt);
  memcpy(sRef->oam, s->oam, sizeof s->oam);
  if (sRefGxCap < s->gx_words) {
    free(sRef->gx);
    sRef->gx = malloc((size_t)s->gx_words * 4 + 16);
    sRefGxCap = sRef->gx ? s->gx_words : 0;
    if (sRef->gx == NULL) return;
  }
  memcpy(sRef->gx, s->gx, (size_t)s->gx_words * 4);
  sRef->gx_words = s->gx_words;
  vcore_render(sRef);
  vcore_frame_rgba(sRef, sRefRgba);
  int a_top = (vc_io16(s, VC_REG_POWCNT1) >> 15) & 1;
  const size_t half = (size_t)VC_W * VC_H;
  unsigned a = 0, b = 0, a_free = 0, l3 = 0;
  /* at 1x: the 3D layers themselves (vcore's s->f3d.out of sRef, the GPU's read back) and the engine A pixels that
   * differ although the layer is equal there: those would be pass C's fault */
  static uint32_t gl3d[VC_W * VC_H];
  int have3d = sScale == 1 && sl->uses3d && sl->geo; /* Resolution 1, no antialiasing */
  if (have3d) gpu_read_3d(sCore, gl3d, NULL);
  for (size_t i = 0; i < half; i++) {
    int ldiff = 0;
    if (have3d) {
      uint32_t p = gl3d[i], q = sRef->f3d.out[i / VC_W][i % VC_W];
      ldiff = VC_A5(p) != VC_A5(q) || (VC_A5(p) && (p & 0xFFFFFF) != (q & 0xFFFFFF));
      l3 += ldiff;
    }
    if (memcmp(&rgba[((a_top ? 0 : half) + i) * 4], &sRefRgba[((a_top ? 0 : half) + i) * 4], 3)) a++, a_free += have3d && !ldiff;
    if (memcmp(&rgba[((a_top ? half : 0) + i) * 4], &sRefRgba[((a_top ? half : 0) + i) * 4], 3)) b++;
  }
  if (a && getenv("BWPORT_GPU_CHECK_PX") && sChkFrames < 3000) { /* the first differing pixels of engine A: x y gpu vcore */
    int shown = 0;
    for (size_t i = 0; i < half && shown < 12; i++) {
      const uint8_t *g = &rgba[((a_top ? 0 : half) + i) * 4], *v = &sRefRgba[((a_top ? 0 : half) + i) * 4];
      if (memcmp(g, v, 3)) shown++, Log("bwport: gpu check px: frame %u (%zu,%zu) gpu %02x%02x%02x vcore %02x%02x%02x\n", sChkFrames + 1, i % VC_W, i / VC_W, g[0], g[1], g[2], v[0], v[1], v[2]);
    }
  }
  sChkFrames++;
  sChkSumA += a;
  sChkFree += a_free;
  sChk3d += l3;
  if (a) sChkBad++;
  if (b) sChkBBad++;
  sChkBPx += b;
  if (a && sChkBad <= 40) Log("bwport: gpu check: frame %u: engine A differs in %u px (3D layer differs in %u px, A differs with an equal layer in %u)\n", sChkFrames, a, l3, a_free);
  if (b && sChkBBad <= 40) Log("bwport: gpu check: frame %u: engine B differs in %u px\n", sChkFrames, b);
  if ((a || b) && sChkBad + sChkBBad <= 8) {
    char path[1024];
    snprintf(path, sizeof path, "%s/f%05u_gpu.png", sCheckDir, sChkFrames);
    vcore_write_png(path, rgba, VC_W, VC_H * 2);
    snprintf(path, sizeof path, "%s/f%05u_vcore.png", sCheckDir, sChkFrames);
    vcore_write_png(path, sRefRgba, VC_W, VC_H * 2);
  }
  if (a + b > sChkWorst) {
    sChkWorst = a + b;
    char path[1024];
    snprintf(path, sizeof path, "%s/gpu.png", sCheckDir);
    vcore_write_png(path, rgba, VC_W, VC_H * 2);
    snprintf(path, sizeof path, "%s/vcore.png", sCheckDir);
    vcore_write_png(path, sRefRgba, VC_W, VC_H * 2);
  }
  if (sChkFrames % 300 == 0)
    Log("bwport: gpu check: %u frames, engine A differs in %u (mean %.1f px, worst frame %u px; with an equal 3D layer: %u px; 3D layer px differing: %u), engine B in %u (%u px)\n",
           sChkFrames, sChkBad, (double)sChkSumA / sChkFrames, sChkWorst, sChkFree, sChk3d, sChkBBad, sChkBPx);
}

/* the check's GPU picture at 1x: pass C's output read back, one pixel per DS pixel */
static const uint8_t *CheckPicture(Slot *sl) {
  int n = sOut;
  if (n == 1) {
    g07_read_frame(sC7, sl->rgba);
    return sl->rgba;
  }
  size_t full = (size_t)VC_W * n * VC_H * 2 * n * 4;
  if (sChkBuf == NULL && (sChkBuf = malloc(full)) == NULL) return NULL;
  g07_read_frame(sC7, sChkBuf);
  for (int y = 0; y < VC_H * 2; y++)
    for (int x = 0; x < VC_W; x++)
      memcpy(&sl->rgba[((size_t)y * VC_W + x) * 4], &sChkBuf[((size_t)(y * n) * VC_W * n + (size_t)x * n) * 4], 4);
  return sl->rgba;
}

/* pass S's self-check (docs/gpu-cost.md P1): three frames of each core also build their span records on the CPU and compare
 * them with the GPU's, field by field (gpu_spans_check: the first differences and the counts go to the log). A core whose GPU
 * records differ in an integer field builds them on the CPU from then on (a test iPhone drew the 3D in pieces with pass S
 * while the Mac, llvmpipe and the Simulator drew it right). */
static int sSpanChecks, sSpanFrames;

/* stage 2 (the calling thread, the GL context current, the GL state saved by the caller): the frame's picture in sPic */
static void Stage2(Slot *sl) {
  VCoreState *s = sl->s;
  const unsigned abl = SIM_Ablation();
  if (abl & 32) return; /* ablate=gpu: no GL/Metal half at all; the picture stays the last one drawn */
#ifdef BR_METAL
  if (sHalf == &kMetal) gpu_mtl_ablate(abl & 31); /* GPU_ABL_R .. GPU_ABL_C */
#endif
  long long tq0 = SIM_PerfOn() ? NowNs() : 0;
  sHalf->drain();
  if (sl->mode == M_VCORE) {
    sHalf->upload1x(sl->rgba);
    return;
  }
  if (sFastOk < 0 && sFastWant) { /* (the next gathered frame draws fast); the settings row says how, or why not */
    sFastOk = gpu_fast_supported(sCore);
    SIM_Renderer3DFast(sFastOk, gpu_fast_how(sCore));
  }
  gpu_set_options(sCore, &sl->opt); /* the GL half (edge, filter) of this frame as it was gathered */
  const int pass = SIM_PassTiming() && sl->uses3d && sl->geo; /* libntr-ios-passes.patch: glFinish after each pass */
  gpu_set_timing(sCore, pass);
  if (pass) sHalf->finish(); /* (the last frame's present and UI out of this frame's uploads) */
  int sc = 1;
  unsigned t3 = 0;
  if (sl->uses3d && sl->geo && sSpanChecks < 3 && ++sSpanFrames == (sSpanChecks == 0 ? 30 : sSpanChecks == 1 ? 300 : 1800)) {
    sSpanChecks++;
    Log("bwport: Renderer=gpu: pass S self-check %d (frame %d):\n", sSpanChecks, sSpanFrames);
    int n = gpu_spans_check(sCore, s, stderr); /* (draws the frame once more on the way; Layer3D draws it again below) */
    if (n > 0) {
      Log("bwport: Renderer=gpu: pass S's records are wrong on this GPU (%d integer fields differ): the CPU builds them from now on\n", n);
      gpu_pass_s_off(sCore);
      sSpanChecks = 3;
    }
    sHalf->drain();
  }
  if (sl->uses3d && sl->geo) t3 = Layer3D(sl, &sc);
  long long tq1 = tq0 != 0 ? NowNs() : 0;
  long long tc0 = pass ? NowNs() : 0;
  if (sDead) { /* three GL errors: this frame is vcore's too */
    Engine1x(s, sl->rgba);
    sHalf->upload1x(sl->rgba);
    return;
  }
  int rc = g07_compose_packed(sC7, s, sl->pack, t3, sc, sc == sOut ? gpu_smaa_weights(sCore) : 0);
  if (pass) {
    double cg = sHalf->finish(); /* (Metal: pass C's command buffer's GPU time; GL: the wall time with glFinish) */
    double up = 0.0, r = 0.0, ep = 0.0;
    gpu_last_passes(sCore, &up, &r, &ep);
    gpu_set_timing(sCore, 0);
#ifdef BR_METAL
    if (sHalf == &kMetal) { /* timing mode: a command buffer a pass, each waited for by the core (S, R, E, P, SMAA) */
      double ps = gpu_last_pass_s(sCore), pe = gpu_last_pass_e(sCore);
      double part[5] = {ps, r - ps, pe, ep - pe, gpu_last_smaa_ms(sCore)};
      for (int i = 0; i < 5; i++) br_mtl_counted(part[i]);
    }
#endif
    if (rc == 0) SIM_PassGpu(up, r, ep, gpu_last_smaa_ms(sCore), cg >= 0.0 ? cg : (double)(NowNs() - tc0) / 1e6);
    if (rc == 0) SIM_PassGpuE(gpu_last_pass_e(sCore));
  }
  if (tq0 != 0) {
    SIM_PerfStage("GL half: 3D (uploads, R, E, P)", 1, (double)(tq1 - tq0) / 1e6);
    SIM_PerfStage("GL half: compose (upload, C)", 1, (double)(NowNs() - tq1) / 1e6);
  }
  if (rc == 0 && sl->geo) sErrors = 0;
  if (rc != 0) {
    if (++sErrors == 1 || sErrors == 3) Log("bwport: Renderer=gpu: GL error 0x%x in pass C; vcore draws the frame\n", -rc);
    if (sErrors >= 3) sDead = 1, sDeadTicks = 0, SIM_RendererFailed("GL errors in 3 frames in a row");
    vc_3d_frame(s);
    Engine1x(s, sl->rgba);
    sHalf->upload1x(sl->rgba);
    return;
  }
  sPic.tex = sHalf->picture(g07_frame_tex(sC7));
  sPic.w = VC_W * sOut;
  sPic.h = VC_H * 2 * sOut;
  if (sCheckDir != NULL) {
    const uint8_t *pic = CheckPicture(sl);
    if (pic != NULL) CheckFrame(sl, pic);
  }
}

/* this thread's CPU time (BWPORT_INTERP_REPORT's GL thread line) */
static long long ThreadCpuNs(void) {
  struct timespec c;
  clock_gettime(CLOCK_THREAD_CPUTIME_ID, &c);
  return (long long)c.tv_sec * 1000000000ll + c.tv_nsec;
}

/* libntr's cap loop (libntr-ib79.patch), on the GL thread right after the slot before was presented: the in-between picture of
 * present slot `slot` of the tick this call launched. The worker prepared its span records (Stage1Interp); the GL half
 * (the passes, then pass C with this frame's 2D layers) runs here and the GPU works while libntr sleeps for the slot.
 * Returns the picture's texture, or 0 (not ready in time, nothing to show: the slot shows the picture before again). */
static unsigned InterpTexture(int slot, int *w, int *h, long long waitNs) {
  Slot *sl = sIpTick;
  if (sl == NULL || slot < 1 || slot >= sl->ipN || sCore == NULL || sC7 == NULL || sDead || (SIM_Ablation() & 32)) return 0;
  IpMid *m = &sl->mid[slot];
  {
    static long long sW0, sC0;
    long long cpu = ThreadCpuNs();
    if (sW0 == 0) sW0 = NowNs(), sC0 = cpu;
    sIc.glWallNs = (unsigned long long)(NowNs() - sW0);
    sIc.glCpuNs = (unsigned long long)(cpu - sC0);
  }
  int st = __atomic_load_n(&m->state, __ATOMIC_ACQUIRE);
  if (st == 0) { /* the worker is still on it: sleep until it settles the mid (MidSettle) or the slot less a margin for the GL half */
    long long tw = NowNs(), cw = ThreadCpuNs();
    long long end = tw + (waitNs > 1500000 ? waitNs - 1500000 : 0);
    pthread_mutex_lock(&sMidM);
    for (long long now = tw; (st = __atomic_load_n(&m->state, __ATOMIC_ACQUIRE)) == 0 && now < end; now = NowNs()) {
      struct timespec ts; /* pthread_cond_timedwait's clock is CLOCK_REALTIME */
      clock_gettime(CLOCK_REALTIME, &ts);
      long long at = (long long)ts.tv_nsec + (end - now);
      ts.tv_sec += (time_t)(at / 1000000000ll);
      ts.tv_nsec = (long)(at % 1000000000ll);
      pthread_cond_timedwait(&sMidC, &sMidM, &ts);
    }
    pthread_mutex_unlock(&sMidM);
    long long waited = NowNs() - tw;
    IC(waits, 1);
    IC(waitNs, waited);
    IC(waitCpuNs, ThreadCpuNs() - cw);
    if ((unsigned long long)waited > sIc.waitMax) sIc.waitMax = (unsigned long long)waited; /* this thread alone writes it */
  }
  if (st != 1) {
    if (st == 0) { IC(midsLate, 1); SIM_InterpLate(); }
    else IC(midsNone, 1);
    return 0;
  }
  long long t0 = NowNs();
  GLSave gs;
  sHalf->save(&gs);
  sHalf->drain();
  gpu_set_options(sCore, &sl->opt);
  unsigned tex = 0;
  gpu_use_texdata(sCore, sl->tdOk ? sl->td : NULL); /* the keyframe's VRAM: uploaded once, by the first mid */
  if (m->prep != NULL && gpu_prep_scale(m->prep) == sScale && gpu_render_prepared(sCore, sl->s, m->prep) == 0 &&
      g07_compose_packed(sC7, sl->s, sl->pack, gpu_3d_tex(sCore), sOut, gpu_smaa_weights(sCore)) == 0) {
    tex = sHalf->picture(g07_frame_tex(sC7));
    *w = VC_W * sOut;
    *h = VC_H * 2 * sOut;
    sPic.tex = tex; /* the picture a skipped frame shows again */
    sPic.w = *w;
    sPic.h = *h;
    IC(midsShown, 1);
    IC(glNs, NowNs() - t0);
  } else {
    IC(midsNone, 1);
  }
  sHalf->restore(&gs);
  return tex;
}

static void Destroy(void) {
  if (sC7) g07_destroy(sC7);
  sC7 = NULL;
  if (sCore) gpu_destroy(sCore);
  sCore = NULL;
  if (sTex1) BGL(DeleteTextures)(1, &sTex1);
  sTex1 = 0;
  sPic.tex = 0; /* it was the core's (or sTex1): libntr must not be handed a deleted texture */
  sPic.w = sPic.h = 0;
  free(sChkBuf);
  sChkBuf = NULL;
}

/* everything the GPU path holds, for a new scale and for a renderer that is no longer wanted: the worker is joined, the
 * GL objects deleted, the slots' states and span records freed (they were per scale: each Resolution change used to
 * leave the old ones until a frame came by that replaced them). The next frame starts afresh, as the first. */
static void ReleaseAll(void) {
  if (sInflight) {
    vcore_task_join(&sThread);
    sInflight = 0;
  }
  Destroy();
  for (int i = 0; i < 2; i++) {
    Slot *sl = &sSlot[i];
    gpu_prep_destroy(sl->prep);
    sl->prep = NULL;
    for (int j = 0; j < IP_MAXSLOT; j++) {
      gpu_prep_destroy(sl->mid[j].prep);
      sl->mid[j].prep = NULL;
    }
    sl->ipN = 0;
    vcore_destroy(sl->s);
    sl->s = NULL;
    sl->gxcap = 0;
    free(sl->stage);
    sl->stage = NULL;
    gpu_texdata_destroy(sl->td);
    sl->td = NULL;
    sl->pack = NULL;
    sl->from = NULL;
    sl->tdOk = sl->ok2d = 0;
    sl->has = sl->done2 = sl->prepared = sl->geo = sl->uses3d = 0;
  }
  for (int i = 0; i < 2; i++) g07_pack_destroy(sPack[i]), sPack[i] = NULL;
  sCur = 0;
  sScale = sOut = sAA = 0;
  sIpTick = NULL;
  sIp.have = sIp.haveOld = sIp.valid = 0;
}

/* libntr: the other renderer is wanted now (the GL context is current) */
static void BridgeIdle(void) {
  if (sHalf->api == GPU_API_GL ? BGL(GetError) != NULL : sInit) ReleaseAll(), sHalf->release();
}

static int Setup(int out, int aa) {
  char err[2048];
  sSpanChecks = sSpanFrames = 0;
  int scale = out * (aa == 2 ? 2 : 1);
  ReleaseAll();
  sCore = gpu_create(GetProc, sHalf->api, out, aa, err, sizeof err);
  if (sCore == NULL) {
    Log("bwport: Renderer=gpu: %s; vcore draws\n", err);
    SIM_RendererFailed(err);
    return -1;
  }
  sC7 = sHalf->c_create(out, err, sizeof err);
  if (sC7 == NULL) {
    Log("bwport: Renderer=gpu: pass C: %s; vcore draws\n", err);
    SIM_RendererFailed(err);
    Destroy();
    return -1;
  }
  if (sHalf->oom()) { /* the allocations above: an integrated GPU shares the system's memory */
    snprintf(err, sizeof err, "out of memory for %dx", scale);
    Log("bwport: Renderer=gpu: %s; vcore draws\n", err);
    SIM_RendererFailed(err);
    ReleaseAll();
    return -1;
  }
  sScale = scale;
  sOut = out;
  sAA = aa;
  sFastOk = -1;
  return 0;
}

/* the first call: the functions by name, the environment */
static int Init(void) {
  char err[2048];
#ifdef BR_METAL
  if (SIM_PresentMetal()) { /* the Metal present: no GL; the core on the present's device and queue */
    sHalf = &kMetal;
    br_mtl_use_present();
  } else
#endif
  if (LoadGL(err, sizeof err) != 0) {
    Log("bwport: Renderer=gpu: %s; vcore draws\n", err);
    SIM_RendererFailed(err);
    return -1;
  }
  const char *e = getenv("BWPORT_VCORE_PIPE");
  sPipe = !(e != NULL && e[0] == '0');
  if (getenv("BWPORT_LSTEP") != NULL) sPipe = 0;
  sCheckDir = getenv("BWPORT_GPU_CHECK");
  e = getenv("BWPORT_VCORE_THREADS");
  if (e != NULL && e[0] == '0') vc_threads = 0;
  return 0;
}

/* the hook: libntr calls it for each frame with its GL context current; the texture it returns holds the picture */
static unsigned GpuFrame(int *w, int *h) {
  /* the renderer or the resolution was changed (the Config window, the file): a failure state belongs to the old
   * setting, try again (libntr-onerender.patch) */
  static unsigned sSerial;
  unsigned serial = SIM_RendererSerial();
  if (serial != sSerial) {
    sSerial = serial;
    sDead = 0;
    sErrors = 0;
    sFatal = 0;
    if (sCore == NULL) sInit = 0;
  }
  sIpTick = NULL; /* no in-between pictures for this call until it is launched with them */
  if (sFatal) return 0;
  if (sDead) {
    if (++sDeadTicks < 300) return 0; /* vcore draws; the GPU path is tried again in 5 s */
    sDead = 0;
    sErrors = 0;
    Log("bwport: Renderer=gpu: trying the GPU path again\n");
  }
  if (!sInit) {
    sInit = 1;
    if (Init() != 0) {
      sFatal = 1;
      return 0;
    }
  }
  sHalf->frame();
  /* frame skip (libntr-frameskip.patch): nothing gathered, nothing drawn, libntr shows the last picture again (the
   * worker still running is joined by the next drawn call); never before there is a picture. A worker that has finished
   * is joined here and its frame's GL half run, so the skipped ticks show it rather than the frame before (vcore_bridge.c) */
  if (sPic.tex != 0 && sCore != NULL && SIM_FrameSkipping()) {
    Slot *done = &sSlot[sCur ^ 1];
    if (sInflight && vcore_task_done(&sThread)) {
      vcore_task_join(&sThread);
      sInflight = 0;
      PerfReport1(done);
      if (!done->done2) {
        GLSave gs;
        sHalf->save(&gs);
        Stage2(done);
        done->done2 = 1;
        sHalf->restore(&gs);
      }
    }
    *w = sPic.w;
    *h = sPic.h;
    return sPic.tex;
  }
  GLSave gs;
  sHalf->save(&gs);
  sHalf->drain();
  if (!sLogged) {
    sLogged = 1;
    sHalf->info();
  }
  int scale = SIM_RenderScale();
  scale = scale < 1 ? 1 : scale > GPU_MAX_SCALE ? GPU_MAX_SCALE : scale;
  int aa = SIM_AntialiasWanted() >= 2 ? 2 : 1; /* k x k samples per pixel at any Resolution: the targets stay at N */
  if (SIM_AntialiasWanted() == 3) aa = GPU_AA_SMAA; /* libntr-ios-smaa.patch: one sample, SMAA at N > 1 (port/gpu g08_smaa.c) */
  SIM_RendererAAUsed(aa == GPU_AA_SMAA && scale == 1 ? 1 : aa);
  if (sCore == NULL || scale != sOut || aa != sAA) {
    if (Setup(scale, aa) != 0) { /* not again until the setting changes (a retry every 5 s compiled the shaders, made the
                                  * targets, failed the same way and flipped the picture between the renderers) */
      sFatal = 1;
      sHalf->restore(&gs);
      return 0;
    }
    if (aa == GPU_AA_SMAA && scale > 1 && gpu_smaa_weights(sCore) == 0) Log("bwport: Renderer=gpu: SMAA could not be set up; drawn without\n");
  }
  if (aa == GPU_AA_SMAA && gpu_smaa_weights(sCore) == 0) SIM_RendererAAUsed(1); /* the status line: "SMAA not drawn" */
  long long tA = SIM_PerfOn() ? NowNs() : 0;
  Slot *sl = &sSlot[sCur], *prev = &sSlot[sCur ^ 1];
  /* this slot's own copy: the previous frame's worker may still run on prev's copy */
  sl->opt = (GpuOptions){SIM_GLRendererOption(0), SIM_GLRendererOption(1), SIM_GLRendererOption(2), SIM_GLRendererOption(3), sAA};
  sFastWant = SIM_GLRendererOption(4);
  sl->opt.fast = sFastWant && sFastOk > 0;
  if (Gather(sl) != 0) {
    sHalf->restore(&gs);
    return 0;
  }
  sl->has = 1;
  sl->done2 = 0;
  sl->mode = (sl->cap & 0x80000000u) ? M_VCORE : M_GPU;
  sl->pfOn = tA != 0 || SIM_PassTiming(); /* (libntr-ios-passes.patch: the worker times its parts for the row too) */
  long long tB = tA != 0 ? NowNs() : 0;
  if (sInflight) {
    vcore_task_join(&sThread);
    sInflight = 0;
    PerfReport1(prev);
  }
  if (tA != 0) {
    SIM_PerfStage("gather", 1, (double)(tB - tA) / 1e6);
    SIM_PerfStage("wait for the worker", 1, (double)(NowNs() - tB) / 1e6);
  }
  Skip2D(sl, prev); /* (prev's stage 1 is done: its state and pack stay as they are until the next call's Gather) */
  int first = !prev->has; /* nothing earlier to show: this frame is drawn now (and shown twice) */
  sl->ipN = 0;
  if (!first && sPipe && sl->mode == M_GPU && SIM_InterpWanted() && (vc_eng32(sl->s, VC_ENG_A, VC_REG_DISPCNT) & 8)) {
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
  if (!first && sPipe && sl->mode == M_GPU) {
    sInflight = vcore_task_start(&sThread, Worker, sl);
    if (!sInflight) Stage1Timed(sl), PerfReport1(sl);
    /* the previous frame's GL half runs while the worker does this frame's CPU half */
    if (!prev->done2) Stage2(prev), prev->done2 = 1;
  } else {
    if (!first && !prev->done2) Stage2(prev), prev->done2 = 1;
    long long t1 = tA != 0 ? NowNs() : 0;
    Stage1Timed(sl);
    if (t1 != 0) SIM_PerfStage("stage 1 (in the call)", 1, (double)(NowNs() - t1) / 1e6);
    PerfReport1(sl);
    if (sl->mode == M_VCORE) StoreCapture(sl->s, sl->cap);
    if (first) Stage2(sl), sl->done2 = 1;
  }
  sCur ^= 1;
  {
    static unsigned n;
    if ((++n % 120) == 0) IpReport();
  }
  sHalf->restore(&gs);
  *w = sPic.w;
  *h = sPic.h;
  return sPic.tex;
}

__attribute__((constructor)) static void BridgeInstall(void) {
  SIM_SetRendererIdle(2, BridgeIdle);
  SIM_SetFrameRendererGL(GpuFrame);
  SIM_SetInterpTexture(InterpTexture);
#ifdef BR_METAL
  SIM_SetFrameTextureMetal(br_mtl_tex_of); /* the Metal present draws a picture by these names */
#endif
  atexit(IpReport);
}
