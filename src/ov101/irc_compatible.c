// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov175/irc_compatible.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/compatible_irc_sys.h"
#include "app/irc_compat_menu.h"
#include "app/irc_compatible.h"
#include "app/irc_rhythm_check.h"
#include "app/irc_sync_check.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "gfl/overlay.h"
#include "gfl/proc.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/mi.h"
#include "nitro/rtc.h"
#include "save/player_info.h"
#include "save/trainer_log.h"
#include "system/game_data.h"
#include "system/game_system.h"

// pokebw2's prototypes of what Black's headers don't declare, under pokebw2's names until tools/port_file.py
// gives them Black's from the relocations
extern const GameProcFunctions data_ov102_021b8ab8;
extern const GameProcFunctions data_ov105_021b8958;
extern const GameProcFunctions data_ov106_021b9490;
extern const GameProcFunctions data_ov104_021b8fa8;
extern const GameProcFunctions data_ov103_021b9304;

// irc_compatible.c, a ROM name: the compatibility checker's proc. It runs one of five screens at a time, each in its
// own process of a manager that it owns, and picks the next one by what the last gave back. The names are descriptive
// guesses

#define HEAP_ID 0x3a
#define HEAP_ID_TAIL HEAPID_TAIL(HEAP_ID)
#define SUB_PROC_COUNT 5

// The steps of the sub process runner
enum {
    SUB_PROC_CREATE_PARAM,
    SUB_PROC_QUEUE,
    SUB_PROC_FREE_PARAM,
    SUB_PROC_DONE,
};

typedef struct {
    GameProcManager *manager;
    int state;
    void *param;
} IrcCompatibleSubProc;

typedef struct IrcCompatibleWork IrcCompatibleWork;
typedef void (*IrcCompatibleStep)(IrcCompatibleWork *wk, u16 *state);

struct IrcCompatibleWork {
    CompatibleIrcSys *sys;
    IrcCompatibleSubProc subProc;
    IrcCompatibleParam *param;
    IrcCompatibleStep step;
    u16 stepState;
    BOOL done;
    int mode;
    int unk24;
    int unk28;
    u8 unk2C;
    u8 unk2D;
    u8 unk2E;
    u8 unk2F;
    int unk30;
    BOOL firstVisit;
    BOOL unk38;
    CompatibleIrcProfile *profile;
};

// Black 2's, not checked against Black
typedef struct {
    u32 overlayId;
    const GameProcFunctions *procFunctions;
    void *(*createParam)(HeapID heapId, IrcCompatibleWork *wk);
    void (*freeParam)(void *param, IrcCompatibleWork *wk);
} IrcCompatibleSubProcDef;

