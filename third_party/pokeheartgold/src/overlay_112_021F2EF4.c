#include "global.h"

#include "constants/heap.h"

#include "error_handling.h"
#include "overlay_manager.h"
#include "screen_fade.h"
#include "sprite_system.h"
#include "unk_02005D10.h"

#include "overlay_112_sprite_app.h"

void ov112_021F1838(void *arg0);

void ov112_021F1A40(Ov112SpriteApp *arg0);

void ov112_021F1B94(void *arg0);

void ov112_021F2DE4(OverlayManager *arg0);

void ov112_021F178C(void *);

u32 ov112_021F1DC0(Ov112SpriteApp *);

void ov112_021F2B80(void *);

int ov112_021F2EF4(OverlayManager *param_1, int *param_2);

s32 ov112_021F2E70(OverlayManager *);
int ov112_021F2FAC(OverlayManager *arg0, int *arg1);

void ov112_021F2338(void *arg0);
void ov112_021F2CC4(void *arg0);
s32 ov112_021F0E60(void *);
s32 ov112_021F0EB4(void *);
s32 ov112_021F0EFC(void *);
void ov112_021F1D28(void *, u32);
s32 ov112_021F2CD4(void *arg0);
int ov112_021F3018(OverlayManager *param_1, int *param_2);

extern const OverlayManagerTemplate ov112_021FF124;

#if defined(SDK_PORT) || defined(NONMATCHING)
const OverlayManagerTemplate ov112_021FF124 = {
    .init = ov112_021F2EF4,
    .exec = ov112_021F3018,
    .exit = ov112_021F2FAC,
    .ovy_id = FS_OVERLAY_ID_NONE,
};
#endif

int ov112_021F2EF4(OverlayManager *param_1, int *param_2) {
    Ov112SpriteApp *arg0;
    s32 iVar1;
    BOOL BVar2;

    arg0 = OverlayManager_GetData(param_1);
    switch (*param_2) {
    case 0:
        ov112_021F2DE4(param_1);
        *param_2 = *param_2 + 1;
        break;
    case 1:
        ov112_021F1838(arg0);
        ov112_021F1A40(arg0);
        *param_2 = *param_2 + 1;
        break;
    case 2:
        iVar1 = ov112_021F1DC0(arg0);
        if (iVar1 != 0) {
            ov112_021F1B94(arg0);
            ov112_021F178C(arg0);
            arg0->bits144.page = 0xff;
            ov112_021F2B80(arg0);
            *param_2 = *param_2 + 1;
        }
        break;
    case 3:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0x7fff, 6, 1, arg0->heapID);
        *param_2 = *param_2 + 1;
        break;
    case 4:
        BVar2 = IsPaletteFadeFinished();
        if (BVar2 != 0) {
            return 1;
        }
        break;
    default:
        GF_AssertFail();
    }
    return 0;
}

int ov112_021F2FAC(OverlayManager *man, int *arg1) {
    s32 v1;
    Ov112SpriteApp *v0;

    v0 = OverlayManager_GetData(man);
    v1 = *arg1;
    switch (v1) {
    case 0:
        if (ManagedSprite_IsAnimated(v0->sprites[2]) == 0) {
            *arg1 += 1;
        }
        break;
    case 1:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0U, 6, 1, v0->heapID);
        *arg1 += 1;
        break;
    case 2:
        if (IsPaletteFadeFinished() != 0) {
            ov112_021F2E70(man);
            return 1;
        }
        break;
    default:
        GF_AssertFail();
        break;
    }
    return 0;
}

int ov112_021F3018(OverlayManager *param_1, int *param_2) {
    Ov112SpriteApp *arg0;
    s32 iVar1;
    u8 remainder;
    arg0 = OverlayManager_GetData(param_1);
    iVar1 = ov112_021F2CD4(arg0);
    if (iVar1 == 0) {
        return 0;
    }
    iVar1 = ov112_021F0EB4(arg0);
    if (iVar1 != 0) {
        remainder = *((s8 *)(((u8 *)arg0) + 0x13d));
        if (remainder != 0) {
            ((Ov112SpriteApp *)arg0)->curIdx = remainder + (-1);
        }
        ov112_021F2CC4(arg0);
        PlaySE(0x5dc);
    } else {
        iVar1 = ov112_021F0EFC(arg0);
        if (iVar1 != 0) {
            remainder = ((s32)((*((u8 *)(((u8 *)arg0) + 0x13d))) + 1)) % ((s32)(*((u8 *)(((u8 *)arg0) + 0x13e))));
            ((Ov112SpriteApp *)arg0)->curIdx = remainder;
            ov112_021F2CC4(arg0);
            PlaySE(0x5dc);
        } else {
            iVar1 = ov112_021F0E60(arg0);
            if (iVar1 != 0) {
                return 1;
            }
        }
    }
    ov112_021F2338((u8 *)arg0);
    ov112_021F1D28(arg0, 5);
    ov112_021F1D28(arg0, 6);
    ov112_021F1D28(arg0, 7);
    ov112_021F1D28(arg0, 8);
    return 0;
}
