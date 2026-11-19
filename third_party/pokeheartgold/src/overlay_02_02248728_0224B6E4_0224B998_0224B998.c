#include "global.h"

#include "battle/battle_setup.h"

#include "field_roamer.h"
#include "field_system.h"
#include "field_types_def.h"
#include "math_util.h"
#include "overlay_02.h"
#include "party.h"
#include "pokemon.h"
#include "pokemon_types_def.h"
#include "roamer.h"
#include "save_vars_flags.h"
#include "sys_vars.h"
#include "trainer_data.h"

Roamer *ov02_0224BAA8(RoamerSaveData *, u32);

s32 ov02_RepelActiveRoamersFromMapNo(RoamerSaveData *, s32);

void ov02_BattleExit_HandleRoamerAction(FieldSystem *fieldSystem, BattleSetup *setup) {
    Roamer *roamer;
    u32 a1;
    BattleSetup *battleSetup;
    Pokemon *mon;
    Roamer *v0;
    RoamerSaveData *roamerSaveData;
    s32 v1;
    u16 val;
    u32 species;
    u8 v2;

    battleSetup = setup;
    mon = Party_GetMonByIndex(setup->party[1], 0);
    roamerSaveData = Save_Roamers_Get(fieldSystem->saveData);
    species = GetMonData(mon, 5, NULL);
    v0 = ov02_0224BAA8(roamerSaveData, species);
    roamer = v0;
    if (v0 != NULL) {
        a1 = (u32)SpeciesToRoamerIdx((u16)species);
        val = (u16)GetMonData(mon, 0xA3, NULL);
        v2 = (u8)GetMonData(mon, 0xA0, NULL);
        v1 = battleSetup->winFlag;
        if ((v1 == 1) && (val == 0)) {
            RoamerMon_Init(&roamer);
            sub_02066BE8(Save_VarsFlags_Get(fieldSystem->saveData), a1, 2U);
        } else if (v1 == 4) {
            RoamerMon_Init(&roamer);
            sub_02066BE8(Save_VarsFlags_Get(fieldSystem->saveData), a1, 1U);
        } else {
            SetRoamerData(roamer, 5, (s32)val);
            SetRoamerData(roamer, 7, (s32)v2);
        }
        ov02_RepelActiveRoamersFromMapNo(roamerSaveData, fieldSystem->location->mapId);
        return;
    }
    if ((u32)(u16)((s32)LCRandom() % 100) < 0x1EU) {
        ov02_RepelActiveRoamersFromMapNo(roamerSaveData, fieldSystem->location->mapId);
    }
}