static BOOL func_ov101_021b6938(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov101_021b69f8(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov101_021b69c4(GameProc *proc, u32 *state, void *param, void *work);
static void func_ov101_021b6a3c(IrcCompatibleWork *wk, IrcCompatibleStep step);
static void func_ov101_021b6a44(IrcCompatibleWork *wk);
static void func_ov101_021b6a4c(IrcCompatibleWork *wk, u16 *state);
static void func_ov101_021b6a60(IrcCompatibleWork *wk, u16 *state);
static void func_ov101_021b6a7c(IrcCompatibleWork *wk, u16 *state);
static void func_ov101_021b6ad8(IrcCompatibleWork *wk, u16 *state);
static void func_ov101_021b6ba8(IrcCompatibleWork *wk, u16 *state);
static void func_ov101_021b6be4(IrcCompatibleSubProc *subProc, HeapID heapId);
static BOOL func_ov101_021b6bfc(IrcCompatibleSubProc *subProc);
static void func_ov101_021b6c08(IrcCompatibleSubProc *subProc);
static BOOL func_ov101_021b6c20(IrcCompatibleSubProc *subProc, int index, HeapID heapId, IrcCompatibleWork *wk);
static void *func_ov101_021b6ca0(HeapID heapId, IrcCompatibleWork *wk);
static void func_ov101_021b6cf8(void *param, IrcCompatibleWork *wk);
static void *func_ov101_021b6d04(HeapID heapId, IrcCompatibleWork *wk);
static void func_ov101_021b6d3c(void *param, IrcCompatibleWork *wk);
static void *func_ov101_021b6d5c(HeapID heapId, IrcCompatibleWork *wk);
static void func_ov101_021b6d90(void *param, IrcCompatibleWork *wk);
static void *func_ov101_021b6dac(HeapID heapId, IrcCompatibleWork *wk);
static void func_ov101_021b6e84(void *param, IrcCompatibleWork *wk);
static void *func_ov101_021b6e8c(HeapID heapId, IrcCompatibleWork *wk);
static void func_ov101_021b6ebc(void *param, IrcCompatibleWork *wk);
static int func_ov101_021b6ecc(u32 a, u32 b, u32 c, u32 d, CompatibleIrcProfile *mine,
                                           CompatibleIrcProfile *other, BOOL isNew, u32 date, HeapID heapId);
static u32 func_ov101_021b6f9c(const u16 *name0, const u16 *name1);
static u32 func_ov101_021b7090(u32 value, u32 count);
static u8 func_ov101_021b70a8(const CompatibleIrcProfile *profile, u32 date);

// pokebw2's IRC_COMPATIBLE_PROC_FUNCTIONS
const GameProcFunctions data_ov101_021b74ac = {
    func_ov101_021b6938,
    func_ov101_021b69f8,
    func_ov101_021b69c4,
};

// pokebw2's sIrcCompatibleSubProcs
static const IrcCompatibleSubProcDef data_ov101_021b74b8[SUB_PROC_COUNT] = {
    { OVERLAY_ID(102), &data_ov102_021b8ab8, func_ov101_021b6ca0, func_ov101_021b6cf8 },
    { OVERLAY_ID(103), &data_ov103_021b9304, func_ov101_021b6d04, func_ov101_021b6d3c },
    { OVERLAY_ID(104), &data_ov104_021b8fa8, func_ov101_021b6d5c, func_ov101_021b6d90 },
    { OVERLAY_ID(105), &data_ov105_021b8958, func_ov101_021b6dac, func_ov101_021b6e84 },
    { OVERLAY_ID(106), &data_ov106_021b9490, func_ov101_021b6e8c, func_ov101_021b6ebc },
};

// pokebw2's IrcCompatible_Init
static BOOL func_ov101_021b6938(GameProc *proc, u32 *state, void *param, void *work) {
    IrcCompatibleWork *wk;
    GameData *gameData;

    GFL_HeapCreateChild(HEAPID_USER, HEAP_ID, 0x8000);
    wk = GFL_ProcInitSubsystem(proc, sizeof(IrcCompatibleWork), HEAP_ID);
    gameData = NULL;
    MI_CpuFill8(wk, 0, sizeof(IrcCompatibleWork));
    wk->param = param;
    wk->profile = GFL_HeapAllocate(HEAP_ID, sizeof(CompatibleIrcProfile), FALSE, "irc_compatible.c", 281);
    MI_CpuFill8(wk->profile, 0, sizeof(CompatibleIrcProfile));
    func_ov101_021b6be4(&wk->subProc, HEAP_ID);
    if (wk->param->gsys != NULL) {
        gameData = GSYS_GetGameData(wk->param->gsys);
    }
    wk->sys = func_ov101_021b6100(-1, gameData, HEAP_ID, FALSE);
    wk->firstVisit = TRUE;
    func_ov101_021b6a3c(wk, func_ov101_021b6a4c);
    GFL_SndBGMSetPaused(TRUE);
    GFL_SndBGMPush();
    return TRUE;
}

// pokebw2's IrcCompatible_Exit
static BOOL func_ov101_021b69c4(GameProc *proc, u32 *state, void *param, void *work) {
    IrcCompatibleWork *wk = work;

    GFL_SndBGMPop();
    GFL_SndBGMSetPaused(FALSE);
    func_ov101_021b6148(wk->sys);
    func_ov101_021b6c08(&wk->subProc);
    GFL_HeapFree(wk->profile);
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(HEAP_ID);
    return TRUE;
}

// pokebw2's IrcCompatible_Main
static BOOL func_ov101_021b69f8(GameProc *proc, u32 *state, void *param, void *work) {
    IrcCompatibleWork *wk = work;

    switch (*state) {
    case 0:
        if (!GFL_SndBGMIsFading()) {
            (*state)++;
        }
        break;
    case 1:
        if (!func_ov101_021b6bfc(&wk->subProc)) {
            wk->step(wk, &wk->stepState);
        }
        if (wk->done) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

// pokebw2's IrcCompatible_SetStep
static void func_ov101_021b6a3c(IrcCompatibleWork *wk, IrcCompatibleStep step) {
    wk->step = step;
    wk->stepState = 0;
}

// pokebw2's IrcCompatible_Finish
static void func_ov101_021b6a44(IrcCompatibleWork *wk) {
    wk->done = TRUE;
}

// pokebw2's IrcCompatible_StepStart
static void func_ov101_021b6a4c(IrcCompatibleWork *wk, u16 *state) {
    if (*state == 0) {
        func_ov101_021b6a3c(wk, func_ov101_021b6a7c);
    }
}

// pokebw2's IrcCompatible_StepEnd
static void func_ov101_021b6a60(IrcCompatibleWork *wk, u16 *state) {
    switch (*state) {
    case 0:
        *state = 1;
        break;
    case 1:
        func_ov101_021b6a44(wk);
        break;
    }
}

// pokebw2's IrcCompatible_StepMenu
static void func_ov101_021b6a7c(IrcCompatibleWork *wk, u16 *state) {
    switch (*state) {
    case 0:
        if (func_ov101_021b6c20(&wk->subProc, 0, HEAP_ID, wk)) {
            *state = 1;
        }
        break;
    case 1:
        switch (wk->mode) {
        case 0:
            func_ov101_021b6a3c(wk, func_ov101_021b6ad8);
            break;
        case 1:
            func_ov101_021b6a3c(wk, func_ov101_021b6ba8);
            break;
        case 2:
            func_ov101_021b6a3c(wk, func_ov101_021b6a60);
            break;
        }
        break;
    }
}

// pokebw2's IrcCompatible_StepFlowA
static void func_ov101_021b6ad8(IrcCompatibleWork *wk, u16 *state) {
    switch (*state) {
    case 0:
        wk->unk2E = 0;
        wk->unk2C = 0;
        *state = 1;
        break;
    case 1:
        if (func_ov101_021b6c20(&wk->subProc, 2, HEAP_ID, wk)) {
            *state = 2;
        }
        break;
    case 2:
        if (wk->unk28 == 0) {
            *state = 3;
        } else if (wk->unk28 == 2) {
            wk->unk2E = 0;
            wk->unk2C = 0;
            *state = 5;
        } else {
            *state = 6;
        }
        break;
    case 3:
        if (func_ov101_021b6c20(&wk->subProc, 1, HEAP_ID, wk)) {
            *state = 4;
        }
        break;
    case 4:
        if (wk->unk24 == 0) {
            *state = 5;
        } else if (wk->unk24 == 2) {
            wk->unk2E = 0;
            wk->unk2C = 0;
            *state = 5;
        } else {
            *state = 6;
        }
        break;
    case 5:
        if (func_ov101_021b6c20(&wk->subProc, 3, HEAP_ID, wk)) {
            *state = 6;
        }
        break;
    case 6:
        func_ov101_021b6a3c(wk, func_ov101_021b6a7c);
        break;
    }
}

// pokebw2's IrcCompatible_StepFlowB
static void func_ov101_021b6ba8(IrcCompatibleWork *wk, u16 *state) {
    switch (*state) {
    case 0:
        *state = 1;
        break;
    case 1:
        if (func_ov101_021b6c20(&wk->subProc, 4, HEAP_ID, wk)) {
            *state = 2;
        }
        break;
    case 2:
        func_ov101_021b6a3c(wk, func_ov101_021b6a7c);
        break;
    }
}

// pokebw2's IrcCompatible_InitSubProc
static void func_ov101_021b6be4(IrcCompatibleSubProc *subProc, HeapID heapId) {
    MI_CpuFill8(subProc, 0, sizeof(IrcCompatibleSubProc));
    subProc->manager = CreateGameProcManager(heapId);
}

// pokebw2's IrcCompatible_UpdateSubProc
static BOOL func_ov101_021b6bfc(IrcCompatibleSubProc *subProc) {
    return GFL_ProcMgrUpdate(subProc->manager);
}

// pokebw2's IrcCompatible_FreeSubProc
static void func_ov101_021b6c08(IrcCompatibleSubProc *subProc) {
    FreeGameProcManager(subProc->manager);
    MI_CpuFill8(subProc, 0, sizeof(IrcCompatibleSubProc));
}

// Runs a screen, and returns TRUE once it has ended and given its result back
// pokebw2's IrcCompatible_RunSubProc
static BOOL func_ov101_021b6c20(IrcCompatibleSubProc *subProc, int index, HeapID heapId, IrcCompatibleWork *wk) {
    switch (subProc->state) {
    case SUB_PROC_CREATE_PARAM:
        if (data_ov101_021b74b8[index].createParam != NULL) {
            subProc->param = data_ov101_021b74b8[index].createParam(heapId, wk);
        } else {
            subProc->param = NULL;
        }
        subProc->state = SUB_PROC_QUEUE;
        break;
    case SUB_PROC_QUEUE:
        QueueGameProc(subProc->manager, data_ov101_021b74b8[index].overlayId,
                      data_ov101_021b74b8[index].procFunctions, subProc->param);
        subProc->state = SUB_PROC_FREE_PARAM;
        break;
    case SUB_PROC_FREE_PARAM:
        if (data_ov101_021b74b8[index].freeParam != NULL) {
            data_ov101_021b74b8[index].freeParam(subProc->param, wk);
        }
        subProc->state = SUB_PROC_DONE;
        break;
    case SUB_PROC_DONE:
        subProc->state = SUB_PROC_CREATE_PARAM;
        return TRUE;
    }
    return FALSE;
}

// pokebw2's IrcCompatible_CreateParam0
static void *func_ov101_021b6ca0(HeapID heapId, IrcCompatibleWork *wk) {
    IrcCompatibleParam0 *param = GFL_HeapAllocate(heapId, sizeof(IrcCompatibleParam0), FALSE, "irc_compatible.c", 878);

    MI_CpuFill8(param, 0, sizeof(IrcCompatibleParam0));
    param->gsys = wk->param->gsys;
    param->sys = wk->sys;
    param->profile = wk->profile;
    if (wk->firstVisit == TRUE) {
        param->visit = 0;
        wk->firstVisit = FALSE;
    } else {
        if (wk->unk38) {
            wk->unk38 = FALSE;
            param->visit = 2;
        } else {
            param->visit = 1;
        }
    }
    return param;
}

// pokebw2's IrcCompatible_FreeParam0
static void func_ov101_021b6cf8(void *param, IrcCompatibleWork *wk) {
    IrcCompatibleParam0 *p = param;

    wk->mode = p->result;
    GFL_HeapFree(p);
}

// pokebw2's IrcCompatible_CreateParam1
static void *func_ov101_021b6d04(HeapID heapId, IrcCompatibleWork *wk) {
    IrcCompatibleParam1 *param = GFL_HeapAllocate(heapId, sizeof(IrcCompatibleParam1), FALSE, "irc_compatible.c", 943);

    MI_CpuFill8(param, 0, sizeof(IrcCompatibleParam1));
    param->gsys = wk->param->gsys;
    param->sys = wk->sys;
    param->profile = wk->profile;
    return param;
}

// pokebw2's IrcCompatible_FreeParam1
static void func_ov101_021b6d3c(void *param, IrcCompatibleWork *wk) {
    IrcCompatibleParam1 *p = param;

    wk->unk24 = p->result;
    wk->unk2E = p->unk10;
    wk->unk2F = p->unk11;
    wk->unk30 = p->unk12;
    GFL_HeapFree(p);
}

// pokebw2's IrcCompatible_CreateParam2
static void *func_ov101_021b6d5c(HeapID heapId, IrcCompatibleWork *wk) {
    IrcCompatibleParam2 *param = GFL_HeapAllocate(heapId, sizeof(IrcCompatibleParam2), FALSE, "irc_compatible.c", 996);

    MI_CpuFill8(param, 0, sizeof(IrcCompatibleParam2));
    param->gsys = wk->param->gsys;
    param->sys = wk->sys;
    param->profile = wk->profile;
    return param;
}

// pokebw2's IrcCompatible_FreeParam2
static void func_ov101_021b6d90(void *param, IrcCompatibleWork *wk) {
    IrcCompatibleParam2 *p = param;

    wk->unk28 = p->result;
    wk->unk2C = p->unk10;
    wk->unk2D = p->unk14;
    GFL_HeapFree(p);
}

// pokebw2's IrcCompatible_CreateParam3
static void *func_ov101_021b6dac(HeapID heapId, IrcCompatibleWork *wk) {
    IrcCompatibleParam3 *param = GFL_HeapAllocate(heapId, sizeof(IrcCompatibleParam3), FALSE, "irc_compatible.c", 1048);

    MI_CpuFill8(param, 0, sizeof(IrcCompatibleParam3));
    param->gsys = wk->param->gsys;
    param->sys = wk->sys;
    param->profile = wk->profile;
    if (wk->unk2C != 0 && wk->unk2E != 0) {
        CompatibleIrcProfile mine;
        TrainerLogSave *log;
        BOOL isNewTrainer;
        BOOL isNew = FALSE;
        PlayerInfo *trainer = &wk->profile->trainer;
        u32 date;

        GSYS_GetGameData(wk->param->gsys);
        log = func_0200a8a0(GameData_GetSaveControl(GSYS_GetGameData(wk->param->gsys)));
        if (!func_0200a9d4(log, getIDAsUInt(trainer))) {
            isNew = TRUE;
        }
        isNewTrainer = FALSE;
        if (wk->unk30 != 0 && isNew) {
            isNewTrainer = TRUE;
        }
        func_ov101_021b658c(wk->param->gsys, &mine);
        if (func_0203d378()) {
            date = mine.date;
        } else {
            date = wk->profile->date;
        }
        param->result = func_ov101_021b6ecc(wk->unk2C, wk->unk2E, wk->unk2D, wk->unk2F, &mine, wk->profile,
                                                        isNewTrainer, date, HEAP_ID);
    }
    return param;
}

// pokebw2's IrcCompatible_FreeParam3
static void func_ov101_021b6e84(void *param, IrcCompatibleWork *wk) {
    GFL_HeapFree(param);
}

// pokebw2's IrcCompatible_CreateParam4
static void *func_ov101_021b6e8c(HeapID heapId, IrcCompatibleWork *wk) {
    IrcCompatibleParam4 *param = GFL_HeapAllocate(heapId, sizeof(IrcCompatibleParam4), FALSE, "irc_compatible.c", 1179);

    MI_CpuFill8(param, 0, sizeof(IrcCompatibleParam4));
    param->gsys = wk->param->gsys;
    return param;
}

// pokebw2's IrcCompatible_FreeParam4
static void func_ov101_021b6ebc(void *param, IrcCompatibleWork *wk) {
    GFL_HeapFree(param);
    wk->unk38 = TRUE;
}

static int func_ov101_021b6ecc(u32 a, u32 b, u32 c, u32 d, CompatibleIrcProfile *mine,
                                           CompatibleIrcProfile *other, BOOL isNew, u32 date, HeapID heapId) {
    u32 birthday0 = func_ov101_021b70a8(mine, date);
    u32 birthday1 = func_ov101_021b70a8(other, date);
    int score = (birthday0 + birthday1) / 2 * 10 / 100;
    u32 nameMatch;
    int ratio;

    score += 50 * a / 100 + 40 * b / 100;
    nameMatch = func_ov101_021b6f9c(GetPlayerName(&mine->trainer), GetPlayerName(&other->trainer));
    ratio = FX32_CONST(nameMatch) / 150;
    if (isNew) {
        ratio = FX32_CONST(0.8);
    } else if (ratio > FX32_ONE) {
        ratio = FX32_ONE;
    } else if (ratio < FX32_ONE / 2) {
        ratio = FX32_ONE / 2;
    }
    score = score + (((100 - score) * ratio) >> FX32_SHIFT) - (d + c);
    if (score > 100) {
        return 100;
    }
    if (score < 0) {
        return 0;
    }
    return score;
}

// How much of the two names' characters agree in a bit, over the bits that the characters have
// pokebw2's IrcCompatible_NameMatch
static u32 func_ov101_021b6f9c(const u16 *name0, const u16 *name1) {
    u32 char0 = *name0;
    u32 char1 = *name1;
    u32 bit0;
    u32 bit1;
    u32 matches;
    u32 total;

    if (char0 >= 0x30a1 && char0 <= 0x30f4) {
        char0 -= 0x60;
    }
    if (char1 >= 0x30a1 && char1 <= 0x30f4) {
        char1 -= 0x60;
    }
    bit0 = func_ov101_021b7090(char0, 8);
    bit1 = func_ov101_021b7090(char1, 8);
    total = 0;
    matches = 0;
    while (TRUE) {
        if (bit0 == 0) {
            name0++;
            char0 = *name0;
            if (char0 == GFL_StrBufGetTerminator()) {
                break;
            }
            if (char0 >= 0x30a1 && char0 <= 0x30f4) {
                char0 -= 0x60;
            }
            bit0 = func_ov101_021b7090(char0, 8);
        }
        if (bit1 == 0) {
            name1++;
            char1 = *name1;
            if (char1 == GFL_StrBufGetTerminator()) {
                break;
            }
            if (char1 >= 0x30a1 && char1 <= 0x30f4) {
                char1 -= 0x60;
            }
            bit1 = func_ov101_021b7090(char1, 8);
        }
        if (bit0 != 0 && bit1 != 0) {
            if (((char0 >> bit0) & 1) == ((char1 >> bit1) & 1)) {
                matches++;
            }
            total++;
            bit0--;
            bit1--;
        }
    }
    return matches * 100 / total;
}

// The highest set bit of a value below the given bit count, or 0
// pokebw2's IrcCompatible_HighestBit
static u32 func_ov101_021b7090(u32 value, u32 count) {
    u32 bit = count - 1;

    while (bit != 0) {
        if ((value >> bit) & 1) {
            break;
        }
        bit--;
    }
    return bit;
}

// pokebw2's IrcCompatible_BirthdayScore
static u8 func_ov101_021b70a8(const CompatibleIrcProfile *profile, u32 date) {
    RTCDate rtcDate;

    rtcDate.year = (u8)(date >> 24);
    rtcDate.month = (u8)(date >> 16);
    rtcDate.day = (u8)(date >> 8);
    rtcDate.week = (u8)date;
    return func_0200aa00(profile->birthMonth, profile->birthDay, &rtcDate);
}
