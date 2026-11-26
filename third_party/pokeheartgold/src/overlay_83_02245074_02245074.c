#include "global.h"
#include "overlay_83_battle_castle_opponent_app.h"

void ov83_02245094(Window *window);

void ov83_02245390(BattleCastleOpponentApp *arg0);

void ov83_0224755C(void *, u32);

void ov83_02245074(BattleCastleOpponentApp *param_1);

void ov83_02245074(BattleCastleOpponentApp *param_1) {
    ov83_02245390(param_1);
    ov83_02245094(&param_1->windows[7]);
    ov83_0224755C(((void *)param_1->sparklesSprite), 0);
    return;
}
