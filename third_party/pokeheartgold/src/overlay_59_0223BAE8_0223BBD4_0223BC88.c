#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "message_format.h"
#include "overlay_59_juice_feed_app.h"
#include "pm_string.h"
#include "string_util.h"
#include "text.h"

void ov59_0223C380(void *, void *);

void ov59_0223C3AC(void *);

extern const u32 ov59_0223C94C[];

void ov59_0223BC88(Ov59JuiceFeedApp *param_1, s32 param_2) {
    Ov59JuiceMon *entry = &param_1->mons[param_2];

    FillWindowPixelBuffer(&param_1->windows[4], 0);
    AddTextPrinterParameterizedWithColor(&param_1->windows[4], FONT_SYSTEM, entry->name, 0, 0, 0xFF, 0xF0200, NULL);
    if (entry->gender != 2) {
        AddTextPrinterParameterizedWithColor(&param_1->windows[4], FONT_SYSTEM, param_1->genderStrings[entry->gender], 0x40, 0, 0xFF, ov59_0223C94C[entry->gender], NULL);
    }
    BufferIntegerAsString(param_1->msgFormat, 0, entry->level, 3, PRINTING_MODE_LEFT_ALIGN, 1);
    StringExpandPlaceholders(param_1->msgFormat, param_1->unk_68, param_1->unk_6C);
    AddTextPrinterParameterizedWithColor(&param_1->windows[4], FONT_SYSTEM, param_1->unk_68, 0x10, 0x10, 0xFF, 0x10200, NULL);
    ScheduleWindowCopyToVram(&param_1->windows[4]);
    ov59_0223C3AC(param_1->monSprite);
    ov59_0223C380(param_1->monSprite, entry);
    ov59_0223BE18(param_1, entry);
}
