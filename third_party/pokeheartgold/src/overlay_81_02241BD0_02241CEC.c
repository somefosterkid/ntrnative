#include "global.h"
#include "overlay_81_battle_factory_app.h"
struct BattleFactoryApp;

s32 ov81_02241CA0(BattleFactoryApp *, s32 *, s32 *);

void ov81_02242D94(BattleFactoryAppPanelSprite *panelSprite, int x, int y);

void ov81_02241CEC(struct BattleFactoryApp *arg0);

void ov81_02241CEC(BattleFactoryApp *arg0) {
    s32 v1;
    s32 v0;

    ov81_02241CA0(arg0, &v1, &v0);
    ov81_02242D94(arg0->unk_390, v1, v0);
}
