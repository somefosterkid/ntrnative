/* port/gpu: the programs' uniforms, one std140 block (shaders/g00_head.glsl's GpuU: the same fields in the same order; the
 * arrays' elements and the vec4 are 16 bytes apart, the scalars 4). A pass fills the fields its program reads and hands the
 * whole to the API: gpu_gl_uniforms (a uniform buffer at binding 0), the Metal core (buffer 0). No GL here: the Metal core
 * includes it. */
#ifndef GPU_UNI_H
#define GPU_UNI_H

#include <stdint.h>

typedef struct GpuU {
  uint32_t uEdgeCol[8][4]; /* [i][0] */
  uint32_t uFogTab[32][4]; /* [i][0] */
  float aMetrics[4];
  int32_t uScale, uSnap, uBandRows;
  uint32_t uDisp3d, uSwap, uClearColor, uClearDepth, uAlphaRef;
  int32_t uTilesX, uTileW;
  uint32_t uFlat, uFilter, uClrOff;
  int32_t uStage;
  uint32_t uFogColor, uFogOffset, uEdgeFine;
  int32_t cScale, cScale3d;
  uint32_t cHave3d, cSmaaOn, cCompose, cATop, cObjOn, cPrios, cBld;
  int32_t cEva, cEvb, cEvy;
  uint32_t cBackdrop;
  int32_t cHofs;
  uint32_t cBright;
  int32_t uNFind;
  uint32_t uZero;
  int32_t uFirst, uPart, uN, uR;
  float aLuma;
  uint32_t pad[1]; /* std140: the block's size is a multiple of 16 */
} GpuU;

_Static_assert(sizeof(GpuU) == 816, "GpuU: std140 layout of shaders/g00_head.glsl's block");

#endif
