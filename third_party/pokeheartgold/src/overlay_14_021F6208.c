#include "global.h"

#include "bg_window.h"
#include "unk_0201956C.h"
#include "overlay_14_box_funcs.h"

s32 sub_02019A60(UnkStruct_0201956C *, s32, Window *);

void ov14_021F6208(UnkStruct_0201956C *a0, s32 a1, Window *window) {
    s32 a3;
    u8 a2;

    a2 = GetWindowBgId(window);
    a3 = (s32)GetWindowWidth(window);
    sub_020195F4(a0, a1, (s32)a2, a3, (s32)GetWindowHeight(window));
    sub_02019A60(a0, a1, window);
}
