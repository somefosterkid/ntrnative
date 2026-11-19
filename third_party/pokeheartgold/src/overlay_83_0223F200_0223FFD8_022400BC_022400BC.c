#include "global.h"
struct BattleCastleSelfApp;

#include "frontier/overlay_80_022340E8.h"

#include "message_printer.h"
#include "party.h"
#include "pokemon.h"
#include "overlay_83_battle_castle_self_app.h"

void ov83_022400BC(struct BattleCastleSelfApp *arg0, Window *arg1, s32 arg2, s32 arg3);

void ov83_022400BC(BattleCastleSelfApp *arg0, Window *arg1, s32 arg2, s32 arg3) {
    u16 sp14;
    u16 sp10;
    Pokemon *spC;
    s32 temp_r1;
    s32 var_r0;
    u16 var_r7;
    u16 var_r4;

    spC = Party_GetMonByIndex(arg0->party, arg2);
    if (ov80_02237D8C(arg0->challengeType) == 0) {
        var_r0 = 0x24;
    } else {
        var_r0 = 4;
    }
    if (arg3 == 0) {
        temp_r1 = arg2 << 6;
        var_r7 = var_r0 + temp_r1;
        sp10 = (u16)(u16)(var_r0 + 0x18 + temp_r1);
        var_r4 = 1;
        sp14 = (u16)(var_r0 + 0x20 + temp_r1);
    } else {
        sp10 = 0x1C;
        var_r7 = 4;
        var_r4 = 0;
        sp14 = 0x24;
    }
    PrintUIntOnWindow(arg0->msgPrinter, GetMonData(spC, 0xA3, NULL), 3U, PRINTING_MODE_RIGHT_ALIGN, arg1, (u16)var_r7, var_r4);
    sub_0200CDAC(arg0->msgPrinter, 0U, arg1, sp10, var_r4);
    PrintUIntOnWindow(arg0->msgPrinter, GetMonData(spC, 0xA4, NULL), 3U, PRINTING_MODE_LEFT_ALIGN, arg1, sp14, var_r4);
}
