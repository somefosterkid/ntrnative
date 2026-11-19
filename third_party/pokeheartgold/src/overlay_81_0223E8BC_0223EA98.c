#include "global.h"

#include "overlay_81.h"
#include "unk_0201956C.h"
#include "unk_02019BA4.h"
#include "overlay_81_battle_factory_app.h"

s32 ov81_022408C4(void *, void *, s32, s32, u8);

s32 ov81_02241A98(void *);

s32 ov81_02241C84(s32, s32, s32 *, s32 *);

s32 ov81_02241F50(void *);

s32 ov81_02242058(void *, s32);

BattleFactoryAppPanelSprite *ov81_02242CBC(BattleFactoryAppSpriteManager *spriteMan, u32 resourceID, u32 animID, int resourcePriority, u32 priority, int x, int y);

BattleFactoryAppPanelSprite *ov81_02242D18(BattleFactoryAppSpriteManager *spriteMan, u32 resourceID, u32 animID, int resourcePriority, u32 priority, int x, int y);

s32 ov81_02243228(void *, s32, s32 *, s32 *);

GridInputHandler *ov81_02243398(void *, GridInputHandler *);

void ov81_0223EA98(BattleFactoryApp *app) {
    s32 v0;
    s32 v1;

    app->inputHandler = ov81_02243398(app, app->inputHandler);
    ov81_02241A98(app);
    ov81_02243228(app->inputHandler, 0, &v0, &v1);
    app->unk_388 = ov81_02242D18(&app->spriteMan, 0, 2, 0, 0, v0, v1);
    ov81_02241C84(app->unk_468, app->numMonsToFade, &v0, &v1);
    app->unk_390 = ov81_02242CBC(&app->spriteMan, 1, 0, 0, 0, v0, v1);
    ov81_02243228(app->inputHandler, 6, &v0, &v1);
    app->unk_38C = ov81_02242D18(&app->spriteMan, 0, 3, 0, 0, v0, v1);
    ov81_02241F50(app);
    ov81_02242058(app, 1);
    ov81_022420B4(app);
    app->monIconSprites[0] = ov81_02242D18(&app->spriteMan, 4, 0, 0, 0, 0x50, 0xC);
    app->monIconSprites[1] = ov81_02242D18(&app->spriteMan, 5, 0, 0, 0, 0x50, 0xC);
    ov81_02242218(app, app->receivableParty, 0, 4);
    ov81_02242420(app, 0);
    sub_020196E8(app->unk_474, 0, 7, 0);
    ov81_022408C4(app, &app->windows[0], 0, 0, 0);
    app->unk_468 = 0;
    ov81_022419E0(app);
}
