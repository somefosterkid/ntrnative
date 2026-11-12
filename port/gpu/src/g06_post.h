/* port/gpu piece G6: the uniforms and textures of the clear (pass R) and the post pass (pass P). OWNED BY G6. */
#ifndef GPU_G06_POST_H
#define GPU_G06_POST_H

#include "gpu_gl.h"
#include "vc_shared.h"

/* with prog in use: the rear-plane bitmap texture (unit 8) */
void gpu_g06_bind_raster(GLuint prog, const VCoreState *s);
/* the rear-plane bitmap (65536 texels: slot 2's colour | slot 3's depth/fog << 16) into img when DISP3DCNT turns it on: 1, else 0 */
int gpu_g06_clr_image(const VCoreState *s, u32 *img);
/* pass R's: CLRIMAGE_OFFSET */
void gpu_g06_uni_raster(GpuU *u, const VCoreState *s);
/* the rear-plane bitmap texture is deleted (the GpuCore goes); a new one is made on the next use */
void gpu_g06_release(void);
/* pass P's: edge colours, fog colour/offset/table, the clear colour, the Edge option (edge_fine: 1 = outline of one N x pixel, 0 = one DS pixel) */
void gpu_g06_uni_post(GpuU *u, const VCoreState *s, int edge_fine);

#endif
