#include "bg_window.h"
#include "font.h"
#include "message_format.h"
#include "msgdata.h"
#include "text.h"
#include "unk_02097268.h"
#include "bg_window.h"
#include "font.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "string_util.h"
#include "text.h"
#include "unk_02097268.h"
extern u32 ov108_021EA724[];

/* invented: layout read from the asm */
typedef struct ov108_Entry {
    u8 msgOffset;
    u8 count;
    struct {
        u8 id;
        u8 pad[3];
    } items[30];
} ov108_Entry;

typedef struct ov108_Ctx {
    u8 pad_000[0x1C];
    ov108_Entry entries[6];
    u8 pad_2e8[0x304 - 0x1C - 6 * sizeof(ov108_Entry)];
    MsgData *msgData;
    MessageFormat *msgFmt;
    String *string;
    u8 pad_310[0x338 - 0x310];
    String *string2;
    u8 pad_33c[0x3B4 - 0x33C];
    Window windows[15];
    u8 pad_4a4[0x184E3 - 0x3B4 - 15 * sizeof(Window)];
    u8 flag;
} ov108_Ctx;

void ov108_021E7CD8(ov108_Ctx *ctx, u32 arg1) {
    u8 counts[5];
    SafariObjectConfig cfg;
    ov108_Entry *entry;
    int i;
    u32 color;
    Window *win;
    Window *win4;

    if (arg1 >= 6) {
        if (ctx->flag != 0) {
            for (i = 0; i < 5; i++) {
                FillWindowPixelBuffer(&ctx->windows[6 + i * 2], (u8)ov108_021EA724[i % 2]);
                ScheduleWindowCopyToVram(&ctx->windows[6 + i * 2]);
            }
        }
        FillWindowPixelBuffer(&ctx->windows[4], 12);
        ScheduleWindowCopyToVram(&ctx->windows[4]);
        return;
    }
    entry = &ctx->entries[arg1];
    win4 = &ctx->windows[4];
    ReadMsgDataIntoString(ctx->msgData, entry->msgOffset + 16, ctx->string);
    i = GetWindowWidth(win4);
    i = (u8)(((i * 8) - FontID_String_GetWidth(FONT_SYSTEM, ctx->string, 0)) >> 1);
    FillWindowPixelBuffer(win4, 12);
    AddTextPrinterParameterizedWithColor(win4, FONT_SYSTEM, ctx->string, i, 4, 0xFF, 0x80B0C, 0);
    ScheduleWindowCopyToVram(win4);
    if (ctx->flag != 0) {
        MI_CpuFill8(counts, 0, 5);
        for (i = 0; i < entry->count; i++) {
            GetSafariObjectConfig(&cfg, entry->items[i].id, 2);
            if (cfg.objectType == 0) {
                counts[4]++;
            } else {
                counts[cfg.objectType - 1]++;
            }
        }
        for (i = 0; i < 5; i++) {
            win = &ctx->windows[6 + i * 2];
            BufferIntegerAsString(ctx->msgFmt, 0, counts[i], 2, PRINTING_MODE_RIGHT_ALIGN, 1);
            StringExpandPlaceholders(ctx->msgFmt, ctx->string, ctx->string2);
            color = ov108_021EA724[i % 2];
            FillWindowPixelBuffer(win, (u8)color);
            AddTextPrinterParameterizedWithColor(win, FONT_SYSTEM, ctx->string, 0, 0, 0xFF, color, 0);
            ScheduleWindowCopyToVram(win);
        }
    }
}
