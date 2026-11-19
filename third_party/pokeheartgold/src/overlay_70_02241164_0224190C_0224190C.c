#include "global.h"

#include "bg_window.h"

extern u32 ov70_02245D60;

extern u32 ov70_02245D6E;

extern u32 ov70_02245D96;

extern u32 ov70_02245DA2;

extern u32 ov70_02245E0E;

extern u32 ov70_02245E10;

void ov70_0224190C(u32 *param_1, u32 param_2);

void ov70_0224190C(u32 *param_1, u32 param_2) {
    u8 *puVar3;
    s32 iVar1;
    u8 *puVar2;
    u32 uVar4;
    s32 iVar5;
    s32 iStack_18;
    s32 iStack_1c;
    s32 iStack_20;
    s32 iStack_24;
    s32 iStack_28;
    s32 iStack_2c;

    switch (param_2) {
    case 4:
        puVar3 = (unsigned char *)(&ov70_02245E0E);
        iStack_18 = 0;
        uVar4 = 0x30;
        iVar1 = 0;
        do {
            AddWindowParameterized((BgConfig *)*param_1, (Window *)(param_1[1] + iVar1), param_1[0x15], *puVar3, puVar3[1], 3, 2, 1, uVar4);
            FillWindowPixelBuffer((Window *)(param_1[1] + iVar1), 0x22);
            uVar4 = uVar4 + 6;
            puVar3 = puVar3 + 2;
            iVar1 = iVar1 + 0x10;
            iStack_18 = iStack_18 + 1;
        } while (iStack_18 < 9);
        AddWindowParameterized((BgConfig *)*param_1, (Window *)(param_1[1] + 0xe0), param_1[0x15], 9, 0x11, 6, 2, 1, 0x12f);
        FillWindowPixelBuffer((Window *)(param_1[1] + 0xe0), 0x22);
        return;
    case 5:
        AddWindowParameterized((BgConfig *)*param_1, (Window *)param_1[1], param_1[0x15], 1, 2, 3, 2, 1, 0x30);
        FillWindowPixelBuffer((Window *)param_1[1], 0x22);
        puVar2 = (unsigned char *)(&ov70_02245E10);
        iVar5 = 1;
        iVar1 = 0x10;
        do {
            AddWindowParameterized((BgConfig *)*param_1, (Window *)(param_1[1] + iVar1), param_1[0x15], puVar2[-2], puVar2[-1], 2, 2, 1, (iVar5 + -1) * 4 + 0x36);
            FillWindowPixelBuffer((Window *)(param_1[1] + iVar1), 0x22);
            puVar2 = puVar2 + 2;
            iVar5 = iVar5 + 1;
            iVar1 = iVar1 + 0x10;
        } while (iVar5 < 4);
        AddWindowParameterized((BgConfig *)*param_1, (Window *)(param_1[1] + 0xe0), param_1[0x15], 9, 0x11, 6, 2, 1, 0x12f);
        FillWindowPixelBuffer((Window *)(param_1[1] + 0xe0), 0x22);
        return;
    case 0:
        puVar3 = (unsigned char *)(&ov70_02245DA2);
        iStack_2c = 0;
        uVar4 = 0x30;
        iVar1 = 0;
        do {
            AddWindowParameterized((BgConfig *)*param_1, (Window *)(param_1[1] + iVar1), param_1[0x15], *puVar3, puVar3[1], 8, 2, 1, uVar4);
            FillWindowPixelBuffer((Window *)(param_1[1] + iVar1), 0x22);
            uVar4 = uVar4 + 0x10;
            puVar3 = puVar3 + 3;
            iVar1 = iVar1 + 0x10;
            iStack_2c = iStack_2c + 1;
        } while (iStack_2c < 4);
        AddWindowParameterized((BgConfig *)*param_1, (Window *)(param_1[1] + 0x40), param_1[0x15], 6, 0xe, 5, 1, 1, 0x70);
        FillWindowPixelBuffer((Window *)(param_1[1] + 0x40), 0x22);
        AddWindowParameterized((BgConfig *)*param_1, (Window *)(param_1[1] + 0xe0), param_1[0x15], 9, 0x11, 6, 2, 1, 0x12f);
        FillWindowPixelBuffer((Window *)(param_1[1] + 0xe0), 0x22);
        return;
    case 6:
        puVar3 = (unsigned char *)(&ov70_02245E0E);
        iStack_28 = 0;
        uVar4 = 0x30;
        iVar1 = 0;
        do {
            AddWindowParameterized((BgConfig *)*param_1, (Window *)(param_1[1] + iVar1), param_1[0x15], *puVar3, puVar3[1], 3, 2, 1, uVar4);
            FillWindowPixelBuffer((Window *)(param_1[1] + iVar1), 0x22);
            uVar4 = uVar4 + 6;
            puVar3 = puVar3 + 2;
            iVar1 = iVar1 + 0x10;
            iStack_28 = iStack_28 + 1;
        } while (iStack_28 < 9);
        AddWindowParameterized((BgConfig *)*param_1, (Window *)(param_1[1] + 0xe0), param_1[0x15], 9, 0x11, 6, 2, 1, 0x12f);
        FillWindowPixelBuffer((Window *)(param_1[1] + 0xe0), 0x22);
        AddWindowParameterized((BgConfig *)*param_1, (Window *)(param_1[1] + 0xf0), param_1[0x15], 2, 2, 9, 2, 1, 0x66);
        FillWindowPixelBuffer((Window *)(param_1[1] + 0xf0), 0x22);
        return;
    case 3:
        puVar3 = (unsigned char *)(&ov70_02245D96);
        iStack_24 = 0;
        uVar4 = 0x30;
        iVar1 = 0;
        do {
            AddWindowParameterized((BgConfig *)*param_1, (Window *)(param_1[1] + iVar1), param_1[0x15], *puVar3, puVar3[1], 0x17, 2, 1, uVar4);
            FillWindowPixelBuffer((Window *)(param_1[1] + iVar1), 0x22);
            uVar4 = uVar4 + 0x2e;
            puVar3 = puVar3 + 2;
            iVar1 = iVar1 + 0x10;
            iStack_24 = iStack_24 + 1;
        } while (iStack_24 < 5);
        AddWindowParameterized((BgConfig *)*param_1, (Window *)(param_1[1] + 0x50), param_1[0x15], 0xd, 0x10, 5, 1, 1, 0x116);
        FillWindowPixelBuffer((Window *)(param_1[1] + 0x50), 0x22);
        AddWindowParameterized((BgConfig *)*param_1, (Window *)(param_1[1] + 0xe0), param_1[0x15], 0x18, 0x11, 6, 2, 1, 0x12f);
        FillWindowPixelBuffer((Window *)(param_1[1] + 0xe0), 0x22);
        return;
    case 1:
        puVar3 = (unsigned char *)(&ov70_02245D60);
        iStack_20 = 0;
        uVar4 = 0x30;
        iVar1 = 0;
        do {
            AddWindowParameterized((BgConfig *)*param_1, (Window *)(param_1[1] + iVar1), param_1[0x15], *puVar3, puVar3[1], 8, 2, 1, uVar4);
            FillWindowPixelBuffer((Window *)(param_1[1] + iVar1), 0x22);
            uVar4 = uVar4 + 0x10;
            puVar3 = puVar3 + 2;
            iVar1 = iVar1 + 0x10;
            iStack_20 = iStack_20 + 1;
        } while (iStack_20 < 3);
        AddWindowParameterized((BgConfig *)*param_1, (Window *)(param_1[1] + 0xe0), param_1[0x15], 9, 0x11, 6, 2, 1, 0x60);
        FillWindowPixelBuffer((Window *)(param_1[1] + 0xe0), 0x22);
        return;
    case 2:
        puVar3 = (unsigned char *)(&ov70_02245D6E);
        iStack_1c = 0;
        uVar4 = 0x30;
        iVar1 = 0;
        do {
            AddWindowParameterized((BgConfig *)*param_1, (Window *)(param_1[1] + iVar1), param_1[0x15], *puVar3, puVar3[1], 0xb, 2, 1, uVar4);
            FillWindowPixelBuffer((Window *)(param_1[1] + iVar1), 0x22);
            uVar4 = uVar4 + 0x16;
            puVar3 = puVar3 + 2;
            iVar1 = iVar1 + 0x10;
            iStack_1c = iStack_1c + 1;
        } while (iStack_1c < 4);
        AddWindowParameterized((BgConfig *)*param_1, (Window *)(param_1[1] + 0x40), param_1[0x15], 6, 0xe, 5, 1, 1, 0x88);
        FillWindowPixelBuffer((Window *)(param_1[1] + 0x40), 0x22);
        AddWindowParameterized((BgConfig *)*param_1, (Window *)(param_1[1] + 0xe0), param_1[0x15], 9, 0x11, 6, 2, 1, 0xa1);
        FillWindowPixelBuffer((Window *)(param_1[1] + 0xe0), 0x22);
        break;
    }
    return;
}
