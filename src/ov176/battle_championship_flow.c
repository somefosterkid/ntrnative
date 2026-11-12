// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov261/battle_championship_flow.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "app/battle_championship_flow.h"
#include "types.h"
#include "app/digitalcardcheck.h"
#include "app/livebattlematch_flow.h"
#include "app/wifibattlematch_util.h"
#include "constants/arc.h"
#include "constants/script_text_banks.h"
#include "constants/sound.h"
#include "constants/text_banks.h"
#include "gfl/clact.h"
#include "gfl/fade.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/overlay.h"
#include "gfl/proc.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/ui.h"
#include "save/box.h"
#include "save/bsubway_save.h"
#include "save/player_info.h"
#include "save/records.h"
#include "save/save_control.h"
#include "save/tournament_block.h"
#include "system/dsi.h"
#include "system/game_data.h"
#include "system/gf_font.h"
#include "system/printsys.h"
#include "system/wordset.h"
#include "text/script/global_10520.h"
#include "text/system/entree_forest.h"

#define HEAPID_BATTLE_CHAMPIONSHIP_FLOW 0x4e

// What the menu's list holds
enum {
    LIST_TOP,
    LIST_WIFI,
    LIST_LIVE_MENU,
    LIST_LIVE,
};

// What the menu is for
enum {
    MENU_MODE_TOP,
    MENU_MODE_REWARD,
    MENU_MODE_LIVE,
};

// What the menu ends with
enum {
    MENU_RESULT_NONE,
    MENU_RESULT_EXIT,
    MENU_RESULT_WIFI,
    MENU_RESULT_LIVE,
};

// What the live competition's flow ends with
enum {
    LIVE_RESULT_NONE,
    LIVE_RESULT_MENU,
};

typedef struct {
    u16 heapId;
    WifiBattleMatchTask *task;
    WifiBattleMatchMsgBox *msgBox;
    Font *font;
    // The Entree Forest's messages
    MsgData *msgData;
    PrintQueue *printQueue;
    u32 unk18;
    u32 unk1C;
    WifiBattleMatchRes *seq;
    WifiBattleMatchGraphic *net;
    BattleChampionshipFlowParam *param;
    // The menu or the live competition's flow, whichever is running
    void *flow;
    // Set when the live competition sends the player back to the menu
    u32 backToMenu;
} BattleChampionshipFlow;

// What the menu is given
typedef struct {
    u32 mode;
    WifiBattleMatchMsgBox *msgBox;
    Font *font;
    MsgData *msgData;
    PrintQueue *printQueue;
    WifiBattleMatchRes *seq;
    WifiBattleMatchGraphic *net;
    GameData *gameData;
} BattleChampionshipMenuParam;

typedef struct {
    WifiBattleMatchTask *task;
    WifiBattleMatchMenu *list;
    u32 listKind;
    // Global script messages
    MsgData *scriptMsgData;
    u32 result;
    BattleChampionshipMenuParam param;
    u16 heapId;
    DigitalCardCheck *cardCheck;
} BattleChampionshipMenu;

void func_ov177_021e5534(WifiBattleMatchGraphic *net);
void func_ov177_021e5980(WifiBattleMatchRes *res);
void func_ov177_021e8a78(WifiBattleMatchTask *task);
s32 func_ov177_021e8a00(WifiBattleMatchMenu *menu);
BOOL func_ov177_021e8844(WifiBattleMatchMsgBox *msgBox);
void func_ov176_0217b93c(BattleChampionshipMenu *menu, u32 text);
void func_ov176_0217b7d8(BattleChampionshipMenu *menu, u32 kind);
void func_ov176_0217b970(BattleChampionshipMenu *menu, u32 result);
ClActUnit *func_ov177_021e5548(WifiBattleMatchGraphic *net);
WifiBattleMatchRes *func_ov177_021e567c(ClActUnit *unit, u32 kind, HeapID heapId);

