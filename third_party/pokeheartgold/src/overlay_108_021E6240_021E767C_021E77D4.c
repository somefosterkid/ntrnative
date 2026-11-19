#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "message_format.h"
#include "pm_string.h"
#include "sprite.h"
#include "string_util.h"
#include "text.h"
#include "overlay_108_safari_area_customizer.h"

void ov108_021E77D4(SafariAreaCustomizer *param_1);

void ov108_021E77D4(SafariAreaCustomizer *param_1) {
    u8 bVar1;
    unsigned int new_var2;
    u32 w;
    FillWindowPixelBuffer(&param_1->windows[3], 2);
    BufferIntegerAsString(param_1->msgFormat, 0, (param_1->page) + 1, 1, PRINTING_MODE_RIGHT_ALIGN, 1);
    StringExpandPlaceholders(param_1->msgFormat, param_1->string, param_1->unk_334);
    bVar1 = GetWindowWidth(&param_1->windows[3]);
    w = FontID_String_GetWidth(FONT_SYSTEM, param_1->string, 0);
    w = (((u32)bVar1 * 8) - w) >> 1;
    new_var2 = 6;
    AddTextPrinterParameterizedWithColor(&param_1->windows[3], FONT_SYSTEM, param_1->string, w, 0, 0, 0x30102, (PrinterCallback_t)0x0);
    ScheduleBgTilemapBufferTransfer(param_1->bgConfig, 3);
    Sprite_SetAnimActiveFlag(param_1->unk_364, 0);
    if (!(param_1->page)) {
        Sprite_SetAnimCtrlSeq(param_1->unk_364, new_var2);
    } else {
        Sprite_SetAnimCtrlSeq(param_1->unk_364, 4);
    }
    Sprite_SetAnimActiveFlag(param_1->unk_368, 0);
    if (param_1->page == 1) {
        Sprite_SetAnimCtrlSeq(param_1->unk_368, 7);
        return;
    }
    Sprite_SetAnimCtrlSeq(param_1->unk_368, 5);
    return;
}
