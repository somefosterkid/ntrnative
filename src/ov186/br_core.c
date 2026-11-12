// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov271/br_core.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// The Battle Recorder's core proc, which sets up the systems its screens share and runs the screens. The name is a
// guess: the ROM gives none, and br_main.c is overlay 272's proc, which runs this one

#include "types.h"
#include "app/battle_recorder/br_core.h"
#include "app/battle_recorder/br_util.h"
#include "field/field_sound.h"
#include "gfl/fade.h"
#include "gfl/net.h"
#include "gfl/overlay.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "save/battle_rec.h"
#include "save/musical_savedata.h"
#include "save/records.h"
#include "save/save_control.h"
#include "save/trainer_card.h"
#include "system/game_data.h"
#include "app/battle_recorder/br_fade.h"
#include "app/battle_recorder/br_graphic.h"
#include "app/battle_recorder/br_musical_look_proc.h"
#include "app/battle_recorder/br_musical_send_proc.h"
#include "app/battle_recorder/br_sidebar.h"
#include "save/bsubway_save.h"

extern const GameProcFunctions data_ov183_021e3268;
extern const GameProcFunctions data_ov183_021e3274;
extern const GameProcFunctions data_ov183_021e329c;
extern const GameProcFunctions data_ov183_021e33b4;
extern const GameProcFunctions data_ov183_021e3510;
extern const GameProcFunctions data_ov183_021e36f4;
extern const GameProcFunctions data_ov183_021e3838;
extern const GameProcFunctions data_ov183_021e3844;
extern const GameProcFunctions data_ov183_021e38e8;
extern const GameProcFunctions data_ov183_021e38f4;
extern const GameProcFunctions data_ov183_021e3938;
extern const GameProcFunctions data_ov185_021f9088;
extern const GameProcFunctions data_ov184_021f8a04;
extern u32 OVERLAY_201_ID[];


struct BrBvRankProcParam {
    u32 mode;
    BOOL isReturn;
    BrFade *fade;
    ClActUnit *unit;
    BrRes *res;
    BrProcSys *procSys;
    BrNet *net;
    u8 *video;
    u32 searchUnk20;
    u32 searchUnk24;
    u8 search[4];
};

typedef struct {
    BrGraphic *graphic;
    BrRes *res;
    BrProcSys *procSys;
    BrFade *fade;
    BrSidebar *sidebar;
    BrNet *net;
    BrCoreParam *param;
} BrCoreWork;
BrRes *func_ov186_021fb884(u8 color, BOOL isBrowse, HeapID heapId);

