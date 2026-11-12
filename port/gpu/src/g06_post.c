/* port/gpu piece G6: the CPU side of the clear and the post pass (docs/gpu.md, G6). OWNED BY G6.
 * The rear-plane bitmap (texture slots 2 and 3, one texel per pixel: colour | depth/fog << 16), and the registers the
 * post pass reads (edge colours, fog colour/offset/table). Bound by g00_frame.c next to the skeleton's uniforms. */
#include "g06_post.h"

#include <stdlib.h>
#include <string.h>

enum { G06_CLR_UNIT = 8 };

static GLuint t_clr; /* the bitmap texture (one GpuCore per process on the render thread) */

void gpu_g06_release(void) {
  if (t_clr == 0) return;
  gpu_gl_data_forget(t_clr);
  GL(DeleteTextures)(1, &t_clr);
  t_clr = 0;
}

int gpu_g06_clr_image(const VCoreState *s, u32 *img) {
  if (!(vc_io16(s, VC_REG_DISP3DCNT) & 0x4000)) return 0;
  const VC_Region *tex = &s->rgn[VC_RGN_TEX];
  for (u32 i = 0; i < 65536; i++)
    img[i] = vc_rd16(tex, 2 * 0x20000 + i * 2) | (u32)vc_rd16(tex, 3 * 0x20000 + i * 2) << 16;
  return 1;
}

void gpu_g06_uni_raster(GpuU *u, const VCoreState *s) { u->uClrOff = vc_io16(s, VC_REG_CLRIMAGE_OFFSET); }

void gpu_g06_bind_raster(GLuint prog, const VCoreState *s) {
  static u32 img[65536];
  /* gpu_gl_data_tex binds the texture it makes to the ACTIVE unit: without this it replaced the palette texture G4
   * had bound to the unit before (the first frame, and every frame with the rear-plane bitmap), so every palette
   * format sampled black on real frames */
  GL(ActiveTexture)(GL_TEXTURE0 + G06_CLR_UNIT);
  if (gpu_g06_clr_image(s, img)) {
    gpu_gl_data_tex_diff(&t_clr, GL_R32UI, GL_RED_INTEGER, GL_UNSIGNED_INT, 4, img, 65536);
  } else if (t_clr == 0) {
    gpu_gl_data_tex(&t_clr, GL_R32UI, GL_RED_INTEGER, GL_UNSIGNED_INT, 4, NULL, 1);
  }
  GL(ActiveTexture)(GL_TEXTURE0 + G06_CLR_UNIT);
  GL(BindTexture)(GL_TEXTURE_2D, t_clr);
  GL(Uniform1i)(gpu_gl_uniform(prog, "uClrImg"), G06_CLR_UNIT);
  GL(ActiveTexture)(GL_TEXTURE0);
}

void gpu_g06_uni_post(GpuU *u, const VCoreState *s, int edge_fine) {
  for (int i = 0; i < 8; i++) u->uEdgeCol[i][0] = vc_io16(s, VC_REG_EDGE_COLOR + i * 2) & 0x7FFF;
  for (int i = 0; i < 32; i++) u->uFogTab[i][0] = vc_io8(s, VC_REG_FOG_TABLE + i) & 0x7F;
  u->uFogColor = vc_io32(s, VC_REG_FOG_COLOR);
  u->uFogOffset = vc_io16(s, VC_REG_FOG_OFFSET) & 0x7FFF;
  u->uClearColor = vc_io32(s, VC_REG_CLEAR_COLOR);
  /* Edge=ds (default): the outline is one DS pixel wide; Edge=fine: one N x pixel (the option comes from the caller) */
  u->uEdgeFine = edge_fine != 0;
}
