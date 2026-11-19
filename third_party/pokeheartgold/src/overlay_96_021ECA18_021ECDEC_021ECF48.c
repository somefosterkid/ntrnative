#include "global.h"

#include "error_handling.h"
#include "math_util.h"
#include "sprite_system.h"

typedef struct UnkStruct_ov96_021ECF48 {
    ManagedSprite *sprite;
    fx32 amplitude;
    fx32 velY;
    fx32 x;
    fx32 y;
    u32 angle : 16;
    u32 speed : 4;
    u32 unk_14_20 : 1;
    u32 unk_14_21 : 1;
    u32 unk_14_22 : 10;
} UnkStruct_ov96_021ECF48;

BOOL ov96_021ECF48(UnkStruct_ov96_021ECF48 *param0);

BOOL ov96_021ECF48(UnkStruct_ov96_021ECF48 *param0) {
    BOOL result = FALSE;
    fx32 sin;
    fx32 amplitude;

    if (param0 == NULL) {
        GF_AssertFail();
    }
    amplitude = param0->amplitude;
    sin = GF_SinDeg(param0->angle * param0->speed);
    param0->x += FX32_CAST(((fx64)sin * amplitude + 0x800LL) >> 12);
    param0->y += param0->velY;
    ManagedSprite_SetPositionFxXYWithSubscreenOffset(param0->sprite, param0->x, param0->y, FX32_CONST(524));
    if ((param0->y >> FX32_SHIFT) > 0xE0) {
        if (param0->unk_14_21) {
            param0->unk_14_20 = 0;
            Sprite_DeleteAndFreeResources(param0->sprite);
            param0->sprite = NULL;
        } else {
            param0->unk_14_21 = 1;
            result = TRUE;
        }
    }
    param0->angle += MTRandom() % 2;
    return result;
}
