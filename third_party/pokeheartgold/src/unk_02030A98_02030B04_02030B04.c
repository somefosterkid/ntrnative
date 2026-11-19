#include "global.h"

#include "save.h"
#include "save_frontier.h"
#include "unk_02030A98.h"

BattleHallSave *sub_02030B04(SaveData *saveData) {
    FrontierSave *frontier = Save_Frontier_GetStatic(saveData);
    return &frontier->hallSave;
}
