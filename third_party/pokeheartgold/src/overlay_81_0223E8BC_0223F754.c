#include "global.h"
#include "overlay_81.h"
#include "overlay_81_battle_factory_app.h"
struct BattleFactoryApp;

s32 ov81_02241804(void *arg0);

void ov81_0223F754(struct BattleFactoryApp *arg0);

void ov81_0223F754(BattleFactoryApp *arg0) {
    ov81_02241804(arg0);
    ov81_02240F38(arg0, 1);
    *((s8 *)&arg0->printerID) = ov81_0224086C(arg0, 9);
}
