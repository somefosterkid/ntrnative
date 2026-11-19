#include "global.h"

#include "bg_window.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

void ov14_021F5FBC(void *param_1, s32 param_2);

void ov14_021F5FBC(void *param_1, s32 param_2) {
    FillWindowPixelBuffer((Window *)(((u8 *)((UnkStruct_ov14_boxapp *)param_1)->sys) + 0x200), 0);
    FillWindowPixelBuffer((Window *)(((u8 *)((UnkStruct_ov14_boxapp *)param_1)->sys) + 0x210), 0);
    FillWindowPixelBuffer((Window *)(((u8 *)((UnkStruct_ov14_boxapp *)param_1)->sys) + 0x220), 0);
    FillWindowPixelBuffer((Window *)(((u8 *)((UnkStruct_ov14_boxapp *)param_1)->sys) + 0x230), 0);
    CopyWindowPixelsToVram_TextMode((Window *)(((u8 *)((UnkStruct_ov14_boxapp *)param_1)->sys) + 0x200));
    CopyWindowPixelsToVram_TextMode((Window *)(((u8 *)((UnkStruct_ov14_boxapp *)param_1)->sys) + 0x210));
    CopyWindowPixelsToVram_TextMode((Window *)(((u8 *)((UnkStruct_ov14_boxapp *)param_1)->sys) + 0x220));
    CopyWindowPixelsToVram_TextMode((Window *)(((u8 *)((UnkStruct_ov14_boxapp *)param_1)->sys) + 0x230));
    if (param_2 == 0) {
        ov14_021F5950(param_1, 0x21, 0x51, 0);
        return;
    }
    ov14_021F5950(param_1, 0x21, 0x50, 0);
    return;
}

void ov14_021F604C(void *arg0) {
    ov14_021F5950(arg0, 0x2A, 0x3F, 0);
}

void ov14_021F605C(void *arg0) {
    ov14_021F5BD8(arg0, 0x29, 0x4D, 0, 3);
}
