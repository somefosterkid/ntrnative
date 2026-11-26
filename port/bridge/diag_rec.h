/* Record diagnostics (Settings > Developer on iOS, Debug on macOS, the cfgscript's diagrec=S): S seconds of what Smooth motion did,
 * in memory, written to one file at the end. Per present slot: when it went out against its slot, what became of it (sent, kept by the
 * present's skip, dropped, not presented) and which picture it showed (the renderer call, the slot's index, so the two keyframes it
 * blends and f); per renderer call: the game's V-blank and 3D swap counts, IpGather's outcome (repeat, pair, cut, no pair), the
 * keyframe sequence numbers it pairs and each slot's f; and every new keyframe's video state (registers, palettes, OAM, the VRAM
 * pages changed since the keyframe before, the 3D command stream), from which tools/diagrec.py rebuilds .vcs files for
 * port/vcore/tools/vcore_ipsweep. The file: "BWDG", version 1, then chunks as in a .vcs (4-byte tag, u32 size, data, padded to 4):
 * MANI (key=value lines: build, device, settings, panel Hz, Low Power Mode, heat state), LOG (the T and P lines), KEY (one keyframe;
 * diag_rec.c's DrKeyHead). Not recording, each hook is one load and a compare. The recording core is plain C (any platform); the
 * buttons and the share sheet are the platforms' (sim_gui.cpp's Debug menu on macOS, port/src/ios_settings.m on iOS). */
#ifndef DIAG_REC_H
#define DIAG_REC_H

#include "vc_shared.h"

#ifdef __cplusplus
extern "C" {
#endif

/* 2 while recording (the hooks test it first) */
extern volatile int bwport_diagrec_state;

/* seconds from the next present on (after a 1 s settle); done(path) runs on the writer's thread once the file is out (path NULL: it
 * could not be written). 0: one is running already, or no memory */
int bwport_diagrec_start(double seconds, void (*done)(const char *path));
/* the whole seconds left (rounded up); 0 none, -1 waiting to start, -2 writing */
int bwport_diagrec_left(void);
/* the last recording's result ("" none) */
const char *bwport_diagrec_message(void);
/* stop a recording now and write it before returning (the cfgscript's quit) */
void bwport_diagrec_flush(void);

/* the GPU bridge's hooks (port/bridge/gpu_bridge.c) */
typedef struct DrTickInfo {
  long long nominal, serial, gap, workNs;
  unsigned id, frame, swaps, swapFrame;
  int ipN, why, nl, nv, valid;
  unsigned blend; /* bit idx: slot idx's picture blended */
  float f[9];     /* idx 1..8 */
} DrTickInfo;
void bwport_diagrec_tick(const DrTickInfo *t);
/* a new keyframe (IpGather took s's lists as KEY): its sequence number (-1 not recording) */
int bwport_diagrec_key(const VCoreState *s, unsigned id, unsigned frame, unsigned swapFrame, unsigned swaps, int haveOld);
/* the GL thread draws picture idx of renderer call id (idx 0: a call with no in-between slots); r3: its 3D layer was the one there */
void bwport_diagrec_shows(unsigned id, int idx, unsigned id3d, int r3);

#ifdef __cplusplus
}
#endif

#endif
