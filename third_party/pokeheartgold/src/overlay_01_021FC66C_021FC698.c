#include "global.h"

#include "battle/battle_setup.h"

#include "encounter.h"
#include "field_types_def.h"
#include "game_stats.h"
#include "heap.h"
#include "map_object.h"
#include "overlay_01_fishing_task.h"
#include "sys_task.h"
#include "task.h"
#include "trainer_data.h"
#include "unk_02092BE8.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
#include "field/encounter_check.h"
#else
s32 FieldSystem_PerformFishEncounterCheck();
#endif

s32 ov01_021FC76C(SysTask *arg0);

s32 ov01_021FC778(SysTask *arg0);

SysTask *ov01_021FC748(FieldSystem *, s32, s32);

void ov01_021FC784(SysTask *);

s32 Task_OverworldFish(TaskManager *taskManager);

s32 Task_OverworldFish(TaskManager *taskManager) {
    BattleSetup *temp_r0_3;
    FieldSystem *temp_r4;
    s32 temp_r7;
    FishingContext *temp_r0;

    temp_r4 = TaskManager_GetFieldSystem(taskManager);
    temp_r0 = TaskManager_GetEnvironment(taskManager);
    temp_r7 = temp_r0->state;
    switch (temp_r7) {
    case 0:
        MapObjectManager_PauseAllMovement(temp_r4->mapObjectManager);
        temp_r0->battleSetup = 0L;
        temp_r0->isFishEncountered = FieldSystem_PerformFishEncounterCheck(temp_r4, temp_r0->rodType, &temp_r0->battleSetup);
        temp_r0->sysTask = ov01_021FC748(temp_r4, temp_r0->rodType, temp_r0->isFishEncountered);
        temp_r0->state = (s32)(temp_r0->state + 1);
        break;
    case 1:
        if (ov01_021FC76C(temp_r0->sysTask) == 1) {
            temp_r7 = ov01_021FC778(temp_r0->sysTask);
            ov01_021FC784(temp_r0->sysTask);
            if (temp_r7 == 1) {
                GearPhoneRingManager_ResetIfActive(FieldSystem_GetGearPhoneRingManager(temp_r4));
                GameStats_Inc(Save_GameStats_Get(temp_r4->saveData), 0xB);
                FieldSystem_StartForcedWildBattle(temp_r4, taskManager, temp_r0->battleSetup);
                Heap_Free(temp_r0);
                return 0;
            }
            temp_r0_3 = temp_r0->battleSetup;
            if (temp_r0_3 != 0L) {
                BattleSetup_Delete(temp_r0_3);
            }
            MapObjectManager_UnpauseAllMovement(temp_r4->mapObjectManager);
            Heap_Free(temp_r0);
            return 1;
        }
        break;
    }
    return 0;
}
