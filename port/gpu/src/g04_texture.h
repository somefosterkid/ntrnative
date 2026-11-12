/* port/gpu piece G4: the texture slots' upload (docs/gpu.md, G4). OWNED BY G4. */
#ifndef GPU_G04_TEXTURE_H
#define GPU_G04_TEXTURE_H

#include "gpu_gl.h"
#include "vc_shared.h"

/* t (gpu_texdata_build: VC_RGN_TEX, 128K words, and VC_RGN_TEXPAL, 24K words + the 16 words of TOON_TABLE) into the data
 * textures t_tex[*cur] and t_pal[*cur] (GPU_RING of each, made on the first use), the rows that changed; nothing when
 * up[*cur] is t's build (an in-between picture of the frame up), else *cur moves to the next pair first (gpu_gl.h,
 * GPU_RING) and up[*cur] = t's build */
void gpu_g04_upload(GLuint *t_tex, GLuint *t_pal, unsigned *up, int *cur, const GpuTexData *t);
/* bind them to texture units unit and unit + 1 of prog (uniforms uTex, uTexPal) */
void gpu_g04_bind(GLuint prog, int unit, GLuint t_tex, GLuint t_pal);
/* t's words for a core that uploads them itself (the Metal core): the texture slots (ntex R32UI texels), the palette slots and the
 * toon table (npal), the build's serial */
void gpu_texdata_bytes(const GpuTexData *t, const void **tex, int *ntex, const void **pal, int *npal, unsigned *serial);

#endif
