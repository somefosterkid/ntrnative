#include "global.h"

#include "overlay_81.h"
#include "overlay_81_battle_factory_app.h"

BattleFactoryAppPanelSprite *ov81_02242CBC(BattleFactoryAppSpriteManager *spriteMan, u32 resourceID, u32 animID, int resourcePriority, u32 priority, int x, int y);

void *ov81_02242D74(BattleFactoryAppPanelSprite *panelSprite);

BattleFactoryAppPanelSprite *ov81_02241340(BattleFactoryApp *app, u32 animID) {
    return ov81_02242CBC(&app->spriteMan, 1, animID, 2, 10, 0x80, 0x7C);
}

void ov81_02241364(BattleFactoryApp *arg0) {
    if (arg0->unk_380 != 0) {
        ov81_02242D74(arg0->unk_380);
        arg0->unk_380 = 0;
    }
    if (arg0->unk_384 != 0) {
        ov81_02242D74(arg0->unk_384);
        arg0->unk_384 = 0;
    }
}
