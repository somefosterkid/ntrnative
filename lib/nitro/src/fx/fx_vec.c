#include "nitro/cp.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_vec.h"

// NitroSDK's vector math. The lengths come from the divider and square root unit, t2 being the squared
// length with 24 fractional bits

void VEC_Add(const VecFx32 *a, const VecFx32 *b, VecFx32 *ab) {
    ab->x = a->x + b->x;
    ab->y = a->y + b->y;
    ab->z = a->z + b->z;
}

void VEC_Subtract(const VecFx32 *a, const VecFx32 *b, VecFx32 *ab) {
    ab->x = a->x - b->x;
    ab->y = a->y - b->y;
    ab->z = a->z - b->z;
}

void VEC_Fx16Add(const VecFx16 *a, const VecFx16 *b, VecFx16 *ab) {
    ab->x = a->x + b->x;
    ab->y = a->y + b->y;
    ab->z = a->z + b->z;
}

fx32 VEC_DotProduct(const VecFx32 *a, const VecFx32 *b) {
    return (fx32)(((fx64)a->x * b->x + (fx64)a->y * b->y + (fx64)a->z * b->z + FX32_HALF) >> FX32_SHIFT);
}

fx32 VEC_Fx16DotProduct(const VecFx16 *a, const VecFx16 *b) {
    fx32 t1 = a->x * b->x + a->y * b->y;
    fx32 t2 = a->z * b->z + FX32_HALF;

    return (fx32)(((fx64)t1 + t2) >> FX32_SHIFT);
}

void VEC_CrossProduct(const VecFx32 *a, const VecFx32 *b, VecFx32 *axb) {
    fx32 x = (fx32)(((fx64)a->y * b->z - (fx64)a->z * b->y + FX32_HALF) >> FX32_SHIFT);
    fx32 y = (fx32)(((fx64)a->z * b->x - (fx64)a->x * b->z + FX32_HALF) >> FX32_SHIFT);
    fx32 z = (fx32)(((fx64)a->x * b->y - (fx64)a->y * b->x + FX32_HALF) >> FX32_SHIFT);

    axb->x = x;
    axb->y = y;
    axb->z = z;
}

void VEC_Fx16CrossProduct(const VecFx16 *a, const VecFx16 *b, VecFx16 *axb) {
    fx16 x = (fx16)((a->y * b->z - a->z * b->y + FX32_HALF) >> FX32_SHIFT);
    fx16 y = (fx16)((a->z * b->x - a->x * b->z + FX32_HALF) >> FX32_SHIFT);
    fx16 z = (fx16)((a->x * b->y - a->y * b->x + FX32_HALF) >> FX32_SHIFT);

    axb->x = x;
    axb->y = y;
    axb->z = z;
}

fx32 VEC_Mag(const VecFx32 *v) {
    fx64 t2 = (fx64)v->x * v->x;

    t2 += (fx64)v->y * v->y;
    t2 += (fx64)v->z * v->z;
    // The root of 4 * t2 is twice the length, with one more bit to round it
    reg_CP_SQRTCNT = 1; // 64-bit
    reg_CP_SQRT_PARAM = (u64)(t2 * 4);
    while (reg_CP_SQRTCNT & REG_CP_SQRTCNT_BUSY_MASK) {
    }
    return ((fx32)reg_CP_SQRT_RESULT + 1) >> 1;
}

fx32 VEC_Fx16Mag(const VecFx16 *v) {
    fx64 t2 = v->x * v->x;

    t2 += v->y * v->y;
    t2 += v->z * v->z;
    reg_CP_SQRTCNT = 1; // 64-bit
    reg_CP_SQRT_PARAM = (u64)(t2 * 4);
    while (reg_CP_SQRTCNT & REG_CP_SQRTCNT_BUSY_MASK) {
    }
    return ((fx32)reg_CP_SQRT_RESULT + 1) >> 1;
}

void VEC_Normalize(const VecFx32 *src, VecFx32 *dest) {
    fx64 t2 = (fx64)src->x * src->x;
    fx64 t;
    s32 sqrt;

    t2 += (fx64)src->y * src->y;
    t2 += (fx64)src->z * src->z;
    // t is 2^45 / length: 2^56 / t2 is 2^32 / length^2, and the root of 4 * t2 is length * 2^13
    reg_CP_DIVCNT = 2; // 64-bit by 64-bit
    reg_CP_DIV_NUMER = 1LL << 56;
    reg_CP_DIV_DENOM = t2;
    reg_CP_SQRTCNT = 1; // 64-bit
    reg_CP_SQRT_PARAM = (u64)(t2 * 4);
    while (reg_CP_SQRTCNT & REG_CP_SQRTCNT_BUSY_MASK) {
    }
    sqrt = (s32)reg_CP_SQRT_RESULT;
    while (reg_CP_DIVCNT & REG_CP_DIVCNT_BUSY_MASK) {
    }
    t = reg_CP_DIV_RESULT;
    t *= sqrt;
    dest->x = (fx32)((t * src->x + (1LL << 44)) >> 45);
    dest->y = (fx32)((t * src->y + (1LL << 44)) >> 45);
    dest->z = (fx32)((t * src->z + (1LL << 44)) >> 45);
}

void VEC_Fx16Normalize(const VecFx16 *src, VecFx16 *dest) {
    fx64 t2 = src->x * src->x;
    fx64 t;
    s32 sqrt;

    t2 += src->y * src->y;
    t2 += src->z * src->z;
    reg_CP_DIVCNT = 2; // 64-bit by 64-bit
    reg_CP_DIV_NUMER = 1LL << 56;
    reg_CP_DIV_DENOM = t2;
    reg_CP_SQRTCNT = 1; // 64-bit
    reg_CP_SQRT_PARAM = (u64)(t2 * 4);
    while (reg_CP_SQRTCNT & REG_CP_SQRTCNT_BUSY_MASK) {
    }
    sqrt = (s32)reg_CP_SQRT_RESULT;
    while (reg_CP_DIVCNT & REG_CP_DIVCNT_BUSY_MASK) {
    }
    t = reg_CP_DIV_RESULT;
    t *= sqrt;
    dest->x = (fx16)((t * src->x + (1LL << 44)) >> 45);
    dest->y = (fx16)((t * src->y + (1LL << 44)) >> 45);
    dest->z = (fx16)((t * src->z + (1LL << 44)) >> 45);
}

void VEC_MultAdd(fx32 scale, const VecFx32 *a, const VecFx32 *b, VecFx32 *dest) {
    dest->x = (fx32)(((fx64)scale * a->x) >> FX32_SHIFT) + b->x;
    dest->y = (fx32)(((fx64)scale * a->y) >> FX32_SHIFT) + b->y;
    dest->z = (fx32)(((fx64)scale * a->z) >> FX32_SHIFT) + b->z;
}

fx32 VEC_Distance(const VecFx32 *a, const VecFx32 *b) {
    fx32 x = a->x - b->x;
    fx32 y = a->y - b->y;
    fx32 z = a->z - b->z;
    fx64 t2 = (fx64)x * x;

    t2 += (fx64)y * y;
    t2 += (fx64)z * z;
    reg_CP_SQRTCNT = 1; // 64-bit
    reg_CP_SQRT_PARAM = (u64)(t2 * 4);
    while (reg_CP_SQRTCNT & REG_CP_SQRTCNT_BUSY_MASK) {
    }
    return ((fx32)reg_CP_SQRT_RESULT + 1) >> 1;
}
