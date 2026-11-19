#include "global.h"

#include "frontier/frontier.h"
#include "frontier/frontier_script_context.h"
#include "frontier/overlay_80_0222BDF4.h"

#include "frontier_data.h"
#include "game_stats.h"

u32 FrtCmd_130(FrontierScriptContext *param_1);

u32 FrtCmd_130(FrontierScriptContext *param_1) {
    FrontierLaunchArgs *pFVar1;
    u16 addend;
    GameStats *gameStats;
    FrontierData *frontierData;

    pFVar1 = Frontier_GetLaunchArgs(param_1->frontierSystem->unk0);
    addend = FrontierScript_ReadVar(param_1);
    gameStats = Save_GameStats_Get(pFVar1->saveData);
    GameStats_Add(gameStats, 0x45, addend);
    frontierData = Save_FrontierData_Get(pFVar1->saveData);
    FrontierData_BattlePointAction(frontierData, addend, 5);
    return 0;
}

u32 FrtCmd_131(FrontierScriptContext *param_1);

u32 FrtCmd_131(FrontierScriptContext *param_1) {
    FrontierLaunchArgs *pFVar1;
    u16 addend;
    GameStats *gameStats;
    FrontierData *frontierData;

    pFVar1 = Frontier_GetLaunchArgs(param_1->frontierSystem->unk0);
    addend = FrontierScript_ReadVar(param_1);
    gameStats = Save_GameStats_Get(pFVar1->saveData);
    GameStats_Add(gameStats, 0x46, addend);
    frontierData = Save_FrontierData_Get(pFVar1->saveData);
    FrontierData_BattlePointAction(frontierData, addend, 6);
    return 0;
}
