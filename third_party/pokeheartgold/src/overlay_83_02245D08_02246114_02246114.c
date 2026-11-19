#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "pokemon.h"
#include "pokemon_types_def.h"
#include "overlay_83_app_02246114.h"

s32 ov83_02244A98(void *, s32, s32, s32, s32);

s32 ov83_02245D08(void *, void *, MsgData *, s32, u32, u32, s32, s32, s32);

u8 ov83_02247768(u8, u8);

u8 ov83_0224777C(s32, u8, u8, s32);

s32 ov83_02247998(void *, String *, s32, u32, s32, s32, s32);

void ov83_022479E4(void *window, void *msgData, s32 strno, u32 x, u32 y, u32 fontId, u32 color, s32 alignment);

typedef struct {
    u8 unk0[0x5C6];
    u8 mode : 7;
    u8 flag : 1;
} UnkStruct_ov83_02246114;

void ov83_02246114(void *arg0, s32 arg1, s32 arg2, s32 arg3);

void ov83_02246114(void *arg0, s32 arg1, s32 arg2, s32 arg3) {
    u8 i;
    u8 j;
    s32 sp14;
    String *var_r7;
    unsigned int temp_r0_2;
    void *new_var;
    u32 var_r4;
    u32 var_r6;
    void *var_r5;
    var_r5 = arg0;
    sp14 = arg1;
    var_r6 = ov83_0224777C(((Ov83App_02246114 *)var_r5)->unk_2BC, *((u8 *)&((Ov83App_02246114 *)var_r5)->unk_0[9]), 2, arg3);
    var_r4 = ov83_02247768(*((u8 *)&((Ov83App_02246114 *)var_r5)->unk_0[20]), *((u8 *)&((Ov83App_02246114 *)var_r5)->unk_0[13]));
    FillWindowPixelBuffer(var_r5 + 0x110, 0U);
    FillWindowPixelBuffer(var_r5 + 0x120, 0U);
    FillWindowPixelBuffer(var_r5 + 0x140, 0U);
    FillWindowPixelBuffer(var_r5 + 0x160, 0U);
    FillWindowPixelBuffer(var_r5 + 0x180, 0U);
    FillWindowPixelBuffer(var_r5 + 0x1A0, 0U);
    FillWindowPixelBuffer(var_r5 + 0x1C0, 0U);
    FillWindowPixelBuffer(var_r5 + 0x1E0, 0U);
    FillWindowPixelBuffer(var_r5 + 0x200, 0U);
    FillWindowPixelBuffer(var_r5 + 0x220, 0U);
    FillWindowPixelBuffer(var_r5 + 0x240, 0U);
    FillWindowPixelBuffer(var_r5 + 0x260, 0U);
    FillWindowPixelBuffer(var_r5 + 0x270, 0U);
    if (((UnkStruct_ov83_02246114 *)var_r5)->flag == 0) {
        temp_r0_2 = ((UnkStruct_ov83_02246114 *)var_r5)->mode;
        if (temp_r0_2 == 0) {
            ov83_022479E4(var_r5 + 0x120, ((Ov83App_02246114 *)var_r5)->unk_20, 0x40, 0U, 0U, 0U, 0x50600, 0);
        } else if (temp_r0_2 == 1) {
            ov83_022479E4(var_r5 + 0x120, ((Ov83App_02246114 *)var_r5)->unk_20, 0x41, 0U, 0U, 0U, 0x30400, 0);
        }
    }
    ov83_02244A98(var_r5, 0, *((u8 *)&((Ov83App_02246114 *)var_r5)->unk_3F4[467]), 3, 0);
    ov83_02245D08(var_r5, var_r5 + 0x140, ((Ov83App_02246114 *)var_r5)->unk_20, 0x48, 0U, 0U, 0, 0x10200, 0);
    if ((*((u8 *)((((Ov83App_02246114 *)var_r5)->unk_54C) + var_r4))) != 0) {
        BufferBoxMonSpeciesName(((Ov83App_02246114 *)var_r5)->unk_24, 0U, Mon_GetBoxMon(((Pokemon *)((Ov83App_02246114 *)var_r5)->unk_5BC)));
        ov83_02245D08(var_r5, var_r5 + 0x110, ((Ov83App_02246114 *)var_r5)->unk_20, 0x45, 0U, 0U, 0, 0x10200, 0);
        ov83_02244A98(var_r5, 0, ((Ov83App_02246114 *)var_r5)->unk_5D0, 3, 0);
        ov83_02244A98(var_r5, 1, ((Ov83App_02246114 *)var_r5)->unk_5D2, 3, 0);
        ov83_02245D08(var_r5, var_r5 + 0x1C0, ((Ov83App_02246114 *)var_r5)->unk_20, 0x4E, GetWindowWidth(var_r5 + 0x1C0) * 8, 0U, 0, 0x10200, 1);
    } else {
        ov83_022479E4(var_r5 + 0x110, ((Ov83App_02246114 *)var_r5)->unk_20, 0x4B, 0U, 0U, 0U, 0x10200, 0);
        ov83_022479E4(var_r5 + 0x1C0, ((Ov83App_02246114 *)var_r5)->unk_20, 0x4D, GetWindowWidth(var_r5 + 0x1C0) * 8, 0U, 0, 0x10200, 1);
    }
    if ((*((u8 *)((((Ov83App_02246114 *)var_r5)->unk_554) + var_r4))) != 0) {
        BufferAbilityName(((Ov83App_02246114 *)var_r5)->unk_24, 0U, (u32)(*((u8 *)&((Ov83App_02246114 *)var_r5)->unk_5C0[8])));
        ov83_02245D08(var_r5, var_r5 + 0x160, ((Ov83App_02246114 *)var_r5)->unk_20, 0x35, 0U, 0U, 0, 0x10200, 0);
        BufferNatureName(((Ov83App_02246114 *)var_r5)->unk_24, 0U, (u32)(*((u8 *)&((Ov83App_02246114 *)var_r5)->unk_5C0[9])));
        ov83_02245D08(var_r5, var_r5 + 0x180, ((Ov83App_02246114 *)var_r5)->unk_20, 0x33, 0U, 0U, 0, 0x10200, 0);
        BufferItemName(((Ov83App_02246114 *)var_r5)->unk_24, 0U, (u32)(((Ov83App_02246114 *)var_r5)->unk_5CA));
        ov83_02245D08(var_r5, var_r5 + 0x1A0, ((Ov83App_02246114 *)var_r5)->unk_20, 0x31, 0U, 0U, 0, 0x10200, 0);
        ov83_02244A98(var_r5, 0, ((Ov83App_02246114 *)var_r5)->unk_5D4, 3, 0);
        ov83_02245D08(var_r5, var_r5 + 0x1E0, ((Ov83App_02246114 *)var_r5)->unk_20, 0x37, GetWindowWidth(var_r5 + 0x1E0) * 8, 0U, 0, 0x10200, 1);
        ov83_02244A98(var_r5, 0, ((Ov83App_02246114 *)var_r5)->unk_5D6, 3, 0);
        ov83_02245D08(var_r5, var_r5 + 0x200, ((Ov83App_02246114 *)var_r5)->unk_20, 0x3B, GetWindowWidth(var_r5 + 0x200) * 8, 0U, 0, 0x10200, 1);
        ov83_02244A98(var_r5, 0, ((Ov83App_02246114 *)var_r5)->unk_5D8, 3, 0);
        ov83_02245D08(var_r5, var_r5 + 0x220, ((Ov83App_02246114 *)var_r5)->unk_20, 0x39, GetWindowWidth(var_r5 + 0x220) * 8, 0U, 0, 0x10200, 1);
        ov83_02244A98(var_r5, 0, ((Ov83App_02246114 *)var_r5)->unk_5DA, 3, 0);
        ov83_02245D08(var_r5, var_r5 + 0x240, ((Ov83App_02246114 *)var_r5)->unk_20, 0x3D, GetWindowWidth(var_r5 + 0x240) * 8, 0U, 0, 0x10200, 1);
        ov83_02244A98(var_r5, 0, ((Ov83App_02246114 *)var_r5)->unk_5DC, 3, 0);
        ov83_02245D08(var_r5, var_r5 + 0x260, ((Ov83App_02246114 *)var_r5)->unk_20, 0x3F, GetWindowWidth(var_r5 + 0x260) * 8, 0U, 0, 0x10200, 1);
    } else {
        ov83_022479E4(var_r5 + 0x160, ((Ov83App_02246114 *)var_r5)->unk_20, 0x4B, 0U, 0U, 0U, 0x10200, 0);
        ov83_022479E4(var_r5 + 0x180, ((Ov83App_02246114 *)var_r5)->unk_20, 0x4B, 0U, 0U, 0U, 0x10200, 0);
        ov83_022479E4(var_r5 + 0x1A0, ((Ov83App_02246114 *)var_r5)->unk_20, 0x4B, 0U, 0U, 0U, 0x10200, 0);
        ov83_022479E4(var_r5 + 0x1E0, ((Ov83App_02246114 *)var_r5)->unk_20, 0x4A, GetWindowWidth(var_r5 + 0x1E0) * 8, 0U, 0U, 0x10200, 1);
        ov83_022479E4(var_r5 + 0x200, ((Ov83App_02246114 *)var_r5)->unk_20, 0x4A, GetWindowWidth(var_r5 + 0x200) * 8, 0U, 0, 0x10200, 1);
        ov83_022479E4(var_r5 + 0x220, ((Ov83App_02246114 *)var_r5)->unk_20, 0x4A, GetWindowWidth(var_r5 + 0x220) * 8, 0U, 0, 0x10200, 1);
        ov83_022479E4(var_r5 + 0x240, ((Ov83App_02246114 *)var_r5)->unk_20, 0x4A, GetWindowWidth(0x240 + var_r5) * 8, 0U, 0, 0x10200, 1);
        ov83_022479E4(var_r5 + 0x260, ((Ov83App_02246114 *)var_r5)->unk_20, 0x4A, GetWindowWidth(var_r5 + 0x260) * 8, 0U, 0, 0x10200, 1);
    }
    if (var_r6 == 1) {
        ov83_022479E4(var_r5 + 0x270, ((Ov83App_02246114 *)var_r5)->unk_20, 0x4C, 0U, 0U, 0U, 0x10200, 0);
    } else if ((*((u8 *)((((Ov83App_02246114 *)var_r5)->unk_558) + var_r4))) == 0) {
        for (i = 0; i < 4; i++) {
            var_r4 = i * 0x10;
            ov83_022479E4(var_r5 + 0x270, ((Ov83App_02246114 *)var_r5)->unk_20, 0x4B, 0U, var_r4, 0, 0x10200, 0);
            var_r7 = NewString_ReadMsgData(((Ov83App_02246114 *)var_r5)->unk_20, 0x44);
            var_r6 = (u8)(FontID_String_GetWidth(FONT_SYSTEM, var_r7, 0U) / 2);
            ov83_02247998(var_r5 + 0x270, var_r7, 0x78, var_r4, 0, 0x10200, 2);
            String_Delete(var_r7);
            var_r7 = NewString_ReadMsgData(((Ov83App_02246114 *)var_r5)->unk_20, 0x49);
            ov83_02247998(var_r5 + 0x270, var_r7, (0x78 - var_r6) - FontID_String_GetWidth(FONT_SYSTEM, var_r7, 0U), var_r4, 0, 0x10200, 0);
            String_Delete(var_r7);
            ov83_022479E4(var_r5 + 0x270, ((Ov83App_02246114 *)var_r5)->unk_20, 0x49, var_r6 + 0x78, var_r4, 0, 0x10200, 0);
        }

    } else {
        for (j = 0; j < 4; j++) {
            BufferMoveName(((Ov83App_02246114 *)var_r5)->unk_24, j, *((u16 *)((((u8 *)var_r5) + (j * 2)) + 0x5E0)));
            new_var = var_r5 + 0x270;
            ov83_02245D08(var_r5, new_var, ((Ov83App_02246114 *)var_r5)->unk_20, j + 0x54, 0U, j * 0x10, 0, 0x10200, 0);
            if ((*((u16 *)((((u8 *)var_r5) + (j * 2)) + 0x5E0))) == 0) {
                ov83_022479E4(new_var, ((Ov83App_02246114 *)var_r5)->unk_20, 0x5A, 0x78U, j * 0x10, 0, 0x10200, 2);
            } else {
                var_r7 = NewString_ReadMsgData(((Ov83App_02246114 *)var_r5)->unk_20, 0x44);
                var_r6 = (u8)(FontID_String_GetWidth(FONT_SYSTEM, var_r7, 0U) / 2);
                ov83_02247998(new_var, var_r7, 0x78, j * 0x10, 0, 0x10200, 2);
                String_Delete(var_r7);
                ov83_02244A98(var_r5, 0, *((u8 *)((((u8 *)var_r5) + j) + 0x5E8)), 2, 0);
                var_r7 = NewString_ReadMsgData(((Ov83App_02246114 *)var_r5)->unk_20, 0x59);
                StringExpandPlaceholders(((Ov83App_02246114 *)var_r5)->unk_24, ((Ov83App_02246114 *)var_r5)->unk_28, var_r7);
                ov83_02247998(new_var, ((Ov83App_02246114 *)var_r5)->unk_28, (0x78 - var_r6) - FontID_String_GetWidth(FONT_SYSTEM, ((Ov83App_02246114 *)var_r5)->unk_28, 0U), j * 0x10, 0, 0x10200, 0);
                String_Delete(var_r7);
                ov83_02244A98(var_r5, 0, *((u8 *)((((u8 *)var_r5) + j) + 0x5EC)), 2, 0);
                var_r6 += 0x78;
                ov83_02245D08(var_r5, new_var, ((Ov83App_02246114 *)var_r5)->unk_20, 0x59, var_r6, j * 0x10, 0, 0x10200, 0);
            }
        }
    }
    if (sp14 == 1) {
        var_r4 = 0xC;
        var_r5 += 0x50;
        do {
            ScheduleWindowCopyToVram(var_r5 + (var_r4 * 0x10));
            var_r4 = (u32)((u8)(var_r4 + 1));
        } while (var_r4 <= 0x22U);
        return;
    }
    CopyWindowPixelsToVram_TextMode(var_r5 + 0x110);
    CopyWindowPixelsToVram_TextMode(var_r5 + 0x120);
    CopyWindowPixelsToVram_TextMode(var_r5 + 0x140);
    CopyWindowPixelsToVram_TextMode(var_r5 + 0x160);
    CopyWindowPixelsToVram_TextMode(var_r5 + 0x180);
    CopyWindowPixelsToVram_TextMode(var_r5 + 0x1A0);
    CopyWindowPixelsToVram_TextMode(var_r5 + 0x1C0);
    CopyWindowPixelsToVram_TextMode(var_r5 + 0x1E0);
    CopyWindowPixelsToVram_TextMode(var_r5 + 0x200);
    CopyWindowPixelsToVram_TextMode(var_r5 + 0x220);
    CopyWindowPixelsToVram_TextMode(var_r5 + 0x240);
    CopyWindowPixelsToVram_TextMode(var_r5 + 0x260);
    CopyWindowPixelsToVram_TextMode(var_r5 + 0x270);
}
