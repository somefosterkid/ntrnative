#include "global.h"
#include "overlay_80_battle_factory.h"
#include "constants/heap.h"
#include "save.h"
#include "pokemon_types_def.h"
#include "sys_task.h"
#include "overlay_12_field_battle_dto.h"
#include "save_vars_flags.h"
#include "save_frontier.h"

struct BattleFactory;

struct BattleFactorySave;

struct BattleFactoryStreakFlags;

struct FrontierSave;

struct Party;

struct SaveData;

struct SysTask;

struct SysTaskQueue;

struct SaveVarsFlags;

Party *SaveArray_Party_Alloc(enum HeapID heapID);

void ov80_0222A840(SaveData *saveData);

void sub_02030940(BattleFactorySave *save);

BattleFactorySave *sub_0203094C(SaveData *saveData);

u32 sub_02030A24(BattleFactorySave *save, u8 field, u8 index, void *unused);

u32 sub_02030AD4(BattleFactoryStreakFlags *flags, u8 field, u8 challengeType, void *unused);

BattleFactoryStreakFlags *sub_02030AE8(SaveData *saveData);

BOOL ov80_02237254(u8 challengeType);

enum BattleFrontierStatsIndex sub_0205BFF0(u8 isOpenLevel, u8 challengeType);

enum BattleFrontierStatsIndex sub_0205C048(u8 isOpenLevel, u8 challengeType);

int sub_0205C268(u32 statIndex);

FrontierSave *Save_Frontier_GetStatic(SaveData *saveData);

u16 FrontierSave_GetStat(FrontierSave *frontier, enum BattleFrontierStatsIndex statIndex, int hostFriendID);

SaveVarsFlags *Save_VarsFlags_Get(SaveData *saveData);

u16 Save_VarsFlags_GetVar4052(SaveVarsFlags *varsFlags);

BattleFactory *ov80_0222FD08(SaveData *saveData, u16 resumingFromSave, u8 challengeType, u8 isOpenLevel);

BattleFactory *ov80_0222FD08(SaveData *saveData, u16 resumingFromSave, u8 challengeType, u8 isOpenLevel) {
    static BattleFactory *factory;
    factory = Heap_Alloc(HEAP_ID_FIELD2, sizeof(BattleFactory));
    MI_CpuClear8(factory, sizeof(BattleFactory));

    factory->factorySave = sub_0203094C(saveData);
    factory->saveData = saveData;
    factory->unused = 11;
    factory->playersParty = SaveArray_Party_Alloc(HEAP_ID_FIELD2);
    factory->opponentsParty = SaveArray_Party_Alloc(HEAP_ID_FIELD2);

    BattleFactorySave *factorySave = factory->factorySave;
    BattleFactoryStreakFlags *streakFlags = sub_02030AE8(saveData);

    if (!resumingFromSave) {
        factory->challengeType = challengeType;
        factory->isOpenLevel = isOpenLevel;
        factory->currentBattle = 0;

        sub_02030940(factorySave);

        u8 streakActive;
        if (factory->challengeType == 3) {
            streakActive = Save_VarsFlags_GetVar4052(Save_VarsFlags_Get(factory->saveData));
        } else {
            streakActive = sub_02030AD4(streakFlags, 10, (factory->isOpenLevel * 4) + factory->challengeType, 0L);
        }

        if (streakActive == 1) {
            factory->currentStreak = FrontierSave_GetStat(Save_Frontier_GetStatic(factory->saveData), sub_0205BFF0(factory->isOpenLevel, factory->challengeType), sub_0205C268(sub_0205BFF0(factory->isOpenLevel, factory->challengeType)));
            factory->tradeCount = FrontierSave_GetStat(Save_Frontier_GetStatic(factory->saveData), sub_0205C048(factory->isOpenLevel, factory->challengeType), sub_0205C268(sub_0205C048(factory->isOpenLevel, factory->challengeType)));
        } else {
            factory->currentStreak = 0;
            factory->tradeCount = 0;
        }

        factory->unused3 = 0;
    } else {
        factory->challengeType = sub_02030A24(factorySave, 1, 0, 0L);
        factory->isOpenLevel = sub_02030A24(factorySave, 0, 0, 0L);
        factory->currentBattle = sub_02030A24(factorySave, 2, 0, 0L);
        factory->currentStreak = FrontierSave_GetStat(Save_Frontier_GetStatic(factory->saveData), sub_0205BFF0(factory->isOpenLevel, factory->challengeType), sub_0205C268(sub_0205BFF0(factory->isOpenLevel, factory->challengeType)));
        factory->tradeCount = FrontierSave_GetStat(Save_Frontier_GetStatic(factory->saveData), sub_0205C048(factory->isOpenLevel, factory->challengeType), sub_0205C268(sub_0205C048(factory->isOpenLevel, factory->challengeType)));
    }

    factory->currentRound = factory->currentStreak / 7;

    if (ov80_02237254(factory->challengeType) == 1) {
        ov80_0222A840(factory->saveData);
    }

    return factory;
}
