#include "global.h"

#include "bg_window.h"
#include "unk_struct_ov70_02242e58.h"

// iVar1 walks the window array: a byte offset on the DS (sizeof(Window) == 0x10), an element index on the PC
#if defined(SDK_PORT) || defined(NONMATCHING)
#define LOOP_WINDOW_STEP 1
#define LOOP_WINDOW(p, i) (&(p)->unk_4[i])
#else
#define LOOP_WINDOW_STEP 0x10
#define LOOP_WINDOW(p, i) ((Window *)((u8 *)(p)->unk_4 + (i)))
#endif

extern u32 ov70_02245D60;

extern u32 ov70_02245D6E;

extern u32 ov70_02245D96;

extern u32 ov70_02245DA2;

extern u32 ov70_02245E0E;

extern u32 ov70_02245E10;

void ov70_0224190C(UnkStruct_ov70_02242E58 *param_1, u32 param_2);

void ov70_0224190C(UnkStruct_ov70_02242E58 *param_1, u32 param_2) {
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
            AddWindowParameterized(param_1->unk_0, LOOP_WINDOW(param_1, iVar1), param_1->unk_54, *puVar3, puVar3[1], 3, 2, 1, uVar4);
            FillWindowPixelBuffer(LOOP_WINDOW(param_1, iVar1), 0x22);
            uVar4 = uVar4 + 6;
            puVar3 = puVar3 + 2;
            iVar1 = iVar1 + LOOP_WINDOW_STEP;
            iStack_18 = iStack_18 + 1;
        } while (iStack_18 < 9);
        AddWindowParameterized(param_1->unk_0, &param_1->unk_4[14], param_1->unk_54, 9, 0x11, 6, 2, 1, 0x12f);
        FillWindowPixelBuffer(&param_1->unk_4[14], 0x22);
        return;
    case 5:
        AddWindowParameterized(param_1->unk_0, param_1->unk_4, param_1->unk_54, 1, 2, 3, 2, 1, 0x30);
        FillWindowPixelBuffer(param_1->unk_4, 0x22);
        puVar2 = (unsigned char *)(&ov70_02245E10);
        iVar5 = 1;
        iVar1 = LOOP_WINDOW_STEP;
        do {
            AddWindowParameterized(param_1->unk_0, LOOP_WINDOW(param_1, iVar1), param_1->unk_54, puVar2[-2], puVar2[-1], 2, 2, 1, (iVar5 + -1) * 4 + 0x36);
            FillWindowPixelBuffer(LOOP_WINDOW(param_1, iVar1), 0x22);
            puVar2 = puVar2 + 2;
            iVar5 = iVar5 + 1;
            iVar1 = iVar1 + LOOP_WINDOW_STEP;
        } while (iVar5 < 4);
        AddWindowParameterized(param_1->unk_0, &param_1->unk_4[14], param_1->unk_54, 9, 0x11, 6, 2, 1, 0x12f);
        FillWindowPixelBuffer(&param_1->unk_4[14], 0x22);
        return;
    case 0:
        puVar3 = (unsigned char *)(&ov70_02245DA2);
        iStack_2c = 0;
        uVar4 = 0x30;
        iVar1 = 0;
        do {
            AddWindowParameterized(param_1->unk_0, LOOP_WINDOW(param_1, iVar1), param_1->unk_54, *puVar3, puVar3[1], 8, 2, 1, uVar4);
            FillWindowPixelBuffer(LOOP_WINDOW(param_1, iVar1), 0x22);
            uVar4 = uVar4 + 0x10;
            puVar3 = puVar3 + 3;
            iVar1 = iVar1 + LOOP_WINDOW_STEP;
            iStack_2c = iStack_2c + 1;
        } while (iStack_2c < 4);
        AddWindowParameterized(param_1->unk_0, &param_1->unk_4[4], param_1->unk_54, 6, 0xe, 5, 1, 1, 0x70);
        FillWindowPixelBuffer(&param_1->unk_4[4], 0x22);
        AddWindowParameterized(param_1->unk_0, &param_1->unk_4[14], param_1->unk_54, 9, 0x11, 6, 2, 1, 0x12f);
        FillWindowPixelBuffer(&param_1->unk_4[14], 0x22);
        return;
    case 6:
        puVar3 = (unsigned char *)(&ov70_02245E0E);
        iStack_28 = 0;
        uVar4 = 0x30;
        iVar1 = 0;
        do {
            AddWindowParameterized(param_1->unk_0, LOOP_WINDOW(param_1, iVar1), param_1->unk_54, *puVar3, puVar3[1], 3, 2, 1, uVar4);
            FillWindowPixelBuffer(LOOP_WINDOW(param_1, iVar1), 0x22);
            uVar4 = uVar4 + 6;
            puVar3 = puVar3 + 2;
            iVar1 = iVar1 + LOOP_WINDOW_STEP;
            iStack_28 = iStack_28 + 1;
        } while (iStack_28 < 9);
        AddWindowParameterized(param_1->unk_0, &param_1->unk_4[14], param_1->unk_54, 9, 0x11, 6, 2, 1, 0x12f);
        FillWindowPixelBuffer(&param_1->unk_4[14], 0x22);
        AddWindowParameterized(param_1->unk_0, &param_1->unk_4[15], param_1->unk_54, 2, 2, 9, 2, 1, 0x66);
        FillWindowPixelBuffer(&param_1->unk_4[15], 0x22);
        return;
    case 3:
        puVar3 = (unsigned char *)(&ov70_02245D96);
        iStack_24 = 0;
        uVar4 = 0x30;
        iVar1 = 0;
        do {
            AddWindowParameterized(param_1->unk_0, LOOP_WINDOW(param_1, iVar1), param_1->unk_54, *puVar3, puVar3[1], 0x17, 2, 1, uVar4);
            FillWindowPixelBuffer(LOOP_WINDOW(param_1, iVar1), 0x22);
            uVar4 = uVar4 + 0x2e;
            puVar3 = puVar3 + 2;
            iVar1 = iVar1 + LOOP_WINDOW_STEP;
            iStack_24 = iStack_24 + 1;
        } while (iStack_24 < 5);
        AddWindowParameterized(param_1->unk_0, &param_1->unk_4[5], param_1->unk_54, 0xd, 0x10, 5, 1, 1, 0x116);
        FillWindowPixelBuffer(&param_1->unk_4[5], 0x22);
        AddWindowParameterized(param_1->unk_0, &param_1->unk_4[14], param_1->unk_54, 0x18, 0x11, 6, 2, 1, 0x12f);
        FillWindowPixelBuffer(&param_1->unk_4[14], 0x22);
        return;
    case 1:
        puVar3 = (unsigned char *)(&ov70_02245D60);
        iStack_20 = 0;
        uVar4 = 0x30;
        iVar1 = 0;
        do {
            AddWindowParameterized(param_1->unk_0, LOOP_WINDOW(param_1, iVar1), param_1->unk_54, *puVar3, puVar3[1], 8, 2, 1, uVar4);
            FillWindowPixelBuffer(LOOP_WINDOW(param_1, iVar1), 0x22);
            uVar4 = uVar4 + 0x10;
            puVar3 = puVar3 + 2;
            iVar1 = iVar1 + LOOP_WINDOW_STEP;
            iStack_20 = iStack_20 + 1;
        } while (iStack_20 < 3);
        AddWindowParameterized(param_1->unk_0, &param_1->unk_4[14], param_1->unk_54, 9, 0x11, 6, 2, 1, 0x60);
        FillWindowPixelBuffer(&param_1->unk_4[14], 0x22);
        return;
    case 2:
        puVar3 = (unsigned char *)(&ov70_02245D6E);
        iStack_1c = 0;
        uVar4 = 0x30;
        iVar1 = 0;
        do {
            AddWindowParameterized(param_1->unk_0, LOOP_WINDOW(param_1, iVar1), param_1->unk_54, *puVar3, puVar3[1], 0xb, 2, 1, uVar4);
            FillWindowPixelBuffer(LOOP_WINDOW(param_1, iVar1), 0x22);
            uVar4 = uVar4 + 0x16;
            puVar3 = puVar3 + 2;
            iVar1 = iVar1 + LOOP_WINDOW_STEP;
            iStack_1c = iStack_1c + 1;
        } while (iStack_1c < 4);
        AddWindowParameterized(param_1->unk_0, &param_1->unk_4[4], param_1->unk_54, 6, 0xe, 5, 1, 1, 0x88);
        FillWindowPixelBuffer(&param_1->unk_4[4], 0x22);
        AddWindowParameterized(param_1->unk_0, &param_1->unk_4[14], param_1->unk_54, 9, 0x11, 6, 2, 1, 0xa1);
        FillWindowPixelBuffer(&param_1->unk_4[14], 0x22);
        break;
    }
    return;
}
