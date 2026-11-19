#include "global.h"
#include "overlay_83_battle_castle_opponent_app.h"

void ov83_02245094(void *);

void ov83_02245390(void *arg0);

void ov83_0224755C(void *, u32);

void ov83_02245074(u8 *param_1);

void ov83_02245074(u8 *param_1) {
    ov83_02245390(param_1);
    ov83_02245094(param_1 + 0xc0);
    ov83_0224755C(((void *)((BattleCastleOpponentApp *)param_1)->sparklesSprite), 0);
    return;
}
