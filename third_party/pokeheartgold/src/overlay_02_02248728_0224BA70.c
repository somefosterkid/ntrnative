#include "global.h"

#include "field_roamer.h"
#include "roamer.h"
#include "overlay_2/overlay_02_02248728.h"
#include "field_types_def.h"
#include "save.h"
#include "script_manager.h"

void ov02_RepelActiveRoamersFromMapNo(RoamerSaveData *arg0, s32 arg1);

void ov02_RepelActiveRoamersFromMapNo(RoamerSaveData *roamerSave, s32 arg1) {
    u8 roamerId;

    roamerId = 0;
    do {
        if ((GetRoamerIsActiveByIndex(roamerSave, roamerId) != 0) && (arg1 == GetRoamMapByLocationIdx(Roamer_GetLocation(roamerSave, roamerId)))) {
            RoamerLocationUpdateRand(roamerSave, roamerId);
        }
        roamerId += 1;
    } while ((u32)roamerId < 4U);
}

Roamer *ov02_0224BAA8(RoamerSaveData *arg0, s32 arg1);

Roamer *ov02_0224BAA8(RoamerSaveData *roamerSave, s32 arg1) {
    Roamer *roamer;
    u8 roamerId;

    roamerId = 0;
loop_1:
    if (GetRoamerIsActiveByIndex(roamerSave, roamerId) != 0) {
        roamer = Roamers_GetRoamMonStats(roamerSave, roamerId);
        if (arg1 == GetRoamerData(roamer, 4)) {
            return roamer;
        }
    }
    roamerId += 1;
    if ((u32)roamerId >= 4U) {
        return NULL;
    }
    goto loop_1;
}

BOOL PlayerStepEvent_RepelCounterDecrement(SaveData *saveData, FieldSystem *fieldSystem) {
    RoamerSaveData *roamerSave;
    u8 *puVar1;
    roamerSave = Save_Roamers_Get(saveData);
    puVar1 = RoamerSave_GetRepelAddr(roamerSave);
    if (((*puVar1) != 0) && (*puVar1 = (*puVar1) + ((char)0xff), (*puVar1) == 0)) {
        StartMapSceneScript(fieldSystem, 0x7e6, (LocalMapObject *)0x0);
        return 1;
    }
    return 0;
}
