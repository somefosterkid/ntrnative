#include "global.h"

#include "constants/heap.h"

#include "heap.h"
#include "save.h"
#include "seal_case.h"
#include "sys_task.h"
#include "overlay_80_battle_factory.h"
#include "pokemon_types_def.h"
#include "overlay_12_field_battle_dto.h"

struct BattleFactory;
struct BattleFactorySave;
struct FieldBattleDTO;
struct Party;
typedef u16 charcode_t;
typedef union MailIconData {
    struct {
        u16 spriteIndex : 12;
        u16 palIndex : 4;
    } asStruct;
    u16 asValue;
} MailIconData;
void SaveArray_Party_Init(void *party);
Pokemon *Party_GetMonByIndex(const Party *party, int slot);
Pokemon *AllocMonZeroed(enum HeapID heapID);
void ov80_0222A140(const FrontierPokemon *frontierMon, Pokemon *mon, s32 setLevel);
void ov80_0222A3BC(SaveData *saveData, Party *party, Pokemon *mon);
u8 ov80_02236DD4(u8 challengeType);
u8 ov80_02236DF8(u8 challengeType, BOOL param1);
s32 ov80_02237120(BattleFactory *battleFactory);
BOOL ov80_0222B108(BattleFactory *battleFactory);
BOOL ov80_0222B174(BattleFactory *battleFactory);
BOOL ov80_0222B1DC(BattleFactory *battleFactory);
BOOL ov80_0222B2C4(BattleFactory *battleFactory);
BOOL ov80_0222B3B0(BattleFactory *battleFactory, u8 param1);
BOOL ov80_0222B3FC(BattleFactory *battleFactory, u8 param1);
BOOL ov80_0222B448(BattleFactory *battleFactory);

void ov80_02230A60(BattleFactory *factory);
void ov80_02230AE4(BattleFactory *factory);
BOOL ov80_02230AF8(BattleFactory *factory, u16 command, u16 arg);

void ov80_02230A60(BattleFactory *factory) {
    (void)(ov80_02236DD4(factory->challengeType));
    u8 partySize = ov80_02236DF8(factory->challengeType, 1);

    SaveArray_Party_Init(factory->opponentsParty);

    Pokemon *mon = AllocMonZeroed(HEAP_ID_FIELD2);

    for (int i = 0; i < partySize; i++) {
        ov80_0222A140(&factory->opponentMons[i], mon, ov80_02237120(factory));
        ov80_0222A3BC(factory->saveData, factory->opponentsParty, mon);
    }

    Heap_Free(mon);

    for (int i = 0; i < partySize; i++) {
        (void)(Party_GetMonByIndex(factory->opponentsParty, i));
    }
}

void ov80_02230AE4(BattleFactory *factory) {
    if (factory->tradeCount < 9999) {
        factory->tradeCount++;
    }
}

BOOL ov80_02230AF8(BattleFactory *factory, u16 command, u16 arg) {
    int success;

    switch (command) {
    case 0:
        success = ov80_0222B108(factory);
        break;
    case 1:
        success = ov80_0222B174(factory);
        break;
    case 2:
        success = ov80_0222B1DC(factory);
        break;
    case 3:
        success = ov80_0222B2C4(factory);
        break;
    case 4:
        success = ov80_0222B3B0(factory, arg);
        break;
    case 5:
        success = ov80_0222B3FC(factory, arg);
        break;
    case 6:
        success = ov80_0222B448(factory);
        break;
    }

    return success;
}
