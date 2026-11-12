// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov272/br_main.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// The Battle Recorder's proc (br_main.c, the name its allocation passes). It holds the data that outlives the core
// proc and switches between the core proc of overlay 271 and the battle system, which plays a battle video

#include "types.h"
#include "app/battle_recorder.h"
#include "app/battle_recorder/br_core.h"
#include "battle/battle_proc.h"
#include "battle/btl_setup.h"
#include "constants/sound.h"
#include "field/bsubway_scr.h"
#include "field/field_sound.h"
#include "gfl/heap.h"
#include "gfl/overlay.h"
#include "gfl/proc.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "save/save_control.h"
#include "system/game_data.h"

extern const GameProcFunctions data_ov186_02200248;

#define HEAPID_BATTLE_RECORDER_SYS 0x59

// The procs that br_main.c switches between, which index data_ov187_0220b2a0
enum {
    BR_MAIN_PROCID_CORE,
    BR_MAIN_PROCID_BATTLE,
};

// Makes a proc's parameter. preID is the proc that ran before it
typedef void *(*BrMainCreateParamFunc)(HeapID heapId, void *work, u32 preID);
// Reads the results from a proc's parameter once it has ended and frees it
typedef void (*BrMainFreeParamFunc)(void *param, void *work);

typedef struct {
    s32 overlayId;
    const GameProcFunctions *procFuncs;
    BrMainCreateParamFunc createParam;
    BrMainFreeParamFunc freeParam;
} BrMainProcData;

// Runs one proc at a time, the next one being the one that the last freeParam asked for
typedef struct {
    GameProcManager *procMgr;
    u32 seq;
    void *p_proc_param;
    u32 unk0c;
    u16 heapId;
    void *work;
    const BrMainProcData *tbl;
    u32 preProcID;
    u32 nextProcID;
    u32 procID;
} BrMainProcSys;

typedef struct {
    BrMainProcSys procSys;
    BrData data;
    BattleRecorderParam *param;
} BrMainWork;

