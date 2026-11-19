#include "global.h"

#include "touchscreen_list_menu.h"
#include "overlay_83_battle_castle_self_app.h"

s32 ov83_02242D5C(BattleCastleSelfApp *);

void ov83_022478B4(BattleCastleYesNoPrompt *arg0);

void ov83_02241730(BattleCastleSelfApp *arg0);

void ov83_02241730(BattleCastleSelfApp *arg0) {
    if (arg0->yesNoMenuIsOpen == 1) {
        TouchscreenListMenu_DestroyButtons(arg0->touchMenu);
        ov83_02242D5C(arg0);
    }
    ov83_022478B4(&arg0->yesNoPrompt);
}

void ov83_02241354(Window *window);

void ov83_022429E4(BattleCastleSelfApp *app);

void ov83_0224175C(BattleCastleSelfApp *arg0);

void ov83_0224175C(BattleCastleSelfApp *arg0) {
    ov83_02241354(&arg0->windows[6]);
    ov83_022429E4(arg0);
}
