#include "global.h"

#include "bg_window.h"
#include "overlay_08_battle_party.h"
#include "battle/battle_party_context.h"

void ov08_0221DC3C(BattleParty *battleParty, u32 screen);

extern u8 ov08_02224FD0[];

void ov08_0221DC00(BattleParty *battleParty);

void ov08_0221DC00(BattleParty *battleParty) {
    WindowTemplate *template;
    Window *window;
    u32 i;

    template = (WindowTemplate *)ov08_02224FD0;
    i = 0;
    window = battleParty->messageBoxWindows;
    do {
        AddWindow(battleParty->background, window, template);
        i++;
        template++;
        window++;
    } while (i < 2);
    ov08_0221DC3C(battleParty, battleParty->currentScreen);
}
