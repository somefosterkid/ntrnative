#include "global.h"

#include "save.h"
#include "save_frontier.h"
#include "unk_02030A98.h"

extern FrontierSave _021D2AFC;

FrontierSave *Save_Frontier_GetStatic(SaveData *saveData) {
    return &_021D2AFC;
}
