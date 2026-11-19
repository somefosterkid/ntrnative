#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "sprite.h"
#include "string_util.h"
#include "text.h"
#include "gf_gfx_loader.h"
#include "overlay_108_021E8850.h"
#include "overlay_108_safari_decoration_app.h"

void ov108_021EA040(SafariDecorationApp *arg0) {
    s32 sp20;
    s32 sp1C;
    s32 temp_r0;
    s32 temp_r0_2;
    u32 temp_r3;
    s32 temp_r3_2;
    s32 var_r4;
    u8 var_r0;
    NNSG2dScreenData *temp_r0_3;
    Window *temp_r7;
    FillBgTilemapRect(arg0->bgConfig, 6U, 0U, 0U, 5U, 0x20U, 0xEU, 0x11U);
    var_r4 = 0;
    temp_r7 = arg0->windows;
loop_1:
    temp_r0 = var_r4 + (6 * (arg0->page));

    sp1C = temp_r0;
    temp_r0_2 = (sp20 = var_r4 + 6);
    FillWindowPixelBuffer(temp_r7 + temp_r0_2, 0U);
    if (temp_r0 >= ((s32)(arg0->numObjects))) {
        ScheduleWindowCopyToVram(temp_r7 + sp20);
    } else {
        if (ov108_021EA52C(arg0, sp1C) > 0) {
            var_r0 = 1;
        } else {
            var_r0 = 0;
        }
        temp_r0_3 = arg0->scrn0;
        temp_r3 = var_r4;
        temp_r3 = temp_r3 >> 0x1F;
        CopyToBgTilemapRect(arg0->bgConfig, 6U, (var_r4 % 2) * 0x10, ((((s32)(var_r4 + temp_r3)) >> 1) + 1) * 5, 0x10U, 4U, temp_r0_3->rawData, (u8)(var_r0 * 0x10), 0U, (u8)((u32)(temp_r0_3->screenWidth) >> 3), (u8)((u32)(temp_r0_3->screenHeight) >> 3));
        ReadMsgDataIntoString(arg0->msgData, (arg0->objects)[sp1C].id + 0xE, arg0->strBuf1);
        temp_r3_2 = 0x70 - FontID_String_GetWidth(FONT_UNK_4, arg0->strBuf1, 0U);
        AddTextPrinterParameterizedWithColor(temp_r7 + sp20, FONT_UNK_4, arg0->strBuf1, (u32)(temp_r3_2 / 2), 0U, 0xFFU, 0x10200U, 0L);
        ScheduleWindowCopyToVram(temp_r7 + sp20);
    }
    var_r4 += 1;
    if (((s32)var_r4) < 6) {
        goto loop_1;
    }
    FillWindowPixelBuffer(&arg0->windows[3], 0U);
    BufferIntegerAsString(arg0->msgFormat, 0U, (arg0->page) + 1, 1U, PRINTING_MODE_RIGHT_ALIGN, 1);
    BufferIntegerAsString(arg0->msgFormat, 1U, (s32)(arg0->numPages), 1U, PRINTING_MODE_RIGHT_ALIGN, 1);
    StringExpandPlaceholders(arg0->msgFormat, arg0->strBuf0, arg0->unk_32C);
    AddTextPrinterParameterizedWithColor(&arg0->windows[3], FONT_SYSTEM, arg0->strBuf0, 0U, 0U, 0xFFU, 0x30400U, 0L);
    ScheduleWindowCopyToVram(&arg0->windows[3]);
    ScheduleBgTilemapBufferTransfer(arg0->bgConfig, 5U);
    ScheduleBgTilemapBufferTransfer(arg0->bgConfig, 6U);
    if ((arg0->page) == 0) {
        Sprite_SetAnimCtrlSeq(arg0->sprites[2], 7);
    } else {
        Sprite_SetAnimCtrlSeq(arg0->sprites[2], 5);
    }
    if ((arg0->page) == ((arg0->numPages) - 1)) {
        Sprite_SetAnimCtrlSeq(arg0->sprites[3], 0xA);
        return;
    }
    Sprite_SetAnimCtrlSeq(arg0->sprites[3], 8);
}
