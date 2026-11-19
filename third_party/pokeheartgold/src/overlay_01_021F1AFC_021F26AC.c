#include "global.h"

#include "field_system.h"
#include "overlay_01.h"
#include "overlay_01_021F1AFC.h"
#include "follow_mon.h"
#include "overlay_02.h"
#include "pokemon.h"
#include "sound_chatot.h"
#include "unk_02005D10.h"

BOOL ov01_021F3068(MonRideTask *monRideTask);

s32 ov01_021F26AC(RockClimbTaskEnv *taskEnv);

s32 ov01_021F26CC(RockClimbTaskEnv *taskEnv);
s32 ov01_021F2700(RockClimbTaskEnv *taskEnv);
s32 ov01_02205EE0(void *);
s32 ov01_021F2734(RockClimbTaskEnv *taskEnv, void *arg1);

s32 ov01_021F26AC(RockClimbTaskEnv *taskEnv) {
    s32 iVar1;

    iVar1 = ov01_021F3068(&taskEnv->monRideTask);
    if (iVar1 == 1) {
        ov01_02205D68(taskEnv->fieldSystem);
        taskEnv->state = taskEnv->state + 1;
    }
    return 0;
}

s32 ov01_021F26CC(RockClimbTaskEnv *taskEnv) {
    u8 a1;

    if (ov02_02250780(taskEnv->fieldSystem, 0U) != 0) {
        FieldSystem_UnkSub108_AddMonMood(taskEnv->fieldSystem->unk108, 1);
        a1 = 2;
    } else {
        a1 = 1;
    }
    ov02_022507B4(taskEnv->fieldSystem, a1);
    taskEnv->state = (u16)(taskEnv->state + 1);
    return 0;
}

s32 ov01_021F2700(RockClimbTaskEnv *taskEnv) {
    u32 species;

    species = GetMonData(taskEnv->monRideTask.partyMon, 5, NULL);
    PlayCry((u16)species, (u8)GetMonData(taskEnv->monRideTask.partyMon, 0x70, NULL));
    taskEnv->state = (u16)(taskEnv->state + 1);
    return 0;
}

s32 ov01_021F2734(RockClimbTaskEnv *taskEnv, void *arg1) {
    if (IsCryFinished() != 0) {
        return 0;
    }
    ov01_02205EE0(arg1);
    taskEnv->state += 1;
    return 0;
}
