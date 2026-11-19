#include "global.h"

#include "save.h"
#include "save_frontier.h"
#include "unk_02030A98.h"

BattleHallStreakFlags *sub_02030C5C(SaveData *saveData) {
    FrontierSave *frontier = Save_Frontier_GetStatic(saveData);
    return &frontier->hall.streakFlags;
}
