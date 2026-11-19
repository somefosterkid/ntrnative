#include "global.h"

#include "filesystem.h"
#include "font.h"
#include "message_format.h"
#include "message_printer.h"
#include "msgdata.h"
#include "palette.h"
#include "pm_string.h"
#include "unk_0203A3B0.h"

u8 ov80_02237B58(u8, s32);

s32 ov83_0224442C(BgConfig *);

s32 ov83_022471FC(void *);

s32 ov83_0224753C(void *);

s32 ov83_02247858(void *);

s32 ov83_0224791C(void *, u8);

SysTask *ov83_02247A18(void *);

SysTask *ov83_02247CC4(void *);

void ov83_02243E30(void *arg0);

void ov83_02243E30(void *arg0) {
    s32 sp4;
    void *sp0;
    s32 temp_r0;
    s32 var_r4;
    s32 var_r6;
    void *var_r5;
    void *var_r7;

    sp0 = arg0;
    ov83_02247858(arg0 + 0x604);
    ov83_02247CC4(*(void **)((u8 *)arg0 + 0x5F4));
    ov83_02247A18(*(void **)((u8 *)arg0 + 0x5F0));
    ov83_0224753C(*(void **)((u8 *)arg0 + 0x508));
    ov83_0224753C(*(void **)((u8 *)arg0 + 0x50C));
    ov83_0224753C(*(void **)((u8 *)arg0 + 0x540));
    ov83_0224753C(*(void **)((u8 *)arg0 + 0x544));
    var_r6 = 0x52;
    var_r7 = sp0;
    sp4 = 0;
    var_r6 = 0x520;
    do {
        var_r4 = 0;
        var_r5 = var_r7;
loop_2:
        ov83_0224753C(*(void **)((u8 *)var_r5 + 0x520));
        var_r4 += 1;
        var_r5 += 4;
        if (var_r4 < 2) {
            goto loop_2;
        }
        var_r7 += 8;
        temp_r0 = sp4 + 1;
        sp4 = temp_r0;
    } while (temp_r0 < 4);
    var_r6 = ov80_02237B58(*(u8 *)((u8 *)sp0 + 9), 1);
    var_r5 = NULL;
    if (var_r6 > 0) {
        var_r7 = (void *)0x51;
        var_r4 = (s32)sp0;
        var_r7 = (void *)0x510;
        do {
            ov83_0224753C(*(void **)((u8 *)var_r4 + 0x4F4));
            ov83_0224753C(*(void **)((u8 *)var_r4 + 0x4E4));
            ov83_0224753C(*(void **)((u8 *)var_r4 + 0x510));
            var_r5 += 1;
            var_r4 += 4;
        } while ((s32)var_r5 < var_r6);
    }
    sub_0203A914();
    PaletteData_FreeBuffers(*(PaletteData **)((u8 *)sp0 + 0x2B0), PLTTBUF_MAIN_OBJ);
    PaletteData_FreeBuffers(*(PaletteData **)((u8 *)sp0 + 0x2B0), PLTTBUF_MAIN_BG);
    PaletteData_Free(*(PaletteData **)((u8 *)sp0 + 0x2B0));
    *(PaletteData **)((u8 *)sp0 + 0x2B0) = NULL;
    ov83_022471FC(sp0 + 0x2C8);
    DestroyMsgData(*(MsgData **)((u8 *)sp0 + 0x20));
    MessageFormat_Delete(*(MessageFormat **)((u8 *)sp0 + 0x24));
    String_Delete(*(String **)((u8 *)sp0 + 0x28));
    String_Delete(*(String **)((u8 *)sp0 + 0x2C));
    MessagePrinter_Delete(*(MessagePrinter **)((u8 *)sp0 + 0x2B4));
    FontID_Release(FONT_UNK_4);
    var_r4 = (s32)sp0;
    var_r5 = NULL;
    do {
        String_Delete(*(String **)((u8 *)var_r4 + 0x30));
        var_r5 += 1;
        var_r4 += 4;
    } while ((s32)var_r5 < 3);
    ov83_0224791C(sp0 + 0x50, 1);
    ov83_0224442C(*(void **)((u8 *)sp0 + 0x4C));
    NARC_Delete(*(NARC **)((u8 *)sp0 + 0x560));
}
