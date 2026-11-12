// Black's code at ov021 0x021c7de0-0x021c7f70: the rest of field_flash.c (pokebw2's src/ov036/field_flash.c), the
// darkness of a dark cave around the player, that Flash widens. No name is known: the module and address.

#include "types.h"
#include "constants/sound.h"
#include "field/field_flash.h"
#include "field/ov021_021c7de0.h"
#include "gfl/heap.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_cp.h"

void FieldFlashAnimation_Reset(FieldFlashAnimation *animation);
void func_ov021_021c7e70(FieldFlashAnimation *animation, fx32 scaleStart, fx32 scaleEnd, s16 alphaStart, u8 alphaEnd,
                         int frameCount);
BOOL FieldFlashAnimation_Update(FieldFlashAnimation *animation);
fx32 FieldFlashAnimation_GetScale(FieldFlashAnimation *animation);
void FieldFlashAnimation_SetScale(FieldFlashAnimation *animation, fx32 scale);
u8 FieldFlashAnimation_GetAlpha(FieldFlashAnimation *animation);
void FieldFlashAnimation_SetAlpha(FieldFlashAnimation *animation, u8 alpha);
void func_ov021_021c7efc(FieldFlash *flash);
void func_ov021_021c7f18(FieldFlash *flash);
void func_ov021_021c7f44(FieldFlash *flash);
void func_ov021_021c7f60(FieldFlash *flash);
void (*const FIELD_FLASH_FUNCTIONS[4])(FieldFlash *flash) = {func_ov021_021c7efc, func_ov021_021c7f18,
                                                             func_ov021_021c7f44, func_ov021_021c7f60};

void func_ov021_021c7de0(FieldFlash *flash) {
    if (flash != NULL) {
        FieldFlashAnimation_Reset(&flash->animation);
        FieldFlashG3D_Free(flash->g3d);
        GFL_HeapFree(flash);
    }
}

void func_ov021_021c7dfc(FieldFlash *flash) {
    if (flash != NULL) {
        if (flash->nextState != 4) {
            FIELD_FLASH_FUNCTIONS[flash->nextState](flash);
            flash->nextState = 4;
        }
        if (flash->state == 2 && FieldFlashAnimation_Update(&flash->animation)) {
            flash->state = 3;
        }
    }
}

void func_ov021_021c7e30(FieldFlash *flash) {
    if (flash != NULL) {
        FieldFlashG3D_Draw(flash->g3d, FieldFlashAnimation_GetScale(&flash->animation),
                           FieldFlashAnimation_GetAlpha(&flash->animation));
    }
}

void func_ov021_021c7e50(FieldFlash *flash, u16 nextState) {
    flash->nextState = nextState;
}

u32 func_ov021_021c7e54(FieldFlash *flash) {
    return flash->state;
}

void FieldFlashAnimation_Init(FieldFlashAnimation *animation) {
    MI_CpuFill8(animation, 0, sizeof(FieldFlashAnimation));
}

void FieldFlashAnimation_Reset(FieldFlashAnimation *animation) {
    MI_CpuFill8(animation, 0, sizeof(FieldFlashAnimation));
}

void func_ov021_021c7e70(FieldFlashAnimation *animation, fx32 scaleStart, fx32 scaleEnd, s16 alphaStart, u8 alphaEnd,
                         int frameCount) {
    animation->scale = scaleStart;
    animation->scaleStart = scaleStart;
    animation->scaleDelta = scaleEnd - scaleStart;
    animation->alpha = alphaStart;
    animation->alphaStart = alphaStart;
    animation->alphaDelta = alphaEnd - alphaStart;
    animation->frame = 0;
    animation->frameCount = frameCount;
}

BOOL FieldFlashAnimation_Update(FieldFlashAnimation *animation) {
    if (animation->frame < animation->frameCount) {
        animation->frame++;
        animation->scale = animation->scaleStart +
                           FX_Div(FX_MUL(animation->scaleDelta, animation->frame << 12), animation->frameCount << 12);
        animation->alpha = animation->alphaStart + animation->alphaDelta * animation->frame / animation->frameCount;
        return FALSE;
    }
    return TRUE;
}

fx32 FieldFlashAnimation_GetScale(FieldFlashAnimation *animation) {
    return animation->scale;
}

void FieldFlashAnimation_SetScale(FieldFlashAnimation *animation, fx32 scale) {
    animation->scale = scale;
}

u8 FieldFlashAnimation_GetAlpha(FieldFlashAnimation *animation) {
    return animation->alpha;
}

void FieldFlashAnimation_SetAlpha(FieldFlashAnimation *animation, u8 alpha) {
    animation->alpha = alpha;
}

void func_ov021_021c7efc(FieldFlash *flash) {
    flash->state = 1;
    FieldFlashAnimation_SetScale(&flash->animation, 0x3000);
    FieldFlashAnimation_SetAlpha(&flash->animation, 0x1f);
}

void func_ov021_021c7f18(FieldFlash *flash) {
    flash->state = 2;
    func_ov021_021c7e70(&flash->animation, 0x3000, 0xaa00, 0x1f, 0x1f, 0xf);
    GFL_SndSEPlay(SEQ_SE_FLD_127);
}

void func_ov021_021c7f44(FieldFlash *flash) {
    flash->state = 3;
    FieldFlashAnimation_SetScale(&flash->animation, 0xaa00);
    FieldFlashAnimation_SetAlpha(&flash->animation, 0x1f);
}

void func_ov021_021c7f60(FieldFlash *flash) {
    flash->state = 0;
    FieldFlashAnimation_SetScale(&flash->animation, 0);
}
