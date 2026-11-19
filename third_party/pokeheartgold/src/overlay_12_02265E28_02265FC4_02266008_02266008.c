#include "global.h"

#include "battle/battle.h"
#include "battle/battle_02265E28.h"

s32 ov12_02265F68();

s32 ov12_02265F7C(UnkBattleSystemSub17C *);

void ov12_02266008(UnkBattleSystemSub17C *a0) {
    ov12_02265F68();
    ov12_02265F7C(a0);
    MIi_CpuClearFast(0U, a0, sizeof(UnkBattleSystemSub17C));
}
