#include "global.h"

#include "battle/trainer_intro_data.h"

#include "battle/battle_system.h"

#include "heap.h"
#include "overlay_12_022378C0.h"
#include "overlay_manager.h"
#include "poke_overlay.h"
#include "unk_02035900.h"
#include "unk_0203A3B0.h"

s32 ov05_0221BA00(TrainerIntroData *);

s32 ov12_02238A68(BattleSystem *, void *);

FS_EXTERN_OVERLAY(OVY_5);

s32 ov12_0223A0D4(OverlayManager *man) {
    BattleSystem *battleSystem;
    u32 v0;
    u32 v1;
    u32 v2;
    u8 v4;
    void *v3;

    battleSystem = OverlayManager_CreateAndGetData(man, sizeof(BattleSystem), HEAP_ID_BATTLE);
    v3 = OverlayManager_GetArgs(man);
    MIi_CpuClearFast(0U, battleSystem, sizeof(BattleSystem));
    ov12_02238A68(battleSystem, v3);
    v0 = battleSystem->battleType;
    if (!(4 & v0) || (0x10 & battleSystem->battleSpecial) || (v0 & 0x80)) {
        sub_0203A914();
        return 0;
    }
    HandleLoadOverlay(FS_OVERLAY_ID(OVY_5), OVY_LOAD_ASYNC);
    if (!(8 & battleSystem->battleType)) {
        return 0;
    }
    battleSystem->unk1C4 = Heap_Alloc(HEAP_ID_BATTLE, sizeof(TrainerIntroData));
    MIi_CpuClearFast(0U, battleSystem->unk1C4, sizeof(TrainerIntroData));
    v4 = (u8)sub_0203769C();
    v1 = (u32)sub_020378AC((s32)v4);
    switch (v1) {
    case 0:
    case 3:
        battleSystem->unk1C4->party[0] = battleSystem->trainerParty[v4];
        battleSystem->unk1C4->party[2] = battleSystem->trainerParty[BattleSystem_GetBattlerIdPartner(battleSystem, (s32)v4)];
        break;
    case 1:
    case 2:
        battleSystem->unk1C4->party[0] = battleSystem->trainerParty[BattleSystem_GetBattlerIdPartner(battleSystem, (s32)v4)];
        battleSystem->unk1C4->party[2] = battleSystem->trainerParty[v4];
        break;
    }
    battleSystem->unk1C4->heapID = HEAP_ID_BATTLE;
    battleSystem->unk1C4->mode = 0;
    v2 = (u32)sub_020378AC((s32)v4);
    switch (v2) {
    case 0:
    case 3:
        battleSystem->unk1C4->playerSide = 0;
        break;
    case 1:
    case 2:
        battleSystem->unk1C4->playerSide = 1;
        break;
    }
    ov05_0221BA00(battleSystem->unk1C4);
    return 1;
}

s32 ov12_0223A218(OverlayManager *man) {
    s32 i;
    BattleSystem *battleSystem;
    String *name;

    battleSystem = OverlayManager_GetData(man);
    if (battleSystem->unk1C4->isDone != 0) {
        i = 0;
        do {
            name = battleSystem->unk1C4->trainerNames[i];
            if (name != NULL) {
                Heap_Free(name);
            }
            i += 1;
        } while (i < 4);
        Heap_Free(battleSystem->unk1C4);
        return 1;
    }
    return 0;
}
