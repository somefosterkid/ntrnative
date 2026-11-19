#include "global.h"

#include "overlay_70_gtsapplication_state.h"

void ov70_02238F9C(struct Sprite *, s32, s32);

s32 ov70_0223C2EC(GTSApplicationState *);

extern void *ov70_022464FE;

extern void *ov70_02246500;

extern u32 ov70_02246528;

u32 ov70_0223B6EC(GTSApplicationState *param_1);

u32 ov70_0223B6EC(GTSApplicationState *param_1) {
    u32 uVar1;
    s32 iVar2;
    s32 iVar3;

    iVar3 = param_1->currentScreenInstruction;
    uVar1 = (*(u32(**)(GTSApplicationState *))(((u8 *)&ov70_02246528) + iVar3 * 4))(param_1);
    iVar3 = 0;
    iVar3 = 0;
    do {
        ov70_02238F9C(param_1->avatarSprites[iVar3], param_1->avatarPositions[iVar3][0], param_1->unk_F14 + param_1->avatarPositions[iVar3][1]);
        ov70_02238F9C(param_1->unk_F10, 0x37, param_1->unk_F14 + 0x1a8);
        iVar3 = iVar3 + 1;
    } while (iVar3 < 8);
    iVar2 = ov70_0223C2EC(param_1);
    iVar3 = ov70_0223C2EC(param_1);
    ov70_02238F9C(param_1->cursorSprite, *(u16 *)(((u8 *)&ov70_022464FE) + iVar2 * 6), (u32) * (u16 *)(((u8 *)&ov70_02246500) + iVar3 * 6) - param_1->unk_F14);
    return uVar1;
}
