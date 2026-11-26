#include "global.h"
struct BattleCastleOpponentApp;

#include "frontier/overlay_80_022340E8.h"

#include "bg_window.h"
#include "message_printer.h"
#include "party.h"
#include "pokemon.h"
#include "unk_0200CE7C.h"
#include "overlay_83_battle_castle_opponent_app.h"
#include "overlay_83_battle_castle_self_app.h"

u8 ov80_02237B58(u8, s32);

void ov83_022448E4(BattleCastleOpponentApp *arg0, Window *window);

s32 ov83_02244BA8(void *, Window *, s32, s32, s32, s32);
void ov83_022449D4(BattleCastleOpponentApp *arg0, Window *window);

void ov83_022448E4(BattleCastleOpponentApp *arg0, Window *window) {
    Pokemon *pokemon;
    u32 v0;
    u32 x;
    s32 v1;
    Pokemon *mon;
    s32 v2;
    s32 slot;
    u32 v3;

    FillWindowPixelBuffer(window, 0U);
    if (ov80_02237D8C(arg0->challengeType) == 0) {
        v3 = 0x24;
    } else {
        v3 = 4;
    }
    v2 = ov80_02237B58(arg0->challengeType, 1);
    slot = 0;
    v1 = v2;
    if (v2 > 0) {
        v0 = v3;
        v0 = v3 + 0x18;
        x = v3;
        x = v3 + 0x20;
        do {
            if (*(u8 *)(arg0->identityUnlocked + slot) == 0) {
                FillWindowPixelRect(window, 0U, (u16)v3, 1U, 0x40U, 0x10U);
            } else {
                mon = Party_GetMonByIndex(arg0->opponentsParty, slot);
                pokemon = mon;
                PrintUIntOnWindow(arg0->msgPrinter, GetMonData(mon, 0xA3, NULL), 3U, PRINTING_MODE_RIGHT_ALIGN, window, v3, 1U);
                sub_0200CDAC(arg0->msgPrinter, 0U, window, v0, 1U);
                PrintUIntOnWindow(arg0->msgPrinter, GetMonData(mon, 0xA4, NULL), 3U, PRINTING_MODE_LEFT_ALIGN, window, x, 1U);
            }
            v0 += 0x40;
            v3 += 0x40;
            x += 0x40;
            slot += 1;
        } while (slot < v1);
    }
    ScheduleWindowCopyToVram(window);
}

void ov83_022449D4(BattleCastleOpponentApp *arg0, Window *window) {
    Pokemon *pokemon;
    s32 v1;
    Window *v0;
    Pokemon *mon;
    s32 v2;
    s32 v3;
    s32 slot;
    u32 x;

    v0 = window;
    FillWindowPixelBuffer(window, 0U);
    if (ov80_02237D8C(((BattleCastleOpponentApp *)arg0)->challengeType) == 0) {
        x = 0x28;
        v3 = 0x50;
    } else {
        x = 8;
        v3 = 0x30;
    }
    v2 = ov80_02237B58(((BattleCastleOpponentApp *)arg0)->challengeType, 1);
    slot = 0;
    v1 = v2;
    if (v2 > 0) {
        do {
            mon = Party_GetMonByIndex(((BattleCastleOpponentApp *)arg0)->opponentsParty, slot);
            pokemon = mon;
            FontSpecialChars_DrawPartyScreenText(((BattleCastleOpponentApp *)arg0)->msgPrinter, 1U, GetMonData(mon, 0xA1, NULL), 3U, PRINTING_MODE_LEFT_ALIGN, v0, x, 1U);
            ov83_02244BA8(arg0, v0, v3, 1, 0, (s32)(u8)GetMonData(mon, 0x6F, NULL));
            slot += 1;
            x += 0x40;
            v3 += 0x40;
        } while (slot < v1);
    }
    ScheduleWindowCopyToVram(v0);
}
