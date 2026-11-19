#include "global.h"

#include "bag.h"
#include "battle/trainer_intro_data.h"
#include "overlay_05.h"
#include "overlay_05_unk_struct_ov10_0221_fb28_t.h"
#include "sys_task.h"
#include "systask_environment.h"

void ov05_0221BA70(SysTask *task, void *data);

s32 sub_020304B4(void);

void ov05_0221BA00(TrainerIntroData *arg0) {
    SaveData *saveData;
    UnkStruct_ov10_0221FB28 *v0;
    BattleSetup *v1;

    v0 = SysTask_GetData(CreateSysTaskAndEnvironment(ov05_0221BA70, sizeof(UnkStruct_ov10_0221FB28), 0x64U, arg0->heapID));
    memset(v0, 0, sizeof(UnkStruct_ov10_0221FB28));
    v0->trainerIntroData = arg0;
    v0->unk_B7F = 0;
    v0->unk_BC8 = sub_020304B4();
    if (v0->unk_BC8 == 1) {
        v1 = v0->trainerIntroData->setup;
        if (v1 != NULL) {
            saveData = v1->saveData;
            if (saveData != NULL) {
                v0->unk_BC8 = Bag_HasItem(Save_Bag_Get(saveData), 0x1D1U, 1U, arg0->heapID);
            }
        }
    }
}
