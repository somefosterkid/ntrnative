#include "global.h"

#include "constants/heap.h"

#include "camera.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "overlay_01_021EFB38.h"
#include "overlay_120.h"
#include "screen_fade.h"
#include "sprite.h"
#include "unk_02009D48.h"

typedef enum FadeType FadeType;

// Work area of the ov119_0225F670 encounter effect (EncounterEffect.param), 0x1e8 bytes on the DS.
typedef struct Ov119_0225F670Work {
    QuadraticInterpolationTaskFX32 scale;
    LinearInterpolationTaskS32 blendAlpha;
    LinearInterpolationTaskS32 rotation;
    ScreenShakeEffect screenShake;
    BOOL screenShakeActive;
    UnkStruct_ov5_021DE47C spriteEnv;
    UnkStruct_ov5_021DE5A4 spriteRes;
    Sprite *sprites[2];
    Camera *camera;
    QuadraticInterpolationTaskFX32 cameraDistance;
    int shakeDelay;
} Ov119_0225F670Work;

void ov01_021F1310(ScreenShakeEffect *screenShake, u32 interval);

void ov119_0225F670(SysTask *task, EncounterEffect *effect);

void ov119_0225F670(SysTask *task, EncounterEffect *effect) {
    s32 i;
    Ov119_0225F670Work *work;
    s32 done;
    u32 rot;
    fx32 dist;
    u32 scale;
    VecFx32 vec1;
    VecFx32 vec2;
    VecFx32 tmp1;
    VecFx32 tmp2;

    work = effect->param;
    switch (effect->state) {
    case 0:
        effect->param = Heap_Alloc(HEAP_ID_FIELD1, sizeof(Ov119_0225F670Work));
        memset(effect->param, 0, sizeof(Ov119_0225F670Work));
        work = effect->param;
        work->camera = effect->fieldSystem->camera;
        ov01_021F12B4(&work->screenShake, HEAP_ID_FIELD1);
        work->shakeDelay = 12;
        ov01_021F05C4(&work->spriteEnv, 2, 1);
        ov01_021F0614(effect->narc, &work->spriteEnv, &work->spriteRes, 0, 1, 7, 9, 8, 600000);
        for (i = 0; i < 2; i++) {
            work->sprites[i] = ov01_021F0718(&work->spriteEnv, &work->spriteRes, 0x80000, 0x60000, 0, 0);
            Sprite_SetDrawFlag(work->sprites[i], 0);
            Sprite_SetDrawPriority(work->sprites[i], i);
        }
        GfGfx_EngineATogglePlanes(0x10, 1);
        effect->state++;
        break;
    case 1:
        ov01_021EFCF8(1, -16, -16, &effect->effectComplete, 2);
        effect->state++;
        break;
    case 2:
        if (--work->shakeDelay == 0) {
            ov01_021F12E8(&work->screenShake, 0, 0xbf, 0x2aa, 0xc000, 800, (void *)REG_BG0OFS_ADDR, 0, 4);
            work->screenShakeActive = TRUE;
        }
        if (effect->effectComplete != 0) {
            effect->state++;
        }
        break;
    case 3:
        ov01_021EFE34(&work->blendAlpha, 0, 0x10, 8);
        G2_SetBlendAlpha(0, 0xf, work->blendAlpha.currentValue, 0x10 - work->blendAlpha.currentValue);
        for (i = 0; i < 2; i++) {
            Sprite_SetDrawFlag(work->sprites[i], 1);
            Sprite_SetAffineOverwriteMode(work->sprites[i], 2);
            Sprite_SetOamMode(work->sprites[i], GX_OAM_MODE_XLU);
        }
        ov01_021EFE34(&work->rotation, 0, 0xffff, 8);
        effect->state++;
        break;
    case 4:
        done = ov01_021EFE44(&work->blendAlpha);
        reg_G2_BLDALPHA = work->blendAlpha.currentValue | ((0x10 - work->blendAlpha.currentValue) << 8);
        rot = (u16)work->rotation.currentValue;
        if (ov01_021EFE44(&work->rotation) == 0) {
            Sprite_SetAffineZRotation(work->sprites[0], (u16)work->rotation.currentValue);
            Sprite_SetAffineZRotation(work->sprites[1], (u16)rot);
        } else {
            Sprite_SetAffineZRotation(work->sprites[0], 0);
            Sprite_SetAffineZRotation(work->sprites[1], 0);
        }
        if (done == 1) {
            reg_G2_BLDCNT = 0;
            for (i = 0; i < 2; i++) {
                Sprite_SetOamMode(work->sprites[i], GX_OAM_MODE_NORMAL);
            }
            effect->state++;
        }
        break;
    case 5:
        ov01_021EFEC8(&work->scale, 0x1000, 0x29, 0x19a, 8);
        scale = work->scale.currentValue;
        tmp1 = ov01_021F074C(scale, scale, scale);
        vec1 = tmp1;
        for (i = 0; i < 2; i++) {
            Sprite_SetAffineScale(work->sprites[i], &vec1);
        }
        dist = Camera_GetDistance(work->camera);
        ov01_021EFEC8(&work->cameraDistance, dist, dist - 0x1f4000, 0xffff6000, 8);
        BeginNormalPaletteFade(FADE_MAIN_ONLY, (FadeType)0x18, FADE_TYPE_BRIGHTNESS_OUT, (FadeType)0, 8, 1, HEAP_ID_FIELD1);
        effect->state++;
        break;
    case 6:
        done = ov01_021EFF28(&work->scale);
        scale = work->scale.currentValue;
        tmp2 = ov01_021F074C(scale, scale, scale);
        vec2 = tmp2;
        for (i = 0; i < 2; i++) {
            Sprite_SetAffineScale(work->sprites[i], &vec2);
        }
        ov01_021EFF28(&work->cameraDistance);
        Camera_SetDistance(work->cameraDistance.currentValue, work->camera);
        if (done == 1 && IsPaletteFadeFinished() == 1) {
            effect->state++;
        }
        break;
    case 7:
        sub_0200FBF4(PM_LCD_BOTTOM, 0);
        if (effect->done != NULL) {
            *effect->done = 1;
        }
        ov01_021F12D0(&work->screenShake);
        work->screenShakeActive = FALSE;
        for (i = 0; i < 2; i++) {
            Sprite_Delete(work->sprites[i]);
        }
        ov01_021F06EC(&work->spriteEnv, &work->spriteRes);
        ov01_021F05F4(&work->spriteEnv);
        ov01_021EFCDC(effect, task);
        return;
    }
    if (work->screenShakeActive == TRUE) {
        ov01_021F1310(&work->screenShake, 2);
    }
    if (effect->state != 7) {
        SpriteList_RenderAndAnimateSprites(work->spriteEnv.unk_00);
    }
}
