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
#include "g07_capture.h"
#include "g07_compose.h"
#include "gpu.h"
#include "vc_shared.h"
#include "diag_rec.h"

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
int SIM_GpuPassesWanted(void); /* sim_main.cpp: the GPU passes' times wanted (BWPORT_GPUPASSES, cfgscript gpupasses=, Detailed metrics) */
unsigned SIM_RendererSerial(void); /* changes with the renderer or the resolution: start afresh */
void SIM_SetRendererIdle(int which, void (*fn)(void)); /* libntr-switchcheck.patch: fn runs when the other renderer is wanted */
void SIM_SetGLLostHook(void (*fn)(void)); /* glr2: fn runs when the GL context was lost and a new one is current */
void SIM_RendererFailed(const char *reason); /* the status line says why the GPU path is not drawing */
void SIM_VCapSwapInfo(uint32_t *swaps, uint32_t *swapFrame, uint32_t *frame); /* libntr-diagrec.patch: the 3D swaps, for diag_rec.c */
int SIM_FrameSkipping(void); /* libntr-frameskip.patch: this frame is not to be drawn */
int SIM_AntialiasWanted(void); /* libntr-aa70.patch, libntr-ios-msaa.patch: Antialiasing off|4 samples as k = 1, 2; 3 SMAA (libntr-ios-smaa.patch); 4 Smooth edges */
int SIM_AntialiasLite(void); /* 1: 4 samples (lite), the DS's own antialiasing left out under the samples (GpuOptions.aalite) */
int SIM_PerfExperiments(void); /* sim_config.ini PerfExperiments: GpuOptions.exp (GPU_EXP_*) */
void SIM_RendererAAUsed(int k); /* the k the GPU path draws with (k x k samples per pixel; never lowered) */
void SIM_Renderer3DFast(int ok, const char *how); /* libntr-ios-fastgl.patch: Renderer3D=fast drawn (1) or not (0), how or why */
int SIM_GLRendererOption(int which); /* libntr-gpuopt.patch: 0 dot, 1 line, 2 edge, 3 filter (bilinear); 4 the 3D renderer fast; 5 the 2D filter (0 off, 1 Scale2x, 2 MMPX), 6 texture smoothing (0 off, 1 bilinear, 2 mips), 7 fog, 8 colour (the ini keys) */
uint32_t SIM_VCapGather(uint8_t *io9, uint8_t *io9b, uint8_t *vram, uint8_t *pltt, uint8_t *oam, uint32_t **gx,
                        uint32_t *cap);
uint8_t *SIM_VCapBankHome(int bank);
/* libntr-hdma.patch: channel ch's H-blank DMA into the 2D registers, line by line (sim_vcapture.cpp) */
int SIM_VCapHDma(int ch, int *eng, unsigned *ofs, unsigned *len, uint8_t *rows, unsigned stride);

/* the frame's H-blank DMAs into s (after SIM_VCapGather: line 0 is the registers as gathered); vcore_bridge.c has the same */
static void GatherHDma(VCoreState *s) {
  uint8_t rows[VC_H * VC_IO9B_SIZE];
  vcore_hdma_clear(s);
  for (int ch = 0; ch < 4; ch++) {
    int e;
    unsigned ofs, len;
    if (SIM_VCapHDma(ch, &e, &ofs, &len, rows, VC_IO9B_SIZE)) vcore_hdma_add(s, e, ofs, len, rows, VC_IO9B_SIZE);
  }
}
/* libntr-ib78.patch / libntr-ib79.patch: in-between pictures (docs/interp.md) */
int SIM_InterpWanted(void);
int SIM_InterpPace(long long *serial, long long *nominal, long long *tick, int max, long long *off, long long *next);
int SIM_InterpLeadOn(void); /* 0: BWPORT_INTERPLEAD=0, a blend at f 0 shows the newer keyframe as before the lead */
void SIM_InterpLate(void); /* libntr-ios-smoothdrop.patch: this slot's in-between was not ready in time */
void SIM_InterpShows(long long gameNs); /* the game time the picture about to be presented shows (BWPORT_PRESENTLOG's P lines) */
long long SIM_TickNominal(void);       /* the tick's nominal time on the present timeline (0: none) */
int SIM_PresentLogOn(void);            /* BWPORT_PRESENTLOG is open: the W, I and G lines below (the CPU halves' times) */
void SIM_PresentLogNote(const char *line);
void SIM_SetInterpTexture(unsigned (*fn)(int slot, int *w, int *h, long long waitNs));
void SIM_SetNextTexture(unsigned (*fn)(int *w, int *h)); /* the next tick's first picture, drawn after this tick's last present */
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
void SIM_SetFrameTextureGen(unsigned long long (*fn)(unsigned name)); /* the present's skip (sim_main.cpp SpKeep) */
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
  unsigned id3d; /* its 3D inputs' number (R3Id; set before it is settled) */
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
  int capHi;       /* M_GPU: the frame captures and the capture is drawn at N x too (CapHiStage2); its 1x bytes came from vcore
                      on sCapRef (CapHi) */
  unsigned capSer; /* capHi: that capture's number (sCapCount) */
  unsigned rowSer[4][256]; /* banks A-D: the capture number each bank row holds as gathered (0: none, or the CPU changed its
                              bytes since): Gather's compare with sCapShadow */
  int cbg;         /* M_GPU at N x: engine A's BG (2 or 3) that shows a captured bank as it is (CapBgFind, on the worker), else 0 */
  int cbgBank, cbgOx, cbgOy; /* that bank; DS pixel (x, y) of the BG is its texel (x + cbgOx) & 255 of bank row y + cbgOy */
  uint32_t cbgLines[6];      /* the lines that show it so (bit y) */
  unsigned fno;    /* the frame's number (CapLog's count of gathered frames) */
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
  long long ipGame[IP_MAXSLOT + 1], gameNs; /* the game time each picture shows (1..ipN; the keyframe's own: gameNs) */
  IpMid mid[IP_MAXSLOT];
  /* G12, the texture cache (opt.texcache; Exact and Fast): this frame's layers as gpu_texcache_frame gave them (on the worker, after
   * td), drawn by the frame and its in-between pictures (gpu_use_tcframe before each gpu_render_prepared) */
  GpuTcFrame *tc;
  int tcOk; /* tc holds this frame's */
  unsigned id3d; /* the 3D inputs' number of the picture Stage2 draws (R3Id; 0: none known) */
  /* Record diagnostics (diag_rec.c): this renderer call's number, and at its gather the game's 3D swaps, the V-blank count of the
   * shown 3D's swap and the V-blank count */
  unsigned drId;
  uint32_t drSwaps, drSwapFrame, drFrame;
} Slot;
enum { M_GPU = 0, M_VCORE = 1 };
static Slot *sIpTick; /* the slot this call launched, when it has in-between pictures */
static G07Pack *sPack[2]; /* the slots' packs: a slot writes the one the frame before does not use */

static Slot sSlot[2];
static int sCur;       /* the slot this call gathers into */
static int sPipe = -1; /* the pipeline is on */
static int sInflight;  /* a worker runs stage 1 of slot sCur ^ 1 */
static int sNextOk;    /* the frame the last drawn call gathered (slot sCur ^ 1) went into the pipeline: its GL half is NextPicture's */
static VCTask sThread;  /* vcore's persistent task threads (p22_task.c), not a new thread a frame */

static int sInit, sDead, sLogged;
static GpuCore *sCore;
/* G12, the texture cache's CPU half: one for the stream of frames, made on the GL thread before the first worker that wants it
 * and kept across a new GpuCore (Resolution, AA: the core takes it over or starts a new generation); its options from the
 * environment once (BWPORT_GPU_TEXCACHE=off|exact|on, _TEXMIPS, _ANISO, _TEXMAG, _TEXCACHEMB) */
static GpuTexCache *sTc;
static GpuOptions sTcEnv;
static int sTcEnvRead;
static int sTcAB; /* BWPORT_GPU_TCAB=1: each picture with the cache drawn without it first and the two 3D layers compared (RenderTc) */
static uint32_t *sTcA, *sTcB;
static size_t sTcABn;
static struct {
  unsigned pics, mids, used, differ;
  unsigned long long polys, px;
} sTcN;
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

/* hi-res display capture (docs/gpu.md "Display capture"), N > 1 (GL and Metal): each bank's bytes as its last capture left them
 * (StoreCapture) and that capture's number per bank row (0: none, or vcore's 1x capture); the GL thread's bank textures
 * (sCap) and the capture number each of their rows holds (sGlSer). A row is drawn from its texture only while the
 * gathered bytes equal the shadow (the game did not change them) and the GL thread drew that same capture. */