BOOL func_ov186_021f90c0(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov186_021f9244(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov186_021f92f4(GameProc *proc, u32 *state, void *param, void *work);
void func_ov186_021f9390(void *param, void *work, const void *preParam, u32 preID);
void func_ov186_021f93d0(void *param, void *work);
void func_ov186_021f93d4(void *param, void *work, const void *preParam, u32 preID);
void func_ov186_021f94d0(void *param, void *work);
void func_ov186_021f94e4(void *param, void *work, const void *preParam, u32 preID);
void func_ov186_021f961c(void *param, void *work);
void func_ov186_021f962c(void *param, void *work, const void *preParam, u32 preID);
void func_ov186_021f9668(void *param, void *work);
void func_ov186_021f966c(void *param, void *work, const void *preParam, u32 preID);
void func_ov186_021f96a8(void *param, void *work);
void func_ov186_021f96ac(void *param, void *work, const void *preParam, u32 preID);
void func_ov186_021f9728(void *param, void *work);
void func_ov186_021f972c(void *param, void *work, const void *preParam, u32 preID);
void func_ov186_021f9750(void *param, void *work);
void func_ov186_021f9754(void *param, void *work, const void *preParam, u32 preID);
void func_ov186_021f9770(void *param, void *work);
void func_ov186_021f9774(void *param, void *work, const void *preParam, u32 preID);
void func_ov186_021f979c(void *param, void *work);
void func_ov186_021f97a0(void *param, void *work, const void *preParam, u32 preID);
void func_ov186_021f97ec(void *param, void *work);
void func_ov186_021f9810(void *param, void *work, const void *preParam, u32 preID);
void func_ov186_021f9868(void *param, void *work);
void func_ov186_021f98a8(void *param, void *work, const void *preParam, u32 preID);
void func_ov186_021f98cc(void *param, void *work);
void func_ov186_021f98d0(void *param, void *work, const void *preParam, u32 preID);
void func_ov186_021f98f4(void *param, void *work);
static void func_ov186_021f98f8(BrRecordInfo *info, BOOL keepLoaded, GameData *gameData, HeapID heapId);

// pokebw2's BR_CORE_PROC_FUNCTIONS
const GameProcFunctions data_ov186_02200248 = {
    func_ov186_021f90c0,
    func_ov186_021f92f4,
    func_ov186_021f9244,
};

// pokebw2's sBrProcTable
static const BrProcData data_ov186_02200254[BR_PROCID_MAX] = {
    [BR_PROCID_START] = { &data_ov183_021e3268, sizeof(BrStartProcParam), OVERLAY_ID(183), func_ov186_021f9390,
                          func_ov186_021f93d0 },
    [BR_PROCID_MENU] = { &data_ov183_021e3274, sizeof(BrMenuProcParam), OVERLAY_ID(183), func_ov186_021f93d4,
                         func_ov186_021f94d0 },
    [BR_PROCID_RECORD] = { &data_ov183_021e329c, 0x40, OVERLAY_ID(183), func_ov186_021f94e4,
                           func_ov186_021f961c },
    [BR_PROCID_BTLSUBWAY] = { &data_ov183_021e33b4, sizeof(BrBtlSubwayProcParam), OVERLAY_ID(183),
                              func_ov186_021f962c, func_ov186_021f9668 },
    [BR_PROCID_RNDMATCH] = { &data_ov183_021e3510, sizeof(BrRndMatchProcParam), OVERLAY_ID(183), func_ov186_021f966c,
                             func_ov186_021f96a8 },
    [BR_PROCID_BV_RANK] = { &data_ov183_021e36f4, sizeof(BrBvRankProcParam), OVERLAY_ID(183), func_ov186_021f96ac,
                            func_ov186_021f9728 },
    [BR_PROCID_BV_SEARCH] = { &data_ov183_021e3838, sizeof(BrBvSearchProcParam), OVERLAY_ID(183), func_ov186_021f972c,
                              func_ov186_021f9750 },
    [BR_PROCID_CODEIN] = { &data_ov183_021e3844, sizeof(BrCodeInProcParam), OVERLAY_ID(183), func_ov186_021f9754,
                           func_ov186_021f9770 },
    [BR_PROCID_BV_SEND] = { &data_ov183_021e38e8, sizeof(BrBvSendProcParam), OVERLAY_ID(183), func_ov186_021f9774,
                            func_ov186_021f979c },
    [BR_PROCID_BV_DELETE] = { &data_ov183_021e38f4, sizeof(BrBvDeleteProcParam), OVERLAY_ID(183), func_ov186_021f97a0,
                              func_ov186_021f97ec },
    [BR_PROCID_BV_SAVE] = { &data_ov183_021e3938, sizeof(BrBvSaveProcParam), OVERLAY_ID(183), func_ov186_021f9810,
                            func_ov186_021f9868 },
    [BR_PROCID_MUSICAL_LOOK] = { &data_ov185_021f9088, sizeof(BrMusicalLookProcParam), OVERLAY_ID(185),
                                 func_ov186_021f98a8, func_ov186_021f98cc },
    [BR_PROCID_MUSICAL_SEND] = { &data_ov184_021f8a04, sizeof(BrMusicalSendProcParam), OVERLAY_ID(184),
                                 func_ov186_021f98d0, func_ov186_021f98f4 },
};

// pokebw2's BrCore_ProcExit
extern u32 OVERLAY_125_ID[];

BOOL func_ov186_021f90c0(GameProc *proc, u32 *state, void *param, void *work) {
    BrCoreParam *coreParam = param;
    BrCoreWork *wk;
    BOOL isMusical = TRUE;
    BOOL isBrowse;
    u8 color;

    GFL_HeapCreateChild(HEAPID_USER, HEAPID_BATTLE_RECORDER, 0x90000);
    wk = GFL_ProcInitSubsystem(proc, sizeof(BrCoreWork), HEAPID_BATTLE_RECORDER);
    MI_CpuFill8(wk, 0, sizeof(BrCoreWork));
    wk->param = coreParam;
    coreParam->mainParam->result = 0;
    if (wk->param->mainParam->mode != BR_MODE_GLOBAL_MUSICAL) {
        isMusical = FALSE;
    }

    wk->graphic = func_ov186_021fc3bc(isMusical, 1, HEAPID_BATTLE_RECORDER);
    isBrowse = TRUE;
    color = func_0200c3e0(getTrainerCardDataBlkAddress(wk->param->mainParam->gameData));
    if (wk->param->mainParam->mode != BR_MODE_BROWSE) {
        isBrowse = FALSE;
    }
    wk->res = func_ov186_021fb884(color, isBrowse, HEAPID_BATTLE_RECORDER);
    func_ov186_021fb964(wk->res, 0, HEAPID_BATTLE_RECORDER);
    func_ov186_021fb964(wk->res, 1, HEAPID_BATTLE_RECORDER);
    wk->procSys = func_ov186_021f99cc(BR_PROCID_START, data_ov186_02200254, BR_PROCID_MAX, wk,
                                      &wk->param->data->procRecovery, HEAPID_BATTLE_RECORDER);
    wk->fade = func_ov186_021fe6c0(HEAPID_BATTLE_RECORDER);
    func_ov186_021fe7b4(wk->fade);
    func_ov186_021fe7f4(wk->fade, func_ov186_021fc224(wk->res));
    if (wk->param->mode == BR_CORE_MODE_INIT) {
        func_ov186_021fe7f8(wk->fade, BR_FADE_DISPLAY_BOTH);
    }
    wk->sidebar = func_ov186_021fed50(func_ov186_021fc4dc(wk->graphic), wk->fade, wk->res, HEAPID_BATTLE_RECORDER);
    func_ov186_021f98f8(&wk->param->data->recordInfo, TRUE, wk->param->mainParam->gameData, HEAPID_BATTLE_RECORDER);

    if (wk->param->mainParam->mode != BR_MODE_BROWSE) {
        wk->net = func_ov186_021ff408(wk->param->mainParam->gameData, wk->param->mainParam->loginBuffer, HEAPID_BATTLE_RECORDER);
    } else {
        GFL_OvlLoad(OVERLAY_ID(125));
    }
    if (func_0203cd98()) {
        func_0203d34c(FALSE, HEAPID_BATTLE_RECORDER);
    }

    if (wk->param->mainParam->mode == BR_MODE_BROWSE) {
        if (wk->param->mode != BR_CORE_MODE_RETURN) {
            FieldSnd_SetPlayerVolumeFade(GameData_GetFieldSoundSystem(wk->param->mainParam->gameData), 0x40, 0);
        }
    } else {
        GFL_SndBGMPlay(0x484, 0xffff);
    }

    if (wk->param->mode == BR_CORE_MODE_RETURN) {
        func_ov186_021fee9c(wk->sidebar);
        func_ov186_021fee4c(wk->sidebar);
        func_ov186_021fe840(wk->fade, 3, 0);
    }
    return TRUE;
}

static BOOL func_ov186_021f9244(GameProc *proc, u32 *state, void *param, void *work) {
    BrCoreWork *wk = work;

    if (wk->param->mainParam->mode == BR_MODE_BROWSE) {
        FieldSnd_SetPlayerVolumeFade(GameData_GetFieldSoundSystem(wk->param->mainParam->gameData), 127, 0);
    }

    if (wk->net != NULL) {
        if (func_ov186_021ff8d4(wk->net)) {
            wk->param->mainParam->result = 2;
        }
        func_ov186_021ff4e4(wk->net);
        wk->net = NULL;
    } else {
        GFL_OvlUnload(OVERLAY_ID(125));
    }

    func_ov186_021fedc4(wk->sidebar, wk->res);
    func_ov186_021fe734(wk->fade);
    func_ov186_021f9a6c(wk->procSys);
    if (wk->param->mainParam->mode == BR_MODE_BROWSE) {
        TrainerCardSave *trainerCard = getTrainerCardDataBlkAddress(wk->param->mainParam->gameData);
        func_0200c3ec(trainerCard, func_ov186_021fc238(wk->res));
    }
    func_ov186_021fbda4(wk->res, 0);
    func_ov186_021fbda4(wk->res, 1);
    func_ov186_021fb92c(wk->res);
    func_ov186_021fc474(wk->graphic);

    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(HEAPID_BATTLE_RECORDER);
    return TRUE;
}

// pokebw2's BrCore_ProcMain
static BOOL func_ov186_021f92f4(GameProc *proc, u32 *state, void *param, void *work) {
    BrCoreWork *wk = work;

    switch (*state) {
    case 0:
        *state = 1;
        break;
    case 1:
        GFL_FadeSet(FADE_ENGINE_A_BLACK | FADE_ENGINE_B_BLACK, 16, 0, 0);
        *state = 2;
        break;
    case 2:
        if (!GFL_FadeIsRunning()) {
            *state = 3;
        }
        break;
    case 3:
        func_ov186_021fe768(wk->fade);
        func_ov186_021fee00(wk->sidebar);
        func_ov186_021f9a80(wk->procSys);
        func_ov186_021fc4c8(wk->graphic);
        if (wk->net != NULL) {
            func_ov186_021ff52c(wk->net);
        }
        if (func_ov186_021f9b98(wk->procSys)) {
            *state = 4;
        }
        break;
    case 4:
        GFL_FadeSet(FADE_ENGINE_A_BLACK | FADE_ENGINE_B_BLACK, 0, 16, 0);
        *state = 5;
        break;
    case 5:
        if (!GFL_FadeIsRunning()) {
            *state = 6;
        }
        break;
    case 6:
        return TRUE;
    }
    return FALSE;
}

// pokebw2's BrCore_StartBefore
void func_ov186_021f9390(void *param, void *work, const void *preParam, u32 preID) {
    BrStartProcParam *p = param;
    BrCoreWork *wk = work;

    p->res = wk->res;
    p->fade = wk->fade;
    p->procSys = wk->procSys;
    p->unit = func_ov186_021fc4dc(wk->graphic);
    p->sidebar = wk->sidebar;
    if (preID == BR_PROCID_MENU) {
        p->mode = 1;
    } else if (wk->param->mode == BR_CORE_MODE_INIT) {
        p->mode = 0;
    } else {
        p->mode = 2;
    }
}

// pokebw2's BrCore_StartAfter
void func_ov186_021f93d0(void *param, void *work) {
}

// pokebw2's BrCore_MenuBefore
void func_ov186_021f93d4(void *param, void *work, const void *preParam, u32 preID) {
    BrMenuProcParam *p = param;
    BrCoreWork *wk = work;

    p->fadeType = 2;
    p->result = &wk->param->mainParam->result;
    p->btnRecovery = &wk->param->data->btnRecovery;

    if (wk->param->mode == BR_CORE_MODE_RETURN) {
        switch (wk->param->mainParam->mode) {
        case BR_MODE_BROWSE:
            p->menuID = 1;
            break;
        case BR_MODE_GLOBAL_BV:
            p->menuID = 7;
            break;
        case BR_MODE_GLOBAL_MUSICAL:
            p->menuID = 10;
            break;
        }
    } else {
        switch (preID) {
        case BR_PROCID_RECORD: {
            const BrRecordProcParam *recordParam = preParam;

            switch (recordParam->mode) {
            case BR_RECORD_MODE_MINE:
                p->menuID = 1;
                break;
            case BR_RECORD_MODE_OTHER1:
            case BR_RECORD_MODE_OTHER2:
            case BR_RECORD_MODE_OTHER3:
                p->menuID = 2;
                break;
            case BR_RECORD_MODE_BROWSE:
                GFL_ASSERT(0);
                p->menuID = 7;
                break;
            case BR_RECORD_MODE_CODEIN:
                GFL_ASSERT(0);
                p->menuID = 7;
                break;
            }
            break;
        }
        case BR_PROCID_BV_RANK:
            p->menuID = 7;
            break;
        case BR_PROCID_BV_SEARCH:
        case BR_PROCID_CODEIN:
            p->menuID = 6;
            break;
        case BR_PROCID_MUSICAL_SEND: {
            const BrMusicalSendProcParam *musicalParam = preParam;

            if (!musicalParam->isSend) {
                p->fadeType = 4;
            }
            p->menuID = 10;
            break;
        }
        default:
            switch (wk->param->mainParam->mode) {
            case BR_MODE_BROWSE:
                p->menuID = 0;
                break;
            case BR_MODE_GLOBAL_BV:
                p->menuID = 5;
                break;
            case BR_MODE_GLOBAL_MUSICAL:
                p->menuID = 10;
                break;
            }
        }
    }

    p->res = wk->res;
    p->fade = wk->fade;
    p->procSys = wk->procSys;
    p->unit = func_ov186_021fc4dc(wk->graphic);
    p->recordInfo = &wk->param->data->recordInfo;
}

// pokebw2's BrCore_MenuAfter
void func_ov186_021f94d0(void *param, void *work) {
    BrMenuProcParam *p = param;
    BrBtnRecovery *recovery = p->btnRecovery;
    u16 last = recovery->stack_num - 1;

    if (recovery->stack[last].menuID == 8) {
        recovery->stack_num = recovery->stack_num - 1;
    }
}

// pokebw2's BrCore_RecordBefore
void func_ov186_021f94e4(void *param, void *work, const void *preParam, u32 preID) {
    BrRecordProcParam *p = param;
    BrCoreWork *wk = work;
    u32 result;

    p->res = wk->res;
    p->fade = wk->fade;
    p->procSys = wk->procSys;
    p->unit = func_ov186_021fc4dc(wk->graphic);
    p->net = wk->net;
    p->gameData = wk->param->mainParam->gameData;
    p->video = wk->param->data->unk_1720;
    p->isRecovery = FALSE;
    p->recordInfo = &wk->param->data->recordInfo;
    p->unk3C = &wk->param->data->unk_17bc;

    switch (preID) {
    case BR_PROCID_MENU: {
        const BrMenuProcParam *menuParam = preParam;
        GameData *gameData;

        p->mode = menuParam->nextMode;
        p->outline = NULL;
        wk->param->data->recordMode = menuParam->nextMode;
        gameData = wk->param->mainParam->gameData;
        func_0200b570(GameData_GetSaveControl(gameData), HEAPID_BATTLE_RECORDER_SYS, &result, p->mode);
        if (result == 1) {
            if (p->mode == BR_RECORD_MODE_MINE) {
                func_0200d5d0(getVSPlayerAllocation(), gameData);
            }
        } else {
            GFL_ASSERT(0);
        }
        break;
    }
    case BR_PROCID_BV_RANK:
        p->mode = BR_RECORD_MODE_BROWSE;
        p->outline = (BrOutlineData *)wk->param->data;
        wk->param->data->recordMode = BR_RECORD_MODE_BROWSE;
        break;
    case BR_PROCID_CODEIN: {
        const BrCodeInProcParam *codeInParam = preParam;

        p->mode = BR_RECORD_MODE_CODEIN;
        p->videoNumber = codeInParam->videoNumber;
        p->outline = NULL;
        wk->param->data->recordMode = BR_RECORD_MODE_CODEIN;
        break;
    }
    case BR_PROCID_BV_SAVE: {
        const BrBvSaveProcParam *saveParam = preParam;

        p->mode = wk->param->data->recordMode;
        p->outline = (BrOutlineData *)wk->param->data;
        p->videoNumber = saveParam->videoNumber;
        p->isRecovery = TRUE;
        break;
    }
    case BR_PROCID_START:
        break;
    case BR_PROCID_RECOVERY:
        p->mode = wk->param->data->recordMode;
        p->outline = (BrOutlineData *)wk->param->data;
        p->isRecovery = TRUE;
        break;
    }
}

// pokebw2's BrCore_RecordAfter
void func_ov186_021f961c(void *param, void *work) {
    BrRecordProcParam *p = param;
    BrCoreWork *wk = work;

    if (p->result == 1) {
        wk->param->result = 1;
    }
}

// pokebw2's BrCore_BtlSubwayBefore
void func_ov186_021f962c(void *param, void *work, const void *preParam, u32 preID) {
    BrBtlSubwayProcParam *p = param;
    BrCoreWork *wk = work;
    SaveControl *save = GameData_GetSaveControl(wk->param->mainParam->gameData);

    p->res = wk->res;
    p->fade = wk->fade;
    p->procSys = wk->procSys;
    p->unit = func_ov186_021fc4dc(wk->graphic);
    p->score = SaveControl_GetBlockPtr(save, 0x3a);
    p->gameData = wk->param->mainParam->gameData;
}

// pokebw2's BrCore_BtlSubwayAfter
void func_ov186_021f9668(void *param, void *work) {
}

// pokebw2's BrCore_RndMatchBefore
void func_ov186_021f966c(void *param, void *work, const void *preParam, u32 preID) {
    BrRndMatchProcParam *p = param;
    BrCoreWork *wk = work;
    SaveControl *save = GameData_GetSaveControl(wk->param->mainParam->gameData);

    p->res = wk->res;
    p->fade = wk->fade;
    p->procSys = wk->procSys;
    p->unit = func_ov186_021fc4dc(wk->graphic);
    p->gameData = wk->param->mainParam->gameData;
    p->record = func_0200ea04(getRecordBlkAddress(save));
}

// pokebw2's BrCore_RndMatchAfter
void func_ov186_021f96a8(void *param, void *work) {
}

void func_ov186_021f96ac(void *param, void *work, const void *preParam, u32 preID) {
    BrBvRankProcParam *p = param;
    BrCoreWork *wk = work;

    p->p_outline = (BrOutlineData *)wk->param->data;
    p->listPos = (BrListPos *)wk->param->data->bvRankSearch;
    if (preID == BR_PROCID_MENU) {
        const BrMenuProcParam *menuParam = preParam;

        p->mode = menuParam->nextMode;
        wk->param->data->bvRankMode = menuParam->nextMode;
        MI_CpuFill8(p->listPos, 0, sizeof(BrListPos));
    } else if (preID == BR_PROCID_BV_SEARCH) {
        const BrBvSearchProcParam *searchParam = preParam;

        p->mode = 3;
        p->search = searchParam->search;
        wk->param->data->bvRankMode = 3;
        MI_CpuFill8(p->listPos, 0, sizeof(BrListPos));
    } else if (preID == BR_PROCID_RECORD) {
        p->mode = wk->param->data->bvRankMode;
        p->isReturn = TRUE;
    }
    p->res = wk->res;
    p->fade = wk->fade;
    p->procSys = wk->procSys;
    p->unit = func_ov186_021fc4dc(wk->graphic);
    p->net = wk->net;
}


// pokebw2's BrCore_BvRankAfter
void func_ov186_021f9728(void *param, void *work) {
}

// pokebw2's BrCore_BvSearchBefore
void func_ov186_021f972c(void *param, void *work, const void *preParam, u32 preID) {
    BrBvSearchProcParam *p = param;
    BrCoreWork *wk = work;

    p->res = wk->res;
    p->fade = wk->fade;
    p->procSys = wk->procSys;
    p->unit = func_ov186_021fc4dc(wk->graphic);
    p->gameData = wk->param->mainParam->gameData;
}

// pokebw2's BrCore_BvSearchAfter
void func_ov186_021f9750(void *param, void *work) {
}

// pokebw2's BrCore_CodeInBefore
void func_ov186_021f9754(void *param, void *work, const void *preParam, u32 preID) {
    BrCodeInProcParam *p = param;
    BrCoreWork *wk = work;

    p->res = wk->res;
    p->fade = wk->fade;
    p->procSys = wk->procSys;
    p->unit = func_ov186_021fc4dc(wk->graphic);
}

// pokebw2's BrCore_CodeInAfter
void func_ov186_021f9770(void *param, void *work) {
}

// pokebw2's BrCore_BvSendBefore
void func_ov186_021f9774(void *param, void *work, const void *preParam, u32 preID) {
    BrBvSendProcParam *p = param;
    BrCoreWork *wk = work;

    p->res = wk->res;
    p->fade = wk->fade;
    p->procSys = wk->procSys;
    p->unit = func_ov186_021fc4dc(wk->graphic);
    p->net = wk->net;
    p->gameData = wk->param->mainParam->gameData;
}

// pokebw2's BrCore_BvSendAfter
void func_ov186_021f979c(void *param, void *work) {
}

// pokebw2's BrCore_BvDeleteBefore
void func_ov186_021f97a0(void *param, void *work, const void *preParam, u32 preID) {
    BrBvDeleteProcParam *p = param;
    BrCoreWork *wk = work;

    if (preID == BR_PROCID_MENU) {
        const BrMenuProcParam *menuParam = preParam;

        p->mode = menuParam->nextMode;
        p->res = wk->res;
        p->fade = wk->fade;
        p->procSys = wk->procSys;
        p->unit = func_ov186_021fc4dc(wk->graphic);
        p->gameData = wk->param->mainParam->gameData;
        p->recordInfo = &wk->param->data->recordInfo;
    } else {
        GFL_ASSERT(0);
    }
}

// pokebw2's BrCore_BvDeleteAfter
void func_ov186_021f97ec(void *param, void *work) {
    BrBvDeleteProcParam *p = param;
    BrCoreWork *wk = work;

    if (p->isDelete) {
        func_ov186_021f98f8(&wk->param->data->recordInfo, FALSE, wk->param->mainParam->gameData, HEAPID_BATTLE_RECORDER);
    }
}

// pokebw2's BrCore_BvSaveBefore
void func_ov186_021f9810(void *param, void *work, const void *preParam, u32 preID) {
    BrBvSaveProcParam *p = param;
    BrCoreWork *wk = work;
    const BrRecordProcParam *recordParam = preParam;

    GFL_ASSERT(preID == BR_PROCID_RECORD);
    p->res = wk->res;
    p->fade = wk->fade;
    p->net = wk->net;
    p->procSys = wk->procSys;
    p->unit = func_ov186_021fc4dc(wk->graphic);
    p->videoNumber = recordParam->videoNumber;
    p->gameData = wk->param->mainParam->gameData;
    p->recordInfo = &wk->param->data->recordInfo;
    p->unk24 = recordParam->unk4;
}

// pokebw2's BrCore_BvSaveAfter
void func_ov186_021f9868(void *param, void *work) {
    BrBvSaveProcParam *p = param;
    BrCoreWork *wk = work;
    u32 result;

    if (p->isSave) {
        func_ov186_021f98f8(&wk->param->data->recordInfo, FALSE, wk->param->mainParam->gameData, HEAPID_BATTLE_RECORDER);
        func_0200b570(GameData_GetSaveControl(wk->param->mainParam->gameData), 0x805a,
                      &result, p->saveSlot);
    }
}

// pokebw2's BrCore_MusicalLookBefore
void func_ov186_021f98a8(void *param, void *work, const void *preParam, u32 preID) {
    BrMusicalLookProcParam *p = param;
    BrCoreWork *wk = work;

    p->res = wk->res;
    p->fade = wk->fade;
    p->procSys = wk->procSys;
    p->graphic = wk->graphic;
    p->net = wk->net;
    p->sidebar = wk->sidebar;
    p->gameData = wk->param->mainParam->gameData;
}

// pokebw2's BrCore_MusicalLookAfter
void func_ov186_021f98cc(void *param, void *work) {
}

// pokebw2's BrCore_MusicalSendBefore
void func_ov186_021f98d0(void *param, void *work, const void *preParam, u32 preID) {
    BrMusicalSendProcParam *p = param;
    BrCoreWork *wk = work;

    p->res = wk->res;
    p->fade = wk->fade;
    p->procSys = wk->procSys;
    p->graphic = wk->graphic;
    p->net = wk->net;
    p->sidebar = wk->sidebar;
    p->gameData = wk->param->mainParam->gameData;
}

// pokebw2's BrCore_MusicalSendAfter
void func_ov186_021f98f4(void *param, void *work) {
}

// Loads the names and numbers of the saved videos, unless keepLoaded is set and they are loaded already
// pokebw2's BrCore_LoadRecordInfo
static void func_ov186_021f98f8(BrRecordInfo *info, BOOL keepLoaded, GameData *gameData, HeapID heapId) {
    BOOL load = TRUE;
    SaveControl *save;
    int i;
    u32 result;

    if (keepLoaded && info->isInit) {
        load = FALSE;
    }
    if (load) {
        save = GameData_GetSaveControl(gameData);
        if (info->isInit) {
            for (i = 0; i < BR_RECORD_NUM; i++) {
                if (info->name[i] != NULL) {
                    GFL_StrBufFree(info->name[i]);
                    info->name[i] = NULL;
                }
            }
        }
        MI_CpuFill8(info, 0, sizeof(BrRecordInfo));

        for (i = 0; i < BR_RECORD_NUM; i++) {
            func_0200b570(save, HEAPID_TAIL(heapId), &result, i);
            if (result == 1) {
                GdsProfile *profile = getVSPlayerAllocation();

                if (i == 0) {
                    func_0200d5d0(profile, gameData);
                }
                info->isValid[i] = TRUE;
                info->name[i] = func_0200d6d0(profile, HEAPID_BATTLE_RECORDER_SYS);
                info->sex[i] = func_0200d6ec(profile);
                info->videoNumber[i] = func_0200b98c(func_0200b928(), 4, 0);
            }
        }
        info->hasMusicalShot = func_0200a630(getAddressOfMusicalDataInfo(GameData_GetSaveControl(gameData)));
        info->isInit = TRUE;
    }
}
