// pokebw2's encounter_savedata.c (the name is pokebw2's guess: the ROM has no string for it): the wild encounters'
// save, block 0x38: the two roaming Pokémon, the swarm's location and the repel's steps.

#include "types.h"
#include "gfl/std.h"
#include "save/encounter.h"
#include "save/save_control.h"
#include "struct_decls.h"
#include "gfl/random.h"

u32 EncountSave_GetBlockSize(void) {
    return sizeof(EncountSave);
}

EncountSave *SaveControl_GetEncountSave(SaveControl *save) {
    EncountSave *pEVar1;

    pEVar1 = SaveControl_GetBlockPtr(save, 0x38);
    return pEVar1;
}

// The save table's init of the block
void EncountSave_Init(void *block) {
    EncountSave *save = block;

    MI_CpuFill8(save, 0, sizeof(EncountSave));
    save->swarmLocation = 0;
    save->repelSteps = 0;
}

void EncountSave_RerollSwarmLocation(SaveControl *save) {
    EncountSave *encount = SaveControl_GetEncountSave(save);

    encount->swarmLocation = GFL_RandomLCAlt(0x11);
}

u8 EncountSave_GetSwarmLocation(EncountSave *save) {
    return save->swarmLocation;
}

void func_0200d45c(EncountSave *save, u16 zoneId) {
    if (save->zone != zoneId) {
        save->prevZone = save->zone;
        save->zone = zoneId;
    }
}

u8 func_0200d468(EncountSave *save, u8 slot) {
    return save->roamingStates[slot];
}

void func_0200d470(EncountSave *save, u8 slot, u8 state) {
    save->roamingStates[slot] = state;
}

u8 EncountSave_GetRoamingPkmZoneClock(EncountSave *save, u8 slot) {
    return save->roamingZoneClocks[slot];
}

void func_0200d480(EncountSave *save, u8 slot, u8 clock) {
    save->roamingZoneClocks[slot] = clock;
}

BOOL EncountSave_GetRoamingPkmStatus(EncountSave *save, u8 slot) {
    return save->roamingPkm[slot].active;
}

void func_0200d494(RoamingPkm *roamingPkm) {
    MI_CpuFill8(roamingPkm, 0, sizeof(RoamingPkm));
    roamingPkm->zone = 0xffff;
}

RoamingPkm *EncountSave_GetRoamingPkm(EncountSave *save, u8 slot) {
    return &save->roamingPkm[slot];
}

u32 EncountSave_GetRoamingPkmParam(const RoamingPkm *roamingPkm, u32 param) {
    switch (param) {
    case ROAMING_PKM_PARAM_ZONE:
        return roamingPkm->zone;
    case ROAMING_PKM_PARAM_IVS:
        return roamingPkm->ivs;
    case ROAMING_PKM_PARAM_PERSONALITY:
        return roamingPkm->personality;
    case ROAMING_PKM_PARAM_NATURE:
        return roamingPkm->nature;
    case ROAMING_PKM_PARAM_SPECIES:
        return roamingPkm->species;
    case ROAMING_PKM_PARAM_HP:
        return roamingPkm->hp;
    case ROAMING_PKM_PARAM_LEVEL:
        return roamingPkm->level;
    case ROAMING_PKM_PARAM_STATUS:
        return roamingPkm->status;
    case ROAMING_PKM_PARAM_ACTIVE:
        return roamingPkm->active;
    case ROAMING_PKM_PARAM_UNK13:
        return roamingPkm->unk13;
    }
    return 0;
}

void func_0200d50c(RoamingPkm *roamingPkm, u32 param, u32 value) {
    switch (param) {
    case ROAMING_PKM_PARAM_ZONE:
        roamingPkm->zone = value;
        return;
    case ROAMING_PKM_PARAM_IVS:
        roamingPkm->ivs = value;
        return;
    case ROAMING_PKM_PARAM_PERSONALITY:
        roamingPkm->personality = value;
        return;
    case ROAMING_PKM_PARAM_NATURE:
        roamingPkm->nature = value;
        return;
    case ROAMING_PKM_PARAM_SPECIES:
        roamingPkm->species = value;
        return;
    case ROAMING_PKM_PARAM_HP:
        roamingPkm->hp = value;
        return;
    case ROAMING_PKM_PARAM_LEVEL:
        roamingPkm->level = value;
        return;
    case ROAMING_PKM_PARAM_STATUS:
        roamingPkm->status = value;
        return;
    case ROAMING_PKM_PARAM_ACTIVE:
        roamingPkm->active = value;
        return;
    case ROAMING_PKM_PARAM_UNK13:
        roamingPkm->unk13 = value;
    default:
        return;
    }
}

void EncountSave_SetRepelSteps(EncountSave *encount, u8 steps) {
    encount->repelSteps = steps;
}

u32 EncountSave_DecrementRepelSteps(EncountSave *param_1) {
    if (param_1->repelSteps != 0) {
        param_1->repelSteps--;
        if (param_1->repelSteps == 0) {
            return 1;
        }
    }
    return 0;
}

BOOL EncountSave_IsRepelDepleted(EncountSave *encount) {
    if (encount->repelSteps == 0) {
        return 1;
    }
    return 0;
}
