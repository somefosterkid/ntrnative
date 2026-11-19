#include "global.h"
struct Ov27Task;

#include "field_system.h"
#include "field_types_def.h"
#include "overlay_27.h"
#include "save_vars_flags.h"
#include "sys_flags.h"
#include "unk_02066EDC.h"
#include "player_avatar.h"
#include "save_local_field_data.h"
#include "unk_02092BE8.h"
#include "overlay_27_ov27task.h"


s32 ov27_0225BDAC(Ov27Task *arg0);

s32 ov27_0225BD50(FieldSystem *fieldSystem) {
    SaveVarsFlags *state;

    state = Save_VarsFlags_Get(fieldSystem->saveData);
    if (FieldSystem_MapIsBattleTowerMultiPartnerSelectRoom(fieldSystem) != 0) {
        return 6;
    }
    if (Save_VarsFlags_CheckSafariSysFlag(state) != 0) {
        return 1;
    }
    if (Save_VarsFlags_CheckBugContestFlag(state) != 0) {
        return 2;
    }
    if (Save_VarsFlags_CheckPalParkSysFlag(state) != 0) {
        return 3;
    }
    if (fieldSystem->bottomScreenType == 3) {
        return 4;
    }
    if (fieldSystem->mapLoadType == 3) {
        return 5;
    }
    return 0;
}

s32 ov27_0225BDAC(Ov27Task *arg0) {
    if ((u32)((arg0->unk_51C_1) - 2) > 4U) {
        return 1;
    }
    return 0;
}

BOOL ov27_0225BDC8(Ov27Task *arg0) {
    return PlayerSaveData_CheckRunningShoes(LocalFieldData_GetPlayer(Save_LocalFieldData_Get(arg0->fieldSystem->saveData)));
}

void ov27_0225BDDC(Ov27PhoneIcon *arg0, Ov27Task *arg1) {
    arg0->gearPhone = FieldSystem_GetGearPhoneRingManager(arg1->fieldSystem);
    arg0->sprite = arg1->unk_390[3];
    arg0->bgConfig = arg1->bgConfig;
    arg0->unk_1C = 0;
}
