#include "global.h"

#include "options.h"
#include "party.h"
#include "pokemon.h"
#include "touchscreen_list_menu.h"
#include "unk_02005D10.h"
#include "overlay_83_battle_castle_opponent_app.h"

u8 ov83_022448AC(BattleCastleOpponentApp *param_1, s32, u8);

s32 ov83_02244AB0(BattleCastleOpponentApp *app, s32, BoxPokemon *boxmon);

s32 ov83_02245300(BattleCastleOpponentApp *param_1, s32);

s32 ov83_02245C80(BattleCastleOpponentApp *app);

s32 ov83_02245D48(BattleCastleOpponentApp *arg0);

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov83_02246114(BattleCastleOpponentApp *arg0, s32 arg1);
#else
void ov83_02246114(BattleCastleOpponentApp *arg0, s32 arg1);
#endif

u8 ov83_02247768(u8, s32);

s32 ov83_02247944(Window *window, u32);

void ov83_02245318(BattleCastleOpponentApp *arg0, s32 arg1);

void ov83_02245318(BattleCastleOpponentApp *arg0, s32 arg1) {
    ov83_02247944(&arg0->windows[7], Options_GetFrame(arg0->options));
    ov83_02244AB0(arg0, 0, Mon_GetBoxMon(Party_GetMonByIndex(arg0->opponentsParty, ov83_02247768(arg0->numSlots, arg1))));
    *((s8 *)&arg0->printerID) = ov83_022448AC(arg0, 0x53, 1);
    ov83_02245300(arg0, arg1);
    ov83_02245C80(arg0);
    if (arg0->selectedMonSlot == arg1) {
        ov83_02245D48(arg0);
        ov83_02246114(arg0, 0);
    }
    PlaySE(0x623U);
}

s32 ov83_02246C70(BattleCastleOpponentApp *arg0);

s32 ov83_022478B4(BattleCastleYesNoPrompt *arg0);

void ov83_02245390(BattleCastleOpponentApp *arg0);

void ov83_02245390(BattleCastleOpponentApp *arg0) {
    if (((u32)(*((u8 *)((u8 *)&arg0->levelAdjustment + 0x1)) << 0x1D) >> 0x1F) == 1) {
        TouchscreenListMenu_DestroyButtons(arg0->touchMenu);
        ov83_02246C70(arg0);
    }
    ov83_022478B4(&arg0->yesNoPrompt);
}

void ov83_022453C0(BattleCastleOpponentApp *arg0);

void ov83_022453C0(BattleCastleOpponentApp *arg0) {
    ov83_02247944(&arg0->windows[7], Options_GetFrame(arg0->options));
}
