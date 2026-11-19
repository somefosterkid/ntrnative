#include "global.h"

#include "overlay_81.h"
#include "overlay_81_battle_factory_app.h"

void ov81_0224258C(BattleFactoryApp *app, u8 x, u8 y, u8 width, u8 height, u8 nextSubState) {
    app->blink.x = x;
    app->blink.y = y;
    app->blink.width = width;
    app->blink.height = height;
    app->blink.nextSubState = nextSubState;
    app->blink.count = 0;
    app->blink.timer = 0;
    app->blink.palSwap = 0;
}
