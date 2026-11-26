#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "pokemon.h"
#include "pokemon_types_def.h"
#include "overlay_83_battle_castle_opponent_app.h"

s32 ov83_02244A98(BattleCastleOpponentApp *app, s32, s32, s32, s32);

s32 ov83_02245D08(void *, void *, MsgData *, s32, u32, u32, s32, s32, s32);

u8 ov83_02247768(u8, u8);

u8 ov83_0224777C(SaveData *saveData, u8, u8);

s32 ov83_02247998(void *, String *, s32, u32, s32, s32, s32);

void ov83_022479E4(void *window, void *msgData, s32 strno, u32 x, u32 y, u32 fontId, u32 color, s32 alignment);

void ov83_02246114(BattleCastleOpponentApp *arg0, s32 arg1);

void ov83_02246114(BattleCastleOpponentApp *arg0, s32 arg1) {
    u8 i;
    u8 j;
    s32 sp14;
    String *var_r7;
    unsigned int temp_r0_2;
    void *new_var;
    u32 var_r4;
    u32 var_r6;
    BattleCastleOpponentApp *var_r5;
    var_r5 = arg0;
    sp14 = arg1;
    var_r6 = ov83_0224777C(var_r5->saveData, var_r5->challengeType, 2);
    var_r4 = ov83_02247768(var_r5->numSlots, var_r5->selectedMonSlot);
    FillWindowPixelBuffer(&var_r5->windows[12], 0U);
    FillWindowPixelBuffer(&var_r5->windows[13], 0U);
    FillWindowPixelBuffer(&var_r5->windows[15], 0U);
    FillWindowPixelBuffer(&var_r5->windows[17], 0U);
    FillWindowPixelBuffer(&var_r5->windows[19], 0U);
    FillWindowPixelBuffer(&var_r5->windows[21], 0U);
    FillWindowPixelBuffer(&var_r5->windows[23], 0U);
    FillWindowPixelBuffer(&var_r5->windows[25], 0U);
    FillWindowPixelBuffer(&var_r5->windows[27], 0U);
    FillWindowPixelBuffer(&var_r5->windows[29], 0U);
    FillWindowPixelBuffer(&var_r5->windows[31], 0U);
    FillWindowPixelBuffer(&var_r5->windows[33], 0U);
    FillWindowPixelBuffer(&var_r5->windows[34], 0U);
    if (var_r5->noPrintGender == 0) {
        temp_r0_2 = var_r5->gender;
        if (temp_r0_2 == 0) {
            ov83_022479E4(&var_r5->windows[13], var_r5->msgLoader, 0x40, 0U, 0U, 0U, 0x50600, 0);
        } else if (temp_r0_2 == 1) {
            ov83_022479E4(&var_r5->windows[13], var_r5->msgLoader, 0x41, 0U, 0U, 0U, 0x30400, 0);
        }
    }
    ov83_02244A98(var_r5, 0, var_r5->level, 3, 0);
    ov83_02245D08(var_r5, &var_r5->windows[15], var_r5->msgLoader, 0x48, 0U, 0U, 0, 0x10200, 0);
    if ((*((u8 *)((var_r5->identityUnlocked) + var_r4))) != 0) {
        BufferBoxMonSpeciesName(var_r5->strTemplate, 0U, Mon_GetBoxMon(((Pokemon *)var_r5->mon)));
        ov83_02245D08(var_r5, &var_r5->windows[12], var_r5->msgLoader, 0x45, 0U, 0U, 0, 0x10200, 0);
        ov83_02244A98(var_r5, 0, var_r5->stats[0], 3, 0);
        ov83_02244A98(var_r5, 1, var_r5->stats[1], 3, 0);
        ov83_02245D08(var_r5, &var_r5->windows[23], var_r5->msgLoader, 0x4E, GetWindowWidth(&var_r5->windows[23]) * 8, 0U, 0, 0x10200, 1);
    } else {
        ov83_022479E4(&var_r5->windows[12], var_r5->msgLoader, 0x4B, 0U, 0U, 0U, 0x10200, 0);
        ov83_022479E4(&var_r5->windows[23], var_r5->msgLoader, 0x4D, GetWindowWidth(&var_r5->windows[23]) * 8, 0U, 0, 0x10200, 1);
    }
    if ((*((u8 *)((var_r5->statsUnlocked) + var_r4))) != 0) {
        BufferAbilityName(var_r5->strTemplate, 0U, (u32)(var_r5->ability));
        ov83_02245D08(var_r5, &var_r5->windows[17], var_r5->msgLoader, 0x35, 0U, 0U, 0, 0x10200, 0);
        BufferNatureName(var_r5->strTemplate, 0U, (u32)(var_r5->nature));
        ov83_02245D08(var_r5, &var_r5->windows[19], var_r5->msgLoader, 0x33, 0U, 0U, 0, 0x10200, 0);
        BufferItemName(var_r5->strTemplate, 0U, (u32)(var_r5->heldItem));
        ov83_02245D08(var_r5, &var_r5->windows[21], var_r5->msgLoader, 0x31, 0U, 0U, 0, 0x10200, 0);
        ov83_02244A98(var_r5, 0, var_r5->stats[2], 3, 0);
        ov83_02245D08(var_r5, &var_r5->windows[25], var_r5->msgLoader, 0x37, GetWindowWidth(&var_r5->windows[25]) * 8, 0U, 0, 0x10200, 1);
        ov83_02244A98(var_r5, 0, var_r5->stats[3], 3, 0);
        ov83_02245D08(var_r5, &var_r5->windows[27], var_r5->msgLoader, 0x3B, GetWindowWidth(&var_r5->windows[27]) * 8, 0U, 0, 0x10200, 1);
        ov83_02244A98(var_r5, 0, var_r5->stats[4], 3, 0);
        ov83_02245D08(var_r5, &var_r5->windows[29], var_r5->msgLoader, 0x39, GetWindowWidth(&var_r5->windows[29]) * 8, 0U, 0, 0x10200, 1);
        ov83_02244A98(var_r5, 0, var_r5->stats[5], 3, 0);
        ov83_02245D08(var_r5, &var_r5->windows[31], var_r5->msgLoader, 0x3D, GetWindowWidth(&var_r5->windows[31]) * 8, 0U, 0, 0x10200, 1);
        ov83_02244A98(var_r5, 0, var_r5->stats[6], 3, 0);
        ov83_02245D08(var_r5, &var_r5->windows[33], var_r5->msgLoader, 0x3F, GetWindowWidth(&var_r5->windows[33]) * 8, 0U, 0, 0x10200, 1);
    } else {
        ov83_022479E4(&var_r5->windows[17], var_r5->msgLoader, 0x4B, 0U, 0U, 0U, 0x10200, 0);
        ov83_022479E4(&var_r5->windows[19], var_r5->msgLoader, 0x4B, 0U, 0U, 0U, 0x10200, 0);
        ov83_022479E4(&var_r5->windows[21], var_r5->msgLoader, 0x4B, 0U, 0U, 0U, 0x10200, 0);
        ov83_022479E4(&var_r5->windows[25], var_r5->msgLoader, 0x4A, GetWindowWidth(&var_r5->windows[25]) * 8, 0U, 0U, 0x10200, 1);
        ov83_022479E4(&var_r5->windows[27], var_r5->msgLoader, 0x4A, GetWindowWidth(&var_r5->windows[27]) * 8, 0U, 0, 0x10200, 1);
        ov83_022479E4(&var_r5->windows[29], var_r5->msgLoader, 0x4A, GetWindowWidth(&var_r5->windows[29]) * 8, 0U, 0, 0x10200, 1);
        ov83_022479E4(&var_r5->windows[31], var_r5->msgLoader, 0x4A, GetWindowWidth(&var_r5->windows[31]) * 8, 0U, 0, 0x10200, 1);
        ov83_022479E4(&var_r5->windows[33], var_r5->msgLoader, 0x4A, GetWindowWidth(&var_r5->windows[33]) * 8, 0U, 0, 0x10200, 1);
    }
    if (var_r6 == 1) {
        ov83_022479E4(&var_r5->windows[34], var_r5->msgLoader, 0x4C, 0U, 0U, 0U, 0x10200, 0);
    } else if ((*((u8 *)((var_r5->movesUnlocked) + var_r4))) == 0) {
        for (i = 0; i < 4; i++) {
            var_r4 = i * 0x10;
            ov83_022479E4(&var_r5->windows[34], var_r5->msgLoader, 0x4B, 0U, var_r4, 0, 0x10200, 0);
            var_r7 = NewString_ReadMsgData(var_r5->msgLoader, 0x44);
            var_r6 = (u8)(FontID_String_GetWidth(FONT_SYSTEM, var_r7, 0U) / 2);
            ov83_02247998(&var_r5->windows[34], var_r7, 0x78, var_r4, 0, 0x10200, 2);
            String_Delete(var_r7);
            var_r7 = NewString_ReadMsgData(var_r5->msgLoader, 0x49);
            ov83_02247998(&var_r5->windows[34], var_r7, (0x78 - var_r6) - FontID_String_GetWidth(FONT_SYSTEM, var_r7, 0U), var_r4, 0, 0x10200, 0);
            String_Delete(var_r7);
            ov83_022479E4(&var_r5->windows[34], var_r5->msgLoader, 0x49, var_r6 + 0x78, var_r4, 0, 0x10200, 0);
        }

    } else {
        for (j = 0; j < 4; j++) {
            BufferMoveName(var_r5->strTemplate, j, var_r5->moves[j]);
            new_var = &var_r5->windows[34];
            ov83_02245D08(var_r5, new_var, var_r5->msgLoader, j + 0x54, 0U, j * 0x10, 0, 0x10200, 0);
            if (var_r5->moves[j] == 0) {
                ov83_022479E4(new_var, var_r5->msgLoader, 0x5A, 0x78U, j * 0x10, 0, 0x10200, 2);
            } else {
                var_r7 = NewString_ReadMsgData(var_r5->msgLoader, 0x44);
                var_r6 = (u8)(FontID_String_GetWidth(FONT_SYSTEM, var_r7, 0U) / 2);
                ov83_02247998(new_var, var_r7, 0x78, j * 0x10, 0, 0x10200, 2);
                String_Delete(var_r7);
                ov83_02244A98(var_r5, 0, var_r5->movePP[j], 2, 0);
                var_r7 = NewString_ReadMsgData(var_r5->msgLoader, 0x59);
                StringExpandPlaceholders(var_r5->strTemplate, var_r5->displayStr, var_r7);
                ov83_02247998(new_var, var_r5->displayStr, (0x78 - var_r6) - FontID_String_GetWidth(FONT_SYSTEM, var_r5->displayStr, 0U), j * 0x10, 0, 0x10200, 0);
                String_Delete(var_r7);
                ov83_02244A98(var_r5, 0, var_r5->moveMaxPP[j], 2, 0);
                var_r6 += 0x78;
                ov83_02245D08(var_r5, new_var, var_r5->msgLoader, 0x59, var_r6, j * 0x10, 0, 0x10200, 0);
            }
        }
    }
    if (sp14 == 1) {
        var_r4 = 0xC;
        do {
            ScheduleWindowCopyToVram(&var_r5->windows[var_r4]);
            var_r4 = (u32)((u8)(var_r4 + 1));
        } while (var_r4 <= 0x22U);
        return;
    }
    CopyWindowPixelsToVram_TextMode(&var_r5->windows[12]);
    CopyWindowPixelsToVram_TextMode(&var_r5->windows[13]);
    CopyWindowPixelsToVram_TextMode(&var_r5->windows[15]);
    CopyWindowPixelsToVram_TextMode(&var_r5->windows[17]);
    CopyWindowPixelsToVram_TextMode(&var_r5->windows[19]);
    CopyWindowPixelsToVram_TextMode(&var_r5->windows[21]);
    CopyWindowPixelsToVram_TextMode(&var_r5->windows[23]);
    CopyWindowPixelsToVram_TextMode(&var_r5->windows[25]);
    CopyWindowPixelsToVram_TextMode(&var_r5->windows[27]);
    CopyWindowPixelsToVram_TextMode(&var_r5->windows[29]);
    CopyWindowPixelsToVram_TextMode(&var_r5->windows[31]);
    CopyWindowPixelsToVram_TextMode(&var_r5->windows[33]);
    CopyWindowPixelsToVram_TextMode(&var_r5->windows[34]);
}
