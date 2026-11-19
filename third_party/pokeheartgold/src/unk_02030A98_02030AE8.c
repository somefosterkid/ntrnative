#include "global.h"
#include "simple3dmodel.h"

#include "save.h"
#include "save_frontier.h"
#include "unk_02030A98.h"


void sub_02030AF8(Simple3DRenderObj *renderObj);

BattleFactoryStreakFlags *sub_02030AE8(SaveData *saveData) {
    FrontierSave *frontier = Save_Frontier_GetStatic(saveData);
    return &frontier->factory.streakFlags;
}

void sub_02030AF8(Simple3DRenderObj *renderObj) {
    MI_CpuFill8(renderObj, 0, sizeof(Simple3DRenderObj));
}
