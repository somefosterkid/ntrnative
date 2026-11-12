// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/encounter.h, the lines in use so far.
#ifndef BWPORT_FIELD_ENCOUNTER_H
#define BWPORT_FIELD_ENCOUNTER_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"
#include "nitro/fx/fx.h"

// Names and layouts from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)
struct EncountSystem {
    Field *field;
    GameSystem *gsys;
    GameData *gameData;
    EncData *encData;
    EffectEncountState *effectEncountState;
};

// Black's is 0x20 bytes, laid out unlike Black 2's; only the fields in use are mapped
struct RareEncountSpot {
    u16 zoneId;
    s16 x;
    s16 y;
    s16 z;
    fx32 height;
    // The kind of tile, as func_ov036_021a2e18 gives it
    u8 kind;
    u8 active : 4;
    u8 suspended : 2;
    u8 triggered : 2;
};

struct EncountState {
    u16 gridX;
    u16 gridY;
    u16 gridZ;
    u16 unk_6;
    u32 rateStepCounter;
    u32 unkC;
    RareEncountSpot rareSpot;
};

// Names and layouts from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)
typedef struct {
    u16 species : 11;
    u16 form : 5;
    u8 minLevel;
    u8 maxLevel;
} WildEncSlot;

// A zone's wild encounters for a season, 0xe8 bytes
struct EncData {
    // The encounter rate of each kind of encounter, 0 for none
    u8 userData[7];
    u8 flags : 7;
    u8 fishingEnable : 1;
    WildEncSlot grass[12];
    WildEncSlot grassRare[12];
    WildEncSlot grassShaking[12];
    WildEncSlot surf[5];
    WildEncSlot surfRare[5];
    WildEncSlot fishing[5];
    WildEncSlot fishingRare[5];
};

void EncountSystem_CancelPhenomenon(EncountSystem *encountSystem);
BOOL EncountState_CheckSpecialEncountPos(EncountSystem *encounter, const s16 *gridPos);
BtlSetup *BtlSetup_CreateFishing(EncountSystem *encounter, BOOL rare);
BOOL EncData_Load(EncData *encData, ArcTool *arc, u16 zoneId, u8 season);
EncountState *EncountState_Create(HeapID heapId);
void EncountState_Free(EncountState *state);
void func_ov010_0215e33c(Field *field);
void func_ov010_0215e34c(GameData *gameData);
// The overworld patches (a grass patch, a dust cloud, a water ripple or a flying shadow) of effect_encount.c (pokebw2's
// func_ov036_021a203c)
void func_ov021_021aa7dc(EncountSystem *system, EffectEncountState *state);

void EffectEncountState_Free(EffectEncountState *state);
u32 ConvFieldWeatherToBtl(Field *field);

// The item the phenomenon gave
u16 func_ov021_021aab48(EncountSystem *system);
// The wild battle of a static encounter, its level and flags as the script gave them
GameEvent *func_ov021_021aa0a0(EncountSystem *system, u16 species, u8 level, u16 flags);

u16 getSwarmLevelRangeFromData(GameData *gameData);

// Black 2's, not checked against Black
// How the encounter rate of a kind of encounter climbs while the player walks
struct EncountRateConfig {
    u8 increase;
    u8 threshold;
    u8 max;
    u8 steps;
};

void PrepareFieldEncountSystem(EncountSystem *system, EffectEncountState *state);
void EncSys_Free(EncountSystem *system);
void func_ov021_021aa7cc(EncountSystem *system, EffectEncountState *state);

// Black 2's; twinned code confirms the offsets of x, z, height and kind in Black
// Where a phenomenon (shaking grass, a dust cloud, rippling water or a flying shadow) is
// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// The battle's encounter effect and music, against the wild species or the trainer
// Black's has no form (EventWildBattleCall_Callback's call)
void EventBattleCall_DecideEnvWild(s32 species, s32 a2, BOOL a3, Field *field, u32 *encEffect, u16 *bgm);
void EventBattleCall_DecideEnvTrainer(u32 trainerId, Field *field, u32 *encEffect, u16 *bgm);
// The style is 0 to 3 for single, double, triple and rotation; a double battle is against two trainers with a
// partner, and a single one with a second trainer
void BtlSetup_SetTrainerLocal(EncountSystem *encountSystem, BtlSetup *setup, u32 style, u32 partnerId, u32 trainerId,
                              u32 trainerId2, HeapID heapId);
void func_ov021_021aaabc(EncountSystem *system);
// The distance to the phenomenon, along the axis it is farther on, and whether there is one
BOOL func_ov021_021aaa4c(EncountSystem *system, u16 *distance);
// The overworld patches (a grass patch, a dust cloud, a water ripple or a flying shadow) of effect_encount.c
EffectEncountState *EffectEncountState_Create(HeapID heapId);
// The wild encounters of field_encount.c
EncountSystem *EncSys_Create(Field *field);
void UpdatePhenomenon(EncountSystem *system);
BOOL EncountSystem_CancelPhenomenonIfActorHit(EncountSystem *system);
GameEvent *CreateRandomPhenomenonEvent(EncountSystem *system);
void func_ov021_021aab1c(EncountSystem *system, u32 a1);

#endif // BWPORT_FIELD_ENCOUNTER_H