BOOL func_ov176_0217ae20(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov176_0217ae88(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov176_0217aecc(GameProc *proc, u32 *state, void *param, void *work);
void func_ov176_0217aef4(BattleChampionshipFlow *wk);
void func_ov176_0217af04(BattleChampionshipFlow *wk);
static void func_ov176_0217af14(BattleChampionshipFlow *wk);
void func_ov176_0217af2c(BattleChampionshipFlow *wk);
void func_ov176_0217af44(BattleChampionshipFlow *wk);
void func_ov176_0217af50(BattleChampionshipFlow *wk);
void func_ov176_0217af9c(BattleChampionshipFlow *wk);
void func_ov176_0217afbc(BattleChampionshipFlow *wk);
void func_ov176_0217afd0(WifiBattleMatchTask *task, u32 *seq, void *work);
static void func_ov176_0217b00c(WifiBattleMatchTask *task, u32 *seq, void *work);
void func_ov176_0217b014(WifiBattleMatchTask *task, u32 *seq, void *work);
void func_ov176_0217b064(WifiBattleMatchTask *task, u32 *seq, void *work);
void func_ov176_0217b0c8(WifiBattleMatchTask *task, u32 *seq, void *work);
void func_ov176_0217b1bc(WifiBattleMatchTask *task, u32 *seq, void *work);
BattleChampionshipMenu *func_ov176_0217b2e4(const BattleChampionshipMenuParam *param, HeapID heapId);
void func_ov176_0217b344(BattleChampionshipMenu *menu);
void func_ov176_0217b35c(BattleChampionshipMenu *menu);
u32 func_ov176_0217b368(BattleChampionshipMenu *menu);
void func_ov176_0217b380(WifiBattleMatchTask *task, u32 *seq, void *work);
static void func_ov176_0217b3b8(WifiBattleMatchTask *task, u32 *seq, void *work);
void func_ov176_0217b3c0(WifiBattleMatchTask *task, u32 *seq, void *work);
void func_ov176_0217b468(WifiBattleMatchTask *task, u32 *seq, void *work);
static void func_ov176_0217b4a0(WifiBattleMatchTask *task, u32 *seq, void *work);
void func_ov176_0217b55c(WifiBattleMatchTask *task, u32 *seq, void *work);
void func_ov176_0217b594(WifiBattleMatchTask *task, u32 *seq, void *work);
void func_ov176_0217b620(WifiBattleMatchTask *task, u32 *seq, void *work);
void func_ov176_0217b55c(WifiBattleMatchTask *task, u32 *seq, void *work);
void func_ov176_0217b594(WifiBattleMatchTask *task, u32 *seq, void *work);
void func_ov176_0217b6ac(WifiBattleMatchTask *task, u32 *seq, void *work);
void func_ov176_0217b768(WifiBattleMatchTask *task, u32 *seq, void *work);
void func_ov176_0217b7a0(WifiBattleMatchTask *task, u32 *seq, void *work);
static void BattleChampionshipFlow_MenuTaskWifiEntry(WifiBattleMatchTask *task, u32 *seq, void *work);
static void func_ov176_0217b4a0(WifiBattleMatchTask *task, u32 *seq, void *work);
static void BattleChampionshipFlow_MenuOpenList(BattleChampionshipMenu *menu, u32 kind);
static void BattleChampionshipFlow_MenuCloseList(BattleChampionshipMenu *menu);
static s32 BattleChampionshipFlow_MenuGetListChoice(BattleChampionshipMenu *menu);
void func_ov176_0217b93c(BattleChampionshipMenu *menu, u32 msgId);
void func_ov176_0217b950(BattleChampionshipMenu *menu, u32 msgId);
BOOL func_ov176_0217b964(BattleChampionshipMenu *menu);
s32 func_ov176_0217b930(BattleChampionshipMenu *menu);
void func_ov176_0217b924(BattleChampionshipMenu *menu);
extern u32 OVERLAY_177_ID[];
WifiBattleMatchGraphic *func_ov177_021e5440(u32 layout, HeapID heapId);
void func_ov176_0217b7a0(WifiBattleMatchTask *task, u32 *seq, void *work);
void func_ov176_0217b768(WifiBattleMatchTask *task, u32 *seq, void *work);
void func_ov176_0217b620(WifiBattleMatchTask *task, u32 *seq, void *work);
void func_ov176_0217b55c(WifiBattleMatchTask *task, u32 *seq, void *work);
void func_ov176_0217b594(WifiBattleMatchTask *task, u32 *seq, void *work);
void func_ov176_0217b970(BattleChampionshipMenu *menu, u32 result);

// pokebw2's BATTLE_CHAMPIONSHIP_FLOW_PROC_FUNCTIONS
const GameProcFunctions data_ov176_0217e538 = {
    func_ov176_0217ae20,
    func_ov176_0217aecc,
    func_ov176_0217ae88,
};

// pokebw2's BattleChampionshipFlow_Main


BOOL func_ov176_0217ae20(GameProc *proc, u32 *state, void *param, void *work) {
    BattleChampionshipFlow *wk;

    GFL_HeapCreateChild(HEAPID_USER, HEAPID_BATTLE_CHAMPIONSHIP_FLOW, 0x30200);
    wk = GFL_ProcInitSubsystem(proc, sizeof(BattleChampionshipFlow), HEAPID_BATTLE_CHAMPIONSHIP_FLOW);
    MI_CpuFill8(wk, 0, sizeof(BattleChampionshipFlow));
    wk->param = param;
    wk->heapId = HEAPID_BATTLE_CHAMPIONSHIP_FLOW;
    GFL_OvlLoad(OVERLAY_ID(67));
    GFL_OvlLoad(OVERLAY_ID(177));
    func_ov176_0217aef4(wk);
    func_ov176_0217af2c(wk);
    func_ov176_0217af50(wk);
    wk->task = func_ov177_021e8a3c(wk, func_ov176_0217b014, HEAPID_BATTLE_CHAMPIONSHIP_FLOW);
    return TRUE;
}

BOOL func_ov176_0217ae88(GameProc *proc, u32 *state, void *param, void *work) {
    BattleChampionshipFlow *wk = work;

    func_ov177_021e8a78(wk->task);
    func_ov176_0217af9c(wk);
    func_ov176_0217af44(wk);
    func_ov176_0217af04(wk);
    GFL_OvlUnload(OVERLAY_ID(177));
    GFL_OvlUnload(OVERLAY_ID(67));
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(HEAPID_BATTLE_CHAMPIONSHIP_FLOW);
    return TRUE;
}

static BOOL func_ov176_0217aecc(GameProc *proc, u32 *state, void *param, void *work) {
    BattleChampionshipFlow *wk = work;

    func_ov176_0217af14(wk);
    func_ov176_0217afbc(wk);
    func_ov177_021e8a80(wk->task);
    if (func_ov177_021e8a90(wk->task)) {
        return TRUE;
    }
    return FALSE;
}

void func_ov176_0217aef4(BattleChampionshipFlow *wk) {
    wk->net = func_ov177_021e5440(TRUE, wk->heapId);
}

void func_ov176_0217af04(BattleChampionshipFlow *wk) {
    func_ov177_021e54e0(wk->net);
    wk->net = NULL;
}

static void func_ov176_0217af14(BattleChampionshipFlow *wk) {
    if (wk->net != NULL) {
        func_ov177_021e5534(wk->net);
        func_ov177_021e5980(wk->seq);
    }
}

void func_ov176_0217af2c(BattleChampionshipFlow *wk) {
    wk->seq = func_ov177_021e567c(func_ov177_021e5548(wk->net), 1, wk->heapId);
}





// pokebw2's BattleChampionshipFlow_DeleteSeq
void func_ov176_0217af44(BattleChampionshipFlow *wk) {
    func_ov177_021e5868(wk->seq);
}

// pokebw2's BattleChampionshipFlow_CreateUI
void func_ov176_0217af50(BattleChampionshipFlow *wk) {
    wk->font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, wk->heapId);
    wk->msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_ENTREE_FOREST, wk->heapId);
    wk->printQueue = func_0201c440(wk->heapId);
    wk->msgBox = func_ov177_021e8568(0, 15, 14, 10, wk->printQueue, wk->font, wk->heapId);
}

