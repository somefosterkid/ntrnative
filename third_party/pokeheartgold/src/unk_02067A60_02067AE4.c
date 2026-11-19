#include "global.h"

#include "field_roamer.h"
#include "field_system.h"
#include "field_types_def.h"
#include "map_header.h"
#include "player_avatar.h"
#include "roamer.h"
#include "save_local_field_data.h"
#include "save_vars_flags.h"
#include "sys_flags.h"
#include "unk_0206793C.h"

void FlypointFlagAction(FieldSystem *, s32);

void sub_02067AE4(FieldSystem *fieldSystem) {
    SaveVarsFlags *pSVar1;
    RoamerSaveData *pRVar2;
    BOOL BVar3;
    LocalFieldData *localFieldData;
    PlayerSaveData *pPVar4;

    if (fieldSystem->unkAC != 1) {
        pSVar1 = Save_VarsFlags_Get(fieldSystem->saveData);
        ClearFlag972(pSVar1);
        pSVar1 = Save_VarsFlags_Get(fieldSystem->saveData);
        StrengthFlagAction(pSVar1, 0);
        FlypointFlagAction(fieldSystem, fieldSystem->location->mapId);
        pRVar2 = Save_Roamers_Get(fieldSystem->saveData);
        RoamerSave_SetFlute(pRVar2, 0);
        fieldSystem->encounterInhibitSteps = 0;
        fieldSystem->reverseTurnFrameSteps = 0;
        pRVar2 = Save_Roamers_Get(fieldSystem->saveData);
        UpdatePlayerLocationHistoryIfAnyRoamersActive(pRVar2, fieldSystem->location->mapId);
        BVar3 = MapHeader_IsCave(fieldSystem->location->mapId);
        if (BVar3 == 0) {
            pSVar1 = Save_VarsFlags_Get(fieldSystem->saveData);
            SysFlagFlashClear(pSVar1);
            SysFlagDefogClear(pSVar1);
        }
        localFieldData = Save_LocalFieldData_Get(fieldSystem->saveData);
        pPVar4 = LocalFieldData_GetPlayer(localFieldData);
        if ((pPVar4->state == 1) && (BVar3 = MapHeader_IsBikeAllowed(fieldSystem->location->mapId), BVar3 == 0)) {
            pPVar4->state = 0;
            return;
        }
        if (pPVar4->state == 2) {
            pPVar4->state = 0;
        }
    }
    return;
}
