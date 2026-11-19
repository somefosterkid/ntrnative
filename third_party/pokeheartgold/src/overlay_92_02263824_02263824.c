#include "global.h"

#include "overlay_92.h"

s32 ov92_022634F8(void *, MtxFx44 *);

void ov92_02263824(void *arg0, MtxFx43 *pDst) {
    MtxFx44 pSrc;

    ov92_022634F8(arg0, &pSrc);
    MTX_Copy44To43_(&pSrc, pDst);
}
