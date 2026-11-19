#include "global.h"

#include "bg_window.h"
#include "options.h"
#include "overlay_83_battle_castle_opponent_app.h"
#include "overlay_83_battle_castle_self_app.h"

struct BattleCastleSelfApp;

enum Gender {
    GENDER_MALE = 0,
    GENDER_FEMALE = 1,
    GENDER_NONE = 2,
};

u8 ov83_0224484C(BattleCastleSelfApp *app, Window *window, int entryID, u32 xOffset, u32 yOffset, u32 renderDelay, u8 fgColor, u8 shadowColor, u8 bgColor, u8 font);

void ov83_02244BA8(BattleCastleSelfApp *app, Window *window, u32 xOffset, u32 yOffset, u8 font, u8 gender);

u8 ov83_022447E0(u8 *, u8 *, u32, u32, u32, u32, u8, u8, u8, u8);
void ov83_02247944(void *, u32);
void ov83_02244BEC(u8 *param_1);

void ov83_02244BA8(BattleCastleSelfApp *app, Window *window, u32 xOffset, u32 yOffset, u8 font, u8 gender) {
    u32 entryID;
    u8 fgColor, shadowColor, bgColor;

    if (gender == GENDER_MALE) {
        entryID = 64;
        fgColor = 7;
        shadowColor = 8;
        bgColor = 0;
    } else if (gender == GENDER_FEMALE) {
        entryID = 65;
        fgColor = 3;
        shadowColor = 4;
        bgColor = 0;
    } else {
        return;
    }

    ov83_0224484C(app, window, entryID, xOffset, yOffset, 0xFF, fgColor, shadowColor, bgColor, font);
}

void ov83_02244BEC(u8 *param_1) {
    u8 uVar1;
    u32 uVar2;

    uVar1 = ov83_022447E0(param_1, param_1 + 0x60, 5, 0, 5, 0xff, 1, 2, 0, 0);
    ((BattleCastleOpponentApp *)param_1)->printerID = uVar1;
    uVar2 = Options_GetFrame(((BattleCastleOpponentApp *)param_1)->options);
    ov83_02247944(param_1 + 0xd0, uVar2);
    uVar1 = ov83_022447E0(param_1, param_1 + 0xd0, 4, 1, 1, 0xff, 1, 2, 0xf, 1);
    ((BattleCastleOpponentApp *)param_1)->printerID = uVar1;
    return;
}
