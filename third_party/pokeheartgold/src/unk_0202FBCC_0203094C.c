#include "global.h"

#include "save.h"
#include "save_frontier.h"
#include "unk_02030A98.h"

BOOL sub_0203095C(BattleFactorySave *save);
void sub_02030964(BattleFactorySave *save, BOOL didSave);
void sub_02030978(BattleFactorySave *save, u8 field, u8 index, const void *value);
u32 sub_02030A24(BattleFactorySave *save, u8 field, u8 index, void *unused);

BattleFactorySave *sub_0203094C(SaveData *saveData) {
    FrontierSave *frontier = Save_Frontier_GetStatic(saveData);
    return &frontier->factorySave;
}

BOOL sub_0203095C(BattleFactorySave *save) {
    return save->didSave;
}

void sub_02030964(BattleFactorySave *save, BOOL didSave) {
    save->didSave = didSave;
}

void sub_02030978(BattleFactorySave *save, u8 field, u8 index, const void *value) {
    switch (field) {
    case 0:
        save->isOpenLevel = *(u8 *)value;
        break;
    case 1:
        save->challengeType = *(u8 *)value;
        break;
    case 2:
        save->currentBattleNum = *(u8 *)value;
        break;
    case 3:
        save->trainerIDs[index] = *(u16 *)value;
        break;
    case 4:
        save->playerPartySets[index] = *(u16 *)value;
        break;
    case 5:
        save->playerIVs[index] = *(u8 *)value;
        break;
    case 6:
        save->playerPersonality[index] = *(u32 *)value;
        break;
    case 7:
        save->opponentPartySets[index] = *(u16 *)value;
        break;
    case 8:
        save->opponentIVs[index] = *(u8 *)value;
        break;
    case 9:
        save->opponentPersonality[index] = *(u32 *)value;
        break;
    }
}

u32 sub_02030A24(BattleFactorySave *save, u8 field, u8 index, void *unused) {
    switch (field) {
    case 1:
        return save->challengeType;
    case 0:
        return save->isOpenLevel;
    case 2:
        return save->currentBattleNum;
    case 3:
        return save->trainerIDs[index];
    case 4:
        return save->playerPartySets[index];
    case 5:
        return save->playerIVs[index];
    case 6:
        return save->playerPersonality[index];
    case 7:
        return save->opponentPartySets[index];
    case 8:
        return save->opponentIVs[index];
    case 9:
        return save->opponentPersonality[index];
    }

    return 0;
}