static uint8_t sCapShadow[4][0x20000];
static unsigned sCapNo[4][256];
static unsigned sCapCount;     /* the captures numbered so far */
static VCoreState *sCapRef;    /* a capHi frame's inputs again: vcore writes the 1x capture the game reads from it */
static uint32_t sCapRefGxCap;
static G07Cap *sCap;           /* the bank textures (GL thread) */
static int sCapFailed;         /* g07cap_create failed at this scale: not again until Setup */
static unsigned sGlSer[4][256];
static unsigned sCapGen[4];    /* the captures drawn into each bank's texture (GL thread): pass C's reuse key of a BG showing it */

static int BankLcdc(const VCoreState *s, int bank) {
  u8 c = s->io9[VC_REG_VRAMCNT_A + bank];
  return (c & 0x80) && (c & 7) == 0;
}

/* vcore wrote a capture into its copy of the bank (as vcore_bridge.c's StoreCapture): the same bytes into libntr's bank;
 * ser: the capture's number when it is also drawn at N x (the rows' bytes kept in sCapShadow), 0 for a 1x capture */
static void StoreCapture(VCoreState *s, u32 cnt, unsigned ser) {
  int bank = (cnt >> 16) & 3;
  u8 vramcnt = s->io9[VC_REG_VRAMCNT_A + bank];
  if (!(vramcnt & 0x80) || (vramcnt & 7) != 0) return; /* not LCDC: vcore captured nothing */
  uint8_t *home = SIM_VCapBankHome(bank);
  if (home == NULL) return;
  const u8 *src = vc_bank(s, bank);
  u32 at = ((cnt >> 18) & 3) * 0x8000u;
  u32 n = (u32)kCapW[(cnt >> 20) & 3] * kCapH[(cnt >> 20) & 3] * 2;
  for (u32 i = 0, r0 = at / 512; i < n / 512; i++) {
    u32 r = (r0 + i) & 255;
    sCapNo[bank][r] = ser;
    if (ser != 0) memcpy(&sCapShadow[bank][r * 512], src + r * 512, 512);
  }
  while (n > 0) {
    u32 run = 0x20000 - at < n ? 0x20000 - at : n;
    memcpy(home + at, src + at, run);
    n -= run;
    at = (at + run) & 0x1FFFF;
  }
}

/* a capture frame drawn by the GPU at N x: the bank bytes the game reads come from vcore on a copy of the frame's inputs
 * (CheckFrame's pattern), with a new capture number. After the worker join (vcore's static buffers, docs/gpu.md). */
static int CapHi(Slot *sl) {
  VCoreState *s = sl->s;
  u32 cnt = sl->cap;
  if (sOut < 2 || sCapFailed || ((cnt >> 20) & 3) == 0 || !BankLcdc(s, (cnt >> 16) & 3)) return 0;
  if (sCapRef == NULL && (sCapRef = vcore_create()) == NULL) return 0;
  if (sCapRefGxCap < s->gx_words) {
    free(sCapRef->gx);
    sCapRef->gx = malloc((size_t)s->gx_words * 4 + 16);
    sCapRefGxCap = sCapRef->gx ? s->gx_words : 0;
    if (sCapRef->gx == NULL) return 0;
  }
  memcpy(sCapRef->io9, s->io9, sizeof s->io9);
  memcpy(sCapRef->io9b, s->io9b, sizeof s->io9b);
  vcore_hdma_copy(sCapRef, s);
  memcpy(sCapRef->vram, s->vram, sizeof s->vram);
  memcpy(sCapRef->pltt, s->pltt, sizeof s->pltt);
  memcpy(sCapRef->oam, s->oam, sizeof s->oam);
  memcpy(sCapRef->gx, s->gx, (size_t)s->gx_words * 4);
  sCapRef->gx_words = s->gx_words;
  vcore_render(sCapRef);
  if (++sCapCount == 0) sCapCount = 1;
  StoreCapture(sCapRef, cnt, sCapCount);
  sl->capSer = sCapCount;
  return 1;
}

/* BWPORT_CAPLOG=1: a log line for each frame whose display state differs from the frame before's (DISPCAPCNT, engine A's DISPCNT
 * and BG0-3CNT, engine B's DISPCNT, VRAMCNT A-D, BLDCNT): which scenes capture, and what shows the capture */
static unsigned sCapFirst; /* the number of the first frame with DISPCAPCNT's enable bit (BWPORT_GPU_SHOTS' +K) */
static unsigned CapLog(VCoreState *s) {
  static int on = -1;
  static unsigned n, last[11];
  if (on < 0) on = getenv("BWPORT_CAPLOG") != NULL;
  n++;
  if (sCapFirst == 0 && ((unsigned)vc_io32(s, VC_REG_DISPCAPCNT) >> 31) != 0) sCapFirst = n;
  if (!on) return n;
  unsigned v[11] = {(unsigned)vc_io32(s, VC_REG_DISPCAPCNT), (unsigned)vc_eng32(s, VC_ENG_A, VC_REG_DISPCNT),
                    (unsigned)vc_eng32(s, VC_ENG_B, VC_REG_DISPCNT), 0, 0, 0, 0, 0, 0, 0, 0};
  for (int i = 0; i < 4; i++) v[3 + i] = s->io9[VC_REG_BG0CNT + 2 * i] | s->io9[VC_REG_BG0CNT + 2 * i + 1] << 8;
  v[7] = s->io9[VC_REG_VRAMCNT_A] | s->io9[VC_REG_VRAMCNT_A + 1] << 8 | s->io9[VC_REG_VRAMCNT_A + 2] << 16 |
         (unsigned)s->io9[VC_REG_VRAMCNT_A + 3] << 24;
  v[8] = s->io9[VC_REG_BLDCNT] | s->io9[VC_REG_BLDCNT + 1] << 8;
  if (n > 1 && memcmp(v, last, sizeof v) == 0) return n;
  memcpy(last, v, sizeof v);
  Log("bwport: caplog f%u cap %08x dispA %08x dispB %08x bgcnt %04x %04x %04x %04x vramcnt %08x bldcnt %04x\n", n, v[0], v[1], v[2],
      v[3], v[4], v[5], v[6], v[7], v[8]);
  return n;
}

/* BWPORT_GPU_SHOTS=DIR:F1,F2,...: the picture the bridge hands to libntr for those frames (CapLog's numbers) as
 * DIR/gpu_fF.png at its own size (a 1x picture, vcore's, written N x by nearest neighbour so that shots compare).
 * +K is the Kth frame after the first one with display capture on (a title's frame numbers move with the run's speed; its
 * picture is written as gpu_f<number>.png all the same) */
