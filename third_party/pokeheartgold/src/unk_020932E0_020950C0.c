#include "global.h"
#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "unk_020932E0.h"

void sub_020950C0(UnkStruct_020932E0 *arg0) {
    if (arg0->unk_D == 1) {
        sub_0209501C(arg0, 5, FONT_MESSAGE);
    }
}

void sub_020950D4(UnkStruct_020932E0 *arg0) {
    if ((arg0->unk_D == 1) && (GetWindowBgConfig(&arg0->unk_4660) != NULL)) {
        RemoveWindow(&arg0->unk_4660);
    }
}

void sub_020950F8(Window *param_1, s32 param_2) {
    if (param_1->bgConfig != NULL) {
        if (param_2 != 0) {
            GfGfx_EngineATogglePlanes(8, 0);
        }
        ClearWindowTilemapAndCopyToVram(param_1);
        RemoveWindow(param_1);
    }
    return;
}
