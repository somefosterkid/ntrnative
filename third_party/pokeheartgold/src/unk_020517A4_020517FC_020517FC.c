#include "global.h"

#include "battle/battle_setup.h"

#include "map_header.h"
#include "save_vars_flags.h"
#include "sys_flags.h"
#include "trainer_data.h"
#include "overlay_manager.h"

u32 BattleSetup_GetTransitionAndMusicParam(void *arg0);

s32 BattleStartGetMusic(u32, BattleSetup *);

s32 sub_020517F8(u32);

s32 BattleSetup_GetWildBattleMusic(BattleSetup *setup);

s32 BattleSetup_GetWildBattleMusic(BattleSetup *setup) {
    u32 uVar1;
    s32 iVar2;
    SaveVarsFlags *state;
    BOOL BVar3;
    u32 uVar4;
    s32 iVar5;

    uVar1 = BattleSetup_GetTransitionAndMusicParam(setup);
    iVar2 = BattleStartGetMusic(uVar1, setup);
    state = Save_VarsFlags_Get(setup->saveData);
    BVar3 = Save_VarsFlags_FlypointFlagAction(state, 2, 5);
    if ((BVar3 == 1) && (uVar4 = MapHeader_GetRegionNo(setup->mapNumber), uVar4 != 0)) {
        if (iVar2 == 0x45c) {
            iVar2 = 0x465;
        } else if (iVar2 == 0x45d) {
            iVar2 = 0x466;
        } else if ((iVar2 == 0x45e) && (iVar5 = sub_020517F8(uVar1), iVar5 == 0)) {
            iVar2 = 0x467;
        }
    }
    return iVar2;
}
