#include "global.h"

#include "move.h"
#include "overlay_83_battle_castle_opponent_app.h"
#include "party.h"
#include "pokemon.h"
#include "bg_window.h"
#include "msgdata.h"

u8 ov83_02247768(u8, u8);

void ov83_02245D48(BattleCastleOpponentApp *arg0);

void ov83_022479E4(Window *, MsgData *, s32, u32, u32, u32, u32, s32);
void ov83_02245F24(BattleCastleOpponentApp *app);

void ov83_02245D48(BattleCastleOpponentApp *arg0) {
    s32 decrypt_result;
    Pokemon *mon;
    u16 i;
    u16 ppUps;
    u8 ppUpsByte;

    mon = Party_GetMonByIndex(arg0->opponentsParty, ov83_02247768(arg0->numSlots, arg0->selectedMonSlot));
    decrypt_result = AcquireMonLock(mon);
    arg0->mon = mon;
    arg0->boxMon = Mon_GetBoxMon(mon);
    arg0->species = (u16)GetMonData(mon, MON_DATA_SPECIES, 0L);
    arg0->level = (u8)GetMonData(mon, 0xA1, 0L);
    arg0->ability = (u8)GetMonData(mon, 0xA, 0L);
    arg0->nature = GetMonNature(mon);
    arg0->heldItem = (u16)GetMonData(mon, 6, 0L);
    arg0->stats[0] = (u16)GetMonData(mon, 0xA3, 0L);
    arg0->stats[1] = (u16)GetMonData(mon, 0xA4, 0L);
    arg0->stats[2] = (u16)GetMonData(mon, 0xA5, 0L);
    arg0->stats[3] = (u16)GetMonData(mon, 0xA8, 0L);
    arg0->stats[4] = (u16)GetMonData(mon, 0xA6, 0L);
    arg0->stats[5] = (u16)GetMonData(mon, 0xA9, 0L);
    arg0->stats[6] = (u16)GetMonData(mon, 0xA7, 0L);
    arg0->unk_5DE = (s8)GetMonData(mon, 0x70, 0L);
    arg0->personality = GetMonData(mon, 0, 0L);
    if (GetMonData(mon, MON_DATA_NO_PRINT_GENDER, 0L) == 1) {
        arg0->noPrintGender = 0;
    } else {
        arg0->noPrintGender = 1;
    }
    arg0->gender = GetMonGender(mon);
    i = 0;
    do {
        arg0->moves[i] = (u16)GetMonData(mon, i + MON_DATA_MOVE1, 0L);
        arg0->movePP[i] = (u8)GetMonData(mon, i + 0x3A, 0L);
        ppUps = (u16)GetMonData(mon, i + 0x3E, 0L);
        ppUpsByte = (u8)ppUps;
        arg0->moveMaxPP[i] = GetMoveMaxPP(arg0->moves[i], ppUpsByte);
        i += 1;
    } while (((u32)i) < 4U);
    ReleaseMonLock(mon, decrypt_result);
}

void ov83_02245F24(BattleCastleOpponentApp *app) {
    FillWindowPixelBuffer(&app->windows[14], 0);
    FillWindowPixelBuffer(&app->windows[16], 0);
    FillWindowPixelBuffer(&app->windows[18], 0);
    FillWindowPixelBuffer(&app->windows[20], 0);
    FillWindowPixelBuffer(&app->windows[22], 0);
    FillWindowPixelBuffer(&app->windows[24], 0);
    FillWindowPixelBuffer(&app->windows[26], 0);
    FillWindowPixelBuffer(&app->windows[28], 0);
    FillWindowPixelBuffer(&app->windows[30], 0);
    FillWindowPixelBuffer(&app->windows[32], 0);
    ov83_022479E4(&app->windows[14], app->msgLoader, 0x42, 0, 0, 0, 0x10200, 0);
    ov83_022479E4(&app->windows[16], app->msgLoader, 0x34, 0, 0, 0, 0x10200, 0);
    ov83_022479E4(&app->windows[18], app->msgLoader, 0x32, 0, 0, 0, 0x10200, 0);
    ov83_022479E4(&app->windows[20], app->msgLoader, 0x30, 0, 0, 0, 0x10200, 0);
    ov83_022479E4(&app->windows[22], app->msgLoader, 0x43, 0, 0, 0, 0x10200, 0);
    ov83_022479E4(&app->windows[24], app->msgLoader, 0x36, 0, 0, 0, 0x10200, 0);
    ov83_022479E4(&app->windows[26], app->msgLoader, 0x3a, 0, 0, 0, 0x10200, 0);
    ov83_022479E4(&app->windows[28], app->msgLoader, 0x38, 0, 0, 0, 0x10200, 0);
    ov83_022479E4(&app->windows[30], app->msgLoader, 0x3c, 0, 0, 0, 0x10200, 0);
    ov83_022479E4(&app->windows[32], app->msgLoader, 0x3e, 0, 0, 0, 0x10200, 0);
    CopyWindowPixelsToVram_TextMode(&app->windows[14]);
    CopyWindowPixelsToVram_TextMode(&app->windows[16]);
    CopyWindowPixelsToVram_TextMode(&app->windows[18]);
    CopyWindowPixelsToVram_TextMode(&app->windows[20]);
    CopyWindowPixelsToVram_TextMode(&app->windows[22]);
    CopyWindowPixelsToVram_TextMode(&app->windows[24]);
    CopyWindowPixelsToVram_TextMode(&app->windows[26]);
    CopyWindowPixelsToVram_TextMode(&app->windows[28]);
    CopyWindowPixelsToVram_TextMode(&app->windows[30]);
    CopyWindowPixelsToVram_TextMode(&app->windows[32]);
    return;
}
