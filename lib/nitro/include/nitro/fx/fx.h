// NitroSDK's nitro/fx/fx.h, the fixed point types and their arithmetic.
// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/nitro/include/nitro/fx.h, trimmed to what
// Black's C uses so far.
#ifndef BWPORT_NITRO_FX_FX_H
#define BWPORT_NITRO_FX_FX_H

#include "types.h"
#include "nitro/misc.h"

// Fixed point numbers with 12 fractional bits
typedef s16 fx16;
typedef s32 fx32;
typedef s64 fx64;
typedef s64 fx64c;
#define FX64C_SHIFT 32
#define FX16_CONST(x) ((fx16)(((x) > 0) ? ((x) * FX16_ONE + 0.5f) : ((x) * FX16_ONE - 0.5f)))
#define FX32_SHIFT 12
#define FX32_CONST(x) ((fx32)(((x) > 0) ? ((x) * FX32_ONE + 0.5f) : ((x) * FX32_ONE - 0.5f)))
#define FX32_DEC_MASK (FX32_ONE - 1)
#define FX32_MIN ((fx32)0x80000000)
#define FX_Whole(a) ((s32)((a) >> FX32_SHIFT))
#define FX_FX32_TO_F32(x) ((f32)((x) / (f32)FX32_ONE))
typedef struct {
    fx32 x;
    fx32 y;
    fx32 z;
} VecFx32;
typedef struct {
    fx16 x;
    fx16 y;
    fx16 z;
} VecFx16;
typedef union {
    struct {
        signed long _00;
        signed long _01;
        signed long _10;
        signed long _11;
    };
    signed long m[2][2];
} MtxFx22;
typedef struct {
    fx32 m[3][3];
} MtxFx33;
// The rotation and scale in _00 to _22, then the translation in _30, _31 and _32
typedef union {
    struct {
        fx32 _00, _01, _02;
        fx32 _10, _11, _12;
        fx32 _20, _21, _22;
        fx32 _30, _31, _32;
    };
    fx32 m[4][3];
    // The rotation and scale, then the translation
    struct {
        MtxFx33 rot;
        VecFx32 trans;
    } rt;
    fx32 a[12];
} MtxFx43;
typedef struct {
    fx32 m[4][4];
} MtxFx44;

// Returns the fraction of x and writes its whole part, both with the sign of x
fx32 FX_Modf(fx32 x, fx32 *whole);
static inline fx32 FX_Mul(fx32 v1, fx32 v2) {
    return (fx32)(((fx64)v1 * v2 + 0x800LL) >> FX32_SHIFT);
}

// The SDK's macro form, whose operands keep their types: an fx16 one makes a full 64-bit multiply
#define FX_MUL(v1, v2) ((fx32)(((fx64)(v1) * (v2) + 0x800LL) >> FX32_SHIFT))
#define FX32_MAX ((fx32)0x7fffffff)

// Conversions between fixed point numbers and floats, rounded to the nearest
#define FX_F32_TO_FX16(x) ((fx16)(((x) > 0) ? (fx16)((x) * FX16_ONE + 0.5f) : (fx16)((x) * FX16_ONE - 0.5f)))
#define FX_F32_TO_FX32(x) ((fx32)(((x) > 0) ? (fx32)((x) * FX32_ONE + 0.5f) : (fx32)((x) * FX32_ONE - 0.5f)))
#define FX_FX16_TO_F32(x) ((f32)((x) / (f32)FX16_ONE))

// A 2D affine matrix, its last row the translation
typedef struct {
    fx32 _00;
    fx32 _01;
    fx32 _10;
    fx32 _11;
    fx32 _20;
    fx32 _21;
} MtxFx32;
static inline fx32 FX_Mul32x64c(fx32 v32, fx64c v64c) {
    fx64c tmp = v64c * v32 + 0x80000000LL;
    return (fx32)(tmp >> FX64C_SHIFT);
}

// Sets the divider and square root unit up (fx.c)
void FX_Init(void);

// NitroSDK's static inline functions that Black's headers lacked, as a final ROM compiles them (no assertions).
// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91: include/nitro/fx/fx.h
// NitroSDK's static inline functions that Black's headers lacked, as a final ROM compiles them (no assertions).
    static inline fx32 FX32_CAST (s64 res)
    {
        SDK_WARNING(res >= FX32_MIN && res <= FX32_MAX, "FX_Mul: Overflow/Underflow");
        return (fx32)res;
    }
    static inline fx32 FX_MulInline (fx32 v1, fx32 v2)
    {
        return FX32_CAST(((s64)(v1) * v2 + 0x800LL) >> FX32_SHIFT);
    }
    static inline fx32 FX_Mul32x64cInline (fx32 v32, fx64c v64c)
    {
        fx64c tmp = v64c * v32 + 0x80000000LL;
        return FX32_CAST(tmp >> FX64C_SHIFT);
    }

fx32 FX_Mul32x64cFunc(fx32 v32, fx64c v64c);

fx32 FX_MulFunc(fx32 v1, fx32 v2);

#endif // BWPORT_NITRO_FX_FX_H
