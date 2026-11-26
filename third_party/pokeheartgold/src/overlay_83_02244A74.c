#include "global.h"
#include "sys_task.h"
#include "overlay_83_battle_castle_opponent_app.h"

void ov83_02247864(void *, void *);

void ov83_02244A74(BattleCastleOpponentApp *param_1);

void ov83_02246C2C(BattleCastleOpponentApp *arg0);
void ov83_02246C70(BattleCastleOpponentApp *arg0);
void ov83_02244A88(BattleCastleOpponentApp *arg0);
void ov83_02244A90(BattleCastleOpponentApp *arg0);

void ov83_02244A74(BattleCastleOpponentApp *param_1) {
    ov83_02247864(&param_1->yesNoPrompt, ((u8 *)param_1->bgConfig));
    return;
}

void ov83_02244A88(BattleCastleOpponentApp *arg0) {
    ov83_02246C2C(arg0);
}

void ov83_02244A90(BattleCastleOpponentApp *arg0) {
    ov83_02246C70(arg0);
}
