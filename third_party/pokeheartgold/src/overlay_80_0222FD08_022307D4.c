#include "global.h"

#include "constants/heap.h"

#include "heap.h"
#include "save.h"
#include "seal_case.h"
#include "sys_task.h"
#include "overlay_80_battle_factory.h"
#include "pokemon_types_def.h"
#include "overlay_12_field_battle_dto.h"

struct BattleFactorySave;
struct FieldBattleDTO;
struct Party;
void ov80_0223049C(BattleFactory *factory, u8 saveType);
typedef u16 charcode_t;
typedef union MailIconData {
    struct {
        u16 spriteIndex : 12;
        u16 palIndex : 4;
    } asStruct;
    u16 asValue;
} MailIconData;
void SaveArray_Party_Init(void *party);
Pokemon *AllocMonZeroed(enum HeapID heapID);
void ov80_0222A140(const FrontierPokemon *frontierMon, Pokemon *mon, s32 setLevel);
void ov80_0222A3BC(SaveData *saveData, Party *party, Pokemon *mon);
u8 ov80_02236DD4(u8 challengeType);
u8 ov80_02236DF8(u8 challengeType, BOOL param1);
s32 ov80_02237120(BattleFactory *battleFactory);

void ov80_022307D4(BattleFactory *factory);
void ov80_022307F0(BattleFactory *factory);

void ov80_022307D4(BattleFactory *factory) {
    factory->saveStreak = 1;

    if (factory->currentRound < ((49 / 7) + 1)) {
        factory->currentRound++;
    }

    factory->currentBattle = 0;
    ov80_0223049C(factory, 0);
}

void ov80_022307F0(BattleFactory *factory) {
    u8 playerPartySize = ov80_02236DD4(factory->challengeType);
    u8 opponentPartySize = ov80_02236DF8(factory->challengeType, 1);

    SaveArray_Party_Init(factory->playersParty);

    Pokemon *mon = AllocMonZeroed(HEAP_ID_FIELD2);

    for (int i = 0; i < playerPartySize; i++) {
        ov80_0222A140(&factory->initialRentalMons[factory->selectedAppSlots[i]], mon, ov80_02237120(factory));
        ov80_0222A3BC(factory->saveData, factory->playersParty, mon);
        factory->playerMonSetIDs[i] = factory->initialRentalSetIDs[factory->selectedAppSlots[i]];
    }

    for (int i = 0; i < opponentPartySize; i++) {
        ov80_0222A140(&factory->opponentMons[i], mon, ov80_02237120(factory));
        ov80_0222A3BC(factory->saveData, factory->opponentsParty, mon);
    }

    Heap_Free(mon);
}
