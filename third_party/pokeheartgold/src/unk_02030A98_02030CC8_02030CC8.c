#include "global.h"

#include "save.h"
#include "save_frontier.h"
#include "unk_02030A98.h"

BattleCastleSave *sub_02030CC8(SaveData *saveData) {
    FrontierSave *frontier = Save_Frontier_GetStatic(saveData);
    return &frontier->castleSave;
}
