#include "global.h"

#include "battle/battle.h"
#include "battle/battle_02265E28.h"

#include "sprite.h"
#include "sprite_system.h"
#include "error_handling.h"
#include "overlay_12_02265E28.h"


void ov12_02265E28(void *);
void ov12_02265F34(void *);

void ov12_02265FC4(UnkBattleSystemSub17C *unkBattleSystemSub17C, s32 flag) {
    ManagedSprite *managedSprite;

    managedSprite = unkBattleSystemSub17C->unk0;
    if (managedSprite != NULL) {
        ManagedSprite_SetDrawFlag(managedSprite, flag);
    }
}

void ov12_02265FD4(UnkBattleSystemSub17C *destp, BattleSystem *battleSystem, s8 arg2, s8 arg3) {
    MIi_CpuClearFast(0U, destp, sizeof(UnkBattleSystemSub17C));
    destp->battleSystem = battleSystem;
    destp->unk8 = arg2;
    destp->unk9 = arg3;
    if ((s32)arg3 >= 0x18) {
        GF_AssertFail();
        destp->unk9 = 0;
    }
    ov12_02265E28(destp);
    ov12_02265F34(destp);
}
