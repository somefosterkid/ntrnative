#include "global.h"

#include "bg_window.h"

void sub_02019B1C(u32, u32, s8 *, u8 *);

void ov70_02241DB4(u32 *param_1, u32 param_2);

void ov70_02241DB4(u32 *param_1, u32 param_2) {
    s32 iVar1;
    s32 iVar2;
    u8 uStack_18;
    s8 acStack_17[3];

    sub_02019B1C(param_1[7], 0, acStack_17, &uStack_18);
    switch (param_2) {
    case 4:
        iVar2 = 0;
        iVar1 = 0;
        do {
            FillWindowPixelBuffer((Window *)(param_1[1] + iVar1), 0x22);
            CopyWindowPixelsToVram_TextMode((Window *)(param_1[1] + iVar1));
            RemoveWindow((Window *)(param_1[1] + iVar1));
            iVar2 = iVar2 + 1;
            iVar1 = iVar1 + 0x10;
        } while (iVar2 < 9);
        if (acStack_17[0] == 0x10) {
            FillBgTilemapRect((BgConfig *)*param_1, 2, 5, 0x11, 1, 0xf, 0xf, 0x10);
            BgCommitTilemapBufferToVram((BgConfig *)*param_1, 2);
        }
        RemoveWindow((Window *)(param_1[1] + 0xe0));
        return;
    case 5:
        iVar2 = 0;
        iVar1 = 0;
        do {
            FillWindowPixelBuffer((Window *)(param_1[1] + iVar1), 0x22);
            CopyWindowPixelsToVram_TextMode((Window *)(param_1[1] + iVar1));
            RemoveWindow((Window *)(param_1[1] + iVar1));
            iVar2 = iVar2 + 1;
            iVar1 = iVar1 + 0x10;
        } while (iVar2 < 4);
        if (acStack_17[0] == 0x10) {
            FillBgTilemapRect((BgConfig *)*param_1, 2, 5, 0x11, 1, 0xf, 0xf, 0x10);
            BgCommitTilemapBufferToVram((BgConfig *)*param_1, 2);
        }
        RemoveWindow((Window *)(param_1[1] + 0xe0));
        return;
    case 0:
        iVar2 = 0;
        iVar1 = 0;
        do {
            FillWindowPixelBuffer((Window *)(param_1[1] + iVar1), 0x22);
            CopyWindowPixelsToVram_TextMode((Window *)(param_1[1] + iVar1));
            RemoveWindow((Window *)(param_1[1] + iVar1));
            iVar2 = iVar2 + 1;
            iVar1 = iVar1 + 0x10;
        } while (iVar2 < 5);
        if (acStack_17[0] == 0x10) {
            FillBgTilemapRect((BgConfig *)*param_1, 2, 5, 0x11, 1, 0xf, 0xf, 0x10);
            BgCommitTilemapBufferToVram((BgConfig *)*param_1, 2);
        }
        RemoveWindow((Window *)(param_1[1] + 0xe0));
        return;
    case 6:
        iVar2 = 0;
        iVar1 = 0;
        do {
            FillWindowPixelBuffer((Window *)(param_1[1] + iVar1), 0x22);
            CopyWindowPixelsToVram_TextMode((Window *)(param_1[1] + iVar1));
            RemoveWindow((Window *)(param_1[1] + iVar1));
            iVar2 = iVar2 + 1;
            iVar1 = iVar1 + 0x10;
        } while (iVar2 < 9);
        if (acStack_17[0] == 0x10) {
            FillBgTilemapRect((BgConfig *)*param_1, 2, 5, 0x11, 1, 0xf, 0xf, 0x10);
            BgCommitTilemapBufferToVram((BgConfig *)*param_1, 2);
        }
        RemoveWindow((Window *)(param_1[1] + 0xe0));
        RemoveWindow((Window *)(param_1[1] + 0xf0));
        return;
    case 3:
        iVar2 = 0;
        iVar1 = 0;
        do {
            FillWindowPixelBuffer((Window *)(param_1[1] + iVar1), 0x22);
            CopyWindowPixelsToVram_TextMode((Window *)(param_1[1] + iVar1));
            RemoveWindow((Window *)(param_1[1] + iVar1));
            iVar2 = iVar2 + 1;
            iVar1 = iVar1 + 0x10;
        } while (iVar2 < 6);
        if (((acStack_17[0] + -0x10) * 0x1000000 >> 0x18 & 0xffU) <= 1) {
            FillBgTilemapRect((BgConfig *)*param_1, 2, 5, 0x12, 1, 0xe, 0xf, 0x10);
            BgCommitTilemapBufferToVram((BgConfig *)*param_1, 2);
        }
        RemoveWindow((Window *)(param_1[1] + 0xe0));
        return;
    case 1:
        iVar2 = 0;
        iVar1 = 0;
        do {
            RemoveWindow((Window *)(param_1[1] + iVar1));
            iVar2 = iVar2 + 1;
            iVar1 = iVar1 + 0x10;
        } while (iVar2 < 3);
        RemoveWindow((Window *)(param_1[1] + 0xe0));
        return;
    case 2:
        iVar2 = 0;
        iVar1 = 0;
        do {
            RemoveWindow((Window *)(param_1[1] + iVar1));
            iVar2 = iVar2 + 1;
            iVar1 = iVar1 + 0x10;
        } while (iVar2 < 5);
        RemoveWindow((Window *)(param_1[1] + 0xe0));
        break;
    }
    return;
}
