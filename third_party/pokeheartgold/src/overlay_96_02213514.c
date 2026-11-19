#include "global.h"

void ov96_02213514(VecFx32 *arg0, VecFx32 *arg1);

void ov96_02213514(VecFx32 *pSrc, VecFx32 *arg1) {
    VecFx32 v1;
    VecFx32 v0;

    VEC_Normalize(pSrc, &v1);
    VEC_Normalize(arg1, &v0);
    VEC_DotProduct(&v1, &v0);
}