// pokebw2's BattleChampionshipFlow_DeleteUI
void func_ov176_0217af9c(BattleChampionshipFlow *wk) {
    func_ov177_021e8628(wk->msgBox);
    func_0201c4c0(wk->printQueue);
    GFL_MsgDataFree(wk->msgData);
    GFL_FontFree(wk->font);
}

void func_ov176_0217afbc(BattleChampionshipFlow *wk) {
    func_ov177_021e867c(wk->msgBox);
    func_0201c4e4(wk->printQueue);
}


// pokebw2's BattleChampionshipFlow_TaskDispatch
void func_ov176_0217afd0(WifiBattleMatchTask *task, u32 *seq, void *work) {
    BattleChampionshipFlow *wk = work;

    switch (wk->param->mode) {
    case BATTLE_CHAMPIONSHIP_FLOW_MODE_MENU:
    case BATTLE_CHAMPIONSHIP_FLOW_MODE_REWARD:
    case BATTLE_CHAMPIONSHIP_FLOW_MODE_MENU_2:
        func_ov177_021e8aa0(task, func_ov176_0217b0c8);
        break;
    case BATTLE_CHAMPIONSHIP_FLOW_MODE_LIVE_JOIN:
    case BATTLE_CHAMPIONSHIP_FLOW_MODE_LIVE_AFTER_MATCH:
    case BATTLE_CHAMPIONSHIP_FLOW_MODE_LIVE_QUIT:
    case BATTLE_CHAMPIONSHIP_FLOW_MODE_LIVE_MENU:
        func_ov177_021e8aa0(task, func_ov176_0217b1bc);
        break;
    }
}

