#include "global.h"

#include "constants/heap.h"

#include "save.h"
#include "overlay_80_frontier_opponent.h"
#include "overlay_73_wifi_battle_tower_app_state.h"

struct WifiBattleTowerDownloadData;
typedef struct UnkStruct_ov80_02236450_02236698_11C {
    u32 trainerID;
    u16 trainerType;
    u16 objectID;
    u16 trainerName[7 + 1];
    u16 introMsg[4];
    u16 winMsg[4];
    u16 loseMsg[4];
} UnkStruct_ov80_02236450_02236698_11C;
BOOL sub_0202D7B0(WifiBattleTowerDownloadData *downloadData);
void sub_0202D804(WifiBattleTowerDownloadData *downloadData, FrontierOpponent *opponent, const u8 opponentNum);
WifiBattleTowerDownloadData *sub_0202D928(SaveData *saveData);
void ov80_022366D4(FrontierOpponent *opponent, const u8 opponentID);

void ov80_02236698(SaveData *saveData, FrontierOpponent *oppponent, const u8 opponentID);

extern const FrontierPokemon ov80_0223C0AC[17];
extern const UnkStruct_ov80_02236450_02236698_11C ov80_0223C07C[7];
typedef struct BattleTowerDummyTrainer {
    u16 objectID;
    u8 mons[4];
} BattleTowerDummyTrainer;
extern const BattleTowerDummyTrainer ov80_0223C050[7];

void ov80_02236698(SaveData *saveData, FrontierOpponent *oppponent, const u8 opponentID) {
    MI_CpuClear8(oppponent, sizeof(FrontierOpponent));

    WifiBattleTowerDownloadData *wifiTowerData = sub_0202D928(saveData);

    if (!sub_0202D7B0(wifiTowerData)) {
        ov80_022366D4(oppponent, opponentID);
        return;
    }

    sub_0202D804(wifiTowerData, oppponent, opponentID);
}

void ov80_022366D4(FrontierOpponent *opponent, const u8 opponentID) {
    int i;
    MI_CpuClear8(opponent, sizeof(FrontierOpponent));

    const UnkStruct_ov80_02236450_02236698_11C *trainer = &(ov80_0223C07C[opponentID]);
    const FrontierPokemon *sDummyMons = ov80_0223C0AC;
    const BattleTowerDummyTrainer *sDummyTrainers = &ov80_0223C050[opponentID];

    MI_CpuCopy8(trainer, &opponent->trainer, sizeof(UnkStruct_ov80_02236450_02236698_11C));
    opponent->trainer.objectID = sDummyTrainers->objectID;

    for (i = 0; i < 4; i++) {
        MI_CpuCopy8(&sDummyMons[sDummyTrainers->mons[i]], &opponent->pokemon[i], sizeof(FrontierPokemon));
    }
}
