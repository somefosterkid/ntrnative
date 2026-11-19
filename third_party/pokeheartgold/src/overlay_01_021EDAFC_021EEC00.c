#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "field_types_def.h"
#include "gf_gfx_planes.h"
#include "overlay_01_021EDAFC.h"
#include "render_window.h"

Window *ov01_021EEC00(FieldSystem *param_1, u8 param_2, u8 param_3, u8 param_4);

Window *ov01_021EEC00(FieldSystem *param_1, u8 param_2, u8 param_3, u8 param_4) {
    Window *window;

    window = AllocWindows(HEAP_ID_FIELD1, 1);
    AddWindowParameterized(param_1->bgConfig, window, 3, param_3, param_4, 0xa, 2, 0xd, 0x29);
    LoadUserFrameGfx1(param_1->bgConfig, GF_BG_LYR_MAIN_3, 0x3d9, 0xb, 0, HEAP_ID_FIELD1);
    DrawFrameAndWindow1(window, 1, 0x3d9, 0xb);
    ov01_021EEC7C(param_1, window, param_2);
    return window;
}

void ov01_021EEC68(Window *window) {
    ClearWindowPixelBuffer(window, 0);
    WindowArray_Delete(window, 1);
}
