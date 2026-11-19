#include "global.h"

#include "frontier_data.h"
#include "save.h"
#include "save_frontier.h"
#include "unk_0202D230.h"
#include "unk_02030A98.h"

BattleTowerSave *sub_0202D908(SaveData *saveData) {
    FrontierSave *pFVar1;

    pFVar1 = Save_Frontier_GetStatic(saveData);
    return &pFVar1->towerSave;
}

FrontierData *Save_FrontierData_Get(SaveData *saveData) {
    FrontierSave *pFVar1;

    pFVar1 = Save_Frontier_GetStatic(saveData);
    return (FrontierData *)&pFVar1->unk_950.wifiBattleTowerRecord;
}

struct WifiBattleTowerDownloadData *sub_0202D928(SaveData *saveData) {
    FrontierSave *pFVar1;

    pFVar1 = Save_Frontier_GetStatic(saveData);
    return &pFVar1->unk_950.wifiBattleTowerDownloadData;
}
