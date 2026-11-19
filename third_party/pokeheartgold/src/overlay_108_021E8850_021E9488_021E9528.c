#include "global.h"

#include "unk_02005D10.h"
#include "overlay_108_021E8850.h"
#include "overlay_108_safari_decoration_app.h"

s32 ov108_021E9528(SafariDecorationApp *arg0, u32 arg1) {
    u8 temp_r5;

    if (arg1 >= 6U) {
        PlaySE(0x5DCU);
        ov108_021E9F20(arg0, 1);
        arg0->result = 3;
        return 3;
    }
    arg0->selected = (u8)((arg0->cursor) + (6 * (arg0->page)));
    temp_r5 = (u32)(arg0->selected);
    if (temp_r5 >= ((u32)(arg0->numObjects))) {
        return 0;
    }
    temp_r5 = ov108_021EA52C(arg0, temp_r5);
    if (temp_r5 != 0) {
        PlaySE(0x5F3U);
        ov108_021EA418(arg0, temp_r5 - 1);
        return 2;
    }
    PlaySE(0x5DCU);
    arg0->selectedId = arg0->objects[arg0->selected].id;
    return 1;
}
