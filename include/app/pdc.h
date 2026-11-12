// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/pdc.h: the lines in use so far.
#ifndef BWPORT_APP_PDC_H
#define BWPORT_APP_PDC_H

#include "types.h"
#include "struct_decls.h"
#include "gfl/heap.h"

// Black's; the heap the scene's process makes for itself
#define HEAPID_PDC 0x85

// What the scene is given, 0x20 bytes. Only pdc.c reads the fields
typedef struct PdcWork PdcWork;

// pokebw2's PdcWork_Create
PdcWork *func_ov098_021b6100(GameData *gameData, PartyPkm *pkm, BtlFieldStatus *status, PlayerInfo *playerInfo,
                             BagSave *bag, TrainerDataSave *trainerData, PokeDexSave *pokedex, HeapID heapId);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's PdcWork_GetFieldStatus
BtlFieldStatus *func_ov098_021b6144(const PdcWork *work);
// pokebw2's PdcWork_GetGameData
GameData *func_ov098_021b613c(const PdcWork *work);
// pokebw2's PdcWork_GetPkm
PartyPkm *func_ov098_021b6140(const PdcWork *work);
// pokebw2's PdcWork_GetTrainerData
TrainerDataSave *func_ov098_021b6150(const PdcWork *work);
// pokebw2's PdcWork_GetPokedex
PokeDexSave *func_ov098_021b6154(const PdcWork *work);
// pokebw2's PdcWork_GetBag
BagSave *func_ov098_021b614c(const PdcWork *work);
// pokebw2's PdcWork_GetPlayerInfo
PlayerInfo *func_ov098_021b6148(const PdcWork *work);
// pokebw2's PdcWork_IsCaught
// Whether the Pokémon was caught
BOOL func_ov098_021b6158(const PdcWork *work);

// The Poké Transfer catch screen's proc
extern const GameProcFunctions data_ov098_021b6d74;

#endif // BWPORT_APP_PDC_H
