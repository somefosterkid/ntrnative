#include "global.h"

#include "constants/heap.h"

#include "overlay_manager.h"
#include "screen_fade.h"

s32 ov45_0222A330(void *);

void ov45_0222A4A8(void *);

void ov45_0222A520(void *, u32);

s32 ov45_0222D844(void);

void ov47_02258A80(void *);

void ov47_022593A0(s32);

extern s32 (*ov47_02259FA8[])(void *, void *, void *, void *, void *, s32);

u32 ov47_02258898(OverlayManager *param_1, s32 *param_2);

u32 ov47_02258898(OverlayManager *param_1, s32 *param_2) {
    void *pvVar1;
    void *pvVar2;
    s32 iVar3;
    BOOL BVar4;

    pvVar1 = OverlayManager_GetData(param_1);
    pvVar2 = OverlayManager_GetArgs(param_1);
    switch (*param_2) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0, 6, 1, HEAP_ID_129);
        ov45_0222A520(*(void **)((u8 *)pvVar2 + 4), 1);
        *param_2 = *param_2 + 1;
        break;
    case 1:
        BVar4 = IsPaletteFadeFinished();
        if (BVar4 == 1) {
            *param_2 = *param_2 + 1;
        }
        break;
    case 2:
        BVar4 = IsPaletteFadeFinished();
        if (BVar4 == 1) {
            iVar3 = ov45_0222A330(*(void **)((u8 *)pvVar2 + 4));
            if (iVar3 == 1) {
                ov45_0222A4A8(*(void **)((u8 *)pvVar2 + 4));
                *param_2 = *param_2 + 1;
                break;
            }
            iVar3 = ov45_0222D844();
            if (iVar3 == 1) {
                *param_2 = *param_2 + 1;
                break;
            }
        }
        iVar3 = (ov47_02259FA8[*(s32 *)((u8 *)pvVar2 + 8)])((u8 *)pvVar1 + 0x178, (u8 *)pvVar1 + 0x144, pvVar1, (u8 *)pvVar1 + 0x268, pvVar2, 0x81);
        if (iVar3 == 1) {
            *param_2 = *param_2 + 1;
        }
        break;
    case 3:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0, 6, 1, HEAP_ID_129);
        *param_2 = *param_2 + 1;
        break;
    case 4:
        BVar4 = IsPaletteFadeFinished();
        if (BVar4 == 1) {
            ov47_022593A0((s32)pvVar1 + 0x268);
            return 1;
        }
    }
    ov47_02258A80(pvVar1);
    return 0;
}
