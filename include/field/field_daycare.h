// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/field_daycare.h, the lines in use so
// far.
#ifndef BWPORT_FIELD_FIELD_DAYCARE_H
#define BWPORT_FIELD_FIELD_DAYCARE_H

#include "types.h"
#include "struct_decls.h"
#include "app/p_status.h"
#include "app/pokelist.h"
#include "system/game_event.h"

// The field's day care block: the heap block func_ov021_021c31d4 makes around the save's DayCareSave
struct DayCare {
    HeapID heapId;
    u16 unk2;
    Field *field;
    DayCareSave *save;
    u16 unkC;
    u16 unkE;
};

DayCare *Field_GetDayCare(Field *field);
PartyPkm *DayCare_GetPkm(DayCare *dayCare, u32 slot);

u32 getNameGenderStatus(PartyPkm *pkm);

// Black 2's, not checked against Black
// The event that picks a party Pokémon to leave at the day care
struct DayCarePokeSelectWork {
    GameSystem *gsys;
    Field *field;
    PokeListParam *partyParam;
    PStatusParam *summaryParam;
    u16 *result;
};

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void DayCare_Breed(DayCare *dayCare, PokeParty *party);
GameEvent *EventDayCarePokeSelect_Create(GameSystem *gsys, Field *field, u16 *result);
void DayCare_AddPkm(DayCare *dayCare, PokeParty *party, u32 slot);
void DayCare_RemovePkm(DayCare *dayCare, u32 slot, PokeParty *party);
u32 DayCare_CalcNewLevel(DayCare *dayCare, u32 slot);
BOOL DayCare_CheckSpawnFlag(DayCare *dayCare);
void DayCare_CheckResetSeed(DayCare *dayCare);
GameEventReturnCode EventDayCarePokeSelect_Callback(GameEvent *event, u32 *state, void *data);
u8 DayCare_GetPkmCount(DayCare *dayCare);
u8 DayCare_CalcEggSpawnChance(DayCare *dayCare);
u32 DayCare_CalcLevelGain(DayCare *dayCare, u32 slot);

#endif // BWPORT_FIELD_FIELD_DAYCARE_H
