#include "global.h"

#include "options.h"
#include "party.h"
#include "pokemon.h"
#include "touchscreen_list_menu.h"
#include "unk_02005D10.h"
#include "overlay_83_battle_castle_opponent_app.h"

u8 ov83_022448AC(void *, s32, u8);

s32 ov83_02244AB0(void *, s32, BoxPokemon *);

s32 ov83_02245300(void *, s32);

s32 ov83_02245C80(void *);

s32 ov83_02245D48(void *);

s32 ov83_02246114(void *, s32);

u8 ov83_02247768(u8, s32);

s32 ov83_02247944(void *, u32);

void ov83_02245318(void *arg0, s32 arg1);

void ov83_02245318(void *arg0, s32 arg1) {
    ov83_02247944(arg0 + 0xC0, Options_GetFrame(((BattleCastleOpponentApp *)arg0)->options));
    ov83_02244AB0(arg0, 0, Mon_GetBoxMon(Party_GetMonByIndex(((BattleCastleOpponentApp *)arg0)->opponentsParty, ov83_02247768(((BattleCastleOpponentApp *)arg0)->numSlots, arg1))));
    *((s8 *)&((BattleCastleOpponentApp *)arg0)->printerID) = ov83_022448AC(arg0, 0x53, 1);
    ov83_02245300(arg0, arg1);
    ov83_02245C80(arg0);
    if (((BattleCastleOpponentApp *)arg0)->selectedMonSlot == arg1) {
        ov83_02245D48(arg0);
        ov83_02246114(arg0, 0);
    }
    PlaySE(0x623U);
}

s32 ov83_02246C70(void *);

s32 ov83_022478B4(void *);

void ov83_02245390(void *arg0);

void ov83_02245390(void *arg0) {
    if (((u32)(*((u8 *)((u8 *)&((BattleCastleOpponentApp *)arg0)->levelAdjustment + 0x1)) << 0x1D) >> 0x1F) == 1) {
        TouchscreenListMenu_DestroyButtons(*((TouchscreenListMenu **)&((BattleCastleOpponentApp *)arg0)->unk_5F8));
        ov83_02246C70(arg0);
    }
    ov83_022478B4(arg0 + 0x604);
}

void ov83_022453C0(void *arg0);

void ov83_022453C0(void *arg0) {
    ov83_02247944(arg0 + 0xC0, Options_GetFrame(((BattleCastleOpponentApp *)arg0)->options));
}
