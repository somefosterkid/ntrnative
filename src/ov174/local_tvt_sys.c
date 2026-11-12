// Black's code at ov174 0x021b6994-0x021b6c70. The name is the file name its asserts pass (tools/name_files.py).

#include "types.h"
#include "gfl/bg_sys.h"
#include "gfl/bmpwin.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/proc.h"
#include "gfl/str.h"
#include "ov174_021b6100.h"
#include "struct_decls.h"
#include "system/bmp_winframe.h"
#include "system/game_data.h"
#include "system/printsys.h"
#include "system/wordset.h"
#include "gfl/bmp.h"
#include "gfl/msg.h"
#include "field/beacon_view.h"
#include "local_tvt_chara.h"

static BOOL func_ov174_021b6bc4(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov174_021b6c5c(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov174_021b6c2c(GameProc *proc, u32 *state, void *param, void *work);

// The proc of the Xtransceiver's calls with the game's characters (pokebw2 defines it here too), which
// scrcmd_proc.c runs: in .data, so not const
GameProcFunctions data_ov174_021b74a0 = {
    func_ov174_021b6bc4,
    func_ov174_021b6c5c,
    func_ov174_021b6c2c,
};

void func_ov174_021b6994(Ov174Work *work) {
    u8 bg;
    LocalTvtFace *face;
    BmpWin *window;
    u32 i;

    if (work->unk_5c != NULL) {
        func_0201f7a8(work->unk_5c, 1);
        window = work->unk_5c;
        func_020456f8(window);
        bg = BmpWin_GetBGIndex(window);
        GFL_BGSysQueueScrLoad(bg);
        BmpWin_Free(work->unk_5c);
        work->unk_5c = NULL;
    }
    if (work->unk_64 != NULL) {
        func_0201ce48(work->unk_64);
        work->unk_64 = NULL;
    }
    if (work->unk_70 != NULL) {
        GFL_StrBufFree(work->unk_70);
        work->unk_70 = NULL;
    }
    i = 0;
    if (0 < work->unk_08) {
        do {
            face = work->unk_1c[i];
            if (face != NULL) {
                func_ov174_021b7350(work, face);
            }
            i = i + 1 & 0xff;
        } while ((s32)i < work->unk_08);
    }
}

void func_ov174_021b6a04(Ov174Work *work, u8 msgId) {
    StrBuf *pStrBuf;
    PrintStream *pPrintStream;

    if (work->unk_64 != NULL) {
        func_0201ce48(work->unk_64);
        work->unk_64 = NULL;
    }
    if (work->unk_70 != NULL) {
        GFL_StrBufFree(work->unk_70);
        work->unk_70 = NULL;
    }
    GFL_BitmapFill(BmpWin_GetBitmap(work->unk_5c), 0xf);
    work->unk_70 = GFL_MsgDataLoadStrbufNew(work->unk_6c, msgId);
    if (work->unk_74 != NULL) {
        pStrBuf = GFL_StrBufCreate(0x400, work->heapId);
        GFL_WordSetFormatStrbuf(work->unk_74, pStrBuf, work->unk_70);
        GFL_StrBufFree(work->unk_70);
        work->unk_70 = pStrBuf;
    }
    pPrintStream = func_0201cce4(work->unk_5c, 0, 0, work->unk_70, work->unk_60, func_02012ffc(), work->unk_58, 2, work->heapId, 0xf);
    work->unk_64 = pPrintStream;
}

void func_ov174_021b6a90(Ov174Work *work) {
    u32 val;

    val = work->unk_40[work->unk_89].state;
    if (work->unk_34 == 1) {
        work->unk_34 = 0;
        data_ov174_021b7398[val].unk_0(work);
    }
    if (data_ov174_021b7398[val].unk_4(work) == 1) {
        work->unk_89 = work->unk_89 + 1;
        val = work->unk_40[work->unk_89].state;
        if (val != 0) {
            data_ov174_021b7398[val].unk_0(work);
            return;
        }
        work->unk_0c = 2;
    }
}

void func_ov174_021b6af8(Ov174Work *work) {
    u32 idx;
    u8 msgId;

    idx = work->unk_40[work->unk_89].face;
    msgId = work->unk_40[work->unk_89].param;
    work->unk_88 = idx;
    func_ov174_021b6704(work, idx);
    if (idx < 2) {
        func_ov174_021b68f8(work, 1);
    } else {
        func_ov174_021b68f8(work, 0);
    }
    if (work->unk_74 != NULL) {
        GFL_WordSetSystemFree(work->unk_74);
    }
    work->unk_74 = GFL_WordSetSystemCreateDefault(work->heapId);
    copyVarForText(work->unk_74, 0, GetGameDataPlayerInfo(work->unk_30->gameData));
    func_ov174_021b6a04(work, msgId);
}

u32 func_ov174_021b6b5c(Ov174Work *work) {
    if (func_ov174_021b68e8(work) == 1) {
        func_ov174_021b6754(work);
        func_ov174_021b6994(work);
        func_ov174_021b7378(work->unk_1c[work->unk_88]);
        return 1;
    }
    return 0;
}

void func_ov174_021b6b8c(Ov174Work *work) {
    work->unk_8a = 0;
}

u32 func_ov174_021b6b94(Ov174Work *work) {
    u8 val;

    val = work->unk_40[work->unk_89].param;
    work->unk_8a = work->unk_8a + 1;
    if (work->unk_8a > val) {
        return 1;
    }
    return 0;
}

static BOOL func_ov174_021b6bc4(GameProc *proc, u32 *state, void *param, void *work) {
    Ov174Work *wk;
    struct Ov174Init *init;

    init = param;
    GFL_HeapCreateChild(1, 0x55, 0x40000);
    wk = GFL_ProcInitSubsystem(proc, sizeof(Ov174Work), 0x55);
    if (init == NULL) {
        init = GFL_HeapAllocate(0x55, sizeof(struct Ov174Init), 0, "local_tvt_sys.c", 0x3b8);
        init->gameData = GameData_Create(1);
        init->unk_00 = 1;
        if ((GCTX_HIDGetHeldKeys() & 0x100) != 0) {
            init->unk_00 = 5;
        }
    }
    wk->heapId = 0x55;
    wk->unk_30 = init;
    func_ov174_021b6100(wk);
    return TRUE;
}

static BOOL func_ov174_021b6c2c(GameProc *proc, u32 *state, void *param, void *work) {
    Ov174Work *wk = work;

    func_ov174_021b61c4(wk);
    if (param == NULL) {
        GameData_Free(wk->unk_30->gameData);
        GFL_HeapFree(wk->unk_30);
    }
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(0x55);
    return TRUE;
}

static BOOL func_ov174_021b6c5c(GameProc *proc, u32 *state, void *param, void *work) {
    if (func_ov174_021b622c(work) == 1) {
        return TRUE;
    }
    return FALSE;
}
