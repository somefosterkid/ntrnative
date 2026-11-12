// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/save/encounter.h, the lines in use so far.
// Black keeps two roaming Pokémon, so its roaming functions take a slot.
#ifndef BWPORT_SAVE_ENCOUNTER_H
#define BWPORT_SAVE_ENCOUNTER_H

#include "types.h"
#include "struct_decls.h"

// The fields of a roaming Pokémon, for EncountSave_GetRoamingPkmParam
enum {
    ROAMING_PKM_PARAM_ZONE = 1,
    ROAMING_PKM_PARAM_IVS,
    ROAMING_PKM_PARAM_PERSONALITY,
    ROAMING_PKM_PARAM_SPECIES,
    ROAMING_PKM_PARAM_HP,
    ROAMING_PKM_PARAM_LEVEL,
    ROAMING_PKM_PARAM_STATUS,
    ROAMING_PKM_PARAM_ACTIVE,
    ROAMING_PKM_PARAM_UNK13,
    ROAMING_PKM_PARAM_NATURE,
};

typedef struct {
    u16 zone;
    u8 nature;
    u32 ivs;
    u32 personality;
    u16 species;
    u16 hp;
    u8 level;
    u8 status;
    // Whether the Pokémon roams
    u8 active;
    u8 unk13;
} RoamingPkm;

// The save table's size and init of the block
u32 EncountSave_GetBlockSize(void);
void EncountSave_Init(void *block);
// Picks a new swarm location (an index of overlay 36's swarm table)
void EncountSave_RerollSwarmLocation(SaveControl *save);
// Takes a step off the repel: TRUE when this step ran it out
u32 EncountSave_DecrementRepelSteps(EncountSave *encount);
// Moves the zone the player is in into the save, keeping the one before
void func_0200d45c(EncountSave *save, u16 zoneId);
// The state of the roaming Pokémon of the slot
u8 func_0200d468(EncountSave *save, u8 slot);
void func_0200d470(EncountSave *save, u8 slot, u8 state);
// The roaming Pokémon's place in ROAMING_POKEMON_ZONES
u8 EncountSave_GetRoamingPkmZoneClock(EncountSave *save, u8 slot);
void func_0200d480(EncountSave *save, u8 slot, u8 clock);
// The roaming Pokémon of the slot, whether it roams, and its parameters
BOOL EncountSave_GetRoamingPkmStatus(EncountSave *save, u8 slot);
// Clears a roaming Pokémon
void func_0200d494(RoamingPkm *roamingPkm);
RoamingPkm *EncountSave_GetRoamingPkm(EncountSave *save, u8 slot);
u32 EncountSave_GetRoamingPkmParam(const RoamingPkm *roamingPkm, u32 param);
// Sets a field of a roaming Pokemon (ROAMING_PKM_PARAM_*)
void func_0200d50c(RoamingPkm *roamingPkm, u32 param, u32 value);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
u8 EncountSave_GetSwarmLocation(EncountSave *save);
BOOL EncountSave_IsRepelDepleted(EncountSave *encount);
void EncountSave_SetRepelSteps(EncountSave *encount, u8 steps);

#define ENCOUNT_SAVE_ROAMING_COUNT 2

// Block 0x38. Black 2's has one roaming Pokemon, no zones and the repel's item and the caught flags after the repel's
// steps
struct EncountSave {
    // The zone the player is in, and the one before (func_0200d45c)
    /* 0x00 */ u16 zone;
    /* 0x02 */ u16 prevZone;
    /* 0x04 */ RoamingPkm roamingPkm[ENCOUNT_SAVE_ROAMING_COUNT];
    // Each roaming Pokemon's place in ROAMING_POKEMON_ZONES
    /* 0x2c */ u8 roamingZoneClocks[ENCOUNT_SAVE_ROAMING_COUNT];
    // The state of each roaming Pokemon (func_0200d468)
    /* 0x2e */ u8 roamingStates[ENCOUNT_SAVE_ROAMING_COUNT];
    // An index of overlay 36's swarm table
    /* 0x30 */ u8 swarmLocation;
    /* 0x31 */ u8 repelSteps;
    /* 0x32 */ u8 unk_32[2];
}; // size: 0x34

#endif // BWPORT_SAVE_ENCOUNTER_H