static void ShotMaybe(unsigned fno, const uint8_t *rgba1x) {
  static int on = -1;
  static char dir[512];
  static unsigned want[64];
  static char rel[64];
  static int nwant;
  if (on < 0) {
    const char *e = getenv("BWPORT_GPU_SHOTS");
    const char *c = e != NULL ? strchr(e, ':') : NULL;
    on = c != NULL;
    if (on) {
      snprintf(dir, sizeof dir, "%.*s", (int)(c - e), e);
      for (const char *q = c + 1; *q && nwant < 64;) {
        rel[nwant] = *q == '+';
        if (rel[nwant]) q++;
        want[nwant++] = (unsigned)strtoul(q, (char **)&q, 10);
        while (*q == ',' || *q == ' ') q++;
      }
    }
  }
  if (!on) return;
  int hit = 0;
  for (int i = 0; i < nwant; i++) hit |= rel[i] ? sCapFirst != 0 && sCapFirst + want[i] == fno : want[i] == fno;
  if (!hit) return;
  int n = sOut, w = VC_W * n, h = VC_H * 2 * n;
  uint8_t *buf = malloc((size_t)w * h * 4);
  if (buf == NULL) return;
  if (rgba1x != NULL) {
    for (int y = 0; y < h; y++)
      for (int x = 0; x < w; x++) memcpy(&buf[((size_t)y * w + x) * 4], &rgba1x[((size_t)(y / n) * VC_W + x / n) * 4], 4);
  } else {
    g07_read_frame(sC7, buf);
  }
  char path[600];
  snprintf(path, sizeof path, "%s/gpu_f%u%s.png", dir, fno, rgba1x != NULL ? "_1x" : "");
  vcore_write_png(path, buf, w, h);
  free(buf);
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
  GatherHDma(s);
  u16 pow = (u16)(s->io9[VC_REG_POWCNT1] | s->io9[VC_REG_POWCNT1 + 1] << 8);
  pow = pow == 0 ? 0x820F : (u16)(pow | 1);
  s->io9[VC_REG_POWCNT1] = (u8)pow;
  s->io9[VC_REG_POWCNT1 + 1] = (u8)(pow >> 8);
  sl->cap = vc_io32(s, VC_REG_DISPCAPCNT);
  sl->fno = CapLog(s);
  sl->capHi = 0;
  sl->capSer = 0;
  for (int b = 0; b < 4; b++)
    for (int r = 0; r < 256; r++) {
      unsigned no = sCapNo[b][r];
      sl->rowSer[b][r] = no != 0 && memcmp(vc_bank(s, b) + r * 512, &sCapShadow[b][r * 512], 512) == 0 ? no : 0;
    }
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

/* engine A's BG2 or BG3 showing a captured bank as it is (docs/gpu.md "Display capture"), at N x: display mode 1, the BG extended
 * (BG mode 3-5) as a 256 x 256 direct-colour bitmap without mosaic, PA = 0x100 and PC = 0 on every line, each line's row alone in
 * one of banks A-D, the same bank and the same offsets on every line (cbg*); the first of BG2, BG3. A scaled, rotated or wavy BG
 * (HDMA) stays 1x. The GL thread draws its lines from the bank's N x texture where that holds this capture (CapBgSet). (Worker,
 * VRAM mapped) */
static void CapBgFind(Slot *sl) {
  VCoreState *s = sl->s;
  sl->cbg = 0;
  memset(sl->cbgLines, 0, sizeof sl->cbgLines);
  u32 dispcnt = vc_eng32(s, VC_ENG_A, VC_REG_DISPCNT);
  const int mode = dispcnt & 7;
  const VC_Region *rg = vc_rgn_bg(s, VC_ENG_A);
  if (sOut < 2 || ((dispcnt >> 16) & 3) != 1 || mode < 3 || mode > 5 || rg->npages == 0) return;
  for (int n = mode == 5 ? 2 : 3; n <= 3 && sl->cbg == 0; n++) {
    u16 cnt = vc_eng16(s, VC_ENG_A, VC_REG_BG0CNT + 2 * n);
    if (!((dispcnt >> (8 + n)) & 1) || (cnt & 0xC0C4) != 0x4084) continue; /* (size 256 x 256, bitmap, no mosaic, direct colour) */
    const u32 base = ((cnt >> 8) & 31) * 0x4000u, pa = VC_REG_BG2PA + 0x10u * (u32)(n - 2), rx = VC_REG_BG2X + 0x10u * (u32)(n - 2);
    int bank = -1, ox = 0, oy = 0, bad = 0;
    for (int y = 0; y < VC_H && !bad; y++) {
      s32 x0 = vc_aff_ref(s, VC_ENG_A, y, rx, pa + 2), ty = vc_aff_ref(s, VC_ENG_A, y, rx + 4, pa + 6) >> 8;
      if (vc_ln16(s, VC_ENG_A, y, pa) != 0x100 || vc_ln16(s, VC_ENG_A, y, pa + 4) != 0 ||
          vc_ln16(s, VC_ENG_A, y, VC_REG_BG0CNT + 2u * (u32)n) != cnt)
        bad = 1;
      if (bad || ty < 0 || ty > 255) continue; /* (outside the bitmap: transparent, or wrapped: 1x) */
      const u32 a = base + (u32)ty * 512;
      const VC_Page *p = &rg->page[(a >> VC_PAGE_SHIFT) % rg->npages];
      int b = 0;
      while (b < 4 && p->n == 1 && !(p->src[0] >= vc_bank(s, b) && p->src[0] < vc_bank(s, b) + 0x20000)) b++;
      if (p->n != 1 || b == 4) {
        bad = 1;
        continue;
      }
      int row = (int)((p->src[0] - vc_bank(s, b) + (a & (VC_PAGE_SIZE - 1))) / 512);
      if (bank < 0) bank = b, ox = x0 >> 8, oy = row - y;
      if (b != bank || x0 >> 8 != ox || row - y != oy) bad = 1;
      else sl->cbgLines[y >> 5] |= 1u << (y & 31);
    }
    if (!bad && bank >= 0) sl->cbg = n, sl->cbgBank = bank, sl->cbgOx = ox, sl->cbgOy = oy;
    else memset(sl->cbgLines, 0, sizeof sl->cbgLines);
  }
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
  CapBgFind(sl);
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

/* G12 (a worker): the texture cache's frame of sl from s->geo and sl->td, when the frame wants the cache */
static void TcFrame(Slot *sl) {
  if (sl->tcOk || !sl->opt.texcache || sTc == NULL || sl->tc == NULL || !sl->tdOk) return;
  gpu_texcache_frame(sTc, sl->td, sl->s, sl->tc);
  sl->tcOk = 1;
}

/* ---- the 3D layer reused (docs/perf.md "Work whose result cannot change", 2026-11-25). A picture's 3D layer (a keyframe's or an
 * in-between's) is a function of what passes S, R, E and P read: the geometry's output (s->geo: the vertices after transform,
 * lighting and clipping, the polygons with their attributes, the drawing order, SWAP_BUFFERS' parameter), the 3D registers
 * (DISP3DCNT, the edge colours, alpha test, the clear colour, depth and image offset, fog, the toon table, 1DOT_DEPTH), the
 * texture and palette VRAM with the toon table (the slot's GpuTexData, and the texture regions' sizes), the options and the
 * grid. The worker numbers each picture as it makes it (R3Id): the number of the picture before when all of that is equal,
 * else a new one. The GL thread draws no S, R, E, P for a picture whose number the 3D layer holds already (R3Hit): the layer
 * is what they would draw. Not with the texture cache (its layers carry state from picture to picture) nor in the modes that
 * draw a picture twice or differently (TCAB, the timing mode, ablations). BWPORT_3D_REUSE=0: off; =check: every picture drawn,
 * and one that would have been reused compared with the layer it would have reused (a log line every 600 pictures and at
 * each mismatch). ---- */
static int sR3 = -1; /* 1 on, 0 off, 2 check */
static struct {      /* the worker's: the last picture's inputs */
  int have;
  unsigned id, next;    /* its number; the last number given */
  const GpuTexData *td; /* its textures (its slot's) and their build */
  unsigned tdSerial;
  GpuOptions opt;
  int scale, out, aa;
  u32 npages[2];
  u8 reg[4 + 0x90 + 2];
  VC_GeomOut *geo; /* its geometry (the used part) */
} sR3W;
static unsigned long long sR3Pics, sR3Same, sR3Ns; /* the worker's: pictures numbered, numbered as the one before, the time it took */

static int R3On(void) {
  if (sR3 < 0) {
    const char *e = getenv("BWPORT_3D_REUSE");
    sR3 = e != NULL && e[0] == '0' ? 0 : e != NULL && strcmp(e, "check") == 0 ? 2 : 1;
    if (sR3 != 1) Log("bwport: Renderer=gpu: BWPORT_3D_REUSE=%s\n", sR3 ? "check: every picture drawn, the reuse compared" : "0: off");
  }
  return sR3;
}

static void R3Regs(const VCoreState *s, u8 *r) {
  memcpy(r, s->io9 + VC_REG_DISP3DCNT, 4);
  memcpy(r + 4, s->io9 + VC_REG_EDGE_COLOR, 0x90); /* 0x330-0x3BF: the edge colours to the toon table */
  memcpy(r + 4 + 0x90, s->io9 + VC_REG_DISP_1DOT_DEPTH, 2);
}

static int R3VtxSame(const VC_Vertex *a, const VC_Vertex *b, u32 n) { /* (field by field: a vertex's padding is not defined) */
  for (u32 i = 0; i < n; i++, a++, b++)
    if (a->clip[0] != b->clip[0] || a->clip[1] != b->clip[1] || a->clip[2] != b->clip[2] || a->clip[3] != b->clip[3] ||
        a->color[0] != b->color[0] || a->color[1] != b->color[1] || a->color[2] != b->color[2] || a->tex[0] != b->tex[0] ||
        a->tex[1] != b->tex[1] || a->sx != b->sx || a->sy != b->sy || a->sz != b->sz || a->clipped != b->clipped)
      return 0;
  return 1;
}

static int R3GeoSame(const VC_GeomOut *a, const VC_GeomOut *b) {
  return a->nvtx == b->nvtx && a->npoly == b->npoly && a->swap_param == b->swap_param && a->swapped == b->swapped &&
         a->overflow == b->overflow && memcmp(a->poly, b->poly, a->npoly * sizeof a->poly[0]) == 0 && /* (polygons are cleared first) */
         memcmp(a->order, b->order, a->npoly * sizeof a->order[0]) == 0 && R3VtxSame(a->vtx, b->vtx, a->nvtx);
}

/* the worker: the number of sl's picture just made (s->geo final, sl->td built); 0: not to be reused */
static unsigned R3Id(Slot *sl) {
  R3On(); /* (numbered also when the reuse is off: an A/B then differs only in the drawing; pass C's screen reuse reads them) */
  if (sl->tcOk || sTcAB || !sl->tdOk) return sR3W.have = 0, 0u; /* (tcOk: the texture cache drew it; Metal has none) */
  long long t0 = NowNs();
  const VCoreState *s = sl->s;
  const VC_GeomOut *g = &s->geo;
  u8 reg[sizeof sR3W.reg];
  R3Regs(s, reg);
  u32 np[2] = {s->rgn[VC_RGN_TEX].npages, s->rgn[VC_RGN_TEXPAL].npages};
  unsigned ser = gpu_texdata_serial(sl->td);
  int same = sR3W.have && sR3W.scale == sScale && sR3W.out == sOut && sR3W.aa == sAA && !memcmp(&sR3W.opt, &sl->opt, sizeof sl->opt) &&
             !memcmp(sR3W.reg, reg, sizeof reg) && !memcmp(sR3W.npages, np, sizeof np);
  if (same) same = R3GeoSame(sR3W.geo, g);
  if (same) /* the slot before's textures are as that picture had them until the worker after this one builds them again */
    same = sR3W.td == sl->td ? sR3W.tdSerial == ser : gpu_texdata_serial(sR3W.td) == sR3W.tdSerial && gpu_texdata_same(sR3W.td, sl->td);
  if (!same) {
    if (sR3W.geo == NULL && (sR3W.geo = malloc(sizeof *sR3W.geo)) == NULL) return sR3W.have = 0, 0u;
    VC_GeomOut *d = sR3W.geo;
    d->nvtx = g->nvtx, d->npoly = g->npoly, d->swap_param = g->swap_param, d->swapped = g->swapped, d->overflow = g->overflow;
    memcpy(d->vtx, g->vtx, g->nvtx * sizeof g->vtx[0]);
    memcpy(d->poly, g->poly, g->npoly * sizeof g->poly[0]);
    memcpy(d->order, g->order, g->npoly * sizeof g->order[0]);
    memcpy(sR3W.reg, reg, sizeof reg);
    memcpy(sR3W.npages, np, sizeof np);
    sR3W.opt = sl->opt;
    sR3W.scale = sScale, sR3W.out = sOut, sR3W.aa = sAA;
    if (++sR3W.next == 0) sR3W.next = 1;
    sR3W.id = sR3W.next;
    sR3W.have = 1;
  }
  sR3W.td = sl->td, sR3W.tdSerial = ser; /* (the same bytes, the newer build) */
  sR3Pics++;
  sR3Same += same;
  sR3Ns += (unsigned long long)(NowNs() - t0);
  return sR3W.id;
}

/* stage 1 with in-between pictures (a worker): the 2D first (engine B's lines, engine A's layers: the mids reuse them, only the
 * 3D is redone), then for each present slot after the first the 3D's geometry with its vertices blended toward the previous
 * keyframe (IpGather's pairing) and its span records, and last the picture the next tick's first slot shows (this slot's own
 * prep, which Stage2 draws) */
static void Stage1Interp(Slot *sl) {
  VCoreState *s = sl->s;
  long long p0 = sl->pfOn ? NowNs() : 0; /* the readout's parts of the keyframe (libntr-ios-passes.patch; the mids' are not in them) */
  const int note = SIM_PresentLogOn();
  long long w0 = note ? NowNs() : 0, wTd = 0, wMid[IP_MAXSLOT] = {0};
  const int drOn = bwport_diagrec_state == 2; /* Record diagnostics (diag_rec.c) */
  DrTickInfo dr;
  if (drOn) memset(&dr, 0, sizeof dr), dr.workNs = NowNs();
  vc_vram_map(s);
  Build2D(sl); /* before the first mid is settled: the mids read the pack */
  if (note) wTd = NowNs();
  gpu_texdata_build(sl->td, s), sl->tdOk = 1;
  long long tA = NowNs();
  if (note) wTd = tA - wTd;
  if (p0 != 0) sl->pf2d = (double)(tA - p0) / 1e6 + sl->pfCmp;
  long long gap = IpGather(s, sl->ipSerial, sl->ipNominal, sl->ipTick);
  if (drOn && sIp.why != 0) bwport_diagrec_key(s, sl->drId, sl->drFrame, sl->drSwapFrame, sl->drSwaps, sIp.haveOld);
  for (int idx = 1; idx <= sl->ipN; idx++) {
    long long off = idx < sl->ipN ? sl->ipOff[idx] : sl->ipNext;
    double f = sIp.valid ? (double)(sl->ipNominal + off - sIp.keyNs) / (double)gap : 2.0;
    int blend = sIp.valid && f < 1.0 && (f > 0.0 || SIM_InterpLeadOn()); /* (f at or below 0: the older keyframe, b 4096) */
    IpMetric(idx, f, blend);
    if (drOn && idx < 9) dr.f[idx] = (float)f, dr.blend |= (unsigned)blend << idx;
    sl->ipGame[idx] = sIp.valid ? sIp.keyNs + (long long)(((f < 0.0 ? 0.0 : f > 1.0 ? 1.0 : f) - 1.0) * (double)gap) : sIp.keyNs;
    long long t0 = NowNs();
    if (blend) {
      const double fc = !(f > 0.0) ? 0.0 : f > 1.0 ? 1.0 : f; /* (clamped before the s32 conversion: a huge or NaN f is undefined there) */
      s32 b = (s32)((1.0 - fc) * 4096.0 + 0.5);
      vc_ip_apply_begin(s, &sIp.old, sIp.map, sIp.texhold, b < 0 ? 0 : b > 4096 ? 4096 : b);
      vc_ip_apply_keep(s, sIp.keep, sIp.nkeep);
      vc_ip_apply_enter(s, (const double (*)[16])sIp.enter);
    }
    if (idx < sl->ipN) {
      IpMid *m = &sl->mid[idx];
      if (!blend) {
        MidSettle(m, 2);
        if (note) wMid[idx] = -(NowNs() - w0);
        continue;
      }
      int ok = GeometryAgain(s);
      vc_ip_apply_end(s);
      if (ok) TcFrame(sl); /* from the first geometry, before a mid is settled (the GL thread may draw it at once); a key the
                            * other pictures add samples the texture slots as without the cache */
      if (ok) PrepareInto(sl, &m->prep);
      m->id3d = ok ? R3Id(sl) : 0; /* (before it is settled: the GL thread reads it then) */
      if (ok && m->prep != NULL) {
        MidSettle(m, 1);
        IC(mids, 1);
        IC(midNs, NowNs() - t0);
        if (note) wMid[idx] = NowNs() - w0;
      } else {
        MidSettle(m, 2);
        if (note) wMid[idx] = -(NowNs() - w0);
      }
    } else {
      long long g0 = sl->pfOn ? NowNs() : 0;
      sl->geo = GeometryAgain(s);
      if (blend) vc_ip_apply_end(s);
      if (sl->geo) TcFrame(sl);
      sl->id3d = sl->geo ? R3Id(sl) : 0;
      long long g1 = g0 != 0 ? NowNs() : 0;
      if (sl->geo) PrepareInto(sl, &sl->prep), sl->prepared = sl->prep != NULL;
      if (g0 != 0) sl->pfGeo = (double)(g1 - g0) / 1e6, sl->pfSpans = (double)(NowNs() - g1) / 1e6;
      if (blend) IC(nextMid, 1), IC(midNs, NowNs() - t0);
      else IC(nextPlain, 1), IC(plainNs, NowNs() - t0);
    }
  }
  IC(wholeNs, NowNs() - tA);
  if (drOn) {
    dr.nominal = sl->ipNominal, dr.serial = sl->ipSerial, dr.gap = gap, dr.workNs = NowNs() - dr.workNs;
    dr.id = sl->drId, dr.frame = sl->drFrame, dr.swaps = sl->drSwaps, dr.swapFrame = sl->drSwapFrame;
    dr.ipN = sl->ipN, dr.why = sIp.why, dr.nl = (int)sIp.key.nl, dr.nv = (int)sIp.key.nv, dr.valid = sIp.valid;
    bwport_diagrec_tick(&dr);
  }
  if (note) { /* I due tick why lists verts unpaired vtpaired gap slots | texdata before-pairing pairing | each mid settled (us from the
               * start; negative: settled with none) | the end */
    char b[384];
    int k = snprintf(b, sizeof b, "I %lld %lld %d %u %u %d %d %lld %d %lld %lld %lld %d %d", sl->gameNs / 1000, sl->ipTick, sIp.why,
                     (unsigned)sIp.key.nl, (unsigned)sIp.key.nv, sIp.unp, sIp.vtp, gap / 1000, sl->ipN, wTd / 1000, (tA - w0) / 1000,
                     sIp.recNs / 1000, sIp.uns, sIp.nls);
    for (int idx = 1; idx < sl->ipN && k > 0 && k < (int)sizeof b - 24; idx++) k += snprintf(b + k, sizeof b - (size_t)k, " %lld", wMid[idx] / 1000);
    if (k > 0 && k < (int)sizeof b - 24) snprintf(b + k, sizeof b - (size_t)k, " %lld", (NowNs() - w0) / 1000);
    SIM_PresentLogNote(b);
  }
}

/* stage 1 (a worker): vcore alone for a capture frame; else the geometry and engine B */
static void Stage1(Slot *sl) {
  VCoreState *s = sl->s;
  long long t0 = sl->pfOn ? NowNs() : 0;
  sl->pfGeo = sl->pfSpans = sl->pf2d = 0.0;
  sl->tcOk = 0;
  sl->id3d = 0;
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
  if (sl->geo) gpu_texdata_build(sl->td, s), sl->tdOk = 1, TcFrame(sl);
  sl->id3d = sl->geo ? R3Id(sl) : 0;
  if (bwport_diagrec_state == 2) { /* Record diagnostics: a call with no in-between slots (IpGather did not see it) */
    DrTickInfo dr;
    memset(&dr, 0, sizeof dr);
    dr.nominal = SIM_TickNominal(), dr.id = sl->drId, dr.frame = sl->drFrame, dr.swaps = sl->drSwaps, dr.swapFrame = sl->drSwapFrame;
    dr.why = -1, dr.workNs = t0 != 0 ? NowNs() - t0 : 0;
    bwport_diagrec_tick(&dr);
  }
  if (t0 != 0) {
    sl->pfGeo = (double)(t1 - t0) / 1e6;
    sl->pfSpans = (double)(t2 - t1) / 1e6;
    sl->pf2d = (double)(NowNs() - t2) / 1e6 + sl->pfCmp;
  }
}

/* stage 1 timed for the readout (its parts: Stage1) */
static void Stage1Timed(Slot *sl) {
  long long t0 = sl->pfOn ? NowNs() : 0;
  long long w0 = SIM_PresentLogOn() ? NowNs() : 0;
  Stage1(sl);
  sl->pfWhole = t0 != 0 ? (double)(NowNs() - t0) / 1e6 : 0.0;
  if (w0 != 0) { /* W due uses3d slots the stage's time (us) */
    char b[96];
    snprintf(b, sizeof b, "W %lld %d %d %lld", sl->gameNs / 1000, sl->uses3d, sl->ipN, (NowNs() - w0) / 1000);
    SIM_PresentLogNote(b);
  }
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

/* the title snapshot (port/src/title_fork.c), before its fork: the worker running the last frame's stage 1 joined (it is not in
 * the child, which would wait for it in its next frame), as the next call would join it; that frame's GL half stays to run.
 * The task pool itself starts empty in the child (bwport_vcore_forked: vcore_task_forked) */
void bwport_gpu_fork_prepare(void) {
  if (sInflight) {
    vcore_task_join(&sThread);
    sInflight = 0;
    PerfReport1(&sSlot[sCur ^ 1]);
  }
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
  const char *rn = (const char *)BGL(GetString)(GL_RENDERER);
  Log("bwport: Renderer=gpu: %s, %s (draw buffers %d, colour attachments %d, transform feedback components %d)\n",
      rn, (const char *)BGL(GetString)(GL_VERSION), (int)mdb, (int)mca, (int)mtf);
  /* 2026-11-13: the GPU renderer on a software GL runs its fragment passes on the CPU and is slower than vcore there
     (uncapped, Nuvema Town: WSL llvmpipe 65 fps against vcore's 181; Windows over Remote Desktop, where the system's GL is
     GDI Generic and the bundled Mesa's llvmpipe draws: 150 against 300). Renderer=auto picks vcore on it; say why it is slow */
  if (rn != NULL && (strstr(rn, "llvmpipe") != NULL || strstr(rn, "softpipe") != NULL || strstr(rn, "SwiftShader") != NULL ||
                     strstr(rn, "Basic Render") != NULL))
    Log("bwport: Renderer=gpu on a software GL (%s): its passes run on the CPU, slower than vcore here (Renderer=auto picks vcore)\n",
        rn);
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
  br_mtl_pt_frame(!on && SIM_GpuPassesWanted()); /* (not beside the readout's split: one or the other) */
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
/* the GL thread: p's 3D (the frame's records or an in-between picture's) with the frame's textures and texture cache frame
 * (gpu_use_tcframe always, so the core runs no cache of its own); BWPORT_GPU_TCAB=1 draws it without the cache first and
 * compares the two 3D layers (the log line every 300 pictures and at the first differences) */
static int RenderTc(Slot *sl, const GpuPrep *p, int mid) {
  VCoreState *s = sl->s;
  const GpuTcFrame *f = sl->tcOk ? sl->tc : NULL;
  size_t n = (size_t)VC_W * VC_H * sScale * sScale; /* (the 3D layer is sOut x sOut of it) */
  if (sTcAB && f != NULL && n > sTcABn) {
    free(sTcA), free(sTcB);
    sTcA = malloc(n * 4), sTcB = malloc(n * 4);
    sTcABn = sTcA != NULL && sTcB != NULL ? n : 0;
  }
  if (sTcAB && f != NULL && sTcABn >= n) {
    gpu_use_texdata(sCore, sl->tdOk ? sl->td : NULL);
    gpu_use_tcframe(sCore, sTc, NULL);
    if (gpu_render_prepared(sCore, s, p) == 0) {
      gpu_read_3d(sCore, sTcA, NULL);
      gpu_use_texdata(sCore, sl->tdOk ? sl->td : NULL);
      gpu_use_tcframe(sCore, sTc, f);
      int rc = gpu_render_prepared(sCore, s, p);
      if (rc != 0) return rc;
      gpu_read_3d(sCore, sTcB, NULL);
      size_t m = (size_t)VC_W * VC_H * sOut * sOut;
      unsigned long long d = 0;
      for (size_t i = 0; i < m; i++) d += sTcA[i] != sTcB[i];
      int used = gpu_tc_last_polys(sCore);
      sTcN.pics++, sTcN.mids += mid != 0, sTcN.used += used > 0, sTcN.polys += (unsigned)used;
      sTcN.differ += d != 0, sTcN.px += d;
      if (sTcN.pics % 300 == 0 || (d != 0 && sTcN.differ <= 5))
        Log("bwport: Renderer=gpu: TCAB: %u pictures (%u in-between) compared at %dx, %u drew %llu polygons from the cache, %u differ "
            "(%llu px)%s\n", sTcN.pics, sTcN.mids, sOut, sTcN.used, sTcN.polys, sTcN.differ, sTcN.px, d != 0 ? ": this one differs" : "");
      return 0;
    }
  }
  gpu_use_texdata(sCore, sl->tdOk ? sl->td : NULL);
  gpu_use_tcframe(sCore, sTc, f);
  return gpu_render_prepared(sCore, s, p);
}

/* ---- the 3D layer reused, the GL thread's half (R3Id above): the number of the picture the 3D layer holds (0: unknown: a new
 * core, a failed or test draw), the counts, and the check mode's copy of the layer ---- */
static unsigned sR3Layer;
static unsigned long long sR3Key, sR3Mid, sR3KeyUsed, sR3MidUsed, sR3Checked, sR3Differ;
static uint8_t *sR3Ref, *sR3Tmp;
static size_t sR3RefN;
static unsigned sR3RefId;

static int R3Hit(unsigned id) { return sR3 == 1 && id != 0 && id == sR3Layer && SIM_Ablation() == 0 && !SIM_PassTiming(); }

static void R3Read(uint8_t *buf) { /* the 3D layer's bytes (Metal: as they are; GL: gpu_read_3d's words) */
#ifdef BR_METAL
  if (sHalf == &kMetal) {
    gpu_mtl_read_rgba(gpu_mtl_texture(gpu_3d_tex(sCore)), VC_W * sOut, VC_H * sOut, buf);
    return;
  }
#endif
  gpu_read_3d(sCore, (uint32_t *)buf, NULL);
}

static void R3Report(void) {
  unsigned long long n = sR3Key + sR3Mid;
  if (n == 0 || n % 600 != 0) return;
  char chk[96] = "";
  if (sR3 == 2) snprintf(chk, sizeof chk, "; check: %llu compared, %llu differ", sR3Checked, sR3Differ);
  Log("bwport: Renderer=gpu: 3D reused for %llu of %llu keyframes, %llu of %llu in-betweens; the worker numbered %llu pictures, %llu as "
      "the one before, %.1f us each%s\n", sR3KeyUsed, sR3Key, sR3MidUsed, sR3Mid, sR3Pics, sR3Same,
      sR3Pics ? (double)sR3Ns / 1e3 / (double)sR3Pics : 0.0, chk);
}

/* picture id (mid: an in-between) is to be drawn: 1 when its 3D layer is there already (nothing to draw) */
static int sDrR3; /* Record diagnostics: the picture being drawn found its 3D layer there already */
static int R3Reuse(unsigned id, int mid) {
  int hit = R3Hit(id);
  sDrR3 = hit;
  if (mid) sR3Mid++, sR3MidUsed += hit;
  else sR3Key++, sR3KeyUsed += hit;
  R3Report();
  return hit;
}

/* picture id was drawn into the 3D layer (rc 0) or failed. Check mode: the layer compared with the one drawn before when that was
 * the same picture (the reuse would have shown that one) */
static void R3Drawn(unsigned id, int rc) {
  if (rc != 0 || id == 0 || SIM_Ablation() != 0) {
    sR3Layer = 0;
    return;
  }
  if (sR3 == 2) {
    size_t n = (size_t)VC_W * VC_H * sOut * sOut * 4;
    if (n > sR3RefN) {
      free(sR3Ref), free(sR3Tmp);
      sR3Ref = calloc(1, n), sR3Tmp = calloc(1, n);
      sR3RefN = sR3Ref != NULL && sR3Tmp != NULL ? n : 0;
      sR3RefId = 0;
    }
    if (sR3RefN >= n) {
      R3Read(sR3Tmp);
      if (id == sR3Layer && id == sR3RefId) {
        sR3Checked++;
        if (memcmp(sR3Tmp, sR3Ref, n) != 0 && ++sR3Differ <= 20) {
          size_t px = 0;
          for (size_t i = 0; i < n; i += 4) px += memcmp(sR3Tmp + i, sR3Ref + i, 4) != 0;
          Log("bwport: Renderer=gpu: 3D REUSE check: picture %u drawn again differs (%zu px)\n", id, px);
        }
      }
      uint8_t *t = sR3Ref;
      sR3Ref = sR3Tmp, sR3Tmp = t;
      sR3RefId = id;
    }
  }
  sR3Layer = id;
}

/* ---- pass C's screen reuse (g07_set_reuse): a screen whose 2D, 3D layer (R3's number) and uniforms are as when it was drawn is
 * not drawn again. BWPORT_COMPOSE_REUSE=0: off; =check: drawn whole, a kept screen compared (a line every 600 composes) ---- */
static int C7Mode(void) {
  static int m = -1;
  if (m < 0) {
    const char *e = getenv("BWPORT_COMPOSE_REUSE");
    m = e != NULL && e[0] == '0' ? 0 : e != NULL && strcmp(e, "check") == 0 ? 2 : 1;
    if (m != 1) Log("bwport: Renderer=gpu: BWPORT_COMPOSE_REUSE=%s\n", m ? "check: pass C drawn whole, the kept screens compared" : "0: off");
  }
  return m;
}
static void C7Report(void) {
  unsigned long long n[5];
  g07_reuse_counts(sC7, n);
  if (n[0] == 0 || n[0] % 600 != 0) return;
  char chk[96] = "";
  if (C7Mode() == 2) snprintf(chk, sizeof chk, "; check: %llu screens compared, %llu differ", n[3], n[4]);
  Log("bwport: Renderer=gpu: pass C drew one screen in %llu of %llu composes, none in %llu%s\n", n[1], n[0], n[2], chk);
}

static unsigned Layer3D(Slot *sl, int *sc) {
  VCoreState *s = sl->s;
  sHalf->drain(); /* libntr's own, not ours */
  int rc;
  /* the textures: uploaded by this frame's in-between pictures already, if it had any */
  if (sl->prepared && gpu_prep_scale(sl->prep) == sScale) {
    if (R3Reuse(sl->id3d, 0)) { /* the 3D layer is this picture's already */
      *sc = sOut;
      return gpu_3d_tex(sCore);
    }
    rc = RenderTc(sl, sl->prep, 0);
    R3Drawn(sl->id3d, rc);
  } else { /* no records yet (the first frame, a scale change): spans here */
    gpu_use_texdata(sCore, sl->tdOk ? sl->td : NULL);
    gpu_use_tcframe(sCore, sTc, sl->tcOk ? sl->tc : NULL); /* (always: the core runs no cache of its own then) */
    rc = gpu_render_3d(sCore, s);
    R3Drawn(0, rc);
  }
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
  vcore_hdma_copy(sRef, s);
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

/* the rows of bank's texture that hold what sl's frame gathered (bit r: bank row r) */
static int CapRows(const Slot *sl, int bank, uint32_t rows[8]) {
  int any = 0;
  memset(rows, 0, 8 * sizeof rows[0]);
  for (int r = 0; r < 256; r++)
    if (sl->rowSer[bank][r] != 0 && sl->rowSer[bank][r] == sGlSer[bank][r]) rows[r >> 5] |= 1u << (r & 31), any = 1;
  return any;
}

/* pass C's captured-bank BG for sl's frame (CapBgFind; GL thread, before each g07_compose_packed): the longest run of its lines
 * whose bank rows hold this capture at N x (the others stay 1x), or none */
static void CapBgSet(const Slot *sl) {
  int y0 = 0, y1 = 0;
  uint32_t rows[8];
  if (sl->cbg != 0 && sCap != NULL && g07cap_scale(sCap) == sOut && CapRows(sl, sl->cbgBank, rows))
    for (int y = 0, run = 0; y < VC_H; y++) {
      int r = y + sl->cbgOy; /* (0..255 on the lines CapBgFind marked) */
      run = ((sl->cbgLines[y >> 5] >> (y & 31)) & 1) && ((rows[r >> 5] >> (r & 31)) & 1) ? run + 1 : 0;
      if (run > y1 - y0) y0 = y + 1 - run, y1 = y + 1;
    }
  if (y1 > y0) g07_set_capbg(sC7, g07cap_bank(sCap, sl->cbgBank), sl->cbg, sl->cbgOx, sl->cbgOy, y0, y1, sCapGen[sl->cbgBank]);
  else g07_set_capbg(sC7, 0, 0, 0, 0, 0, 0, 0);
  static int log = -1, last[4];
  if (log < 0) log = getenv("BWPORT_CAPLOG") != NULL;
  const int now[4] = {sl->cbg, sl->cbgBank, y0, y1};
  if (log && memcmp(now, last, sizeof now) != 0) { /* (BWPORT_CAPLOG: what a BG showing a captured bank is drawn from) */
    memcpy(last, now, sizeof now);
    if (sl->cbg == 0) Log("bwport: caplog f%u capbg none\n", sl->fno);
    else if (y1 == 0) Log("bwport: caplog f%u capbg BG%d bank %c: no N x rows (1x)\n", sl->fno, sl->cbg, 'A' + sl->cbgBank);
    else Log("bwport: caplog f%u capbg BG%d bank %c offset %d %d, N x rows %d-%d\n", sl->fno, sl->cbg, 'A' + sl->cbgBank, sl->cbgOx,
             sl->cbgOy, y0, y1 - 1);
  }
}

/* N > 1 (GL or Metal), pass C done: engine A showing a bank (display mode 2) drawn again from its texture where it is valid,
 * then this frame's capture at N x into its bank's texture (docs/gpu.md "Display capture") */
static void CapHiStage2(Slot *sl, unsigned t3, int sc) {
  VCoreState *s = sl->s;
  u32 dispcnt = vc_eng32(s, VC_ENG_A, VC_REG_DISPCNT);
  int bankD = (dispcnt >> 18) & 3;
  int show = ((dispcnt >> 16) & 3) == 2 && BankLcdc(s, bankD);
  if (!show && !sl->capHi) return;
  if (sCap != NULL && g07cap_scale(sCap) != sOut) g07cap_destroy(sCap), sCap = NULL;
  if (sCap == NULL) {
    if (sCapFailed) return;
    char err[512];
    memset(sGlSer, 0, sizeof sGlSer);
    sCap = sHalf->api == GPU_API_METAL ? g07cap_create_metal(sOut, err, sizeof err) : g07cap_create(sOut, err, sizeof err);
    if (sCap == NULL) {
      sCapFailed = 1;
      Log("bwport: Renderer=gpu: display capture at %dx: %s; captures stay 1x\n", sOut, err);
      return;
    }
  }
  unsigned texA = 0;
  if (sl->capHi) texA = g07_compose_capsrc(sC7, s, sl->pack, t3, sc, sc == sOut ? gpu_smaa_weights(sCore) : 0);
  uint32_t rows[8];
  if (show && CapRows(sl, bankD, rows)) {
    g07cap_show(sCap, g07_frame_tex(sC7), (vc_io16(s, VC_REG_POWCNT1) >> 15) & 1, bankD, vc_eng16(s, VC_ENG_A, VC_REG_MASTER_BRIGHT),
                rows);
    g07_forget(sC7); /* (pass C's output drawn into) */
  }
  if (!sl->capHi) return;
  const uint8_t *b1 = !(sl->cap & (1u << 25)) && BankLcdc(s, bankD) ? vc_bank(s, bankD) : NULL;
  CapRows(sl, bankD, rows);
  int rc = texA != 0 ? g07cap_capture(sCap, sl->cap, dispcnt, texA, t3, t3 != 0 ? sc : 0, b1, rows) : -1;
  int bank = (sl->cap >> 16) & 3, r0 = (int)((sl->cap >> 18) & 3) * 64, h = kCapH[(sl->cap >> 20) & 3];
  for (int i = 0; i < h; i++) sGlSer[bank][(r0 + i) & 255] = rc == 0 ? sl->capSer : 0;
  sCapGen[bank]++;
}

/* stage 2 (the calling thread, the GL context current, the GL state saved by the caller): the frame's picture in sPic */
static void Stage2Run(Slot *sl) {
  VCoreState *s = sl->s;
  const unsigned abl = SIM_Ablation();
  if (abl & 32) return; /* ablate=gpu: no GL/Metal half at all; the picture stays the last one drawn */
  sDrR3 = 0;
#ifdef BR_METAL
  if (sHalf == &kMetal) gpu_mtl_ablate(abl & 31), gpu_mtl_pt_kind(GPU_PT_KEY); /* GPU_ABL_R .. GPU_ABL_C; the passes' kind */
#endif
  long long tq0 = SIM_PerfOn() ? NowNs() : 0;
  sHalf->drain();
  if (sl->mode == M_VCORE) {
    sHalf->upload1x(sl->rgba);
    ShotMaybe(sl->fno, sl->rgba);
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
    sR3Layer = 0;
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
  g07_set_3d_id(sC7, t3 != 0 && sc == sOut ? sR3Layer : 0);
  CapBgSet(sl);
  int rc = g07_compose_packed(sC7, s, sl->pack, t3, sc, sc == sOut ? gpu_smaa_weights(sCore) : 0);
  C7Report();
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
  if (sOut > 1) CapHiStage2(sl, t3, sc);
  sPic.tex = sHalf->picture(g07_frame_tex(sC7));
  sPic.w = VC_W * sOut;
  sPic.h = VC_H * 2 * sOut;
  SIM_InterpShows(sl->ipN > 0 ? sl->ipGame[sl->ipN] : sl->gameNs);
  if (bwport_diagrec_state == 2) bwport_diagrec_shows(sl->drId, sl->ipN, sl->id3d, sDrR3), sDrR3 = 0;
  ShotMaybe(sl->fno, NULL);
  if (sCheckDir != NULL) {
    const uint8_t *pic = CheckPicture(sl);
    if (pic != NULL) CheckFrame(sl, pic);
  }
}

static void Stage2(Slot *sl) {
  long long t0 = SIM_PresentLogOn() ? NowNs() : 0;
  Stage2Run(sl);
  if (t0 != 0) { /* G due the GL half's CPU time (us) */
    char b[64];
    snprintf(b, sizeof b, "G %lld %lld", sl->gameNs / 1000, (NowNs() - t0) / 1000);
    SIM_PresentLogNote(b);
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
  sDrR3 = 0;
#ifdef BR_METAL
  if (sHalf == &kMetal) gpu_mtl_pt_kind(GPU_PT_MID); /* (the GPU passes' times: an in-between's command buffer) */
#endif
  sHalf->save(&gs);
  sHalf->drain();
  gpu_set_options(sCore, &sl->opt);
  unsigned tex = 0;
  /* the keyframe's VRAM (uploaded once, by the first mid) and its cache frame; nothing drawn when the 3D layer is this picture's */
  int ok3 = m->prep != NULL && gpu_prep_scale(m->prep) == sScale;
  if (ok3 && !R3Reuse(m->id3d, 1)) {
    int rc = RenderTc(sl, m->prep, 1);
    R3Drawn(m->id3d, rc);
    ok3 = rc == 0;
  }
  if (ok3) g07_set_3d_id(sC7, sR3Layer), CapBgSet(sl);
  if (ok3 && g07_compose_packed(sC7, sl->s, sl->pack, gpu_3d_tex(sCore), sOut, gpu_smaa_weights(sCore)) == 0) {
    tex = sHalf->picture(g07_frame_tex(sC7));
    *w = VC_W * sOut;
    *h = VC_H * 2 * sOut;
    C7Report();
    sPic.tex = tex; /* the picture a skipped frame shows again */
    SIM_InterpShows(sl->ipGame[slot]);
    if (bwport_diagrec_state == 2) bwport_diagrec_shows(sl->drId, slot, m->id3d, sDrR3);
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

/* libntr's cap loop (SIM_SetNextTexture; its early present, above sim_main.cpp's SpPreMark): the picture the next tick's first slot
 * shows, the GL half (Stage2) of the frame this tick's call launched, drawn now: after this tick's last present, or at the top of
 * the next tick's frame. The next call would have drawn it after the next tick's game logic and its own gather, so a slot early
 * in that tick was presented late and its GPU work went in late; libntr now presents it at its slot, and the next call does not
 * draw it again (done2). 0: none (no frame in the pipeline: a first frame, a capture frame, the pipeline off), the next tick
 * presents that slot itself. */
static unsigned NextPicture(int *w, int *h) {
  Slot *sl = &sSlot[sCur ^ 1];
  if (!sNextOk || !sl->has || sCore == NULL || sC7 == NULL || sDead || sFatal || (SIM_Ablation() & 32)) return 0;
  if (!sl->done2) {
    if (sInflight) { /* (the frame's stage 1 and its in-betweens are done by now: their slots are past) */
      /* not when it still runs: the join would block the render thread (and the next tick) past the tick's last present, hidden from
       * the "behind" rule; libntr's next tick presents that slot itself, as for no picture */
      if (!vcore_task_done(&sThread)) return 0;
      vcore_task_join(&sThread);
      sInflight = 0;
      PerfReport1(sl);
    }
    GLSave gs;
    sHalf->save(&gs);
    Stage2(sl);
    sl->done2 = 1;
    sHalf->restore(&gs);
  }
  if (sPic.tex == 0) return 0;
  *w = sPic.w;
  *h = sPic.h;
  return sPic.tex;
}

static void Destroy(void) {
  if (sC7) g07_destroy(sC7);
  sC7 = NULL;
  if (sCore) gpu_destroy(sCore);
  sCore = NULL;
  sR3Layer = 0;
  if (sTex1) BGL(DeleteTextures)(1, &sTex1);
  sTex1 = 0;
  if (sCap) g07cap_destroy(sCap);
  sCap = NULL;
  sCapFailed = 0;
  memset(sGlSer, 0, sizeof sGlSer);
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
  sNextOk = 0;
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
  sIp.have = sIp.haveOld = sIp.valid = sIp.mapOk = 0;
  sR3W.have = 0; /* (its textures were the slots') */
}

/* libntr: the other renderer is wanted now (the GL context is current) */
static void GL_APIENTRY LostDeleteN(GLsizei n, const GLuint *p) {
  (void)n;
  (void)p;
}
static void BridgeIdle(void) {
  if (sHalf->api == GPU_API_GL ? BGL(GetError) != NULL : sInit) ReleaseAll(), sHalf->release();
}

/* libntr (glr2): the GL context was lost and a new one is current: every GL name this file and the core hold is dead. Nothing
 * is deleted (a delete of a dead name is GL_INVALID_VALUE, or somebody else's object in the new context): the delete
 * functions are no-ops while the state is released, then the next frame sets up as the first. */
static void BridgeGLLost(void) {
  if (sHalf->api != GPU_API_GL) return;
  PFNGLDELETETEXTURESPROC dt = bgl.DeleteTextures;
  PFNGLDELETEQUERIESPROC dq = sTq.DeleteQueries;
  bgl.DeleteTextures = LostDeleteN;
  sTq.DeleteQueries = LostDeleteN;
  gpu_gl_lost_begin();
  ReleaseAll();
  TqRelease();
  gpu_gl_lost_end();
  bgl.DeleteTextures = dt;
  sTq.DeleteQueries = dq;
  memset(&sTq, 0, sizeof sTq); /* (probed again) */
  sTex1 = 0;
  sInit = 0;
  sDead = sErrors = sFatal = 0;
  sDeadTicks = 0;
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
  g07_set_reuse(sC7, C7Mode());
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
  int aa = SIM_AntialiasWanted() == 2 ? 2 : 1; /* k x k samples per pixel at any Resolution: the targets stay at N */
  if (SIM_AntialiasWanted() == 3) aa = GPU_AA_SMAA; /* libntr-ios-smaa.patch: one sample, SMAA at N > 1 (port/gpu g08_smaa.c) */
  int edgeaa = SIM_AntialiasWanted() == 4; /* Smooth edges: one sample, pass P draws the outline with its coverage at N > 1 */
  SIM_RendererAAUsed(aa == GPU_AA_SMAA && scale == 1 ? 1 : edgeaa ? (scale > 1 ? 4 : 1) : aa);
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
  sl->opt.edgeaa = edgeaa;
  sl->opt.aalite = aa == 2 && SIM_AntialiasLite(); /* 4 samples (lite): the DS's own antialiasing left out under the samples */
  sl->opt.exp = SIM_PerfExperiments() & GPU_EXP_ALL; /* PerfExperiments: Exact's pass R cuts' switches (the same picture) */
  sFastWant = SIM_GLRendererOption(4);
  sl->opt.fast = sFastWant && sFastOk > 0;
  if (!sTcEnvRead) {
    gpu_options_from_env(&sTcEnv), sTcEnvRead = 1; /* (ini keys: job J8) */
    const char *ab = getenv("BWPORT_GPU_TCAB");
    sTcAB = ab != NULL && ab[0] && strcmp(ab, "0") != 0;
  }
  sl->opt.texcache = sTcEnv.texcache, sl->opt.mips = sTcEnv.mips, sl->opt.aniso = sTcEnv.aniso;
  if (getenv("BWPORT_GPU_EXP") != NULL) sl->opt.exp = sTcEnv.exp; /* (over PerfExperiments) */
  sl->opt.texmag = sTcEnv.texmag, sl->opt.tcmb = sTcEnv.tcmb;
  sl->opt.fog = sTcEnv.fog, sl->opt.color = sTcEnv.color; /* BWPORT_GPU_FOG, BWPORT_GPU_COLOR (above 1x only) */
  { /* the settings (sim_config.ini TextureSmoothing, Filter2D, Fog, Color): the environment variables above still override them */
    if (getenv("BWPORT_GPU_FOG") == NULL) sl->opt.fog = SIM_GLRendererOption(7);
    if (getenv("BWPORT_GPU_COLOR") == NULL) sl->opt.color = SIM_GLRendererOption(8);
    if (SIM_GLRendererOption(6) == 2 && scale > 1) { /* Mipmaps: the texture cache's trilinear sampling, above 1x (1x stays the DS's nearest) */
      if (getenv("BWPORT_GPU_TEXCACHE") == NULL) sl->opt.texcache = GPU_TC_ON;
      if (getenv("BWPORT_GPU_TEXMIPS") == NULL) sl->opt.mips = 1;
      if (getenv("BWPORT_GPU_TEXMAG") == NULL) sl->opt.texmag = 1;
      /* Mipmaps is the cache's trilinear program, not the Bilinear one (raster_prog picks the cache only when opt.filter is off); the
       * fast renderer and Metal have no cache yet: there Mipmaps stays the bilinear look (docs/settings-layout.md) */
      if (!sl->opt.fast && sHalf->api != GPU_API_METAL) sl->opt.filter = 0;
    }
    sl->opt.up2d = SIM_GLRendererOption(5); /* G07_UP_OFF, _SCALE2X, _MMPX = 0, 2, 3 */
    sl->opt.up2d = sl->opt.up2d == 1 ? G07_UP_SCALE2X : sl->opt.up2d == 2 ? G07_UP_MMPX : G07_UP_OFF;
    if (getenv("BWPORT_FILTER2D") != NULL) sl->opt.up2d = sTcEnv.up2d;
    if (sC7 != NULL) g07_set_up2d(sC7, sl->opt.up2d);
  }
  if (sC7 != NULL) g07_set_hiprec(sC7, sl->opt.color);
  if (sl->opt.texcache && sHalf->api != GPU_API_METAL) { /* (Metal: no layers yet, job J7) */
    if (sTc == NULL) sTc = gpu_texcache_create(&sl->opt);
    if (sl->tc == NULL) sl->tc = gpu_tcframe_create();
  }
  sNextOk = 0;
  if (Gather(sl) != 0) {
    sHalf->restore(&gs);
    return 0;
  }
  sl->has = 1;
  sl->done2 = 0;
  sl->mode = (sl->cap & 0x80000000u) ? M_VCORE : M_GPU;
  { /* Record diagnostics (diag_rec.c): the call's number and the 3D swap it shows (read here: the game swaps again while the worker runs) */
    static unsigned sDrId;
    sl->drId = ++sDrId;
    SIM_VCapSwapInfo(&sl->drSwaps, &sl->drSwapFrame, &sl->drFrame);
  }
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
  /* a capture frame at N x: drawn by the GPU, the 1x bytes the game reads from vcore on a copy (CapHi) */
  if (sl->mode == M_VCORE && CapHi(sl)) sl->mode = M_GPU, sl->capHi = 1;
  Skip2D(sl, prev); /* (prev's stage 1 is done: its state and pack stay as they are until the next call's Gather) */
  int first = !prev->has; /* nothing earlier to show: this frame is drawn now (and shown twice) */
  sl->ipN = 0;
  sl->gameNs = SIM_TickNominal();
  /* (none for a hi-res capture frame, nor for a bank shown at N x: InterpTexture draws neither) */
  const int showsBank = sOut > 1 && ((vc_eng32(sl->s, VC_ENG_A, VC_REG_DISPCNT) >> 16) & 3) == 2;
  IpTagsWanted();
  if (!first && sPipe && sl->mode == M_GPU && !sl->capHi && !showsBank && SIM_InterpWanted() &&
      (vc_eng32(sl->s, VC_ENG_A, VC_REG_DISPCNT) & 8)) {
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
    sNextOk = 1;
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
    if (sl->mode == M_VCORE) StoreCapture(sl->s, sl->cap, 0);
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

#ifdef BR_METAL
/* the present's skip (sim_main.cpp SpKeep; docs/perf.md "The same picture not sent again"): for pass C's output, the number that
 * changes whenever it is drawn into (g07_out_gen); 0 for any other picture (vcore's, uploaded): never kept */
static unsigned long long MtlPicGen(unsigned name) {
  return sC7 != NULL && sHalf == &kMetal && g07_frame_tex(sC7) != 0 && name == BR_MTL_BASE + g07_frame_tex(sC7) ? g07_out_gen(sC7) : 0;
}
#endif

__attribute__((constructor)) static void BridgeInstall(void) {
  SIM_SetRendererIdle(2, BridgeIdle);
  SIM_SetGLLostHook(BridgeGLLost);
  SIM_SetFrameRendererGL(GpuFrame);
  SIM_SetInterpTexture(InterpTexture);
  SIM_SetNextTexture(NextPicture);
#ifdef BR_METAL
  SIM_SetFrameTextureMetal(br_mtl_tex_of); /* the Metal present draws a picture by these names */
  SIM_SetFrameTextureGen(MtlPicGen);
#endif
  atexit(IpReport);
}
