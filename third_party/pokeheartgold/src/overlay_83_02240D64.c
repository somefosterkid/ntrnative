#include "global.h"

#include "bg_window.h"
#include "overlay_83_battle_castle_self_app.h"

struct BattleCastleSelfApp;
enum Gender {
    GENDER_MALE = 0,
    GENDER_FEMALE = 1,
    GENDER_NONE = 2,
};
u8 ov83_0223FCB4(BattleCastleSelfApp *app, Window *window, int entryID, u32 xOffset, u32 yOffset, u32 renderDelay, u8 fgColor, u8 shadowColor, u8 bgColor, u8 font);

void ov83_02240D64(BattleCastleSelfApp *app, Window *window, u32 xOffset, u32 yOffset, u8 font, u8 gender);

void ov83_02240D64(BattleCastleSelfApp *app, Window *window, u32 xOffset, u32 yOffset, u8 font, u8 gender) {
    u32 entryID;
    u8 fgColor, shadowColor, bgColor;

    if (gender == GENDER_MALE) {
        entryID = 86;
        fgColor = 7;
        shadowColor = 8;
        bgColor = 0;
    } else if (gender == GENDER_FEMALE) {
        entryID = 87;
        fgColor = 3;
        shadowColor = 4;
        bgColor = 0;
    } else {
        return;
    }

    ov83_0223FCB4(app, window, entryID, xOffset, yOffset, 0xFF, fgColor, shadowColor, bgColor, font);
}
