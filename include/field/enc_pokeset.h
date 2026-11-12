// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/enc_pokeset.h: the prototypes of
// Black's named functions and the types they need.
#ifndef BWPORT_FIELD_ENC_POKESET_H
#define BWPORT_FIELD_ENC_POKESET_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "struct_decls.h"

#define ENCTYPE_FISHING 5
#define ENCTYPE_FISHING_RARE 6

// The kinds of encounter, which pick the zone's slots
#define ENCTYPE_GRASS 0

#define ENCTYPE_GRASS_RARE 1
#define ENCTYPE_GRASS_SHAKING 2
#define ENCTYPE_SURF 3
#define ENCTYPE_SURF_RARE 4
#define ENCTYPE_MAX 7

// Black 2's; twinned code confirms the offsets of encType, slotFuncIndex, slotCount, pokeCount, leadItem, leadLevel,
// isFishing, repelActive, forced, rateUp, rateDown, fishingRateUp, magnetPull, staticAbility and repelsWeaker in Black
// What an encounter knows of the player's lead Pokémon and how it rolls the wild ones
struct EncountManager {
    GameData *gameData;
    EncountSave *encountSave;
    int encType;
    u32 mode;
    u32 slotFuncIndex;
    u8 slotCount;
    u8 pokeCount;
    u16 leadSpecies;
    u16 leadItem;
    u8 leadIsEgg;
    u8 leadAbility;
    u8 leadSex;
    u8 leadNature;
    u8 leadLevel;
    u32 trainerId;
    PlayerInfo *playerInfo;
    u32 swarmsEnabled : 1;
    u32 isFishing : 1;
    u32 repelActive : 1;
    u32 forced : 1;
    u32 pairActive : 1;
    u32 doubleBattle : 1;
    u32 rateUp : 1;
    u32 rateDown : 1;
    u32 fishingRateUp : 1;
    u32 compoundEyes : 1;
    u32 synchronize : 1;
    u32 cuteCharm : 1;
    u32 magnetPull : 1;
    u32 staticAbility : 1;
    u32 higherLevel : 1;
    u32 repelsWeaker : 1;
};

// Black 2's; twinned code confirms the offsets of level in Black
// A wild Pokémon to make
struct WildPkmParam {
    u16 species;
    u16 item;
    u8 level;
    u8 form;
    // 1 for shiny, 2 for never shiny
    u8 shinyLock;
    u8 hiddenAbility;
    u8 unk08[8];
    // 1 for male, 2 for female
    u8 sex;
    u8 unk11[3];
};

u32 FieldEncount_CalcEncountRate(EncountManager *manager, GameData *gameData, u32 rate);
// Fills the encounter manager for an encounter of the type, in the weather
void CreateEncountManager(EncountManager *manager, GameData *gameData, int encType, u32 mode, u16 weather);
// Picks the wild Pokémon of the map's encounter data for the manager's encounter type
BOOL func_ov021_021a9604(void *mapEncount, EncountManager *manager, WildPkmParam *param);
// The roaming Pokémon of the zone to meet instead of the slots' (NULL for none)
void *func_ov021_021a957c(EncData *encData, EncountManager *manager, u16 zoneId);
u32 FieldEncount_GenWilds(EncData *encData, EncountManager *manager, u16 zoneId, WildPkmParam *params);

// Black 2's; twinned code confirms the offsets of zoneId in Black
// A swarm: its zone, species and form, and level range
struct SwarmData {
    u16 zoneId;
    u16 species;
    u8 form;
    u8 minLevel;
    u8 maxLevel;
};

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
const SwarmData *getSwarmDataPtr(GameData *gameData);

#endif // BWPORT_FIELD_ENC_POKESET_H
