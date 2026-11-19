#include "global.h"
#include "overlay_80_battle_factory.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "seal_case.h"
#include "pokemon_types_def.h"
#include "sys_task.h"
#include "overlay_12_field_battle_dto.h"

struct BallSeal;

struct BattleFactory;

struct BattleFactorySave;

struct FrontierPokemon;

struct Party;

struct Pokemon;

struct SaveData;

struct SysTask;

struct SysTaskQueue;

typedef struct BallSeal {
    u8 type;
    u8 x;
    u8 y;
} BallSeal;

void Heap_Free(void *ptr);

Pokemon *AllocMonZeroed(enum HeapID heapID);

void ov80_0222A140(const FrontierPokemon *frontierMon, Pokemon *mon, s32 setLevel);

void ov80_0222A3BC(SaveData *saveData, Party *party, Pokemon *mon);

extern void ov80_0222A52C(FrontierPokemon *mons, u16 narcIdx[], u8 ivList[], u32 personalities[], u32 resultPersonalities[], int numMons, enum HeapID heapID, enum NarcId narcID);

typedef struct BattleFactorySave BattleFactorySave;

u32 sub_02030A24(BattleFactorySave *save, u8 field, u8 index, void *unused);

u8 ov80_02236DD4(u8 challengeType);

s32 ov80_02237120(BattleFactory *battleFactory);

void ov80_02230270(BattleFactory *factory);

void ov80_02230270(BattleFactory *factory) {
    FrontierPokemon mons[6];
    u8 ivs[6];
    u16 monSetIDs[6];
    u32 personalities[6];

    (void)(ov80_02236DD4(factory->challengeType));

    for (int i = 0; i < 7 * 2; i++) {
        factory->trainerIDs[i] = sub_02030A24(factory->factorySave, 3, i, 0L);
    }

    for (int i = 0; i < (((3) >= (2 * 2)) ? (3) : (2 * 2)); i++) {
        monSetIDs[i] = sub_02030A24(factory->factorySave, 4, i, 0L);
        personalities[i] = sub_02030A24(factory->factorySave, 6, i, 0L);
        ivs[i] = sub_02030A24(factory->factorySave, 5, i, 0L);

        factory->playerMonSetIDs[i] = monSetIDs[i];
    }

    ov80_0222A52C(mons, monSetIDs, ivs, personalities, 0L, (((3) >= (2 * 2)) ? (3) : (2 * 2)), HEAP_ID_FIELD2, (enum NarcId)205);

    Pokemon *mon = AllocMonZeroed(HEAP_ID_FIELD2);

    for (int i = 0; i < (((3) >= (2 * 2)) ? (3) : (2 * 2)); i++) {
        ov80_0222A140(&mons[i], mon, ov80_02237120(factory));
        ov80_0222A3BC(factory->saveData, factory->playersParty, mon);
    }

    Heap_Free(mon);

    for (int i = 0; i < (((3) >= (2 * 2)) ? (3) : (2 * 2)); i++) {
        monSetIDs[i] = sub_02030A24(factory->factorySave, 7, i, 0L);
        personalities[i] = sub_02030A24(factory->factorySave, 9, i, 0L);
        ivs[i] = sub_02030A24(factory->factorySave, 8, i, 0L);

        factory->opponentMonSetIDs[i] = monSetIDs[i];
    }

    ov80_0222A52C(mons, monSetIDs, ivs, personalities, 0L, (((3) >= (2 * 2)) ? (3) : (2 * 2)), HEAP_ID_FIELD2, (enum NarcId)205);

    mon = AllocMonZeroed(HEAP_ID_FIELD2);

    for (int i = 0; i < (((3) >= (2 * 2)) ? (3) : (2 * 2)); i++) {
        ov80_0222A140(&mons[i], mon, ov80_02237120(factory));
        ov80_0222A3BC(factory->saveData, factory->opponentsParty, mon);
    }

    Heap_Free(mon);
}
