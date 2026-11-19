#include "global.h"
#include "overlay_59_apricorn_app.h"

#include "bg_window.h"
#include "font.h"
#include "message_format.h"
#include "pm_string.h"
#include "string_util.h"
#include "text.h"

extern u8 ov59_0223C624;

void ov59_02238A04(Ov59ApricornApp *arg0, s32 arg1);

void ov59_02238A04(Ov59ApricornApp *arg0, s32 arg1) {
    u8 digits[2];
    s32 sp10;
    s32 temp_r0_2;
    u8 *var_r7;
    u8 *var_r6;

    digits[0] = (u8)(arg1 % 10);
    digits[1] = (u8)(arg1 / 10);
    sp10 = 0;
    var_r6 = &ov59_0223C624;
    var_r7 = digits;
loop_1:
    BufferIntegerAsString(arg0->msgFormat, 0U, (s32)*var_r7, 1U, PRINTING_MODE_LEFT_ALIGN, 1);
    StringExpandPlaceholders(arg0->msgFormat, arg0->unk_64, arg0->unk_74);
    FillWindowPixelBuffer(&arg0->windows[sp10 + 8], 0U);
    AddTextPrinterParameterizedWithColor(&arg0->windows[sp10 + 8], FONT_SYSTEM, arg0->unk_64, (u32)*var_r6, 3U, 0xFFU, 0x10200U, NULL);
    ScheduleWindowCopyToVram(&arg0->windows[sp10 + 8]);
    if ((u32)arg0->apricornCounts[arg0->selected] >= 0xAU) {
        var_r7 += 1;
        var_r6 += 1;
        temp_r0_2 = sp10 + 1;
        sp10 = temp_r0_2;
        if (temp_r0_2 < 2) {
            goto loop_1;
        }
    }
}