static void func_ov176_0217b00c(WifiBattleMatchTask *task, u32 *seq, void *work) {
    func_ov177_021e8aa8(task);
}


// pokebw2's BattleChampionshipFlow_TaskFadeIn
void func_ov176_0217b014(WifiBattleMatchTask *task, u32 *seq, void *work) {
    switch (*seq) {
    case 0:
        GFL_FadeSet(FADE_ENGINE_A_BLACK | FADE_ENGINE_B_BLACK, 16, 0, 0);
        *seq = 1;
        break;
    case 1:
        if (!GFL_FadeIsRunning()) {
            *seq = 2;
        }
        break;
    case 2:
        func_ov177_021e8aa0(task, func_ov176_0217afd0);
        break;
    default:
        GFL_ASSERT(0);
        break;
    }
}

// pokebw2's BattleChampionshipFlow_TaskFadeOut
void func_ov176_0217b064(WifiBattleMatchTask *task, u32 *seq, void *work) {
    BattleChampionshipFlow *wk = work;
    u32 mode;

    switch (*seq) {
    case 0:
        if (wk->param->result == BATTLE_CHAMPIONSHIP_FLOW_RESULT_EXIT) {
            GFL_SndBGMFadeOut(6);
            mode = FADE_ENGINE_A_WHITE | FADE_ENGINE_B_WHITE;
        } else {
            mode = FADE_ENGINE_A_BLACK | FADE_ENGINE_B_BLACK;
        }
        GFL_FadeSet(mode, 0, 16, 0);
        *seq = 1;
        break;
    case 1:
        if (!GFL_FadeIsRunning()) {
            *seq = 2;
        }
        break;
    case 2:
        func_ov177_021e8aa0(task, func_ov176_0217b00c);
        break;
    default:
        GFL_ASSERT(0);
        break;
    }
}

// pokebw2's BattleChampionshipFlow_TaskMenu
void func_ov176_0217b0c8(WifiBattleMatchTask *task, u32 *seq, void *work) {
    BattleChampionshipFlow *wk = work;
    BattleChampionshipMenuParam param;

    switch (*seq) {
    case 0:
        MI_CpuFill8(&param, 0, sizeof(BattleChampionshipMenuParam));
        param.msgBox = wk->msgBox;
        param.font = wk->font;
        param.msgData = wk->msgData;
        param.printQueue = wk->printQueue;
        param.seq = wk->seq;
        param.net = wk->net;
        param.gameData = wk->param->gameData;
        if (wk->backToMenu != 0) {
            wk->backToMenu = 0;
            param.mode = MENU_MODE_LIVE;
        } else {
            switch (wk->param->mode) {
            case BATTLE_CHAMPIONSHIP_FLOW_MODE_MENU:
                param.mode = MENU_MODE_TOP;
                break;
            case BATTLE_CHAMPIONSHIP_FLOW_MODE_REWARD:
                param.mode = MENU_MODE_REWARD;
                break;
            }
        }
        wk->flow = func_ov176_0217b2e4(&param, HEAPID_BATTLE_CHAMPIONSHIP_FLOW);
        (*seq)++;
        break;
    case 1:
        func_ov176_0217b35c(wk->flow);
        switch (func_ov176_0217b368(wk->flow)) {
        case MENU_RESULT_EXIT:
            wk->param->result = BATTLE_CHAMPIONSHIP_FLOW_RESULT_EXIT;
            *seq = 2;
            break;
        case MENU_RESULT_WIFI:
            wk->param->result = BATTLE_CHAMPIONSHIP_FLOW_RESULT_WIFI;
            *seq = 2;
            break;
        case MENU_RESULT_LIVE:
            wk->param->mode = BATTLE_CHAMPIONSHIP_FLOW_MODE_LIVE_JOIN;
            *seq = 3;
            break;
        }
        break;
    case 2:
        func_ov176_0217b344(wk->flow);
        func_ov177_021e8aa0(task, func_ov176_0217b064);
        break;
    case 3:
        func_ov176_0217b344(wk->flow);
        func_ov177_021e8aa0(task, func_ov176_0217b1bc);
        break;
    }
}

