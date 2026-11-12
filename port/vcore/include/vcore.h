/* port/vcore: a DS video core copied from the hardware (GBATEK): both 2D engines, the 3D geometry and rendering
 * engines, the VRAM banks and display capture, rendered in software at native resolution from a frame's captured
 * video state (a .vcs file, <ref>/VCS_FORMAT.md). The public interface; docs/vcore.md has the architecture.
 * No libntr dependency. OWNED BY THE SKELETON.
 */
#ifndef VCORE_H
#define VCORE_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct VCoreState VCoreState;

/* a zeroed state (all registers 0, VRAM / palettes / OAM clear, no 3D commands); NULL when out of memory */
VCoreState *vcore_create(void);
void vcore_destroy(VCoreState *s);

/* loads a .vcs file (replaces the input; the outputs are cleared). 0 on success, else -1 and a message in err */
int vcore_load_vcs(VCoreState *s, const char *path, char *err, size_t errlen);
int vcore_load_vcs_mem(VCoreState *s, const void *data, size_t size, char *err, size_t errlen);

/* renders the frame: both engines, the 3D, capture, the two screens */
void vcore_render(VCoreState *s);
/* the same state rendered again (its p23 blend may differ): only the 3D is redone when the last vcore_render of this state left
 * the 2D reusable; else vcore_render. vcore_forget_2d: the state's inputs changed (a new frame gathered), render in full. */
void vcore_render_again(VCoreState *s);
void vcore_forget_2d(VCoreState *s);

/* screen 0 = top, 1 = bottom: 256x192 pixels, r6 | g6 << 8 | b6 << 16 (the DS's 18-bit colour, melonDS's format) */
const uint32_t *vcore_screen(const VCoreState *s, int screen);
/* the same as RGBA8 (c8 = c6 << 2 | c6 >> 4, alpha 255): rgba holds 256 * 192 * 4 bytes */
void vcore_screen_rgba(const VCoreState *s, int screen, uint8_t *rgba);
/* both screens as one 256x384 RGBA8 picture, top first (the compare tool's layout) */
void vcore_frame_rgba(const VCoreState *s, uint8_t *rgba);
/* libntr's own picture of the frame from the .vcs (LSHT, 256x384 RGBA8), NULL if the file had none */
const uint8_t *vcore_libntr_shot(const VCoreState *s);

/* the 3D at N x the DS resolution on the CPU (p21): the scale 1..8 for the next vcore_render (default 1: the DS's pixels),
 * and the options of port/gpu's N x rules: dot_fine (a one-pixel polygon is one N x pixel, not an N x N block), line_thin
 * (lines and wire-frames 1 N x pixel wide, not N), edge_fine (edge marking 1 N x pixel wide, not N) */
void vcore_set_scale(VCoreState *s, int n);
int vcore_get_scale(const VCoreState *s);
/* supersampling: with k = 2 or 4 the 3D is drawn at N k x and vcore_picture is the k x k box average of the
 * 256 N k x 384 N k picture, 256 N x 384 N again (the 2D layers, N x replicated DS pixels, stay sharp: the box is aligned
 * to the N x grid); k = 1 is today's picture. N k is capped at 8 (VC_MAX_SCALE): vcore_aa_limit(N, k) is the k that is
 * used (lowered to 2 or 1), vcore_get_aa the one in force */
void vcore_set_aa(VCoreState *s, int k);
int vcore_get_aa(const VCoreState *s);
int vcore_aa_limit(int n, int k);
void vcore_set_options(VCoreState *s, int dot_fine, int line_thin, int edge_fine);
/* Filter=bilinear (N > 1; port/gpu's g4_filtered rule): 1 = textures bilinear on the decoded texels, 0 = nearest (the DS) */
void vcore_set_filter(VCoreState *s, int bilinear);
/* the last vcore_render's picture when it ran at N > 1: RGBA8, 256 N x 384 N (*w, *h), the top screen first, valid until
 * the next render; NULL when the frame was drawn at 1x (use vcore_frame_rgba) */
const uint8_t *vcore_picture(const VCoreState *s, int *w, int *h);

/* the N x 3D layer of the last N x render (RGB6 | alpha << 24 per pixel, 256 N x 192 N), NULL when the frame was drawn at 1x
 * or showed no 3D layer (the layer is the 1x core's f3d.out then) */
const uint32_t *vcore_layer_n(const VCoreState *s, int *w, int *h);

/* a function run on another thread, joined by the caller (p22_task.c): the threads are kept between tasks, so a frame
 * creates none. vcore_task_start returns 0 when no thread could start (run the function in the caller then). */
typedef struct VCTask {
  volatile int done;
  void *legacy; /* a thread of its own (BWPORT_VCORE_POOL=0): its pthread_t */
} VCTask;
int vcore_task_start(VCTask *t, void *(*fn)(void *), void *arg);
void vcore_task_join(VCTask *t);
int vcore_task_done(VCTask *t); /* the task has finished: a join returns at once (a thread of its own: never reported) */

/* writes an RGBA8 picture as a PNG (no compression library needed). 0 on success */
int vcore_write_png(const char *path, const uint8_t *rgba, int w, int h);

#ifdef __cplusplus
}
#endif

#endif
