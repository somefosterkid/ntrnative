#include "global.h"

#include "bg_window.h"
#include "unk_struct_ov70_02242e58.h"
#include "unk_0201956C_types.h"


void ov70_02241DB4(UnkStruct_ov70_02242E58 *param_1, u32 param_2);

void ov70_02241DB4(UnkStruct_ov70_02242E58 *param_1, u32 param_2) {
    s32 iVar1;
    s32 iVar2;
    s8 uStack_18;
    s8 acStack_17[3];

    sub_02019B1C(param_1->unk_1C, 0, acStack_17, &uStack_18);
    switch (param_2) {
    case 4:
        iVar2 = 0;
        iVar1 = 0;
        do {
            FillWindowPixelBuffer(&param_1->unk_4[iVar1], 0x22);
            CopyWindowPixelsToVram_TextMode(&param_1->unk_4[iVar1]);
            RemoveWindow(&param_1->unk_4[iVar1]);
            iVar2 = iVar2 + 1;
            iVar1 = iVar1 + 1;
        } while (iVar2 < 9);
        if (acStack_17[0] == 0x10) {
            FillBgTilemapRect(param_1->unk_0, 2, 5, 0x11, 1, 0xf, 0xf, 0x10);
            BgCommitTilemapBufferToVram(param_1->unk_0, 2);
        }
        RemoveWindow(&param_1->unk_4[14]);
        return;
    case 5:
        iVar2 = 0;
        iVar1 = 0;
        do {
            FillWindowPixelBuffer(&param_1->unk_4[iVar1], 0x22);
            CopyWindowPixelsToVram_TextMode(&param_1->unk_4[iVar1]);
            RemoveWindow(&param_1->unk_4[iVar1]);
            iVar2 = iVar2 + 1;
            iVar1 = iVar1 + 1;
        } while (iVar2 < 4);
        if (acStack_17[0] == 0x10) {
            FillBgTilemapRect(param_1->unk_0, 2, 5, 0x11, 1, 0xf, 0xf, 0x10);
            BgCommitTilemapBufferToVram(param_1->unk_0, 2);
        }
        RemoveWindow(&param_1->unk_4[14]);
        return;
    case 0:
        iVar2 = 0;
        iVar1 = 0;
        do {
            FillWindowPixelBuffer(&param_1->unk_4[iVar1], 0x22);
            CopyWindowPixelsToVram_TextMode(&param_1->unk_4[iVar1]);
            RemoveWindow(&param_1->unk_4[iVar1]);
            iVar2 = iVar2 + 1;
            iVar1 = iVar1 + 1;
        } while (iVar2 < 5);
        if (acStack_17[0] == 0x10) {
            FillBgTilemapRect(param_1->unk_0, 2, 5, 0x11, 1, 0xf, 0xf, 0x10);
            BgCommitTilemapBufferToVram(param_1->unk_0, 2);
        }
        RemoveWindow(&param_1->unk_4[14]);
        return;
    case 6:
        iVar2 = 0;
        iVar1 = 0;
        do {
            FillWindowPixelBuffer(&param_1->unk_4[iVar1], 0x22);
            CopyWindowPixelsToVram_TextMode(&param_1->unk_4[iVar1]);
            RemoveWindow(&param_1->unk_4[iVar1]);
            iVar2 = iVar2 + 1;
            iVar1 = iVar1 + 1;
        } while (iVar2 < 9);
        if (acStack_17[0] == 0x10) {
            FillBgTilemapRect(param_1->unk_0, 2, 5, 0x11, 1, 0xf, 0xf, 0x10);
            BgCommitTilemapBufferToVram(param_1->unk_0, 2);
        }
        RemoveWindow(&param_1->unk_4[14]);
        RemoveWindow(&param_1->unk_4[15]);
        return;
    case 3:
        iVar2 = 0;
        iVar1 = 0;
        do {
            FillWindowPixelBuffer(&param_1->unk_4[iVar1], 0x22);
            CopyWindowPixelsToVram_TextMode(&param_1->unk_4[iVar1]);
            RemoveWindow(&param_1->unk_4[iVar1]);
            iVar2 = iVar2 + 1;
            iVar1 = iVar1 + 1;
        } while (iVar2 < 6);
        if (((acStack_17[0] + -0x10) * 0x1000000 >> 0x18 & 0xffU) <= 1) {
            FillBgTilemapRect(param_1->unk_0, 2, 5, 0x12, 1, 0xe, 0xf, 0x10);
            BgCommitTilemapBufferToVram(param_1->unk_0, 2);
        }
        RemoveWindow(&param_1->unk_4[14]);
        return;
    case 1:
        iVar2 = 0;
        iVar1 = 0;
        do {
            RemoveWindow(&param_1->unk_4[iVar1]);
            iVar2 = iVar2 + 1;
            iVar1 = iVar1 + 1;
        } while (iVar2 < 3);
        RemoveWindow(&param_1->unk_4[14]);
        return;
    case 2:
        iVar2 = 0;
        iVar1 = 0;
        do {
            RemoveWindow(&param_1->unk_4[iVar1]);
            iVar2 = iVar2 + 1;
            iVar1 = iVar1 + 1;
        } while (iVar2 < 5);
        RemoveWindow(&param_1->unk_4[14]);
        break;
    }
    return;
}