// pokebw2's BattleChampionshipFlow_TaskLive
void func_ov176_0217b1bc(WifiBattleMatchTask *task, u32 *seq, void *work) {
    BattleChampionshipFlow *wk = work;
    LiveBattleMatchFlowParam param;

    switch (*seq) {
    case 0:
        MI_CpuFill8(&param, 0, sizeof(LiveBattleMatchFlowParam));
        param.msgBox = wk->msgBox;
        param.font = wk->font;
        param.printQueue = wk->printQueue;
        param.seq = wk->seq;
        param.net = wk->net;
        param.gameData = wk->param->gameData;
        param.playerData = wk->param->playerData;
        param.partnerData = wk->param->partnerData;
        param.matchResult = wk->param->matchResult;
        switch (wk->param->mode) {
        case BATTLE_CHAMPIONSHIP_FLOW_MODE_LIVE_JOIN:
            param.mode = 0;
            break;
        case BATTLE_CHAMPIONSHIP_FLOW_MODE_LIVE_AFTER_MATCH:
            param.mode = 2;
            break;
        case BATTLE_CHAMPIONSHIP_FLOW_MODE_LIVE_QUIT:
            param.mode = 3;
            break;
        case BATTLE_CHAMPIONSHIP_FLOW_MODE_LIVE_MENU:
            param.mode = 1;
            break;
        default:
            param.mode = 0;
            break;
        }
        wk->flow = func_ov176_0217b984(&param, HEAPID_BATTLE_CHAMPIONSHIP_FLOW);
        (*seq)++;
        break;
    case 1:
        func_ov176_0217baf4(wk->flow);
        switch (func_ov176_0217bb3c(wk->flow)) {
        case LIVE_RESULT_MENU:
            *seq = 3;
            break;
        case 2:
            wk->param->result = 2;
            *seq = 2;
            break;
        case 3:
            wk->param->result = 3;
            *seq = 2;
            break;
        case 4:
            wk->param->result = 4;
            *seq = 2;
            break;
        }
        break;
    case 2:
        func_ov176_0217ba9c(wk->flow);
        func_ov177_021e8aa0(task, func_ov176_0217b064);
        break;
    case 3:
        wk->backToMenu = 1;
        func_ov176_0217ba9c(wk->flow);
        func_ov177_021e8aa0(task, func_ov176_0217b0c8);
        break;
    }
}

BattleChampionshipMenu *func_ov176_0217b2e4(const BattleChampionshipMenuParam *param, HeapID heapId) {
    BattleChampionshipMenu *menu = GFL_HeapAllocate(heapId, sizeof(BattleChampionshipMenu), FALSE, "battle_championship_flow.c", 175);

    MI_CpuFill8(menu, 0, sizeof(BattleChampionshipMenu));
    menu->param = *param;
    menu->heapId = heapId;
    menu->scriptMsgData = GFL_MsgSysLoadData(FALSE, ARCID_SCRIPT_MESSAGE, SCRIPT_TEXT_GLOBAL_10520, heapId);
    menu->task = func_ov177_021e8a3c(menu, func_ov176_0217b380, heapId);
    return menu;
}


void func_ov176_0217b344(BattleChampionshipMenu *menu) {
    func_ov177_021e8a78(menu->task);
    GFL_MsgDataFree(menu->scriptMsgData);
    GFL_HeapFree(menu);
}


// pokebw2's BattleChampionshipFlow_MenuUpdate
void func_ov176_0217b35c(BattleChampionshipMenu *menu) {
    func_ov177_021e8a80(menu->task);
}

// pokebw2's BattleChampionshipFlow_MenuGetResult
u32 func_ov176_0217b368(BattleChampionshipMenu *menu) {
    if (func_ov177_021e8a90(menu->task)) {
        return menu->result;
    }
    return MENU_RESULT_NONE;
}

