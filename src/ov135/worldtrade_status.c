// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov214/worldtrade_status.c, which is the same
// code in Black 2/White 2's overlay 214. The names are Black's dsd names; pokebw2's guesses are in the comments.
#include "app/p_status.h"
#include "gfl/heap.h"
#include "gfl/proc.h"
#include "gfl/std.h"
#include "system/game_system.h"
#include "worldtrade_local.h"

// The Global Trade Station's summary screen of a Pokémon in the boxes, which runs overlay 131's summary. The name is
// the ROM's string, from GFL_HeapAllocate's call

// pokebw2's WorldTrade_Status_Init
int func_ov135_021ff58c(WorldTradeWork *wk, int seq) {
    PStatusParam *status;

    func_ov135_021f50ec(wk);
    wk->subProcParam = GFL_HeapAllocate(HEAPID_WORLDTRADE, sizeof(PStatusParam), FALSE, "worldtrade_status.c", 76);
    MI_CpuFill8(wk->subProcParam, 0, sizeof(PStatusParam));
    status = wk->subProcParam;
    status->trainerData = wk->param->config;
    status->gameData = GSYS_GetGameData(wk->param->gsys);
    if (func_ov135_021f6b34(wk->boxTrayNo)) {
        status->partyCount = func_ov135_021f6b80(wk->param->myparty, wk->param->mybox, wk->boxTrayNo);
    } else {
        status->partyCount = 30;
    }
    status->partyIndex = wk->boxCursorPos;
    status->mode = PSTATUS_MODE_1;
    status->move = 0;
    status->page = PSTATUS_PAGE_INFO;
    status->isNationalDex = wk->param->isNationalDex;
    status->fromFieldMenu = FALSE;
    if (func_ov135_021f6b34(wk->boxTrayNo)) {
        status->party = wk->param->myparty;
        status->dataType = PSTATUS_DATA_PARTY;
    } else {
        status->party = func_ov135_021f6b40(wk->param->myparty, wk->param->mybox, wk->boxTrayNo, 0);
        status->dataType = PSTATUS_DATA_BOX;
    }
    QueueGameProc(wk->procManager, OVERLAY_PSTATUS, &data_ov131_021dc800, status);
    wk->subprocFlag = 1;
    return WT_SEQ_FADEIN;
}

// pokebw2's WorldTrade_Status_Main
int func_ov135_021ff65c(WorldTradeWork *wk, int seq) {
    int ret = WT_SEQ_MAIN;

    if (!wk->procResult) {
        func_ov135_021f4ec0(wk, WORLDTRADE_MYBOX, wk->subProcessMode);
        ret = WT_SEQ_FADEOUT;
    }
    return ret;
}

// pokebw2's WorldTrade_Status_End
int func_ov135_021ff67c(WorldTradeWork *wk, int seq) {
    wk->boxCursorPos = ((PStatusParam *)wk->subProcParam)->partyIndex;
    GFL_HeapFree(wk->subProcParam);
    func_ov135_021f5074(wk);
    func_ov135_021f4ec8(wk);
    return WT_SEQ_INIT;
}
