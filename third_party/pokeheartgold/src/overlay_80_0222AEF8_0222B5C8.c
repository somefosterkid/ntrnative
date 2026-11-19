#include "global.h"
#include "overlay_80_battle_castle.h"
#include "save.h"
#include "pokemon_types_def.h"
#include "overlay_12_field_battle_dto.h"
#include "battle_castle_save.h"
#include "save_frontier.h"

struct BattleCastle;
struct BattleCastlePersistentSave;
struct BattleCastleSave;
struct FrontierSave;
struct FieldBattleDTO;
struct FrontierOpponent;
struct FrontierPokemon;
struct FrontierTrainer;
struct Party;
struct SaveData;
BattleCastlePersistentSave *sub_02030E08(SaveData *saveData);
enum BattleFrontierStatsIndex sub_0205C1F0(u8 challengeType);
int sub_0205C268(u32 statIndex);
FrontierSave *Save_Frontier_GetStatic(SaveData *saveData);
u16 FrontierSave_GetStat(FrontierSave *frontier, enum BattleFrontierStatsIndex statIndex, int hostFriendID);
BOOL sub_02037030(int cmd, const void *data, int size);
u16 sub_0203769C(void);

BOOL ov80_0222B5C8(BattleCastle *battleCastle);
void ov80_0222B628(int netID, int sLotoId, void *partnersCP, void *battleCastle);
BOOL ov80_0222B65C(BattleCastle *battleCastle);
void ov80_0222B690(int netID, int unused, void *trainerIDs, void *battleCastle);

BOOL ov80_0222B5C8(BattleCastle *battleCastle) {
    int index, result, dataSize;
    BattleCastlePersistentSave *unused2 = sub_02030E08(battleCastle->saveData);
    dataSize = ((int)(sizeof(battleCastle->commBuffer) / sizeof(*(battleCastle->commBuffer))));
    index = 0;

    battleCastle->commBuffer[1] = battleCastle->currentStreak;
    battleCastle->commBuffer[2] = battleCastle->currentRound;

    index += 3;
    index += (7 + 1);

    battleCastle->commBuffer[index] = FrontierSave_GetStat(Save_Frontier_GetStatic(battleCastle->saveData), sub_0205C1F0(battleCastle->challengeType), sub_0205C268(sub_0205C1F0(battleCastle->challengeType)));
    index += 1;

    if (sub_02037030(42, battleCastle->commBuffer, dataSize) == 1) {
        result = 1;
    } else {
        result = 0;
    }

    return result;
}

void ov80_0222B628(int netID, int sLotoId, void *partnersCP, void *battleCastle) {
    int index;
    BattleCastle *battleCastle_dupe = battleCastle;
    const u16 *partnersCP_dupe = partnersCP;

    index = 0;
    battleCastle_dupe->msgsReceived++;

    if (sub_0203769C() == netID) {
        return;
    }

    battleCastle_dupe->unk_A16 = partnersCP_dupe[1];
    battleCastle_dupe->unk_A12 = partnersCP_dupe[2];

    index += 3;
    index += (7 + 1);

    battleCastle_dupe->partnersCP = partnersCP_dupe[index];
    index += 1;
}

BOOL ov80_0222B65C(BattleCastle *battleCastle) {
    int i, dummy, result, dataSize;

    dummy = 0;
    dataSize = ((int)(sizeof(battleCastle->commBuffer) / sizeof(*(battleCastle->commBuffer))));

    for (i = 0; i < (7 * 2); i++) {
        battleCastle->commBuffer[i + dummy] = battleCastle->trainerIDs[i];
    }

    dummy += (7 * 2);

    if (sub_02037030(43, battleCastle->commBuffer, dataSize) == 1) {
        result = 1;
    } else {
        result = 0;
    }

    return result;
}

void ov80_0222B690(int netID, int unused, void *trainerIDs, void *battleCastle) {
    int i, dummy;
    BattleCastle *battleCastle_dupe = battleCastle;
    const u16 *trainerIDs_dupe = trainerIDs;

    dummy = 0;
    battleCastle_dupe->msgsReceived++;

    if (sub_0203769C() == netID || sub_0203769C() == 0) {
        return;
    }

    for (i = 0; i < (7 * 2); i++) {
        battleCastle_dupe->trainerIDs[i] = trainerIDs_dupe[i + dummy];
    }

    dummy += (7 * 2);
}
