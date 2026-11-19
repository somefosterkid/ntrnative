#include "global.h"

#include "bg_window.h"
#include "unk_02013534.h"
#include "overlay_96_app_021f8d98.h"

void ov96_021F8D98(void *arg0, Window *arg1, u16 arg2);

void ov96_021F8D98(void *arg0, Window *window, u16 width) {
    UnkStruct_02013910 *a0;

    InitWindow(window);
    AddTextWindowTopLeftCorner(*(BgConfig **)((u8 *)arg0 + 8), window, width, 2U, 0U, 0U);
    a0 = sub_02013910(window, *(enum HeapID *)arg0);
    ((Ov96App_021F8D98 *)window)->unk_10 = a0;
    ((Ov96App_021F8D98 *)window)->unk_14 = sub_02013948(a0, NNS_G2D_VRAM_TYPE_2DMAIN);
}
