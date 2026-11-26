#include "global.h"

#include "overlay_83_battle_castle_self_app.h"
#include "touchscreen_list_menu.h"
#include "overlay_80_02237A70.h"


void ov83_0224773C(void *, u32, u32);

void ov83_02247988(u16 *, u16 *, u32, u32);

#if defined(SDK_PORT) || defined(NONMATCHING)
TouchscreenListMenu *ov83_02247CCC(TouchscreenListMenuSpawner *, TouchscreenListMenuHeader *, u8, u8, u8);
#else
TouchscreenListMenu *ov83_02247CCC(TouchscreenListMenuSpawner *, TouchscreenListMenuHeader *, u32, u32, u8);
#endif

extern const u8 _02247D0C[];

void ov83_02242AEC(BattleCastleSelfApp *app, u8 numWindows, u32 param_3, u32 param_4, u8 param_5);

void ov83_02242AEC(BattleCastleSelfApp *app, u8 numWindows, u32 param_3, u32 param_4, u8 param_5) {
    u16 pos[2];
    TouchscreenListMenuTemplate template;
    TouchscreenListMenuHeader header;
    u32 val;

    ov83_02247988(&pos[1], &pos[0], param_3, param_4);
    template = *(const TouchscreenListMenuTemplate *)(_02247D0C + 0x1E);
    template.baseTile = pos[1];
    template.charOffset = pos[1] + pos[0] - 27;
    template.unkA = pos[0];
    header.template = template;
    header.listMenuItems = app->listMenuItems;
    header.bgConfig = app->bgConfig;
    header.numWindows = numWindows;
    app->touchMenu = ov83_02247CCC(app->touchMenuSpawner, &header, param_3, param_4, param_5);
    app->yesNoMenuIsOpen = TRUE;
    val = ov80_02237B24(app->challengeType, 1);
    ov83_0224773C(app->monIconSprites, val, 1);
    ov83_0224773C(app->itemIconSprites, val, 1);
}
