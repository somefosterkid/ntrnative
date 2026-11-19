#include "global.h"

#include "constants/heap.h"

#include "heap.h"
#include "math_util.h"
#include "save.h"
#include "seal_case.h"
#include "sys_task.h"
#include "overlay_80_battle_factory.h"
#include "pokemon_types_def.h"
#include "overlay_12_field_battle_dto.h"

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
Pokemon *Party_GetMonByIndex(const Party *party, int slot);
Pokemon *AllocMonZeroed(enum HeapID heapID);
void ov80_0222A140(const FrontierPokemon *frontierMon, Pokemon *mon, s32 setLevel);
void ov80_0222A3BC(SaveData *saveData, Party *party, Pokemon *mon);
extern void ov80_02236BE4(u8 challengeType, int currentRound, u16 trainerIDs[], u8 numTrainers);
u8 ov80_02236DF8(u8 challengeType, BOOL param1);
void ov80_02236E24(u32 round, u32 isOpenLevel, u16 *monSetIDs, FrontierPokemon *mons, u8 *ivs, u32 *personalities, u16 tradeCount, u16 *excludedSpecies, u16 *unused);
void ov80_02236E90(u8 numMons, u16 trainerID, u8 isOpenLevel, FrontierPokemon *excludedMons, u16 *monSetIDs, FrontierPokemon *mons, u8 *ivList, u32 *personalities, int numExcluded);
s32 ov80_02237120(BattleFactory *battleFactory);
BOOL ov80_02237254(u8 challengeType);
u16 ov80_022372B4(BattleFactory *battleFactory);
static void ov80_022300D4(BattleFactory *factory, u8 idx1, u8 isPartners);

void ov80_0222FF00(BattleFactory *factory);
static void ov80_022300D4(BattleFactory *factory, u8 idx1, u8 isPartners);

void ov80_0222FF00(BattleFactory *factory) {
    int i;
    FrontierPokemon usedMons[6 * 2];

    ov80_02236BE4(factory->challengeType, ov80_022372B4(factory), factory->trainerIDs, 7 * 2);

    ov80_02236E24(ov80_022372B4(factory), factory->isOpenLevel, factory->initialRentalSetIDs, factory->initialRentalMons, factory->initialRentalIVs, factory->initialRentalPersonalities, factory->tradeCount, 0L, 0L);

    ov80_022300D4(factory, 4, 0);
    ov80_022300D4(factory, 5, 0);

    int numPokemonUsed = 6;

    for (i = 0; i < 6; i++) {
        usedMons[i] = factory->initialRentalMons[i];
    }

    if (ov80_02237254(factory->challengeType) == 1) {
        u16 partnersSpecies[6];
        u16 partnersItems[6];
        for (i = 0; i < 6; i++) {
            partnersSpecies[i] = factory->initialRentalMons[i].species;
            partnersItems[i] = factory->initialRentalMons[i].item;
        }

        ov80_02236E24(ov80_022372B4(factory), factory->isOpenLevel, factory->partnerRentalSetIDs, factory->partnerRentalMons, factory->partnerRentalIVs, factory->partnerRentalPersonalities, factory->unk_580, partnersSpecies, partnersItems);

        ov80_022300D4(factory, 4, 1);
        ov80_022300D4(factory, 5, 1);

        numPokemonUsed = 6 * 2;

        for (i = 0; i < 6; i++) {
            usedMons[i + 6] = factory->partnerRentalMons[i];
        }
    }

    ov80_02236E90(ov80_02236DF8(factory->challengeType, 1), factory->trainerIDs[factory->currentBattle], factory->isOpenLevel, usedMons, factory->opponentMonSetIDs, factory->opponentMons, factory->opponentMonIVs, factory->opponentMonPersonalities, numPokemonUsed);

    for (i = 0; i < 6; i++) {
        Pokemon *mon = AllocMonZeroed(HEAP_ID_FIELD2);
        ov80_0222A140(&factory->initialRentalMons[i], mon, ov80_02237120(factory));
        ov80_0222A3BC(factory->saveData, factory->playersParty, mon);
        Heap_Free(mon);
    }

    for (i = 0; i < 6; i++) {
        Party_GetMonByIndex(factory->playersParty, i);
    }
}

static void ov80_022300D4(BattleFactory *factory, u8 idx1, u8 isPartners) {
    u16 setID;
    u8 ivs;
    u32 personality;

    u16 idx2 = LCRandom() % 6;

    if (!isPartners) {
        setID = factory->initialRentalSetIDs[idx1];
        ivs = factory->initialRentalIVs[idx1];
        personality = factory->initialRentalPersonalities[idx1];
        FrontierPokemon mon = factory->initialRentalMons[idx1];

        factory->initialRentalSetIDs[idx1] = factory->initialRentalSetIDs[idx2];
        factory->initialRentalIVs[idx1] = factory->initialRentalIVs[idx2];
        factory->initialRentalPersonalities[idx1] = factory->initialRentalPersonalities[idx2];
        factory->initialRentalMons[idx1] = factory->initialRentalMons[idx2];

        factory->initialRentalSetIDs[idx2] = setID;
        factory->initialRentalIVs[idx2] = ivs;
        factory->initialRentalPersonalities[idx2] = personality;
        factory->initialRentalMons[idx2] = mon;
    } else {
        setID = factory->partnerRentalSetIDs[idx1];
        ivs = factory->partnerRentalIVs[idx1];
        personality = factory->partnerRentalPersonalities[idx1];
        FrontierPokemon mon = factory->partnerRentalMons[idx1];

        factory->partnerRentalSetIDs[idx1] = factory->partnerRentalSetIDs[idx2];
        factory->partnerRentalIVs[idx1] = factory->partnerRentalIVs[idx2];
        factory->partnerRentalPersonalities[idx1] = factory->partnerRentalPersonalities[idx2];
        factory->partnerRentalMons[idx1] = factory->partnerRentalMons[idx2];

        factory->partnerRentalSetIDs[idx2] = setID;
        factory->partnerRentalIVs[idx2] = ivs;
        factory->partnerRentalPersonalities[idx2] = personality;
        factory->partnerRentalMons[idx2] = mon;
    }
}
