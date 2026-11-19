#include "global.h"
#include "overlay_80_battle_arcade.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "seal_case.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "overlay_12_field_battle_dto.h"

struct BallSeal;

struct BattleArcade;

struct BattleArcadeSave;

struct FrontierPokemon;

struct Party;

struct Pokemon;

typedef struct BallSeal {
    u8 type;
    u8 x;
    u8 y;
} BallSeal;

void Heap_Free(void *ptr);

Pokemon *AllocMonZeroed(enum HeapID heapID);

void ov80_0222A140(const FrontierPokemon *frontierMon, Pokemon *mon, s32 setLevel);

extern void ov80_0222A52C(FrontierPokemon *mons, u16 narcIdx[], u8 ivList[], u32 personalities[], u32 resultPersonalities[], int numMons, enum HeapID heapID, enum NarcId narcID);

typedef struct BattleArcadeSave BattleArcadeSave;

u32 sub_02030F34(BattleArcadeSave *save, u8 field, u8 index, u8 unused, void *unused2);

u32 ov80_02238370(BattleArcade *battleArcade);

void ov80_022383A8(struct BattleArcade *battleArcade, Party *party, Pokemon *mon);

void ov80_022344D4(struct BattleArcade *arcade);

void ov80_02234424(BattleArcade *arcade);

void ov80_02234424(BattleArcade *arcade) {
    FrontierPokemon mons[6];
    u8 ivs[6];
    u16 setIDs[6];
    u32 personalities[6];

    ov80_022344D4(arcade);

    for (int i = 0; i < 7 * 2; i++) {
        arcade->trainerIDs[i] = sub_02030F34(arcade->arcadeSave, 5, i, 0, 0L);
    }

    for (int i = 0; i < (((3) >= (2 * 2)) ? (3) : (2 * 2)); i++) {
        setIDs[i] = sub_02030F34(arcade->arcadeSave, 7, i, 0, 0L);
        arcade->monSetIDs[i] = setIDs[i];
    }

    ov80_0222A52C(mons, setIDs, ivs, 0L, personalities, (((3) >= (2 * 2)) ? (3) : (2 * 2)), HEAP_ID_FIELD2, (enum NarcId)205);

    Pokemon *mon = AllocMonZeroed(HEAP_ID_FIELD2);

    for (int i = 0; i < (((3) >= (2 * 2)) ? (3) : (2 * 2)); i++) {
        ov80_0222A140(&mons[i], mon, ov80_02238370(arcade));
        ov80_022383A8(arcade, arcade->opponentsParty, mon);
    }

    Heap_Free(mon);
}
