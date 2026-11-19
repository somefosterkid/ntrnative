#include "global.h"

#include "field_roamer.h"
#include "field_system.h"
#include "field_types_def.h"
#include "roamer.h"
#include "save_vars_flags.h"
#include "sys_flags.h"
#include "unk_0206793C.h"
#include "unk_02067A80.h"

void sub_02067A80(FieldSystem *fieldSystem, u8 a1) {
    fieldSystem->unkAC = (u32)a1;
}

s32 FlypointFlagAction(FieldSystem *, s32);

void sub_02067A88(FieldSystem *fieldSystem) {
    RoamerSaveData *roamerSaveData;

    StrengthFlagAction(Save_VarsFlags_Get(fieldSystem->saveData), 0U);
    FlypointFlagAction(fieldSystem, fieldSystem->location->mapId);
    RoamerSave_SetFlute(Save_Roamers_Get(fieldSystem->saveData), 0U);
    fieldSystem->encounterInhibitSteps = 0;
    fieldSystem->reverseTurnFrameSteps = 0;
    if (Save_VarsFlags_CheckSafariSysFlag(Save_VarsFlags_Get(fieldSystem->saveData)) == 0) {
        roamerSaveData = Save_Roamers_Get(fieldSystem->saveData);
        UpdatePlayerLocationHistoryIfAnyRoamersActive(roamerSaveData, (u32)fieldSystem->location->mapId);
        Save_UpdateRoamersLocation(roamerSaveData);
    }
}
