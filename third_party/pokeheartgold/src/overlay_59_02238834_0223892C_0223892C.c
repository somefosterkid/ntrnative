#include "global.h"
#include "overlay_59_apricorn_app.h"

#include "bg_window.h"
#include "font.h"
#include "message_format.h"
#include "pm_string.h"
#include "sprite.h"
#include "sprite_system.h"
#include "string_util.h"
#include "text.h"

extern s32 ov59_0223C6C4;

extern s32 ov59_0223C6C6;

void ov59_0223892C(Ov59ApricornApp *arg0, s32 arg1);

void ov59_0223892C(Ov59ApricornApp *arg0, s32 arg1) {
    u8 *sp14;
    Sprite **sp10;
    Window *temp_r7;
    u8 width;

    temp_r7 = &arg0->windows[arg1 + 0xB];
    sp14 = arg0->apricornCounts;
    if (sp14[arg1] == 0) {
        sp10 = &arg0->sprites[arg1 + 2];
        Sprite_SetDrawFlag(*sp10, 0);
        ClearWindowTilemapAndScheduleTransfer(temp_r7);
    } else {
        sp10 = &arg0->sprites[arg1 + 2];
        Sprite_SetDrawFlag(*sp10, 1);
        FillWindowPixelBuffer(temp_r7, 0U);
        BufferIntegerAsString(arg0->msgFormat, 0U, (s32)sp14[arg1], 2U, PRINTING_MODE_RIGHT_ALIGN, 1);
        StringExpandPlaceholders(arg0->msgFormat, arg0->unk_64, arg0->unk_74);
        width = FontID_String_GetWidth(FONT_SYSTEM, arg0->unk_64, 0U);
        AddTextPrinterParameterizedWithColor(temp_r7, FONT_SYSTEM, arg0->unk_64, (0x10 - width) / 2, 0U, 0xFFU, 0x10200U, NULL);
        ScheduleWindowCopyToVram(temp_r7);
    }
    Sprite_SetPositionXY(*sp10, *(s16 *)(((u8 *)&ov59_0223C6C4) + arg1 * 4), *(s16 *)(((u8 *)&ov59_0223C6C6) + arg1 * 4));
}
