#include "global.h"

#include "bg_window.h"
#include "overlay_27.h"
#include "overlay_27_ov27task.h"

void ov27_0225B630(Window *, s32);

void ov27_0225BEB0(Ov27PhoneIcon *arg0) {
    if (arg0->unk_1C != 0) {
        ov27_0225B630(&arg0->window, 2);
        ClearWindowTilemapAndCopyToVram(&arg0->window);
        RemoveWindow(&arg0->window);
    }
}