// pokebw2's BattleChampionshipFlow_MenuTaskStart
void func_ov176_0217b380(WifiBattleMatchTask *task, u32 *seq, void *work) {
    BattleChampionshipMenu *menu = work;

    switch (menu->param.mode) {
    case MENU_MODE_TOP:
        func_ov177_021e8aa0(task, func_ov176_0217b3c0);
        break;
    case MENU_MODE_REWARD:
        func_ov177_021e8aa0(task, func_ov176_0217b4a0);
        break;
    case MENU_MODE_LIVE:
        func_ov177_021e8aa0(task, func_ov176_0217b6ac);
        break;
    }
}

static void func_ov176_0217b3b8(WifiBattleMatchTask *task, u32 *seq, void *work) {
    func_ov177_021e8aa8(task);
}


// pokebw2's BattleChampionshipFlow_MenuTaskTop
void func_ov176_0217b3c0(WifiBattleMatchTask *task, u32 *seq, void *work) {
    BattleChampionshipMenu *menu = work;
    s32 choice;

    switch (*seq) {
    case 0:
        func_ov176_0217b93c(menu, EntreeForest_Text_000);
        *seq = 1;
        break;
    case 1:
        if (func_ov176_0217b964(menu) == TRUE) {
            *seq = 2;
        }
        break;
    case 2:
        func_ov176_0217b7d8(menu, 0);
        *seq = 3;
        break;
    case 3:
        choice = func_ov176_0217b930(menu);
        if (choice != -1) {
            switch (choice) {
            case 0:
                func_ov177_021e8aa0(task, func_ov176_0217b4a0);
                break;
            case 1:
                func_ov177_021e8aa0(task, func_ov176_0217b6ac);
                break;
            case 2:
                func_ov177_021e8aa0(task, func_ov176_0217b468);
                break;
            case 3:
                func_ov176_0217b970(menu, 1);
                break;
            }
            func_ov176_0217b924(menu);
        }
        break;
    }
}

// pokebw2's BattleChampionshipFlow_MenuTaskIntro
void func_ov176_0217b468(WifiBattleMatchTask *task, u32 *seq, void *work) {
    BattleChampionshipMenu *menu = work;

    switch (*seq) {
    case 0:
        func_ov176_0217b93c(menu, EntreeForest_Text_001);
        *seq = 1;
        break;
    case 1:
        if (func_ov176_0217b964(menu) == TRUE) {
            func_ov177_021e8aa0(task, func_ov176_0217b3c0);
        }
        break;
    }
}

// pokebw2's BattleChampionshipFlow_MenuTaskWifi
// pokebw2's BattleChampionshipFlow_MenuTaskReward
void func_ov176_0217b4a0(WifiBattleMatchTask *task, u32 *seq, void *work) {
    BattleChampionshipMenu *menu = work;
    s32 choice;

    switch (*seq) {
    case 0:
        func_ov176_0217b93c(menu, EntreeForest_Text_009);
        *seq = 1;
        break;
    case 1:
        if (func_ov176_0217b964(menu) == TRUE) {
            *seq = 2;
        }
        break;
    case 2:
        func_ov176_0217b7d8(menu, 1);
        *seq = 3;
        break;
    case 3:
        choice = func_ov176_0217b930(menu);
        if (choice != -1) {
            switch (choice) {
            case 0:
                if (!func_0202eb44()) {
                    func_ov177_021e8aa0(task, func_ov176_0217b7a0);
                } else {
                    clearVSPlayer();
                    func_ov176_0217b970(menu, 2);
                }
                break;
            case 1:
                func_ov177_021e8aa0(task, func_ov176_0217b55c);
                break;
            case 2:
                func_ov177_021e8aa0(task, func_ov176_0217b594);
                break;
            case 3:
                func_ov177_021e8aa0(task, func_ov176_0217b3c0);
                break;
            }
            func_ov176_0217b924(menu);
        }
        break;
    }
}

// pokebw2's BattleChampionshipFlow_MenuTaskWifiInfo
void func_ov176_0217b55c(WifiBattleMatchTask *task, u32 *seq, void *work) {
    BattleChampionshipMenu *menu = work;

    switch (*seq) {
    case 0:
        func_ov176_0217b93c(menu, EntreeForest_Text_008);
        *seq = 1;
        break;
    case 1:
        if (func_ov176_0217b964(menu) == TRUE) {
            func_ov177_021e8aa0(task, func_ov176_0217b4a0);
        }
        break;
    }
}

