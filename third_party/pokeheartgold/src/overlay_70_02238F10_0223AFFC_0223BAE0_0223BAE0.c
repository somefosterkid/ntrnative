#include "global.h"

#include "bg_window.h"
#include "overlay_70_gtsapplication_state.h"

void ov70_02239D44(void *, u32);

extern u32 ov70_0224562C;

extern u32 ov70_02245634;

extern u32 ov70_02245640;

void ov70_0223BAE0(GTSApplicationState *param_1);

void ov70_0223BAE0(GTSApplicationState *param_1) {
    u16 *puVar1;
    Window *pWVar2;
    s32 iVar4;
    u32 uVar3;
    s32 iVar5;

    AddWindowParameterized(param_1->bgConfig, &param_1->bottomInstructionWindow, 0, 2, 0x15, 0x1b, 2, 0xd, 0x60);
    FillWindowPixelBuffer(&param_1->bottomInstructionWindow, 0);
    puVar1 = (u16 *)&ov70_02245640;
    iVar4 = 0;
    uVar3 = 1;
    pWVar2 = &param_1->infoWindows[13];
    do {
        AddWindowParameterized(param_1->bgConfig, pWVar2, 3, (u8)*puVar1, (u8)puVar1[1], 0xb, 2, 0xd, uVar3);
        FillWindowPixelBuffer(pWVar2, 0);
        iVar4 = iVar4 + 1;
        uVar3 = uVar3 + 0x16;
        puVar1 = puVar1 + 2;
        pWVar2 = pWVar2 + 1;
    } while (iVar4 < 6);
    puVar1 = (u16 *)&ov70_0224562C;
    iVar4 = 0;
    uVar3 = 0x85;
    pWVar2 = param_1->locationCriteriaWindows;
    do {
        AddWindowParameterized(param_1->bgConfig, pWVar2, 3, (u8)*puVar1, (u8)puVar1[1], 0x1c, 2, 0xd, uVar3);
        FillWindowPixelBuffer(pWVar2, 0);
        iVar4 = iVar4 + 1;
        uVar3 = uVar3 + 0x38;
        puVar1 = puVar1 + 2;
        pWVar2 = pWVar2 + 1;
    } while (iVar4 < 2);
    uVar3 = 0x11d;
    puVar1 = (u16 *)&ov70_02245634;
    iVar4 = 0;
    do {
        iVar5 = iVar4 + 6;
        AddWindowParameterized(param_1->bgConfig, &(&param_1->infoWindows[13])[iVar5], 3, (u8)*puVar1, (u8)puVar1[1], 0xa, 2, 0xd, uVar3);
        FillWindowPixelBuffer(&(&param_1->infoWindows[13])[iVar5], 0);
        iVar4 = iVar4 + 1;
        uVar3 = uVar3 + 0x14;
        puVar1 = puVar1 + 2;
    } while (iVar4 < 3);
    ov70_02239D44(param_1, 4);
    return;
}
