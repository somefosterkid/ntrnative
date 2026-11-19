#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "font_types_def.h"
#include "list_menu.h"
#include "pm_string.h"
#include "text.h"
#include "touch_hitbox_controller.h"
#include "unk_0200A090.h"
#include "overlay_49_unk_struct_ov70_0225_e9_c8.h"

void ov49_0225AC38(UnkStruct_ov70_0225E9C8 *param0);
typedef u32 TextColor;
void DrawFrameAndWindow2(Window *window, s32 skipTransfer, u16 tile, u8 palette);
void *WaitingIcon_New(Window *window, u32 baseTile);

void ov49_0225AB14(UnkStruct_ov70_0225E9C8 *param0);
void ov49_0225AB44(UnkStruct_ov70_0225E9C8 *param0, const String *param1);
void ov49_0225ABA4(UnkStruct_ov70_0225E9C8 *param0, const String *param1);
void ov49_0225AC08(UnkStruct_ov70_0225E9C8 *param0);

void ov49_0225AB14(UnkStruct_ov70_0225E9C8 *param0) {
    if (TextPrinterCheckActive(param0->unk_10)) {
        RemoveTextPrinter(param0->unk_10);
    }

    ov49_0225AC38(param0);

    String_Delete(param0->unk_18);
    RemoveWindow(&param0->unk_00);
}

void ov49_0225AB44(UnkStruct_ov70_0225E9C8 *param0, const String *param1) {
    if (TextPrinterCheckActive(param0->unk_10)) {
        RemoveTextPrinter(param0->unk_10);
    }

    FillWindowPixelBuffer(&param0->unk_00, 15);
    String_Copy(param0->unk_18, param1);

    param0->unk_10 = AddTextPrinterParameterizedWithColor(&param0->unk_00, FONT_MESSAGE, param0->unk_18, 0, 0, param0->unk_14, ((TextColor)(((1 & 0xFF) << 16) | ((2 & 0xFF) << 8) | ((15 & 0xFF) << 0))), 0L);

    DrawFrameAndWindow2(&param0->unk_00, 1, 1, 1);
}

void ov49_0225ABA4(UnkStruct_ov70_0225E9C8 *param0, const String *param1) {
    if (TextPrinterCheckActive(param0->unk_10)) {
        RemoveTextPrinter(param0->unk_10);
    }

    FillWindowPixelBuffer(&param0->unk_00, 15);
    String_Copy(param0->unk_18, param1);
    AddTextPrinterParameterizedWithColor(&param0->unk_00, FONT_MESSAGE, param0->unk_18, 0, 0, 0xFF, ((TextColor)(((1 & 0xFF) << 16) | ((2 & 0xFF) << 8) | ((15 & 0xFF) << 0))), 0L);
    DrawFrameAndWindow2(&param0->unk_00, 1, 1, 1);
    ScheduleWindowCopyToVram(&param0->unk_00);
}

void ov49_0225AC08(UnkStruct_ov70_0225E9C8 *param0) {
    ((param0->unk_1C == 0L) ? (void)0 : GF_AssertFail());
    param0->unk_1C = WaitingIcon_New(&param0->unk_00, 1);
}
