#include "global.h"

#include "save.h"
#include "save_frontier.h"
#include "unk_02030A98.h"

BattleArcadeStreakFlags *sub_02030FA0(SaveData *saveData) {
    FrontierSave *frontier = Save_Frontier_GetStatic(saveData);
    return &frontier->arcade.streakFlags;
}
