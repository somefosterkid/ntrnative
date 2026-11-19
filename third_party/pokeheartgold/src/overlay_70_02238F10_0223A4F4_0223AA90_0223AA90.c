#include "global.h"
#include "overlay_70_gtsapplication_state.h"

void ov70_02238F9C(void *, s32, s32);

void ov70_02241330(GTSApplicationState *, s32, s32);

extern u32 ov70_022464CC;

u32 ov70_0223AA90(GTSApplicationState *param_1);

u32 ov70_0223AA90(GTSApplicationState *param_1) {
    u32 uVar1;
    s32 iVar2;

    uVar1 = ((u32(**)(GTSApplicationState *)) & ov70_022464CC)[param_1->currentScreenInstruction](param_1);
    iVar2 = 0;
    do {
        ov70_02238F9C(param_1->avatarSprites[iVar2], (s32)param_1->avatarPositions[iVar2][0], param_1->unk_F14 + (s32)param_1->avatarPositions[iVar2][1] + 0x20);
        iVar2 = iVar2 + 1;
    } while (iVar2 < 8);
    ov70_02238F9C(param_1->listingCursorSprite, 0xd0, 0x3a - param_1->unk_F14);
    ov70_02241330(param_1, param_1->selectedSearchResult, param_1->unk_F14);
    return uVar1;
}
