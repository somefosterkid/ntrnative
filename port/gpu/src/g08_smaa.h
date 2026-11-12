/* port/gpu: SMAA on the 3D layer at N (g08_smaa.c; docs/gpu.md "Antialiasing: SMAA") */
#ifndef GPU_G08_SMAA_H
#define GPU_G08_SMAA_H

#include "gpu_gl.h"

typedef struct G08 G08;
/* the programs and the targets for a w x h 3D layer; NULL with the reason in err */
G08 *g08_create(int w, int h, char *err, size_t errlen);
void g08_destroy(G08 *a);
/* the edge and weight draws: pass R's samples on unit 7 and the vertex array bound, layer = pass P's 3D layer (RGBA8UI) */
void g08_run(G08 *a, GLuint layer);
/* the blending weights (RGBA8, w x h): pass C blends the 3D layer with them (shaders/g08_smaa.glsl) */
unsigned g08_weights(const G08 *a);
/* the uniforms of the edge and weight draws (aLuma, aMetrics) for a w x h layer */
void g08_uni(GpuU *u, int w, int h);
/* the area texture (RG8 160 x 560) and the search texture (R8 64 x 16), bytes */
void g08_tables(const unsigned char **area, const unsigned char **search);
/* the readbacks (gpu_read_3d): layer blended into a target of its own and read back, 4 words a pixel; 0 on success */
int g08_read(G08 *a, GLuint layer, GLuint vao, uint32_t *rgba);

#endif
