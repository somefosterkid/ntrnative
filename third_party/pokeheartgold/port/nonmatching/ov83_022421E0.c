#include "pokemon_types_def.h"
#include "bg_window.h"
#include "font.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "pokemon.h"
s32 ov83_02240C48(void *, s32, s32, s32, s32);


s32 ov83_02241DD8(void *, void *, MsgData *, s32, s32, s32, s32, s32, s32);
s32 ov83_02247998();

s32 ov83_022479E4();



typedef struct BF_ov83_022421E0_0 { u8 f0_7:7; u8 b7:1; } BF_ov83_022421E0_0; /* invented: bitfield word (hk9) */
void ov83_022421E0(void *arg0, s32 arg1) {
    String *sp2C;
    String *sp28;
    s32 sp24;
    s32 sp20;
    void *sp1C;
    void *sp18;
    s32 sp14;
    String *temp_r0_3;
    Window *var_r4;
    s32 temp_r7;
    s32 temp_r7_2;
    u32 temp_r1;
    u32 var_r5;
    u32 var_r5_2;
    void *temp_r6;

    sp14 = arg1;
    FillWindowPixelBuffer(arg0 + 0x170, 0U);
    FillWindowPixelBuffer(arg0 + 0x180, 0U);
    FillWindowPixelBuffer(arg0 + 0x1A0, 0U);
    FillWindowPixelBuffer(arg0 + 0x1C0, 0U);
    FillWindowPixelBuffer(arg0 + 0x1E0, 0U);
    FillWindowPixelBuffer(arg0 + 0x200, 0U);
    FillWindowPixelBuffer(arg0 + 0x220, 0U);
    FillWindowPixelBuffer(arg0 + 0x240, 0U);
    FillWindowPixelBuffer(arg0 + 0x260, 0U);
    FillWindowPixelBuffer(arg0 + 0x280, 0U);
    FillWindowPixelBuffer(arg0 + 0x2A0, 0U);
    FillWindowPixelBuffer(arg0 + 0x2C0, 0U);
    FillWindowPixelBuffer(arg0 + 0x2D0, 0U);
    FillWindowPixelBuffer(arg0 + 0x2E0, 0U);
    FillWindowPixelBuffer(arg0 + 0x2F0, 0U);
    FillWindowPixelBuffer(arg0 + 0x300, 0U);
    FillWindowPixelBuffer(arg0 + 0x310, 0U);
    FillWindowPixelBuffer(arg0 + 0x320, 0U);
    FillWindowPixelBuffer(arg0 + 0x330, 0U);
    FillWindowPixelBuffer(arg0 + 0x340, 0U);
    BufferBoxMonNickname(*(MessageFormat **)((u8 *)arg0 + 0x24), 0U, Mon_GetBoxMon(*(Pokemon **)((u8 *)arg0 + 0x804)));
    ov83_02241DD8(arg0, arg0 + 0x170, *(MsgData **)((u8 *)arg0 + 0x20), 0x5B, 0, 0, 0, 0x10200, 0);
    if (!(((BF_ov83_022421E0_0 *)((u8 *)arg0 + 0x80E))->b7)) {
        if ((((BF_ov83_022421E0_0 *)((u8 *)arg0 + 0x80E))->f0_7) == 0) {
            ov83_022479E4(arg0 + 0x180, *(MsgData **)((u8 *)arg0 + 0x20), 0x56, 0, 0, 0, 0x50600, 0);
        } else if ((((BF_ov83_022421E0_0 *)((u8 *)arg0 + 0x80E))->f0_7) == 1) {
            ov83_022479E4(arg0 + 0x180, *(MsgData **)((u8 *)arg0 + 0x20), 0x57, 0, 0, 0, 0x30400, 0);
        }
    }
    ov83_02240C48(arg0, 0, *(u8 *)((u8 *)arg0 + 0x80F), 3, 0);
    ov83_02241DD8(arg0, arg0 + 0x1A0, *(MsgData **)((u8 *)arg0 + 0x20), 0x5E, 0, 0, 0, 0x10200, 0);
    BufferAbilityName(*(MessageFormat **)((u8 *)arg0 + 0x24), 0U, (u32) *(u8 *)((u8 *)arg0 + 0x810));
    ov83_02241DD8(arg0, arg0 + 0x1C0, *(MsgData **)((u8 *)arg0 + 0x20), 0x4B, 0, 0, 0, 0x10200, 0);
    BufferNatureName(*(MessageFormat **)((u8 *)arg0 + 0x24), 0U, (u32) *(u8 *)((u8 *)arg0 + 0x811));
    ov83_02241DD8(arg0, arg0 + 0x1E0, *(MsgData **)((u8 *)arg0 + 0x20), 0x49, 0, 0, 0, 0x10200, 0);
    BufferItemName(*(MessageFormat **)((u8 *)arg0 + 0x24), 0U, (u32) *(u16 *)((u8 *)arg0 + 0x812));
    ov83_02241DD8(arg0, arg0 + 0x200, *(MsgData **)((u8 *)arg0 + 0x20), 0x47, 0, 0, 0, 0x10200, 0);
    ov83_02240C48(arg0, 0, *(u16 *)((u8 *)arg0 + 0x818), 3, 0);
    ov83_02240C48(arg0, 1, *(u16 *)((u8 *)arg0 + 0x81A), 3, 0);
    ov83_02241DD8(arg0, arg0 + 0x220, *(MsgData **)((u8 *)arg0 + 0x20), 0x5F, GetWindowWidth(arg0 + 0x220) * 8, 0, 0, 0x10200, 1);
    ov83_02240C48(arg0, 0, *(u16 *)((u8 *)arg0 + 0x81C), 3, 0);
    ov83_02241DD8(arg0, arg0 + 0x240, *(MsgData **)((u8 *)arg0 + 0x20), 0x4D, GetWindowWidth(arg0 + 0x240) * 8, 0, 0, 0x10200, 1);
    ov83_02240C48(arg0, 0, *(u16 *)((u8 *)arg0 + 0x81E), 3, 0);
    ov83_02241DD8(arg0, arg0 + 0x260, *(MsgData **)((u8 *)arg0 + 0x20), 0x51, GetWindowWidth(arg0 + 0x260) * 8, 0, 0, 0x10200, 1);
    ov83_02240C48(arg0, 0, *(u16 *)((u8 *)arg0 + 0x820), 3, 0);
    ov83_02241DD8(arg0, arg0 + 0x280, *(MsgData **)((u8 *)arg0 + 0x20), 0x4F, GetWindowWidth(arg0 + 0x280) * 8, 0, 0, 0x10200, 1);
    ov83_02240C48(arg0, 0, *(u16 *)((u8 *)arg0 + 0x822), 3, 0);
    ov83_02241DD8(arg0, arg0 + 0x2A0, *(MsgData **)((u8 *)arg0 + 0x20), 0x53, GetWindowWidth(arg0 + 0x2A0) * 8, 0, 0, 0x10200, 1);
    ov83_02240C48(arg0, 0, *(u16 *)((u8 *)arg0 + 0x824), 3, 0);
    ov83_02241DD8(arg0, arg0 + 0x2C0, *(MsgData **)((u8 *)arg0 + 0x20), 0x55, GetWindowWidth(arg0 + 0x2C0) * 8, 0, 0, 0x10200, 1);
    var_r5 = 0;
    sp1C = arg0;
    temp_r6 = arg0 + 0x50;
loop_6:
    BufferMoveName(*(MessageFormat **)((u8 *)arg0 + 0x24), var_r5, (u32) *(u16 *)((u8 *)sp1C + 0x828));
    ov83_02241DD8(arg0, temp_r6 + ((var_r5 + 0x28) * 0x10), *(MsgData **)((u8 *)arg0 + 0x20), var_r5 + 0x60, 0, 0, 0, 0x10200, 0);
    if (*(u16 *)((u8 *)sp1C + 0x828) == 0) {
        temp_r7 = (var_r5 + 0x2C) * 0x10;
        ov83_022479E4(temp_r6 + temp_r7, *(MsgData **)((u8 *)arg0 + 0x20), 0x66, ((GetWindowWidth(temp_r6 + temp_r7) * 8) / 2), 0, 0, 0x10200, 2);
    } else {
        sp28 = NewString_ReadMsgData(*(MsgData **)((u8 *)arg0 + 0x20), 0x5A);
        sp24 = (u32) (FontID_String_GetWidth(FONT_SYSTEM, sp28, 0U) << 0xF) >> 0x10;
        temp_r7_2 = (var_r5 + 0x2C) * 0x10;
        temp_r1 = GetWindowWidth(temp_r6 + temp_r7_2) * 8;
        sp20 = (u32) ((temp_r1 + (temp_r1 >> 0x1F)) << 0xF) >> 0x10;
        ov83_02247998(temp_r6 + temp_r7_2, sp28, sp20, 0, 0, 0x10200, 2);
        String_Delete(sp28);
        sp18 = arg0 + var_r5;
        ov83_02240C48(arg0, 0, *(u8 *)((u8 *)sp18 + 0x830), 2, 0);
        temp_r0_3 = NewString_ReadMsgData(*(MsgData **)((u8 *)arg0 + 0x20), 0x65);
        sp2C = temp_r0_3;
        StringExpandPlaceholders(*(MessageFormat **)((u8 *)arg0 + 0x24), *(String **)((u8 *)arg0 + 0x28), sp2C);
        temp_r1 = FontID_String_GetWidth(FONT_SYSTEM, *(String **)((u8 *)arg0 + 0x28), 0U);
        ov83_02247998(temp_r6 + temp_r7_2, *(String **)((u8 *)arg0 + 0x28), (sp20 - sp24) - temp_r1, 0, 0, 0x10200, 0);
        String_Delete(temp_r0_3);
        ov83_02240C48(arg0, 0, *(u8 *)((u8 *)sp18 + 0x834), 2, 0);
        ov83_02241DD8(arg0, temp_r6 + temp_r7_2, *(MsgData **)((u8 *)arg0 + 0x20), 0x65, sp20 + sp24, 0, 0, 0x10200, 0);
    }
    var_r5 += 1;
    sp1C += 2;
    if (var_r5 < 4U) {
        goto loop_6;
    }
    if (sp14 == 1) {
        var_r5_2 = 0x12;
        var_r4 = arg0 + 0x170;
        do {
            ScheduleWindowCopyToVram(var_r4);
            var_r5_2 += 1;
            var_r4 = (void *)((u8 *)var_r4 + (0x10));
        } while (var_r5_2 <= 0x2FU);
        return;
    }
    CopyWindowPixelsToVram_TextMode(arg0 + 0x170);
    CopyWindowPixelsToVram_TextMode(arg0 + 0x180);
    CopyWindowPixelsToVram_TextMode(arg0 + 0x1A0);
    CopyWindowPixelsToVram_TextMode(arg0 + 0x1C0);
    CopyWindowPixelsToVram_TextMode(arg0 + 0x1E0);
    CopyWindowPixelsToVram_TextMode(arg0 + 0x200);
    CopyWindowPixelsToVram_TextMode(arg0 + 0x220);
    CopyWindowPixelsToVram_TextMode(arg0 + 0x240);
    CopyWindowPixelsToVram_TextMode(arg0 + 0x260);
    CopyWindowPixelsToVram_TextMode(arg0 + 0x280);
    CopyWindowPixelsToVram_TextMode(arg0 + 0x2A0);
    CopyWindowPixelsToVram_TextMode(arg0 + 0x2C0);
    CopyWindowPixelsToVram_TextMode(arg0 + 0x2D0);
    CopyWindowPixelsToVram_TextMode(arg0 + 0x2E0);
    CopyWindowPixelsToVram_TextMode(arg0 + 0x2F0);
    CopyWindowPixelsToVram_TextMode(arg0 + 0x300);
    CopyWindowPixelsToVram_TextMode(arg0 + 0x310);
    CopyWindowPixelsToVram_TextMode(arg0 + 0x320);
    CopyWindowPixelsToVram_TextMode(arg0 + 0x330);
    CopyWindowPixelsToVram_TextMode(arg0 + 0x340);
}