#include "battle/battle_02261FD4.h"

#include "battle/battle_system.h"

#include "math_util.h"
#include "pokepic.h"

static void ov12_0226203C(SysTask *task, void *_data);

void ov12_02261FD4(OpponentData *data, BattleSystem *battleSystem) {
    if (data->sysTask == NULL && !(BattleSystem_GetBattleType(battleSystem) & (BATTLE_TYPE_SAFARI | BATTLE_TYPE_PAL_PARK))) {
        data->degrees = 180;
        data->sysTask = SysTask_CreateOnMainQueue(ov12_0226203C, data, 0x3F2);
    }
}

void ov12_02262014(OpponentData *data) {
    if (data->sysTask != NULL) {
        SysTask_Destroy(data->sysTask);
        data->sysTask = NULL;
        data->degrees = 0;
        Pokepic_SetAttr(data->monSprite, POKEPIC_YOFFSET, 0);
    }
}

static void ov12_0226203C(SysTask *task, void *_data) {
    OpponentData *data = _data;

    data->degrees += 20;

    if (data->degrees >= 360) {
        data->degrees -= 360;
    }

    int dy = FX_Mul(GF_SinDegNoWrap(data->degrees), FX32_CONST(1.5)) / FX32_ONE;

    Pokepic_SetAttr(data->monSprite, POKEPIC_YOFFSET, dy);
}
