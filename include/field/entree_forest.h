// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/entree_forest.h, the lines in use so
// far.
#ifndef BWPORT_FIELD_ENTREE_FOREST_H
#define BWPORT_FIELD_ENTREE_FOREST_H

#include "types.h"
#include "nitro/fx/fx.h"
#include "struct_decls.h"
#include "gfl/heap.h"

// A Pokémon of the Entree Forest, packed in a word that its actor keeps in params 1 and 2
typedef union {
    u32 raw;
    struct {
        u32 species : 11;
        u32 move : 10;
        u32 sex : 2;
        u32 form : 6;
        u32 actorType : 3;
    };
} EntreeForestPokemon;

// Function names from swan.
void GetEntreeForestActorParamBits(u32 *paramBits, FieldActor *actor);

struct EntreeForestSpawnContext {
    TPokeData *pokemonData;
    Field *field;
    u32 actorIdBase;
};

void EntreeForest_SpawnAllPokemon(Field *field, u32 actorIdBase, const EntreeForestPokemon *pokemon, u32 unused,
                                  u32 area);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
GameEvent *EventEntreeForestWarp_Create(GameSystem *gsys, u32 mode, const VecFx32 *position, u32 arg3, u32 arg4);
FieldActor *EntreeForest_SpawnPkmActor(const EntreeForestSpawnContext *context, const EntreeForestPokemon *pokemon, u16 index,
                                      u16 x, u16 z, s32 y);
u16 GetActorUserParam0(FieldActor *actor);
extern const u8 data_ov018_02183678[];
extern const u8 data_ov018_021835f8[];
extern const u8 data_ov018_02183650[];
PartyPkm *func_ov018_0217c2a0(HeapID heapId, GameData *gameData, const EntreeForestPokemon *pokemon);

// Whether the zone is the Entree Forest's
BOOL func_ov018_0217c11c(u16 zoneId);

#endif // BWPORT_FIELD_ENTREE_FOREST_H
