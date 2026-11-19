#include "global.h"
#include "overlay_80_battle_factory.h"
#include "save.h"
#include "pokemon_types_def.h"
#include "sys_task.h"
#include "overlay_12_field_battle_dto.h"

struct BattleFactory;

struct BattleFactorySave;

struct FieldBattleDTO;

struct FrontierOpponent;

struct FrontierPokemon;

struct FrontierTrainer;

struct Party;

struct SaveData;

struct SysTask;

struct SysTaskQueue;


enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

typedef struct BattleFactorySave BattleFactorySave;

u16 ov80_02230B4C(BattleFactory *factory);

u16 ov80_02230B4C(BattleFactory *factory) {
    u8 bp;
    static const u8 sBPPerRoundSolo[((49 / 7) + 1) + 1] = { 0, 5, 5, 5, 5, 7, 7, 8, 9 };
    static const u8 sBPPerRoundWifi[((49 / 7) + 1) + 1] = { 0, 10, 11, 12, 13, 16, 17, 19, 21 };

    u16 currentRound = factory->currentRound;

    if (factory->challengeType == 0 || factory->challengeType == 1) {
        if (currentRound >= ((49 / 7) + 1)) {
            bp = sBPPerRoundSolo[((49 / 7) + 1)];
        } else {
            bp = sBPPerRoundSolo[currentRound];
        }
    } else {
        if (currentRound >= ((49 / 7) + 1)) {
            bp = sBPPerRoundWifi[((49 / 7) + 1)];
        } else {
            bp = sBPPerRoundWifi[currentRound];
        }
    }

    if (factory->challengeType == 0) {
        if (factory->currentStreak == 21 || factory->currentStreak == 49) {
            bp = 20;
        }
    }

    return bp;
}
