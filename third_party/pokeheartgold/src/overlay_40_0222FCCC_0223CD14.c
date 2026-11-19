#include "global.h"

#include "bg_window.h"
#include "overlay_40_unk_struct_0208_c06_c.h"

s32 ov40_0223CCBC(void *, s32);

void ov40_0223CD14(void *arg0);

void ov40_0223CD14(void *arg0) {
    Window *window;
    void *v0;

    v0 = ((UnkStruct_0208C06C *)arg0)->unk_860;
    window = v0 + 0x94;
    InitWindow(window);
    AddWindowParameterized(((UnkStruct_0208C06C *)arg0)->unk_14.unk_10, window, 6U, 8U, 3U, 0x10U, 2U, 0xEU, 1U);
    ov40_0223CCBC(arg0, *(s32 *)((u8 *)v0 + 0x10) + 0x79);
}
