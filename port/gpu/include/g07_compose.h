/* port/gpu G7: engine A's compositor at N x (pass C, shaders/g07_compose.frag; docs/gpu.md, "The 2D engines and
 * compositing"). OWNED BY G7.
 *
 * Engine A: vcore draws the 2D layers (OBJs, BGs, windows) at 1x on the CPU (the halves of vc_render_line_a1:
 * g07_stage_build), the shader reads them nearest-neighbour at the DS pixel (x / N, y / N) and the 3D layer at the N x
 * pixel (shifted by BG0HOFS times N), and applies p09's compositing (priorities, windows, blending with the 3D layer's
 * own alpha, semi-transparent and bitmap OBJs) and p10's display (mode 1, master brightness, forced blank / power).
 * Display modes 0, 2, 3 of engine A (and a powered-off or blanked A) are vcore's 1x line (vc_display_line), and engine B
 * is vcore's 1x screen (s->eng_out[1], from vcore_render), both drawn at N x by nearest neighbour.
 *
 * Needs a current GL context and gpu_create (or gpu_gl_load) to have loaded the GL table. The result is one texture,
 * 256 N x 384 N, RGBA8 (8-bit colours v6 << 2 | v6 >> 4, alpha 255), GL row r = screen row r: read back, row 0 is the
 * TOP screen's first row (the engine POWCNT1 bit 15 puts on top first), as the skeleton's gpu_read_3d.
 */
#ifndef G07_COMPOSE_H
#define G07_COMPOSE_H

#include "gpu.h"
#include "vc_types.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef struct G07 G07;

/* scale 1..GPU_MAX_SCALE; NULL on failure with the reason in err */
G07 *g07_create(int scale, char *err, size_t errlen);
/* the same on the Metal core (gpu.h GPU_API_METAL): its texture names are the core's (gpu_mtl_texture) */
G07 *g07_create_metal(int scale, char *err, size_t errlen);
void g07_destroy(G07 *c);

/* engine A's 2D layers for the 192 lines of s (VC_H VC_Line records, as s->p20.stage after vc_render_line_a1): OBJs,
 * BGs (the 3D BG only marked), windows. The 3D layer is not needed. s's VRAM must be mapped (vc_vram_map). */
void g07_stage_build(VCoreState *s, VC_Line *stage);

/* a texture holding a 3D layer from the CPU: l3d is (256 sc) x (192 sc) pixels, RGB6 | alpha5 << 24 (vcore's s->f3d.out
 * is sc = 1). Returns the texture id to give g07_compose as tex3d (owned by c; the next call replaces it), 0 on failure. */
unsigned g07_upload_3d(G07 *c, const uint32_t *l3d, int sc);

/* pass C: both screens at N x from s (registers, palette, VRAM for the 1x lines), the 2D layers stage (192 lines; NULL
 * = built here with g07_stage_build) and the 3D layer: tex3d is a GL_RGBA8UI texture (r6, g6, b6, a5), (256 sc) x (192
 * sc), DS row 0 at GL row 0 (the skeleton's pass P output with sc = N, or g07_upload_3d's with any sc); 0 = no 3D layer
 * (BG0 is then transparent). s->eng_out[1] must hold engine B's picture (vcore_render ran, or its 2D for engine B).
 * smaa: SMAA's blending weights for tex3d (gpu_smaa_weights), or 0: pass C reads the layer through SMAA's last pass.
 * 0 on success, else -(GL error). */
int g07_compose(G07 *c, VCoreState *s, const VC_Line *stage, unsigned tex3d, int sc, unsigned smaa);

/* what pass C uploads, packed on any thread (the port's worker): engine A's 2D layers from stage (one RGBA32UI texel a DS
 * pixel) and the 1x lines it does not compose (engine A's when it shows no compositor picture, engine B's s->eng_out[1]) */
typedef struct G07Pack G07Pack;
G07Pack *g07_pack_create(void);
void g07_pack_destroy(G07Pack *k);
void g07_pack(G07Pack *k, VCoreState *s, const VC_Line *stage);
/* g07_pack with an engine's half taken from an earlier pack instead (its inputs unchanged: port/bridge's 2D skip):
 * reuse bit 0 engine A's (stage unused), bit 1 engine B's (s->eng_out[1] unused); from != k. A new serial */
void g07_pack_reuse(G07Pack *k, VCoreState *s, const VC_Line *stage, const G07Pack *from, int reuse);
/* which of the halves in mask (bit 0 A, bit 1 B) differ between a and b, as pass C reads them for s (BWPORT_2D_SKIPCHECK) */
int g07_pack_differs(const G07Pack *a, const G07Pack *b, VCoreState *s, int mask);
/* g07_compose from k (packed from the same s): the textures are not uploaded again when k's last pack is the one up (the
 * frame's in-between pictures and the frame itself) */
int g07_compose_packed(G07 *c, VCoreState *s, const G07Pack *k, unsigned tex3d, int sc, unsigned smaa);

/* the output texture of the last g07_compose (256 N x 384 N, RGBA8) */
unsigned g07_frame_tex(const G07 *c);
/* the Metal core's: the last pass C's GPU milliseconds once its command buffer is done (gpu_render --time); 0 under GL */
double g07_gpu_ms(const G07 *c);
/* the last output read back: (256 N) x (384 N) RGBA bytes, top row first (the top screen above the bottom one) */
void g07_read_frame(G07 *c, uint8_t *rgba);

#ifdef __cplusplus
}
#endif

#endif
