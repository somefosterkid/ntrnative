#include "global.h"

#include "frontier/overlay_80_02229EE0.h"
#include "overlay_80_battle_castle.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "seal_case.h"
#include "pokemon_types_def.h"
#include "overlay_12_field_battle_dto.h"
#include "battle_castle_save.h"

struct BattleCastleSave;

struct Party;

Pokemon *AllocMonZeroed(enum HeapID heapID);

void Heap_Free(void *ptr);

void ov80_0222A140(const FrontierPokemon *frontierMon, Pokemon *mon, s32 setLevel);

u32 sub_02030D84(BattleCastleSave *svae, u8 field, u8 index, u8 unused, void *unused2);

u32 ov80_02237D88(BattleCastle *battleCastle);

void ov80_02237E18(struct BattleCastle *battleCastle, Party *party, Pokemon *mon);

void ov80_022329B4(struct BattleCastle *castle, s32);

void ov80_02232908(struct BattleCastle *castle, s32 arg1);

void ov80_02232908(struct BattleCastle *castle, s32 arg1) {
    FrontierPokemon mons[6];
    u8 ivs[6];
    u16 setIDs[6];
    u32 personalities[6];

    ov80_022329B4(castle, arg1);

    for (int i = 0; i < 7 * 2; i++) {
        castle->trainerIDs[i] = sub_02030D84(castle->castleSave, 6, i, 0, 0L);
    }

    for (int i = 0; i < 2 * 2; i++) {
        setIDs[i] = sub_02030D84(castle->castleSave, 8, i, 0, 0L);
        castle->monSetIDs[i] = setIDs[i];
    }

    ov80_0222A52C(mons, setIDs, ivs, 0L, personalities, 4, HEAP_ID_FIELD2, (enum NarcId)205);

    Pokemon *mon = AllocMonZeroed(HEAP_ID_FIELD2);
    for (int i = 0; i < 2 * 2; i++) {
        ov80_0222A140(&mons[i], mon, ov80_02237D88(castle));
        ov80_02237E18(castle, castle->opponentsParty, mon);
    }

    Heap_Free(mon);
}
