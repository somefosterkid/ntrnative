#include "global.h"

#include "battle/trainer_intro_data.h"
#include "game_stats.h"
#include "heap.h"
#include "overlay_manager.h"
#include "player_data.h"
#include "poke_overlay.h"
#include "unk_02035900.h"
#include "unk_02037C94.h"

s32 ov05_0221BA00(void *);

FS_EXTERN_OVERLAY(OVY_5);

s32 ov12_0223A3F0(OverlayManager *man);

s32 ov12_0223A3F0(OverlayManager *man) {
    BattleSetup *setup;
    TrainerIntroData *intro;
    u8 netID;

    setup = OverlayManager_GetArgs(man);
    if (!(setup->battleType & BATTLE_TYPE_LINK) || (setup->battleSpecial & BATTLE_SPECIAL_RECORDING) || (setup->battleType & BATTLE_TYPE_FRONTIER)) {
        return 0;
    }
    netID = sub_0203769C();
    HandleLoadOverlay(FS_OVERLAY_ID(OVY_5), OVY_LOAD_ASYNC);
    intro = Heap_Alloc(HEAP_ID_BATTLE, sizeof(TrainerIntroData));
    setup->trainerIntroData = intro;
    MI_CpuClearFast(intro, sizeof(TrainerIntroData));
    intro->setup = setup;
    switch (setup->winFlag) {
    case 0:
    case 4:
        break;
    case 1:
        if (sub_02039998() == 0) {
            GameStats_Inc(setup->gameStats, 0x16);
        } else {
            GameStats_Inc(setup->gameStats, 0x1B);
        }
        break;
    case 2:
        if (sub_02039998() == 0) {
            GameStats_Inc(setup->gameStats, 0x17);
        } else {
            GameStats_Inc(setup->gameStats, 0x1C);
        }
        break;
    case 3:
    case 5:
        if (sub_02039998() == 0) {
            GameStats_Inc(setup->gameStats, 0x18);
        } else {
            GameStats_Inc(setup->gameStats, 0x1D);
        }
        break;
    }
    if (setup->battleType & BATTLE_TYPE_MULTI) {
        int i;

        for (i = 0; i < 4; i++) {
            intro->party[sub_020378AC(i)] = setup->party[i];
            intro->trainerNames[sub_020378AC(i)] = PlayerProfile_GetPlayerName_NewString(setup->profile[i], HEAP_ID_BATTLE);
        }
        intro->heapID = HEAP_ID_BATTLE;
        intro->mode = 2;
        intro->playerSide = 1;
        if (setup->winFlag != 5) {
            intro->battleResult = setup->winFlag;
        } else {
            intro->battleResult = 3;
        }
    } else {
        intro->party[sub_020378AC(netID)] = setup->party[netID];
        intro->party[sub_020378AC(netID ^ 1)] = setup->party[netID ^ 1];
        intro->trainerNames[sub_020378AC(netID)] = PlayerProfile_GetPlayerName_NewString(setup->profile[netID], HEAP_ID_BATTLE);
        intro->trainerNames[sub_020378AC(netID ^ 1)] = PlayerProfile_GetPlayerName_NewString(setup->profile[netID ^ 1], HEAP_ID_BATTLE);
        intro->heapID = HEAP_ID_BATTLE;
        intro->mode = 2;
        intro->playerSide = 0;
        if (setup->winFlag != 5) {
            intro->battleResult = setup->winFlag;
        } else {
            intro->battleResult = 3;
        }
    }
    intro->recordingType = setup->unk1B2;
    ov05_0221BA00(intro);
    return 1;
}
