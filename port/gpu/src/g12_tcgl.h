/* port/gpu piece G12 (texture cache, docs/gpu.md): the GL half. OWNED BY G12.
 * The five layer arrays (GL_TEXTURE_2D_ARRAY, RGBA8, 32-512 texels a side; NEAREST, one level for now), the upload queue applied
 * up to the frame drawn, and the per-polygon table uTcPoly (one RGBA32UI texel per polygon of the prep, as uPoly: x = ready(1) |
 * bucket << 1 | layer << 16, y, z = the texture's width and height, w = 1 / S as float bits; 0 = the texture slots' path).
 * Units 10-14 = uTc0..uTc4, 15 = uTcPoly (shaders/g12_texcache.glsl). */
#ifndef GPU_G12_TCGL_H
#define GPU_G12_TCGL_H

#include "g00_frame.h"
#include "g12_tcframe.h"

enum { G12_UNIT = 10, G12_POLY_TEXELS = 4 }; /* uTcPoly: the mapping texel, the s, t and 1/w planes (shaders/g12_texcache.glsl) */

typedef struct G12Gl G12Gl;

G12Gl *g12_gl_create(void);
void g12_gl_destroy(G12Gl *t);
/* frame f of c for prep p (or an in-between picture of f's frame): takes c's generation (or restarts c), applies c's queue up to
 * f's frame, fills the per-polygon table into ring entry ri. The number of p's polygons that sample a layer (> 0: bind and draw
 * with a G_TEXCACHE program), 0 when the frame draws without the cache. */
int g12_gl_prepare(G12Gl *t, GpuTexCache *c, const GpuTcFrame *f, const GpuPrep *p, int ri);
/* the arrays and ring entry ri's table on units 10-15 of prog */
void g12_gl_bind(G12Gl *t, GLuint prog, int ri);
/* the arrays' sampling from now on: filtered 0 = exact (nearest, level 0: BWPORT_GPU_TEXCACHE=exact), 1 = TexMag (texmag 1:
 * bilinear magnification), the mip chain trilinear when the layers have one, aniso (1 = off; capped by the GPU's maximum) */
void g12_gl_filter(G12Gl *t, int filtered, int texmag, int aniso);

#endif
