#include "global.h"

#include "save.h"
#include "save_frontier.h"
#include "unk_02030A98.h"

BattleCastlePersistentSave *sub_02030E08(SaveData *saveData) {
    FrontierSave *frontier = Save_Frontier_GetStatic(saveData);
    return &frontier->castle.persistentSave;
}
