#include "global.h"

#include "overlay_81.h"
#include "overlay_81_battle_factory_app.h"

void ov81_022425C4(BattleFactoryApp *app, u8 x, u8 y, u8 nextSubState) {
    ov81_0224258C(app, x, y, 0xC, 2, nextSubState);
}

void ov81_022425D8(BattleFactoryApp *app, u8 x, u8 y, u8 nextSubState) {
    ov81_0224258C(app, x, y, 7, 2, nextSubState);
}
