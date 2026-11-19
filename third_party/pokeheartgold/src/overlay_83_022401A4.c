#include "global.h"
struct BattleCastleSelfApp;

#include "frontier/overlay_80_022340E8.h"

#include "party.h"
#include "pokemon.h"
#include "unk_0200CE7C.h"
#include "bg_window.h"
#include "message_printer.h"
#include "options.h"
#include "string_util.h"
#include "overlay_83_battle_castle_self_app.h"

s32 ov83_02240D64(void *, Window *, s32, u32, s32, s32);

void ov83_022401A4(struct BattleCastleSelfApp *arg0, Window *arg1, s32 arg2, u8 arg3);

void ov83_02240238(struct BattleCastleSelfApp *param_1, Window *param_2, s32 param_3, u32 param_4);
u8 ov83_0223FC48(void *, void *, u32, u32, u32, u32, u8, u8, u8, u8);
void ov83_02247944(void *, u32);
void ov83_02240290(void *param_1);

void ov83_022401A4(BattleCastleSelfApp *arg0, Window *arg1, s32 slot, u8 arg3) {
    Pokemon *mon;
    s32 v0;
    Window *window;
    s32 v1;
    s32 v2;
    s32 v3;
    s32 v4;
    u32 y;
    u32 x;

    v0 = arg3;
    window = arg1;
    if (ov80_02237D8C(arg0->challengeType) == 0) {
        v2 = 0x28;
        v3 = 0x50;
    } else {
        v2 = 8;
        v3 = 0x30;
    }
    mon = Party_GetMonByIndex(arg0->party, slot);
    if (v0 == 0) {
        v1 = slot << 6;
        x = v2 + v1;
        y = 1;
        v4 = v3 + v1;
    } else {
        x = 4;
        y = 0;
        v4 = 0x30;
    }
    FontSpecialChars_DrawPartyScreenText(arg0->msgPrinter, 1U, GetMonData(mon, 0xA1, NULL), 3U, PRINTING_MODE_LEFT_ALIGN, window, x, y);
    ov83_02240D64(arg0, window, v4, y, 0, (s32)(u8)GetMonData(mon, 0x6F, NULL));
}

void ov83_02240238(BattleCastleSelfApp *param_1, Window *param_2, s32 param_3, u32 param_4) {
    s32 iVar1;
    u32 x;

    iVar1 = ov80_02237D8C(param_1->challengeType);
    if (iVar1 == 0) {
        iVar1 = 0x24;
    } else {
        iVar1 = 4;
    }
    x = iVar1 + param_3 * 0x40;
    FillWindowPixelRect(param_2, 0, (u16)x, 0, 0x18, 9);
    PrintUIntOnWindow(param_1->msgPrinter, param_4, 3, PRINTING_MODE_RIGHT_ALIGN, param_2, x, 1);
    ScheduleWindowCopyToVram(param_2);
    return;
}

void ov83_02240290(void *param_1) {
    u8 uVar1;
    u32 uVar2;

    uVar1 = ov83_0223FC48(param_1, param_1 + 0x60, 6, 0, 5, 0xff, 1, 2, 0, 0);
    *(u8 *)(param_1 + 10) = uVar1;
    uVar2 = Options_GetFrame(*(Options **)(param_1 + 0x508));
    ov83_02247944(param_1 + 0xc0, uVar2);
    uVar1 = ov83_0223FC48(param_1, param_1 + 0xc0, 5, 1, 1, 0xff, 1, 2, 0xf, 1);
    *(u8 *)(param_1 + 10) = uVar1;
    return;
}