BOOL func_ov187_0220af40(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov187_0220af94(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov187_0220afd8(GameProc *proc, u32 *state, void *param, void *work);
static void func_ov187_0220b008(BrMainProcSys *p_wk, const BrMainProcData *tbl, void *work, HeapID heapId);
static BOOL func_ov187_0220b02c(BrMainProcSys *p_wk);
static void func_ov187_0220b0cc(BrMainProcSys *p_wk);
static void func_ov187_0220b0f8(BrMainProcSys *p_wk, u32 procID);
static void *func_ov187_0220b0fc(HeapID heapId, void *work, u32 preID);
static void func_ov187_0220b144(void *param, void *work);
static void *func_ov187_0220b164(HeapID heapId, void *work, u32 preID);
static void func_ov187_0220b1d8(void *param, void *work);
static void func_ov187_0220b240(u16 *bgm, u16 *winBgm);

// pokebw2's BR_MAIN_PROC_FUNCTIONS
const GameProcFunctions data_ov187_0220b294 = {
    func_ov187_0220af40,
    func_ov187_0220afd8,
    func_ov187_0220af94,
};

// pokebw2's sBrMainProcTable
static const BrMainProcData data_ov187_0220b2a0[] = {
    [BR_MAIN_PROCID_CORE] = { OVERLAY_ID(186), &data_ov186_02200248, func_ov187_0220b0fc, func_ov187_0220b144 },
    [BR_MAIN_PROCID_BATTLE] = { OVERLAY_ID(93), &data_ov093_021efdac, func_ov187_0220b164,
                                func_ov187_0220b1d8 },
};

// pokebw2's BrMain_ProcInit
BOOL func_ov187_0220af40(GameProc *proc, u32 *state, void *param, void *work) {
    BrMainWork *wk;

    GFL_HeapCreateChild(HEAPID_USER, 0x59, 0x12000);
    wk = GFL_ProcInitSubsystem(proc, sizeof(BrMainWork), 0x59);
    MI_CpuFill8(wk, 0, sizeof(BrMainWork));
    wk->param = param;
    func_0200b400(0x59);
    func_ov187_0220b008(&wk->procSys, data_ov187_0220b2a0, wk, 0x59);
    func_ov187_0220b0f8(&wk->procSys, BR_MAIN_PROCID_CORE);
    return TRUE;
}

// pokebw2's BrMain_ProcExit
static BOOL func_ov187_0220af94(GameProc *proc, u32 *state, void *param, void *work) {
    BrMainWork *wk = work;
    int i;

    if (wk->data.recordInfo.isInit) {
        for (i = 0; i < BR_RECORD_NUM; i++) {
            if (wk->data.recordInfo.name[i] != NULL) {
                GFL_StrBufFree(wk->data.recordInfo.name[i]);
            }
        }
    }
    func_ov187_0220b0cc(&wk->procSys);
    freeVSPlayerBlkClearPtr();
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(HEAPID_BATTLE_RECORDER_SYS);
    return TRUE;
}

// pokebw2's BrMain_ProcMain
static BOOL func_ov187_0220afd8(GameProc *proc, u32 *state, void *param, void *work) {
    BrMainWork *wk = work;

    switch (*state) {
    case 0:
        *state = 1;
        break;
    case 1:
        if (func_ov187_0220b02c(&wk->procSys)) {
            *state = 2;
        }
        break;
    case 2:
        return TRUE;
    }
    return FALSE;
}

// pokebw2's BrMainProcSys_Init
static void func_ov187_0220b008(BrMainProcSys *p_wk, const BrMainProcData *tbl, void *work, HeapID heapId) {
    MI_CpuFill8(p_wk, 0, sizeof(BrMainProcSys));
    p_wk->procMgr = CreateGameProcManager(heapId);
    p_wk->work = work;
    p_wk->tbl = tbl;
    p_wk->heapId = heapId;
}

// pokebw2's BrMainProcSys_Main
static BOOL func_ov187_0220b02c(BrMainProcSys *p_wk) {
    switch (p_wk->seq) {
    case 0:
        p_wk->preProcID = p_wk->procID;
        p_wk->procID = p_wk->nextProcID;
        p_wk->seq = 1;
        break;
    case 1:
        if (p_wk->tbl[p_wk->procID].createParam != NULL) {
            p_wk->p_proc_param = p_wk->tbl[p_wk->procID].createParam(p_wk->heapId, p_wk->work, p_wk->preProcID);
        } else {
            p_wk->p_proc_param = NULL;
        }
        QueueGameProc(p_wk->procMgr, p_wk->tbl[p_wk->procID].overlayId, p_wk->tbl[p_wk->procID].procFuncs,
                      p_wk->p_proc_param);
        p_wk->seq = 2;
        break;
    case 2:
        if (!GFL_ProcMgrUpdate(p_wk->procMgr)) {
            p_wk->seq = 3;
        }
        break;
    case 3:
        if (p_wk->tbl[p_wk->procID].freeParam != NULL) {
            p_wk->tbl[p_wk->procID].freeParam(p_wk->p_proc_param, p_wk->work);
            p_wk->p_proc_param = NULL;
        }
        if (p_wk->procID != p_wk->nextProcID) {
            p_wk->seq = 0;
        } else {
            p_wk->seq = 4;
        }
        break;
    case 4:
        return TRUE;
    }
    return FALSE;
}

// pokebw2's BrMainProcSys_Exit
static void func_ov187_0220b0cc(BrMainProcSys *p_wk) {
    GFL_ASSERT(p_wk->p_proc_param == NULL);
    FreeGameProcManager(p_wk->procMgr);
    MI_CpuFill8(p_wk, 0, sizeof(BrMainProcSys));
}

// pokebw2's BrMainProcSys_SetNext
static void func_ov187_0220b0f8(BrMainProcSys *p_wk, u32 procID) {
    p_wk->nextProcID = procID;
}

// pokebw2's BrMain_CoreCreateParam
static void *func_ov187_0220b0fc(HeapID heapId, void *work, u32 preID) {
    BrMainWork *wk = work;
    BrCoreParam *param = GFL_HeapAllocate(heapId, sizeof(BrCoreParam), FALSE, "br_main.c", 487);

    MI_CpuFill8(param, 0, sizeof(BrCoreParam));
    param->mainParam = wk->param;
    param->data = &wk->data;
    if (preID == BR_MAIN_PROCID_BATTLE) {
        param->mode = BR_CORE_MODE_RETURN;
    } else {
        param->mode = BR_CORE_MODE_INIT;
    }
    return param;
}

// pokebw2's BrMain_CoreFreeParam
static void func_ov187_0220b144(void *param, void *work) {
    BrCoreParam *coreParam = param;

    switch (coreParam->result) {
    case 0:
        break;
    case 1:
        func_ov187_0220b0f8(work, BR_MAIN_PROCID_BATTLE);
        break;
    }
    GFL_HeapFree(coreParam);
}

void func_ov188_021eeea8(BtlSetup *setup, HeapID heapId);

// pokebw2's BrMain_BattleCreateParam
static void *func_ov187_0220b164(HeapID heapId, void *work, u32 preID) {
    BrMainWork *wk = work;
    BtlSetup *setup;

    setup = GFL_HeapAllocate(heapId, sizeof(BtlSetup), FALSE, "br_main.c", 542);
    MI_CpuFill8(setup, 0, sizeof(BtlSetup));
    func_020138c4(setup, wk->param->gameData, heapId);
    func_0200ba58();
    func_ov188_021eeea8(setup, heapId);
    func_0200ba68();
    GFL_SndBGMSetPaused(TRUE);
    GFL_SndBGMPush();
    func_ov187_0220b240(&setup->fieldSituation.bgm, &setup->fieldSituation.unk12);
    GFL_SndBGMPlay(setup->fieldSituation.bgm, 0xffff);
    return setup;
}


// pokebw2's BrMain_BattleFreeParam
static void func_ov187_0220b1d8(void *param, void *work) {
    BtlSetup *setup = param;
    BrMainWork *wk = work;
    FieldSound *fieldSound = GameData_GetFieldSoundSystem(wk->param->gameData);

    GFL_SndBGMPop();
    GFL_SndBGMSetPaused(FALSE);
    GFL_SndBGMFadeIn(30);
    if (wk->param->mode == BR_MODE_BROWSE) {
        FieldSnd_SetPlayerVolumeFade(fieldSound, 64, 0);
    }
    wk->data.unk_17bc = setup->unkC1_1;
    GFL_HeapFree(setup->unk34[0]);
    func_02013930(setup);
    GFL_HeapFree(setup);
    func_ov187_0220b0f8(work, BR_MAIN_PROCID_CORE);
}

// A battle video plays with the music of a link battle: any other battle music becomes the Wi-Fi trainer battle's,
// and any other victory music the second one
// pokebw2's BrMain_CheckBgm
static void func_ov187_0220b240(u16 *bgm, u16 *winBgm) {
    u16 win = *winBgm;

    switch (*bgm) {
    case 0x46b:
    case 0x470:
    case 0x48d:
    case 0x490:
        break;
    case 0x48b:
    case 0x48c:
    case 0x48e:
    case 0x48f:
    default:
        *bgm = 0x490;
        break;
    }
    if (win != 0x47d && win != 0x480) {
        *winBgm = 0x47d;
    }
}
