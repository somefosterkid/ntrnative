#include "global.h"

#include "frontier/overlay_80_02229EE0.h"
#include "overlay_80_frontier_opponent.h"
#include "constants/heap.h"
#include "player_data.h"
#include "message_format.h"
#include "filesystem_files_def.h"

struct FrontierPokemon;

struct FrontierTrainerBase;

typedef struct FrontierPokemonBase {
    u16 species;
    u16 moves[4];
    u8 evFlags;
    u8 nature;
    u16 item;
    u16 form;
} FrontierPokemonBase;

typedef struct FrontierTrainerBase FrontierTrainerBase;

void Heap_Free(void *ptr);

void ov80_02229EF4(FrontierPokemonBase *monData, int narcIdx, enum NarcId narcID);

FrontierTrainerBase *ov80_02229F04(FrontierTrainer *trainer, int trainerID, enum HeapID heapID, enum NarcId narcID);

u8 ov80_0222A5A4(u16 trainerID);

extern BOOL ov80_0222A5E0(FrontierTrainerBase *trainer, const u16 partnersSpecies[], const u16 partnersItems[], int partnersNumMons, int numMons, u16 selectedMons[], enum HeapID heapID);

void ov80_0222A6B8(u8 numMons, u16 trainerID, u16 trainerID2, u16 *monSetIDs, FrontierPokemon *resultMons, u8 *ivs, u32 *personalities, u8 isMultiplayerChallenge);

struct MessageFormat;
struct PlayerProfile;
PlayerProfile *sub_02034818(int netId);
u16 sub_0203769C(void);
void ov80_0222A7CC(MessageFormat *template, u32 idx);

void ov80_0222A6B8(u8 numMons, u16 trainerID, u16 trainerID2, u16 *monSetIDs, FrontierPokemon *resultMons, u8 *ivs, u32 *personalities, u8 isMultiplayerChallenge) {
    int i;
    u16 alreadySelectedSpecies[4];
    u16 alreadySelectedItems[4];

    FrontierTrainer trainer;
    FrontierTrainerBase *trainerSpec = ov80_02229F04(&trainer, trainerID, HEAP_ID_FIELD2, (enum NarcId)204);

    for (i = 0; i < numMons; i++) {
        ivs[i] = ov80_0222A5A4(trainerID);
    }

    if (!isMultiplayerChallenge) {
        ov80_0222A5E0(trainerSpec, alreadySelectedSpecies, alreadySelectedItems, 0, numMons, monSetIDs, HEAP_ID_FIELD2);
    } else {
        ov80_0222A5E0(trainerSpec, alreadySelectedSpecies, alreadySelectedItems, 0, numMons / 2, monSetIDs, HEAP_ID_FIELD2);

        for (i = 0; i < (numMons / 2); i++) {
            FrontierPokemonBase mon;
            ov80_02229EF4(&mon, monSetIDs[i], (enum NarcId)205);
            alreadySelectedSpecies[i] = mon.species;
            alreadySelectedItems[i] = mon.item;
        }

        Heap_Free(trainerSpec);
        trainerSpec = ov80_02229F04(&trainer, trainerID2, HEAP_ID_FIELD2, (enum NarcId)204);
        ov80_0222A5E0(trainerSpec, alreadySelectedSpecies, alreadySelectedItems, numMons / 2, numMons / 2, &monSetIDs[numMons / 2], HEAP_ID_FIELD2);

        for (i = 0; i < numMons / 2; i++) {
            ivs[i + (numMons / 2)] = ov80_0222A5A4(trainerID2);
        }
    }

    Heap_Free(trainerSpec);
    ov80_0222A52C(resultMons, monSetIDs, ivs, 0L, personalities, numMons, HEAP_ID_FIELD2, (enum NarcId)205);
}

void ov80_0222A7CC(MessageFormat *template, u32 idx) {
    PlayerProfile *partner = sub_02034818(sub_0203769C() ^ 1);
    BufferPlayersName(template, idx, partner);
}