// pokebw2's BattleChampionshipFlow_MenuTaskWifiCard
void func_ov176_0217b594(WifiBattleMatchTask *task, u32 *seq, void *work) {
    BattleChampionshipMenu *menu = work;
    DigitalCardCheckParam param;

    switch (*seq) {
    case 0:
        MI_CpuFill8(&param, 0, sizeof(DigitalCardCheckParam));
        param.mode = DIGITAL_CARD_CHECK_WIFI;
        param.msgBox = menu->param.msgBox;
        param.font = menu->param.font;
        param.msgData = menu->param.msgData;
        param.printQueue = menu->param.printQueue;
        param.seq = menu->param.seq;
        param.net = menu->param.net;
        param.gameData = menu->param.gameData;
        menu->cardCheck = func_ov176_0217d398(&param, menu->heapId);
        (*seq)++;
        break;
    case 1:
        func_ov176_0217d434(menu->cardCheck);
        if (func_ov176_0217d44c(menu->cardCheck)) {
            (*seq)++;
        }
        break;
    case 2:
        func_ov176_0217d410(menu->cardCheck);
        func_ov177_021e8aa0(task, func_ov176_0217b4a0);
        break;
    }
}

// pokebw2's BattleChampionshipFlow_MenuTaskLiveCard
void func_ov176_0217b620(WifiBattleMatchTask *task, u32 *seq, void *work) {
void func_ov176_0217b55c(WifiBattleMatchTask *task, u32 *seq, void *work);
void func_ov176_0217b594(WifiBattleMatchTask *task, u32 *seq, void *work);
    BattleChampionshipMenu *menu = work;
    DigitalCardCheckParam param;

    switch (*seq) {
    case 0:
        MI_CpuFill8(&param, 0, sizeof(DigitalCardCheckParam));
        param.mode = DIGITAL_CARD_CHECK_LIVE;
        param.msgBox = menu->param.msgBox;
        param.font = menu->param.font;
        param.msgData = menu->param.msgData;
        param.printQueue = menu->param.printQueue;
        param.seq = menu->param.seq;
        param.net = menu->param.net;
        param.gameData = menu->param.gameData;
        menu->cardCheck = func_ov176_0217d398(&param, menu->heapId);
        (*seq)++;
        break;
    case 1:
        func_ov176_0217d434(menu->cardCheck);
        if (func_ov176_0217d44c(menu->cardCheck)) {
            (*seq)++;
        }
        break;
    case 2:
        func_ov176_0217d410(menu->cardCheck);
        func_ov177_021e8aa0(task, func_ov176_0217b6ac);
        break;
    }
}

// pokebw2's BattleChampionshipFlow_MenuTaskLive
void func_ov176_0217b6ac(WifiBattleMatchTask *task, u32 *seq, void *work) {
    BattleChampionshipMenu *menu = work;
    s32 choice;

    switch (*seq) {
    case 0:
        func_ov176_0217b93c(menu, EntreeForest_Text_014);
        *seq = 1;
        break;
    case 1:
        if (func_ov176_0217b964(menu) == TRUE) {
            *seq = 2;
        }
        break;
    case 2:
        func_ov176_0217b7d8(menu, 2);
        *seq = 3;
        break;
    case 3:
        choice = func_ov176_0217b930(menu);
        if (choice != -1) {
            switch (choice) {
            case 0:
                if (!func_0202eb44()) {
                    func_ov177_021e8aa0(task, func_ov176_0217b7a0);
                } else {
                    clearVSPlayer();
                    func_ov176_0217b970(menu, MENU_RESULT_LIVE);
                }
                break;
            case 1:
                func_ov177_021e8aa0(task, func_ov176_0217b768);
                break;
            case 2:
                func_ov177_021e8aa0(task, func_ov176_0217b620);
                break;
            case 3:
                func_ov177_021e8aa0(task, func_ov176_0217b3c0);
                break;
            }
            func_ov176_0217b924(menu);
        }
        break;
    }
}

// pokebw2's BattleChampionshipFlow_MenuTaskLiveInfo
void func_ov176_0217b768(WifiBattleMatchTask *task, u32 *seq, void *work) {
    BattleChampionshipMenu *menu = work;

    switch (*seq) {
    case 0:
        func_ov176_0217b93c(menu, EntreeForest_Text_015);
        *seq = 1;
        break;
    case 1:
        if (func_ov176_0217b964(menu) == TRUE) {
            func_ov177_021e8aa0(task, func_ov176_0217b6ac);
        }
        break;
    }
}

