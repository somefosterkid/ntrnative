// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/battle_facility.h: the prototypes of
// Black's named functions and the types they need.
#ifndef BWPORT_FIELD_BATTLE_FACILITY_H
#define BWPORT_FIELD_BATTLE_FACILITY_H

#include "types.h"
#include "struct_decls.h"
#include "gfl/heap.h"
#include "nitro/math.h"

// Black 2's, not checked against Black
// A file of a battle facility's Pokémon arc
struct BSubwayPokemonData {
    u16 species;
    u16 moves[4];
    // A bit for each stat that gets effort values
    u8 evFlags;
    u8 nature;
    u16 item;
    u16 form;
};

// What a trainer class gives, for the Trial House and Battle Subway
u32 func_ov010_02168aec(u32 trainerClass);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
BtlSetup *SetupTrialHouseBattle(GameSystem *gsys, PokeParty *party, u32 mode, BSubwayTrainer *trainers,
                                BSubwayTrainer *partner, int count);
// Loads the trainer of the file, and count Pokémon for it, none of species and none holding items
BOOL func_ov010_02168878(BSubwayTrainer *trainer, u16 trainerId, u32 count, const u16 *species, const u16 *items,
                         BSubwayTeamConfig *config, HeapID heapId);
// Black's has the arc and the IVs in it, and no random state
BOOL func_ov010_02168924(const u16 *trainerData, u16 trainerId, BSubwayPokemon *pkms, u8 count, const u16 *species,
                         const u16 *items, BSubwayTeamConfig *config, HeapID heapId);
// A trainer's file of the Battle Subway's trainer arc, loaded (Black's has the arc in it)
u16 *func_ov010_0216880c(u16 trainerId, HeapID heapId);

#endif // BWPORT_FIELD_BATTLE_FACILITY_H
