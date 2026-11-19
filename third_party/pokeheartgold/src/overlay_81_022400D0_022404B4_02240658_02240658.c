#include "global.h"

#include "overlay_81_battle_factory_app.h"

BattleFactoryAppPanelSprite *ov81_02241340(BattleFactoryApp *app, u32 animID);

void ov81_02240658(BattleFactoryApp *app, u32 param_2);

void ov81_02240658(BattleFactoryApp *app, u32 param_2) {
    u32 animID;

    switch (param_2) {
    case 0:
        animID = 1;
        break;
    case 1:
        animID = 2;
        break;
    case 4:
        animID = 1;
        break;
    case 5:
        animID = 2;
        break;
    default:
        animID = 4;
        break;
    }
    app->unk_380 = ov81_02241340(app, animID);
}
