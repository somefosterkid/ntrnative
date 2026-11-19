#include "global.h"

#include "overlay_70_gtsapplication_state.h"

#include "sprite.h"

void ov70_02238F9C(Sprite *, int, int);

s32 ov70_0223C2EC(GTSApplicationState *param_1);

extern u8 ov70_022464F0;

extern u8 ov70_022464F1;

extern void *ov70_022464FE;

extern void *ov70_02246500;

extern void *ov70_02246502;

void ov70_0223C420(GTSApplicationState *param_1, s32 param_2);

void ov70_0223C420(GTSApplicationState *param_1, s32 param_2) {
    s32 iVar1;
    s32 iVar2;

    iVar1 = param_2 * 2;
    if ((&ov70_022464F0)[iVar1] == 0) {
        param_1->speciesMenuState->requirementsActivePanel = 0;
        param_1->speciesMenuState->leftPanelCursorPos = (s32)(&ov70_022464F1)[iVar1];
    } else {
        param_1->speciesMenuState->requirementsActivePanel = 1;
        param_1->speciesMenuState->rightPanelCursorPos = (s32)(&ov70_022464F1)[iVar1];
    }
    iVar1 = ov70_0223C2EC(param_1);
    iVar2 = ov70_0223C2EC(param_1);
    ov70_02238F9C(param_1->cursorSprite, *(u16 *)(((u8 *)&ov70_022464FE) + iVar1 * 6), *(u16 *)(((u8 *)&ov70_02246500) + iVar2 * 6));
    iVar1 = ov70_0223C2EC(param_1);
    Sprite_SetAnimCtrlSeq(param_1->cursorSprite, (u32) * (u16 *)(((u8 *)&ov70_02246502) + iVar1 * 6));
    return;
}