// pokebw2's BattleChampionshipFlow_MenuTaskNoWireless
void func_ov176_0217b7a0(WifiBattleMatchTask *task, u32 *seq, void *work) {
    BattleChampionshipMenu *menu = work;

    switch (*seq) {
    case 0:
        func_ov176_0217b950(menu, Global10520_Text_022);
        *seq = 1;
        break;
    case 1:
        if (func_ov176_0217b964(menu) == TRUE) {
            func_ov177_021e8aa0(task, func_ov176_0217b3c0);
        }
        break;
    }
}

void func_ov176_0217b7d8(BattleChampionshipMenu *menu, u32 kind) {
    u32 layout;
    void *tournament = getTournamentBlockAddress(GameData_GetSaveControl(menu->param.gameData));
    WifiBattleMatchMenuParam param;
    void *entry;
    u8 x, y, width, height;

    MI_CpuFill8(&param, 0, sizeof(WifiBattleMatchMenuParam));
    param.msgData = menu->param.msgData;
    param.font = menu->param.font;
    param.printQueue = menu->param.printQueue;
    switch (kind) {
    case LIST_TOP:
        param.msgIds[0] = 2;
        param.msgIds[1] = 3;
        param.msgIds[2] = 5;
        param.msgIds[3] = 6;
        param.count = 4;
        param.canCancel = 1;
        param.cancelValue = 3;
        layout = 0;
        break;
    case LIST_WIFI:
        entry = func_0200b1d4(tournament, 0);
        param.msgIds[0] = 7;
        param.msgIds[1] = 5;
        if (((TournamentEntry *)entry)->unk15B == 1 || ((TournamentEntry *)entry)->unk15B == 2) {
            param.msgIds[2] = 13;
        } else {
            param.msgIds[2] = 4;
        }
        param.msgIds[3] = 6;
        param.count = 4;
        param.canCancel = 1;
        param.cancelValue = 3;
        layout = 0;
        break;
    case LIST_LIVE_MENU:
        entry = func_0200b1d4(tournament, 1);
        param.msgIds[0] = 7;
        param.msgIds[1] = 5;
        if (((TournamentEntry *)entry)->unk15B == 1 || ((TournamentEntry *)entry)->unk15B == 2) {
            param.msgIds[2] = 13;
        } else {
            param.msgIds[2] = 4;
        }
        param.msgIds[3] = 6;
        param.count = 4;
        param.canCancel = 1;
        param.cancelValue = 3;
        layout = 0;
        break;
    case LIST_LIVE:
        param.msgIds[0] = 11;
        param.msgIds[1] = 12;
        param.count = 2;
        param.canCancel = 1;
        param.cancelValue = 1;
        layout = 1;
        break;
    }
    param.bg = 0;
    param.palette = 15;
    param.framePalette = 13;
    param.frameChar = 1;
    switch (layout) {
    case 0:
        width = 28;
        height = param.count * 2;
        x = 2;
        y = 9 - height / 2;
        break;
    case 1:
        width = 12;
        height = param.count * 2;
        x = 19;
        y = 17 - height;
        break;
    }
    menu->list = func_ov177_021e88b0(&param, x, y, width, height, menu->heapId);
    menu->listKind = kind;
}


void func_ov176_0217b924(BattleChampionshipMenu *menu) {
    func_ov177_021e89d0(menu->list);
}

s32 func_ov176_0217b930(BattleChampionshipMenu *menu) {
    return func_ov177_021e8a00(menu->list);
}



static inline s32 BattleChampionshipFlow_MenuGetListChoice(BattleChampionshipMenu *menu) {
    return func_ov177_021e8a00(menu->list);
}

// pokebw2's BattleChampionshipFlow_MenuPrint
void func_ov176_0217b93c(BattleChampionshipMenu *menu, u32 msgId) {
    func_ov177_021e8738(menu->param.msgBox, menu->param.msgData, msgId, 1);
}

// pokebw2's BattleChampionshipFlow_MenuPrintScript
void func_ov176_0217b950(BattleChampionshipMenu *menu, u32 msgId) {
    func_ov177_021e8738(menu->param.msgBox, menu->scriptMsgData, msgId, 1);
}

BOOL func_ov176_0217b964(BattleChampionshipMenu *menu) {
    return func_ov177_021e8844(menu->param.msgBox);
}


// pokebw2's BattleChampionshipFlow_MenuFinish
void func_ov176_0217b970(BattleChampionshipMenu *menu, u32 result) {
    menu->result = result;
    func_ov177_021e8aa0(menu->task, func_ov176_0217b3b8);
}
