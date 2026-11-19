#include "global.h"

#include "heap.h"
#include "save.h"
#include "sys_task.h"
#include "overlay_80_battle_factory.h"
#include "pokemon_types_def.h"
#include "overlay_12_field_battle_dto.h"

struct BattleFactorySave;
struct FieldBattleDTO;
struct Party;
typedef struct BattleFactorySave BattleFactorySave;
typedef struct BattleFactoryAppArgs {
    SaveData *saveData;
    u8 challengeType;
    u8 isOpenLevel;
    u8 isExchangeMode;
    u8 padding_07;
    Party *personalParty;
    Party *receivableParty;
    u16 selectedIndices[6];
    void *battleFactory;
    u8 padding_20[4];
} BattleFactoryAppArgs;
static u16 ov80_02230484(BattleFactoryAppArgs *args, u8 i);

void ov80_02230424(BattleFactory *factory);
void ov80_02230460(BattleFactory *battleFactory, BattleFactoryAppArgs *args);
static u16 ov80_02230484(BattleFactoryAppArgs *args, u8 i);

void ov80_02230424(BattleFactory *factory) {
    if (factory == 0L) {
        return;
    }

    if (factory->playersParty != 0L) {
        Heap_Free(factory->playersParty);
    }

    if (factory->opponentsParty != 0L) {
        Heap_Free(factory->opponentsParty);
    }

    MI_CpuClear8(factory, sizeof(BattleFactory));

    Heap_Free(factory);
    factory = 0L;
}

void ov80_02230460(BattleFactory *battleFactory, BattleFactoryAppArgs *args) {
    for (int i = 0; i < 6; i++) {
        battleFactory->selectedAppSlots[i] = ov80_02230484(args, i);
    }
}

static u16 ov80_02230484(BattleFactoryAppArgs *args, u8 i) {
    if (i >= 6) {
        ((0) ? (void)0 : GF_AssertFail());
        return 0;
    }

    return args->selectedIndices[i];
}
