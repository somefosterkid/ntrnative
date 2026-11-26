#include "global.h"

#include "constants/heap.h"

#include "overlay_manager.h"
#include "screen_fade.h"
#include "touchscreen.h"
#include "unk_02005D10.h"
#include "unk_02026E30.h"
#include "overlay_41_unk_struct_ov22_0225_b85_c.h"

void ov41_022466C8(void *);

void ov41_0224B554(void *);

extern u8 gSystem;

BOOL AccessoryPortrait_Main(OverlayManager *man, s32 *state);

BOOL AccessoryPortrait_Main(OverlayManager *man, s32 *state) {
    UnkStruct_ov22_0225B85C *pvVar1;
    BOOL BVar2;

    pvVar1 = OverlayManager_GetData(man);
    Thunk_G3X_Reset();
    NNS_G2dSetupSoftwareSpriteCamera();
    ov41_0224B554(pvVar1->unk_10);
    RequestSwap3DBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
    ov41_022466C8(&pvVar1->unk_14);
    switch (*state) {
    case 0:
        *state = *state + 1;
        break;
    case 1:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_UNK_5, FADE_TYPE_BRIGHTNESS_IN, 0, 6, 1, HEAP_ID_13);
        *state = *state + 1;
        break;
    case 2:
        BVar2 = IsPaletteFadeFinished();
        if (BVar2 != 0) {
            *state = *state + 1;
        }
        break;
    case 3:
        if (((((u32 *)&gSystem)[0x12] & 3) != 0) || (BVar2 = System_GetTouchNew(), BVar2 != 0)) {
            PlaySE(0x5dd);
            *state = *state + 1;
        }
        break;
    case 4:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_DOWNWARD_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0, 6, 1, HEAP_ID_13);
        *state = *state + 1;
        break;
    case 5:
        BVar2 = IsPaletteFadeFinished();
        if (BVar2 != 0) {
            return 1;
        }
    }
    return 0;
}
