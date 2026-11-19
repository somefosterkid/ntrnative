#include "global.h"

#include "bg_window.h"
#include "sprite.h"
#include "text.h"
#include "unk_0201956C.h"
#include "unk_struct_ov70_02242e58.h"

s32 sub_02019A60(UnkStruct_0201956C *, s32, Window *);

void ov70_02242FC4(UnkStruct_0201956C *arg0, Window *arg1, String *arg2, u32 arg3, u32 arg4);

s32 ov70_0224304C(UnkStruct_ov70_02242E58 *arg0);

s32 ov70_02243068(UnkStruct_ov70_02242E58 *arg0);
s32 ov70_02242D44(void *, s32, s32);
s32 ov70_0224308C(UnkStruct_ov70_02242E58 *arg0);

void ov70_02242FC4(UnkStruct_0201956C *arg0, Window *window, String *string, u32 x, u32 color) {
    AddTextPrinterParameterizedWithColor(window, FONT_SYSTEM, string, x, 0U, 0xFFU, color, NULL);
    CopyWindowPixelsToVram_TextMode(window);
    sub_02019A60(arg0, 0, window);
}

s32 ov70_02242FF4(void);

s32 ov70_02242FF4(void) {
    return -1;
}

s32 ov70_02238F9C(Sprite *, s32, s32);

s32 ov70_02242FFC(UnkStruct_ov70_02242E58 *arg0);

s32 ov70_02242FFC(UnkStruct_ov70_02242E58 *arg0) {
    if (sub_020199E4(arg0->unk_1C, 0) == 0) {
        Sprite_SetDrawFlag(arg0->unk_C, 1);
        if (arg0->unk_58 == 2) {
            ov70_02238F9C(arg0->unk_10, 0xE4, 0x78);
            ov70_02238F9C(arg0->unk_14, 0x9A, 0x78);
            Sprite_SetDrawFlag(arg0->unk_10, 1);
            Sprite_SetDrawFlag(arg0->unk_14, 1);
        }
        arg0->unk_4C = (s32) arg0->unk_50;
    }
    return -1;
}

s32 ov70_0224304C(UnkStruct_ov70_02242E58 *arg0) {
    if (sub_020199E4(arg0->unk_1C, 0) == 0) {
        arg0->unk_4C = (s32) arg0->unk_50;
    }
    return -1;
}

s32 ov70_02243068(UnkStruct_ov70_02242E58 *arg0) {
    if (sub_020199E4(arg0->unk_1C, 0) == 0) {
        sub_0201980C(arg0->unk_1C, 0);
        arg0->unk_4C = (s32) arg0->unk_50;
    }
    return -1;
}

// a handler of the ov70_022466F8 table, read by ov70_02242144: the DS leaves -1 in r0 (`subs r0, #6` after unk_50 = 5), "keep going"
s32 ov70_0224308C(UnkStruct_ov70_02242E58 *arg0) {
    ov70_02242D44(arg0, 4, 0x20);
    Sprite_SetAnimCtrlSeq(arg0->unk_C, 0x3D);
    ov70_02238F9C(arg0->unk_C, 0x88, 0x28);
    sub_020198FC(arg0->unk_1C, 0, -4, 0, 4);
    arg0->unk_4C = 1;
    arg0->unk_50 = 5;
    arg0->unk_3C = -1;
    return -1;
}
