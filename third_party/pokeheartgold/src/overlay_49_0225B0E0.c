#include "global.h"

#include "bg_window.h"
#include "render_window.h"
#include "text.h"
#include "filesystem.h"
#include "list_menu.h"
#include "pm_string.h"
#include "touch_hitbox_controller.h"
#include "unk_0200A090.h"

void ov49_0225B0E0(Window *param_1, u32 *param_2, u32 param_3, u8 param_4, u8 param_5, u8 param_6, u8 param_7);

void ov49_0225B124(Window *arg0);
void ov49_0225B148(Window *arg0, String *arg1, u32 arg2, u32 arg3);

typedef struct {
    Window unk_00;
} UnkStruct_ov70_0225EFD4;
void ov49_0225B178(UnkStruct_ov70_0225EFD4 *param0, u16 param1, u16 param2, u16 param3, u16 param4);

void ov49_0225B0E0(Window *param_1, u32 *param_2, u32 param_3, u8 param_4, u8 param_5, u8 param_6, u8 param_7) {
    AddWindowParameterized((BgConfig *)*param_2, param_1, 1, param_4, param_5, param_6, param_7, 5, 0xca);
    DrawFrameAndWindow1(param_1, 1, 0x55, 3);
    FillWindowPixelBuffer(param_1, 0xf);
    ScheduleWindowCopyToVram(param_1);
    return;
}

void ov49_0225B124(Window *window) {
    if (WindowIsInUse(window) == 1) {
        ClearWindowPixelBuffer(window, 1);
        ClearWindowTilemapAndScheduleTransfer(window);
        RemoveWindow(window);
    }
}

void ov49_0225B148(Window *window, String *string, u32 x, u32 y) {
    AddTextPrinterParameterizedWithColor(window, FONT_SYSTEM, string, x, y, 0xFFU, 0x1020FU, NULL);
    ScheduleWindowCopyToVram(window);
}

void ov49_0225B178(UnkStruct_ov70_0225EFD4 *param0, u16 param1, u16 param2, u16 param3, u16 param4) {
    FillWindowPixelRect(&param0->unk_00, 15, param1, param2, param3, param4);
}
