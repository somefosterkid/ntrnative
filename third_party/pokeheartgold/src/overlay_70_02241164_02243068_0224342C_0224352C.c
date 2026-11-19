#include "global.h"

#include "bg_window.h"
#include "unk_struct_ov70_02242e58.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
#define WIN (&arg0->unk_4[spC])
#else
#define WIN ((Window *)((u8 *)arg0->unk_4 + var_r4))
#endif

s32 ov70_02242FC4(UnkStruct_0201956C *, Window *, String *, s32, s32);

s32 sub_02019A60(UnkStruct_0201956C *, s32, Window *);

void ov70_0224352C(UnkStruct_ov70_02242E58 *arg0, s32 arg1, s32 arg2);

void ov70_0224352C(UnkStruct_ov70_02242E58 *arg0, s32 arg1, s32 arg2) {
    s32 spC;
    s32 sp8;
    s32 var_r7;
    s32 sp4;
    s32 temp_r0;
    s32 var_r4;
    s32 var_r6;
    var_r4 = (spC = 0);
    var_r6 = 0;
    var_r7 = arg1 * 5;
    sp4 = arg2;
    sp8 = var_r7 << 3;
    do {
        FillWindowPixelBuffer(WIN, 0x22U);
        if (var_r7 < sp4) {
#if defined(SDK_PORT) || defined(NONMATCHING)
            ov70_02242FC4(arg0->unk_1C, WIN, arg0->unk_34[var_r7].text, 0, 0xF0E02);
#else
            ov70_02242FC4(arg0->unk_1C, WIN, *(String **)(var_r6 + (sp8 + (s32)arg0->unk_34)), 0, 0xF0E02);
#endif
        } else {
            CopyWindowPixelsToVram_TextMode(WIN);
            sub_02019A60(arg0->unk_1C, 0, WIN);
        }
        var_r4 += 0x10;
        var_r7 += 1;
        temp_r0 = spC + 1;
        var_r6 += 8;
        spC = temp_r0;
    } while (temp_r0 < 5);
}

#undef WIN
