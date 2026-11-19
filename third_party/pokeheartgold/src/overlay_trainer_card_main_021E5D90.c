#include "global.h"

#include "constants/heap.h"

#include "overlay_manager.h"
#include "screen_fade.h"
#include "sprite.h"
#include "trainer_card_main.h"
#include "unk_02009D48.h"

s32 TrainerCardMainApp_Main(OverlayManager *manager, s32 *state);

s32 TrainerCardMainApp_Main(OverlayManager *manager, s32 *state) {
    TrainerCardMain *arg0;
    u32 uVar1;
    BOOL BVar2;
    s32 iVar3;

    arg0 = OverlayManager_GetData(manager);
    switch (*state) {
    case 0:
        BVar2 = IsPaletteFadeFinished();
        if (BVar2 != 0) {
            *state = 1;
        }
        break;
    case 1:
        uVar1 = ov51_021E6B44(arg0);
        if (uVar1 == 3) {
            arg0->subState = 0;
            *state = 3;
        } else if (uVar1 == 4) {
            ov51_021E7D44(&arg0->sprites, 1, 3, 1);
            arg0->subState = 0;
            *state = 4;
        } else if (uVar1 == 5) {
            ov51_021E7D44(&arg0->sprites, 0, 1, 1);
            BeginNormalPaletteFade(FADE_MAIN_THEN_SUB, FADE_TYPE_UPWARD_OUT, FADE_TYPE_UPWARD_OUT, 0, 6, 1, HEAP_ID_25);
            *state = 2;
        }
        ov51_021E6DA8(arg0, arg0->card->liveIgt);
        break;
    case 2:
        BVar2 = IsPaletteFadeFinished();
        if (BVar2 != 0) {
            return 1;
        }
        break;
    case 3:
        iVar3 = ov51_021E6888(arg0);
        if (iVar3 != 0) {
            *state = 1;
        }
        break;
    case 4:
        iVar3 = ov51_021E67A4(arg0);
        switch (iVar3) {
        case 1:
            *state = 1;
            break;
        case 2:
            *state = 5;
            break;
        }
        break;
    case 5:
        BeginNormalPaletteFade(FADE_MAIN_THEN_SUB, FADE_TYPE_UPWARD_OUT, FADE_TYPE_UPWARD_OUT, 0, 6, 1, HEAP_ID_25);
        arg0->args->settings.showBack = 1;
        *state = 2;
    }
    arg0->scrollOffset = arg0->scrollOffset + 1;
    if (arg0->scrollOffset >= 0x80) {
        arg0->scrollOffset = 0;
    }
    SpriteList_RenderAndAnimateSprites(arg0->sprites.spriteList);
    return 0;
}
