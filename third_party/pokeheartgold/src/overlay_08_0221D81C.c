#include "global.h"
#include "bg_window.h"
#include "overlay_08_battle_party.h"
#include "battle/battle_party_context.h"

struct BattleParty;


void ov08_0221D77C(BattleParty *battleParty, u16 fillVal, u8 pointSlot);

void ov08_0221D81C(BattleParty *battleParty);

s32 ov08_0221D6CC(void *, s8);
s32 ov08_0221D8B0();
s32 ov08_0221DC3C(void *, s8);
s32 ov08_0221DD28(void *);
s32 ov08_0221DD70(void *, u32);
s32 ov08_02220C5C(void *, s8);
s32 ov08_0222171C(void *, s8);
s32 ov08_022221CC(void *, s8);
s32 ov08_02222524(void *, s8);
void ov08_0221D840(BattleParty *battleParty, s8 screen);

void ov08_0221D81C(BattleParty *battleParty) {
    for (u16 i = 0; i < 6; i++) {
        ov08_0221D77C(battleParty, 293, i);
    }
}

void ov08_0221D840(BattleParty *battleParty, s8 screen) {
    ov08_0221D8B0();
    BgFillTilemapBufferAndSchedule(battleParty->background, 4U, 0U);
    BgFillTilemapBufferAndSchedule(battleParty->background, 5U, 0U);
    ov08_02220C5C(battleParty, screen);
    ov08_0221DD28(battleParty);
    ov08_0221DC3C(battleParty, screen);
    ov08_0221DD70(battleParty, screen);
    ov08_0221D6CC(battleParty, screen);
    ov08_0222171C(battleParty, screen);
    ov08_022221CC(battleParty, screen);
    ov08_02222524(battleParty, screen);
    battleParty->currentScreen = screen;
}
