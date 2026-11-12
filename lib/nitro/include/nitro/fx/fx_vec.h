// NitroSDK's nitro/fx/fx_vec.h, the vectors.
// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/nitro/include/nitro/fx.h, trimmed to what
// Black's C uses so far.
#ifndef BWPORT_NITRO_FX_FX_VEC_H
#define BWPORT_NITRO_FX_FX_VEC_H

#include "nitro/fx/fx.h"

static inline void VEC_Set(VecFx32 *v, fx32 x, fx32 y, fx32 z) {
    v->x = x;
    v->y = y;
    v->z = z;
}
void VEC_Add(const VecFx32 *a, const VecFx32 *b, VecFx32 *ab);
void VEC_Subtract(const VecFx32 *a, const VecFx32 *b, VecFx32 *ab);
void VEC_Normalize(const VecFx32 *src, VecFx32 *dest);
fx32 VEC_Distance(const VecFx32 *a, const VecFx32 *b);
void VEC_MultAdd(fx32 scale, const VecFx32 *a, const VecFx32 *b, VecFx32 *dest);
fx32 VEC_Mag(const VecFx32 *v);
fx32 VEC_DotProduct(const VecFx32 *a, const VecFx32 *b);

// NitroSDK's VEC_CrossProduct
void VEC_CrossProduct(const VecFx32 *a, const VecFx32 *b, VecFx32 *axb);

// NitroSDK's VEC_Fx16 functions
fx32 VEC_Fx16DotProduct(const VecFx16 *a, const VecFx16 *b);
void VEC_Fx16CrossProduct(const VecFx16 *a, const VecFx16 *b, VecFx16 *axb);
fx32 VEC_Fx16Mag(const VecFx16 *v);
void VEC_Fx16Normalize(const VecFx16 *src, VecFx16 *dest);

// NitroSDK's VEC_Fx16Add
void VEC_Fx16Add(const VecFx16 *a, const VecFx16 *b, VecFx16 *ab);
static inline void VEC_Fx16Set(VecFx16 *v, fx16 x, fx16 y, fx16 z) {
    v->x = x;
    v->y = y;
    v->z = z;
}

#endif // BWPORT_NITRO_FX_FX_VEC_H
