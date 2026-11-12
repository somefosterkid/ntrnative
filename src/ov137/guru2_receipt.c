// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov216/guru2_receipt.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/guru2.h"
#include "constants/arc.h"
#include "constants/sound.h"
#include "constants/text_banks.h"
#include "field/union_app.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/net.h"
#include "gfl/net_command.h"
#include "gfl/net_handle.h"
#include "gfl/net_system.h"
#include "gfl/proc.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "gfl/tcbl.h"
#include "nitro/gx.h"
#include "nnsys/g2d.h"
#include "save/player_info.h"
#include "system/bmp_menu.h"
#include "system/bmp_winframe.h"
#include "system/game_data.h"
#include "system/gf_font.h"
#include "system/printsys.h"
#include "system/wipe.h"
#include "system/wordset.h"
#include "text/system/btl_pokeparam.h"

// The Spin Trade's receipt screen, where the parent waits for the group to gather and closes it, and the others wait
// for the parent. The ROM has no name for the file; it is named after pokeplatinum's guru2_receipt.c, the same screen
// in Gen 4. Names are ours

#define GURU2_RECEIPT_ARCID 184
#define GURU2_RECEIPT_OBJ_ARCID 31
#define GURU2_RECEIPT_TRAINER_ARCID 32

// The proc's sequences
enum {
    GURU2_RECEIPT_PROC_FADE_IN,
    GURU2_RECEIPT_PROC_MAIN,
    GURU2_RECEIPT_PROC_MAIN_NO_UPDATE,
    GURU2_RECEIPT_PROC_FADE_OUT,
    GURU2_RECEIPT_PROC_WAIT_START,
};

// The cell actor resources: a palette and the characters of each member's trainer, and the cells
enum {
    GURU2_RECEIPT_RES_PLTT,
    GURU2_RECEIPT_RES_CHAR = GURU2_RECEIPT_RES_PLTT + GURU2_MEMBER_MAX,
    GURU2_RECEIPT_RES_CELL = GURU2_RECEIPT_RES_CHAR + GURU2_MEMBER_MAX,
    GURU2_RECEIPT_RES_COUNT,
};

// What a member's trainer does next
enum {
    GURU2_RECEIPT_ACTOR_NONE,
    GURU2_RECEIPT_ACTOR_APPEAR,
    GURU2_RECEIPT_ACTOR_SHOWN,
    GURU2_RECEIPT_ACTOR_LEAVE,
};

#define GURU2_RECEIPT_COLOR_NAME PRINT_COLOR(1, 4, 0)
#define GURU2_RECEIPT_COLOR_SELF PRINT_COLOR(2, 3, 0)

// A palette animation of the trainers, which nothing starts
typedef struct {
    BOOL active;
    int index;
    int counter;
    void *file;
    NNSG2dPaletteData *palette;
} Guru2ReceiptPalAnim;

typedef struct {
    u8 wait;
    // The palette's row in the file, 0xff at the end
    u8 palette;
} Guru2ReceiptPalAnimStep;

typedef struct {
    UnionAppMember *member;
    UnionAppMember *prev;
} Guru2ReceiptMember;

typedef union {
    struct {
        u32 id;
        BOOL present;
    } parts;
    u64 value;
} Guru2ReceiptId;

// A member's trainer ID, and what it was before the last update
typedef struct {
    Guru2ReceiptId id;
    Guru2ReceiptId prev;
} Guru2ReceiptMemberId;

typedef struct {
    u16 x;
    u16 y;
} Guru2ReceiptPos;

struct Guru2ReceiptWork {
    int procSeq;
    u8 unk04[4];
    BOOL receiptDone;
    Guru2ProcWork *g2p;
    Guru2CommWork *g2c;
    u8 unk14[4];
    Guru2ReceiptPalAnim palAnim;
    u8 unk2C[4];
    TCB *vblankTcb;
    ArcTool *trainerArc;
    Font *font;
    WordSet *wordSet;
    MsgData *msgData;
    StrBuf *names[GURU2_MEMBER_MAX];
    u8 unk58[4];
    StrBuf *msgBuf;
    StrBuf *titleBuf;
    u8 unk64[4];
    ClActUnit *actorUnit;
    u32 resources[GURU2_RECEIPT_RES_COUNT];
    u8 unk98[4];
    ClActor *actors[GURU2_MEMBER_MAX];
    u8 unkB0[0x20];
    BmpWin *nameWindow;
    BmpWin *msgWindow;
    BmpWin *subWindow;
    BmpWin *titleWindow;
    u8 unkE0[8];
    BmpMenu *yesNo;
    PrintQueue *printQueue;
    PrintStream *stream;
    TCBExManager *tcbManager;
    BmpWin *unkF8;
    u8 unkFC;
    u8 unkFD[0x13];
    void *trainerPaletteFile;
    NNSG2dPaletteData *trainerPalette;
    int seq;
    // The sequence after GURU2_RECEIPT_SEQ_MSG_WAIT
    int nextSeq;
    u8 unk120[4];
    int wait;
    u8 unk128[0x10];
    Guru2ReceiptMember members[GURU2_MEMBER_MAX];
    int actorStates[GURU2_MEMBER_MAX];
    u8 unk174[0x4690];
    int memberCount;
    BOOL entryClosed;
    u32 unk480C;
    u8 cancelSent;
    u8 unk4811;
    u8 startCount;
    u8 unk4813;
    u32 syncBits;
    int leftTimer;
    int leftCount;
    s16 signalTimer;
    u8 unk4822[2];
    Guru2ReceiptMemberId ids[GURU2_MEMBER_MAX];
};

typedef int (*Guru2ReceiptSeqFunc)(Guru2ReceiptWork *wk, int procSeq);

static UnionApp *func_ov137_021dcf50(Guru2ReceiptWork *wk);
static void func_ov137_021dcf58(u8 netId, UnionAppMember *member, void *work);
static void func_ov137_021dcf7c(u8 netId, UnionAppMember *member, void *work);
static void func_ov137_021dd14c(TCB *tcb, void *data);
static void func_ov137_021dd1a4(TCB *tcb, void *data);
static void func_ov137_021dd1b4(Guru2ReceiptWork *wk);
static void func_ov137_021dd1d0(void);
static void func_ov137_021dd1e0(void);
static void func_ov137_021dd2e0(Guru2ReceiptWork *wk, ArcTool *arc);
static void func_ov137_021dd380(Guru2ReceiptWork *wk);
static void func_ov137_021dd3b0(void);
static void func_ov137_021dd3d8(Guru2ReceiptWork *wk, ArcTool *arc);
static void func_ov137_021dd4a8(Guru2ReceiptWork *wk, ArcTool *arc);
static void func_ov137_021dd570(Guru2ReceiptWork *wk);
static void func_ov137_021dd5ac(Guru2ReceiptWork *wk);
static void func_ov137_021dd638(Guru2ReceiptWork *wk);
static void func_ov137_021dd700(BmpWin *window, StrBuf *title, Font *font);
static void func_ov137_021dd760(Guru2ReceiptWork *wk);
static int func_ov137_021dd79c(Guru2ReceiptWork *wk, int procSeq);
static int func_ov137_021dd7e4(Guru2ReceiptWork *wk, int procSeq);
static void func_ov137_021dd80c(Guru2ReceiptWork *wk);
static void func_ov137_021dd924(Guru2ReceiptWork *wk);
static int func_ov137_021dd928(Guru2ReceiptWork *wk, int procSeq);
static void func_ov137_021dd93c(Guru2ReceiptWork *wk, int nextSeq);
static int func_ov137_021dd94c(Guru2ReceiptWork *wk, int procSeq);
static int func_ov137_021dd970(Guru2ReceiptWork *wk, int procSeq);
static int func_ov137_021dd99c(Guru2ReceiptWork *wk, int procSeq);
static int func_ov137_021dd9d4(Guru2ReceiptWork *wk, int procSeq);
static int func_ov137_021ddaec(Guru2ReceiptWork *wk, int procSeq);
static int func_ov137_021ddb04(Guru2ReceiptWork *wk, int procSeq);
static int func_ov137_021ddb90(Guru2ReceiptWork *wk, int procSeq);
static int func_ov137_021ddbb0(Guru2ReceiptWork *wk, int procSeq);
static int func_ov137_021ddbe8(Guru2ReceiptWork *wk, int procSeq);
static int func_ov137_021ddc7c(Guru2ReceiptWork *wk, int procSeq);
static int func_ov137_021ddcb4(Guru2ReceiptWork *wk, int procSeq);
static int func_ov137_021ddcec(Guru2ReceiptWork *wk, int procSeq);
static int func_ov137_021ddd0c(Guru2ReceiptWork *wk, int procSeq);
static int func_ov137_021ddd48(Guru2ReceiptWork *wk, int procSeq);
static int func_ov137_021ddd80(Guru2ReceiptWork *wk, int procSeq);
static int func_ov137_021dde24(Guru2ReceiptWork *wk, int procSeq);
static int func_ov137_021dde74(Guru2ReceiptWork *wk, int procSeq);
static int func_ov137_021dde94(Guru2ReceiptWork *wk, int procSeq);
static int func_ov137_021ddedc(Guru2ReceiptWork *wk, int procSeq);
static int func_ov137_021ddf38(Guru2ReceiptWork *wk, int procSeq);
static int func_ov137_021ddf64(Guru2ReceiptWork *wk, int procSeq);
static int func_ov137_021ddf84(Guru2ReceiptWork *wk, int procSeq);
static int func_ov137_021ddfc0(Guru2ReceiptWork *wk, int procSeq);
static int func_ov137_021ddffc(Guru2ReceiptWork *wk, int procSeq);
static int func_ov137_021de060(Guru2ReceiptWork *wk, int procSeq);
static int func_ov137_021de220(Guru2ReceiptWork *wk);
static BOOL func_ov137_021de248(BmpWin *window, int unused, u16 color, Guru2ReceiptWork *wk);
static void func_ov137_021de350(Guru2ReceiptWork *wk);
static int func_ov137_021de354(Guru2ReceiptWork *wk);
static u32 func_ov137_021de37c(Guru2ReceiptWork *wk);
static BOOL func_ov137_021de388(Guru2ReceiptWork *wk);
static void func_ov137_021de4b4(Guru2ReceiptWork *wk, u32 messageId, int unused);
static BOOL func_ov137_021de55c(Guru2ReceiptWork *wk);
static void func_ov137_021de5c0(Guru2ReceiptWork *wk);
static void func_ov137_021de5d0(Guru2ReceiptWork *wk);
static void func_ov137_021de6c0(Guru2ReceiptWork *wk, ArcTool *arc);
static void func_ov137_021de6dc(Guru2ReceiptWork *wk);
static void func_ov137_021de6ec(Guru2ReceiptWork *wk, NNSG2dPaletteData *palette, int id, int look, int gender);
static int func_ov137_021de738(Guru2ReceiptWork *wk, int unused);
static u32 func_ov137_021de7f4(void);

// pokebw2's sYesNoSetup
static const ConfirmDialogSetup data_ov137_021e1ae8 = { 0, 23, 13, 13, 0xd8 };

// pokebw2's sSeqFuncs
static const Guru2ReceiptSeqFunc data_ov137_021e1c20[GURU2_RECEIPT_SEQ_COUNT] = {
    func_ov137_021dd79c,
    func_ov137_021dd928,
    func_ov137_021dd970,
    func_ov137_021dd7e4,
    func_ov137_021dd99c,
    func_ov137_021dd9d4,
    func_ov137_021ddaec,
    func_ov137_021ddb04,
    func_ov137_021ddb90,
    func_ov137_021ddcec,
    func_ov137_021ddd0c,
    func_ov137_021ddd48,
    func_ov137_021ddd80,
    func_ov137_021dde24,
    func_ov137_021dde74,
    func_ov137_021dde94,
    func_ov137_021ddedc,
    func_ov137_021ddf38,
    func_ov137_021ddf64,
    func_ov137_021ddf84,
    func_ov137_021ddfc0,
    func_ov137_021ddffc,
    func_ov137_021ddbb0,
    func_ov137_021ddbe8,
    func_ov137_021ddc7c,
    func_ov137_021ddcb4,
    func_ov137_021dd94c,
    func_ov137_021de060,
};

// pokebw2's Guru2Receipt_Init
BOOL func_ov137_021dce10(GameProc *proc, u32 *state, void *param, void *work) {
    Guru2ProcWork *g2p = param;
    Guru2ReceiptWork *wk;
    ArcTool *arc;

    switch (*state) {
    case 0:
        GFL_BGSysDisableAllA();
        GFL_BGSysDisableAllB();
        GX_SetVisiblePlane(0);
        GXS_SetVisiblePlane(0);
        GFL_HeapCreateChild(HEAPID_USER, HEAPID_GURU2, 0x80000);
        arc = GFL_ArcSysCreateFileHandle(GURU2_RECEIPT_ARCID, HEAPID_GURU2);
        wk = GFL_ProcInitSubsystem(proc, sizeof(Guru2ReceiptWork), HEAPID_GURU2);
        MI_CpuFill8(wk, 0, sizeof(Guru2ReceiptWork));
        g2p->g2r = wk;
        wk->g2p = g2p;
        wk->g2c = g2p->g2c;
        GFL_BGSysCreate(HEAPID_GURU2);
        wk->wordSet = GFL_WordSetSystemCreateDefault(HEAPID_GURU2);
        wk->msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_BTL_POKEPARAM, HEAPID_GURU2);
        wk->font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, HEAPID_GURU2);
        func_ov137_021dd1d0();
        func_ov137_021dd1e0();
        GFL_WipeSet(WIPE_MODE_BOTH, WIPE_TYPE_FADE_IN, WIPE_TYPE_FADE_IN, WIPE_COLOR_BLACK, 16, 1,
                    HEAPID_GURU2);
        func_ov137_021dd3d8(wk, arc);
        wk->vblankTcb = GFL_VBlankTCBAdd(func_ov137_021dd1a4, wk, 1);
        wk->tcbManager = GFL_TCBExMgrCreate(HEAPID_GURU2, HEAPID_GURU2, 32, 32);
        wk->printQueue = func_0201c440(HEAPID_GURU2);
        func_ov137_021dd2e0(wk, arc);
        func_ov137_021dd4a8(wk, arc);
        func_ov137_021dd5ac(wk);
        func_ov137_021dd638(wk);
        func_ov137_021dcb78(g2p->g2c);
        func_0203d34c(TRUE, HEAPID_GURU2);
        GFL_ArcToolFree(arc);
        (*state)++;
        break;
    case 1:
        *state = 0;
        return TRUE;
    }
    return FALSE;
}

// pokebw2's Guru2Receipt_GetUnionApp
static UnionApp *func_ov137_021dcf50(Guru2ReceiptWork *wk) {
    return wk->g2p->param->app;
}

// pokebw2's Guru2Receipt_OnJoin
static void func_ov137_021dcf58(u8 netId, UnionAppMember *member, void *work) {
    Guru2ReceiptWork *wk = work;

    if (func_02039338() == 0) {
        u8 id = netId;

        func_ov137_021dcbc4(wk->g2c, GURU2_NET_CMD_JOIN, &id, sizeof(id));
    }
}

// pokebw2's Guru2Receipt_OnLeave
static void func_ov137_021dcf7c(u8 netId, UnionAppMember *member, void *work) {
}

// pokebw2's Guru2Receipt_Main
BOOL func_ov137_021dcf80(GameProc *proc, u32 *state, void *param, void *work) {
    Guru2ReceiptWork *wk = work;

    if (func_02039338() == 0 && wk->g2c->leavingBits != 0) {
        wk->g2c->leavingBits &= func_ov022_021841a0(func_ov137_021dcf50(wk));
    }
    switch (wk->procSeq) {
    case GURU2_RECEIPT_PROC_FADE_IN:
        if (GFL_WipeIsFinished()) {
            wk->procSeq = GURU2_RECEIPT_PROC_MAIN;
            func_ov022_02184134(func_ov137_021dcf50(wk));
            func_ov022_021840fc(func_ov137_021dcf50(wk), func_ov137_021dcf58, func_ov137_021dcf7c, wk);
        }
        break;
    case GURU2_RECEIPT_PROC_MAIN:
        if (data_ov137_021e1c20[wk->seq] != NULL) {
            wk->procSeq = data_ov137_021e1c20[wk->seq](wk, wk->procSeq);
        }
        if (!wk->g2c->started) {
            func_ov137_021de248(wk->nameWindow, 0, GURU2_RECEIPT_COLOR_NAME, wk);
        }
        func_ov137_021de5d0(wk);
        if (func_02039338() == 0) {
            int next = func_ov137_021de738(wk, 1);

            if (wk->procSeq == GURU2_RECEIPT_PROC_MAIN) {
                wk->procSeq = next;
            }
        }
        break;
    case GURU2_RECEIPT_PROC_MAIN_NO_UPDATE:
        if (data_ov137_021e1c20[wk->seq] != NULL) {
            wk->procSeq = data_ov137_021e1c20[wk->seq](wk, wk->procSeq);
        }
        break;
    case GURU2_RECEIPT_PROC_FADE_OUT:
        if (GFL_WipeIsFinished()) {
            if (wk->receiptDone) {
                return TRUE;
            }
            func_0203a5e4(GURU2_NET_CMD_BASE);
            func_ov022_02184174(func_ov137_021dcf50(wk));
            wk->procSeq = GURU2_RECEIPT_PROC_WAIT_START;
        }
        break;
    case GURU2_RECEIPT_PROC_WAIT_START:
        if (func_ov022_0218417c(func_ov137_021dcf50(wk))) {
            return TRUE;
        }
        break;
    }
    if (GFL_WipeIsFinished() && func_0200fb24()) {
        return TRUE;
    }
    func_0204a600();
    func_ov137_021dd1b4(wk);
    return FALSE;
}

// pokebw2's Guru2Receipt_Exit
BOOL func_ov137_021dd0b8(GameProc *proc, u32 *state, void *param, void *work) {
    Guru2ReceiptWork *wk = work;

    GFL_TCBExMgrFreeTasks(wk->tcbManager);
    GFL_TCBExMgrFree(wk->tcbManager);
    func_0201c6ec(wk->printQueue);
    func_0201c4c0(wk->printQueue);
    func_ov137_021dd570(wk);
    func_ov137_021dd760(wk);
    func_ov137_021dd3b0();
    GFL_MsgDataFree(wk->msgData);
    GFL_WordSetSystemFree(wk->wordSet);
    GFL_FontFree(wk->font);
    GX_SetDispSelect(GX_DISP_SELECT_MAIN_SUB);
    GFL_TCBRemove(wk->vblankTcb);
    wk->g2p->receiptDone = wk->receiptDone;
    wk->g2p->memberCount = func_ov137_021de354(wk);
    wk->g2p->memberBits = func_ov137_021de37c(wk);
    func_ov137_021dd380(wk);
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(HEAPID_GURU2);
    return TRUE;
}

// pokebw2's sPalAnimSteps
static const Guru2ReceiptPalAnimStep data_ov137_021e1aee[] = {
    { 8, 2 },
    { 8, 3 },
    { 8, 4 },
    { 24, 0 },
    { 0, 0xff },
};

// pokebw2's Guru2Receipt_UpdatePalAnim
static void func_ov137_021dd14c(TCB *tcb, void *data) {
    Guru2ReceiptWork *wk = data;
    Guru2ReceiptPalAnim *anim = &wk->palAnim;

    if (anim->active) {
        if (anim->counter > data_ov137_021e1aee[anim->index].wait) {
            anim->counter = 0;
            anim->index++;
            if (data_ov137_021e1aee[anim->index].palette == 0xff) {
                anim->index = 0;
            }
            GX_LoadOBJPltt((u8 *)anim->palette->pRawData + data_ov137_021e1aee[anim->index].palette * 0x20, 0, 0x20);
        } else {
            wk->palAnim.counter++;
        }
    }
}

// pokebw2's Guru2Receipt_VBlank
static void func_ov137_021dd1a4(TCB *tcb, void *data) {
    func_ov137_021dd14c(tcb, data);
    func_0204a648();
    GFL_BGSysUpdate();
}

// pokebw2's Guru2Receipt_UpdatePrint
static void func_ov137_021dd1b4(Guru2ReceiptWork *wk) {
    if (wk->printQueue != NULL) {
        func_0201c4e4(wk->printQueue);
    }
    GFL_TCBExMgrUpdate(wk->tcbManager);
}

// pokebw2's sVRAMConfig
static const BGSysVRAMConfig data_ov137_021e1b40 = {
    GX_VRAM_BG_128_A,  GX_VRAM_BGEXTPLTT_NONE,  GX_VRAM_SUB_BG_128_C,       GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_128_B, GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_16_I,       GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_NONE,  GX_VRAM_TEXPLTT_NONE,    GX_OBJVRAMMODE_CHAR_1D_32K, GX_OBJVRAMMODE_CHAR_1D_32K,
};

// pokebw2's Guru2Receipt_SetVRAMBanks
static void func_ov137_021dd1d0(void) {
    GFL_BGSysSetVRAMBanks(&data_ov137_021e1b40);
}

// pokebw2's Guru2Receipt_InitBG
static void func_ov137_021dd1e0(void) {
    {
        BGSysLCDConfig config = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_2D };

        GFL_BGSysSetLCDConfig(&config);
    }
    {
        BGSetup setup = {
            0,
            0,
            0x800,
            0,
            BGRES_256x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xf000,
            GX_BG_CHARBASE_0x10000,
            0x4000,
            GX_BG_EXTPLTT_01,
            0,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };

        GFL_BGSysCreateBG(4, &setup, BGMODE_TEXT);
        GFL_BGSysClearScr(4);
    }
    {
        BGSetup setup = {
            0,
            0,
            0x800,
            0,
            BGRES_256x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xf800,
            GX_BG_CHARBASE_0x00000,
            0x4000,
            GX_BG_EXTPLTT_01,
            1,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };

        GFL_BGSysCreateBG(5, &setup, BGMODE_TEXT);
        GFL_BGSysClearScr(5);
    }
    {
        BGSetup setup = {
            0,
            0,
            0x800,
            0,
            BGRES_256x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xd800,
            GX_BG_CHARBASE_0x08000,
            0x4000,
            GX_BG_EXTPLTT_01,
            2,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };

        GFL_BGSysCreateBG(6, &setup, BGMODE_TEXT);
        GFL_BGSysClearCharCore(4, 0x20, 0, HEAPID_GURU2);
    }
    {
        BGSetup setup = {
            0,
            0,
            0x800,
            0,
            BGRES_256x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xf800,
            GX_BG_CHARBASE_0x00000,
            0x4000,
            GX_BG_EXTPLTT_01,
            0,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };

        GFL_BGSysCreateBG(0, &setup, BGMODE_TEXT);
        GFL_BGSysClearScr(0);
    }
    {
        BGSetup setup = {
            0,
            0,
            0x800,
            0,
            BGRES_256x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xf000,
            GX_BG_CHARBASE_0x08000,
            0x4000,
            GX_BG_EXTPLTT_01,
            1,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };

        GFL_BGSysCreateBG(1, &setup, BGMODE_TEXT);
        GFL_BGSysClearCharCore(0, 0x20, 0, HEAPID_GURU2);
    }
}

// pokebw2's Guru2Receipt_InitWork
static void func_ov137_021dd2e0(Guru2ReceiptWork *wk, ArcTool *arc) {
    int i;

    for (i = 0; i < GURU2_MEMBER_MAX; i++) {
        wk->names[i] = GFL_StrBufCreate(8, HEAPID_GURU2);
        wk->members[i].member = NULL;
        wk->members[i].prev = NULL;
        wk->actorStates[i] = GURU2_RECEIPT_ACTOR_NONE;
    }
    wk->msgBuf = GFL_StrBufCreate(180, HEAPID_GURU2);
    wk->titleBuf = GFL_StrBufCreate(40, HEAPID_GURU2);
    wk->seq = GURU2_RECEIPT_SEQ_INIT;
    GFL_MsgDataLoadStrbuf(wk->msgData, BtlPokeparam_Text_017, wk->titleBuf);
    func_ov137_021de6c0(wk, arc);
    wk->palAnim.active = FALSE;
    wk->palAnim.counter = 0;
    wk->palAnim.index = 0;
    wk->palAnim.file = GFL_G2DIOReadNCLRArc(arc, 2, &wk->palAnim.palette, HEAPID_GURU2);
    wk->memberCount = 0;
    wk->yesNo = NULL;
    wk->g2c->joinCount = 2;
}

// pokebw2's Guru2Receipt_FreeWork
static void func_ov137_021dd380(Guru2ReceiptWork *wk) {
    int i;

    func_ov137_021de6dc(wk);
    GFL_HeapFree(wk->palAnim.file);
    for (i = 0; i < GURU2_MEMBER_MAX; i++) {
        GFL_StrBufFree(wk->names[i]);
    }
    GFL_StrBufFree(wk->titleBuf);
    GFL_StrBufFree(wk->msgBuf);
}

// pokebw2's Guru2Receipt_ExitBG
static void func_ov137_021dd3b0(void) {
    GFL_BGSysReleaseBG(6);
    GFL_BGSysReleaseBG(5);
    GFL_BGSysReleaseBG(4);
    GFL_BGSysReleaseBG(1);
    GFL_BGSysReleaseBG(0);
    GFL_BGSysFree();
}

// pokebw2's Guru2Receipt_LoadBG
static void func_ov137_021dd3d8(Guru2ReceiptWork *wk, ArcTool *arc) {
    GFL_G2DIOLoadArcNCLRDefault(arc, 1, PALTYPE_MAIN_BG, 0, 0x200, HEAPID_GURU2);
    GFL_BGSysLoadArcNCGRStatic(arc, 7, 1, 0, 0x2000, FALSE, HEAPID_GURU2);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 13, 1, 0, 0x600, FALSE, HEAPID_GURU2);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, PALTYPE_MAIN_BG, 13 * 0x20, 0x20, HEAPID_GURU2);
    GFL_BGSysLoadNCLRDefault(ARCID_C_GEAR, 13, PALTYPE_SUB_BG, 0, 0x160, HEAPID_GURU2);
    GFL_BGSysLoadNCGRStatic(ARCID_C_GEAR, 14, 6, 0, 0, FALSE, HEAPID_GURU2);
    loadBGScrToVramByNarcNoReserveNegAlign(ARCID_C_GEAR, 3, 6, 0, 0, FALSE, HEAPID_GURU2);
    LoadSysMsgBox(0, 1, 10, 0, HEAPID_GURU2);
    LoadSysMsgBox(0, 31, 11, 0, HEAPID_GURU2);
    GFL_BGSysSetBGEnabled(0, TRUE);
    GFL_BGSysSetBGEnabled(1, TRUE);
    GFL_BGSysSetBGEnabled(6, TRUE);
}

// The files of the trainers' cell actor by the player's gender: characters, palette, cells, and animations
// pokebw2's sActorFiles
static const u32 data_ov137_021e1b20[4][2] = {
    { 20, 23 },
    { 4, 6 },
    { 18, 21 },
    { 19, 22 },
};

// pokebw2's Guru2Receipt_InitActorSys
static void func_ov137_021dd4a8(Guru2ReceiptWork *wk, ArcTool *unused) {
    int gender = getTrainerGender(GetGameDataPlayerInfo(wk->g2p->param->gameData));
    ArcTool *arc = GFL_ArcSysCreateFileHandle(GURU2_RECEIPT_OBJ_ARCID, HEAPID_GURU2);
    int i;

    ClActSys_Create(&data_020a1448, &data_ov137_021e1b40, HEAPID_GURU2);
    wk->actorUnit = func_0204b100(30, 1, HEAPID_GURU2);
    for (i = 0; i < GURU2_MEMBER_MAX; i++) {
        wk->resources[GURU2_RECEIPT_RES_CHAR + i] =
            func_0204a6c8(arc, data_ov137_021e1b20[0][gender], FALSE, 0, HEAPID_GURU2);
        wk->resources[GURU2_RECEIPT_RES_PLTT + i] =
            func_0204ac18(arc, data_ov137_021e1b20[1][gender], 0, i * 0x20, 0, 1, HEAPID_GURU2);
    }
    wk->resources[GURU2_RECEIPT_RES_CELL] =
        func_0204af28(arc, data_ov137_021e1b20[2][gender], data_ov137_021e1b20[3][gender], HEAPID_GURU2);
    GFL_ArcToolFree(arc);
    wk->trainerArc = GFL_ArcSysCreateFileHandle(GURU2_RECEIPT_TRAINER_ARCID, HEAPID_GURU2);
}

// pokebw2's Guru2Receipt_ExitActorSys
static void func_ov137_021dd570(Guru2ReceiptWork *wk) {
    int i;

    GFL_ArcToolFree(wk->trainerArc);
    for (i = 0; i < GURU2_MEMBER_MAX; i++) {
        // BUG: the palettes are freed as characters and the characters as palettes. Each kind's handles count
        // from 0, so the five of one kind are the five of the other
#ifdef BUGFIX
        func_0204a8d4(wk->resources[GURU2_RECEIPT_RES_CHAR + i]);
        func_0204ada4(wk->resources[GURU2_RECEIPT_RES_PLTT + i]);
#else
        func_0204a8d4(wk->resources[GURU2_RECEIPT_RES_PLTT + i]);
        func_0204ada4(wk->resources[GURU2_RECEIPT_RES_CHAR + i]);
#endif
    }
    func_0204afd8(wk->resources[GURU2_RECEIPT_RES_CELL]);
    func_0204b1cc(wk->actorUnit);
    func_0204a5ac();
}

// Where the trainers stand: the middle, then each member's place
// pokebw2's sActorPositions
static const Guru2ReceiptPos data_ov137_021e1b08[1 + GURU2_MEMBER_MAX] = {
    { 204, 84 },
    { 204, 53 },
    { 233, 72 },
    { 223, 103 },
    { 184, 103 },
    { 174, 72 },
};

// pokebw2's Guru2Receipt_CreateActors
static void func_ov137_021dd5ac(Guru2ReceiptWork *wk) {
    ClActorSetup setup;
    int i;

    setup.bgPriority = 1;
    setup.priority = 0;
    setup.sequence = 0;
    for (i = 0; i < GURU2_MEMBER_MAX; i++) {
        setup.x = data_ov137_021e1b08[i + 1].x;
        setup.y = data_ov137_021e1b08[i + 1].y;
        wk->actors[i] = func_0204b294(wk->actorUnit, wk->resources[GURU2_RECEIPT_RES_CHAR + i],
                                      wk->resources[GURU2_RECEIPT_RES_PLTT + i],
                                      wk->resources[GURU2_RECEIPT_RES_CELL], &setup, 0, HEAPID_GURU2);
        func_0204b92c(wk->actors[i], TRUE);
        func_0204b878(wk->actors[i], 0);
        func_0204b3dc(wk->actors[i], FALSE);
    }
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_OBJ, TRUE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, FALSE);
}

// pokebw2's Guru2Receipt_CreateWindows
static void func_ov137_021dd638(Guru2ReceiptWork *wk) {
    BmpWin_InitAllocator(HEAPID_GURU2);
    wk->subWindow = BmpWin_CreateDynamic(5, 26, 20, 6, 2, 13, TRUE);
    GFL_BitmapFill(BmpWin_GetBitmap(wk->subWindow), 0);
    wk->msgWindow = BmpWin_CreateDynamic(0, 2, 19, 27, 4, 13, TRUE);
    GFL_BitmapFill(BmpWin_GetBitmap(wk->msgWindow), 15);
    wk->titleWindow = BmpWin_CreateDynamic(0, 3, 1, 26, 2, 15, TRUE);
    func_ov137_021dd700(wk->titleWindow, wk->titleBuf, wk->font);
    wk->nameWindow = BmpWin_CreateDynamic(0, 2, 6, 16, 11, 15, TRUE);
    GFL_BitmapFill(BmpWin_GetBitmap(wk->nameWindow), 0);
    func_ov137_021de248(wk->nameWindow, 0, GURU2_RECEIPT_COLOR_NAME, wk);
    wk->unkF8 = wk->titleWindow;
    wk->unkFC = 0;
}

// pokebw2's Guru2Receipt_DrawTitle
static void func_ov137_021dd700(BmpWin *window, StrBuf *title, Font *font) {
    int width = GFL_FontGetBlockWidth(title, font, 0);

    GFL_BitmapFill(BmpWin_GetBitmap(window), 0);
    GFL_TextRendererDrawToBitmapEx(BmpWin_GetBitmap(window), (26 * 8 - width) / 2, 1, title, font,
                                   GURU2_RECEIPT_COLOR_NAME);
    BmpWin_TransferNow(window);
}

// pokebw2's Guru2Receipt_FreeWindows
static void func_ov137_021dd760(Guru2ReceiptWork *wk) {
    if (wk->yesNo != NULL) {
        func_0201fef4(wk->yesNo);
    }
    BmpWin_Free(wk->nameWindow);
    BmpWin_Free(wk->titleWindow);
    BmpWin_Free(wk->subWindow);
    BmpWin_Free(wk->msgWindow);
    BmpWin_FreeAllocator();
}

// pokebw2's Guru2ReceiptSeq_Init
static int func_ov137_021dd79c(Guru2ReceiptWork *wk, int procSeq) {
    if (func_02039338() == 0) {
        func_0203d7b0(FALSE);
        if (func_ov022_021841a4(func_ov137_021dcf50(wk)) >= 2) {
            func_ov137_021de4b4(wk, BtlPokeparam_Text_002, 0);
        }
    } else {
        func_0203d7b0(TRUE);
        func_ov137_021de4b4(wk, BtlPokeparam_Text_008, 0);
    }
    func_ov137_021dd93c(wk, GURU2_RECEIPT_SEQ_MAIN);
    return procSeq;
}

// pokebw2's Guru2ReceiptSeq_Main
static int func_ov137_021dd7e4(Guru2ReceiptWork *wk, int procSeq) {
    func_ov137_021dd80c(wk);
    if (func_02039338() == 0) {
        if (func_ov137_021de220(wk) != 1) {
            func_ov137_021de350(wk);
        }
    } else {
        func_ov137_021de350(wk);
    }
    return procSeq;
}

// pokebw2's Guru2Receipt_CheckKeys
static void func_ov137_021dd80c(Guru2ReceiptWork *wk) {
    Guru2CommWork *g2c;

    if (func_ov137_021de7f4() & PAD_BUTTON_A) {
        if (func_02039338() == 0) {
            g2c = wk->g2c;
            if (g2c->joinCount == func_ov137_021de354(wk) && g2c->leavingBits == 0) {
                if (func_ov022_02184114(func_ov137_021dcf50(wk))) {
                    GFL_SndSEPlay(SEQ_SE_DECIDE1);
                    func_ov137_021de4b4(wk, BtlPokeparam_Text_003, 0);
                    func_ov137_021dd93c(wk, GURU2_RECEIPT_SEQ_START_YESNO_INIT);
                }
            } else {
                GFL_SndSEPlay(SEQ_SE_SELECT1);
            }
        }
    } else if (func_ov137_021de7f4() & PAD_BUTTON_B) {
        if (func_02039338() != 0) {
            if (wk->g2c->unk28 == 0) {
                func_ov137_021de4b4(wk, BtlPokeparam_Text_004, 0);
                func_ov137_021dd93c(wk, GURU2_RECEIPT_SEQ_CANCEL_YESNO_INIT);
            } else {
                GFL_SndSEPlay(SEQ_SE_SELECT1);
            }
        } else {
            g2c = wk->g2c;
            if (g2c->joinCount == func_ov022_021841a4(func_ov137_021dcf50(wk)) && g2c->leavingBits == 0) {
                if (func_ov022_02184114(func_ov137_021dcf50(wk))) {
                    func_ov137_021de4b4(wk, BtlPokeparam_Text_004, 0);
                    func_ov137_021dd93c(wk, GURU2_RECEIPT_SEQ_CANCEL_YESNO_INIT);
                    GFL_SndSEPlay(SEQ_SE_CANCEL1);
                }
            } else {
                GFL_SndSEPlay(SEQ_SE_SELECT1);
            }
        }
    } else {
        if (!wk->entryClosed && func_02039338() == 0
            && wk->g2c->joinCount == func_ov022_021841a4(func_ov137_021dcf50(wk))) {
            func_ov022_02184134(func_ov137_021dcf50(wk));
        }
    }
}

// Runs at the end of each sequence
// pokebw2's Guru2Receipt_SeqEnd
static void func_ov137_021dd924(Guru2ReceiptWork *wk) {
}

// pokebw2's Guru2ReceiptSeq_1
static int func_ov137_021dd928(Guru2ReceiptWork *wk, int procSeq) {
    wk->seq = GURU2_RECEIPT_SEQ_2;
    func_ov137_021dd924(wk);
    return procSeq;
}

// pokebw2's Guru2Receipt_SetMsgWait
static void func_ov137_021dd93c(Guru2ReceiptWork *wk, int nextSeq) {
    wk->seq = GURU2_RECEIPT_SEQ_MSG_WAIT;
    wk->nextSeq = nextSeq;
}

// pokebw2's Guru2ReceiptSeq_MsgWait
static int func_ov137_021dd94c(Guru2ReceiptWork *wk, int procSeq) {
    if (func_ov137_021de55c(wk)) {
        wk->seq = wk->nextSeq;
    }
    func_ov137_021dd924(wk);
    return procSeq;
}

// pokebw2's Guru2ReceiptSeq_2
static int func_ov137_021dd970(Guru2ReceiptWork *wk, int procSeq) {
    if (func_02039338() == 0) {
        func_ov022_02184134(func_ov137_021dcf50(wk));
    }
    wk->seq = GURU2_RECEIPT_SEQ_MAIN;
    func_ov137_021dd924(wk);
    return procSeq;
}

// pokebw2's Guru2ReceiptSeq_CancelYesNoInit
static int func_ov137_021dd99c(Guru2ReceiptWork *wk, int procSeq) {
    wk->yesNo = ShopUI_CreateConfirmDialog(&data_ov137_021e1ae8, 31, 11, 0, HEAPID_GURU2);
    wk->seq = GURU2_RECEIPT_SEQ_CANCEL_YESNO;
    func_ov137_021dd924(wk);
    return procSeq;
}

// pokebw2's Guru2ReceiptSeq_CancelYesNo
static int func_ov137_021dd9d4(Guru2ReceiptWork *wk, int procSeq) {
    u32 result;

    if (func_02039338() != 0) {
        if (wk->g2c->unk28 != 0) {
            if (func_ov137_021de7f4() & (PAD_BUTTON_A | PAD_BUTTON_B | PAD_KEY_UP | PAD_KEY_DOWN)) {
                GFL_SndSEPlay(SEQ_SE_SELECT1);
            }
            func_ov137_021dd924(wk);
            return procSeq;
        }
    } else {
        if (wk->g2c->leavingBits != 0) {
            if (func_ov137_021de7f4() & (PAD_BUTTON_A | PAD_BUTTON_B | PAD_KEY_UP | PAD_KEY_DOWN)) {
                GFL_SndSEPlay(SEQ_SE_SELECT1);
            }
            func_ov137_021dd924(wk);
            return procSeq;
        }
    }
    if (func_ov137_021de354(wk) != func_ov022_021841a4(func_ov137_021dcf50(wk))) {
        func_ov137_021dd924(wk);
        return procSeq;
    }
    result = func_0201febc(wk->yesNo);
    if (result != BMPMENU_NULL) {
        if (result == BMPMENU_CANCEL) {
            if (func_02039338() == 0) {
                func_ov022_02184134(func_ov137_021dcf50(wk));
            }
            wk->seq = GURU2_RECEIPT_SEQ_INIT;
        } else {
            if (func_02039338() == 0) {
                func_ov137_021dd93c(wk, GURU2_RECEIPT_SEQ_PARENT_CANCEL_YESNO_INIT);
                func_ov137_021de4b4(wk, BtlPokeparam_Text_014, 0);
            } else {
                Guru2CommSignal signal;

                MI_CpuFill8(&signal, 0, sizeof(signal));
                signal.type = GURU2_SIGNAL_LEAVE_REQUEST;
                signal.netId = func_02039338();
                if (func_ov137_021dcbc4(wk->g2c, GURU2_NET_CMD_SIGNAL, &signal, sizeof(signal))) {
                    wk->cancelSent = TRUE;
                    wk->signalTimer = 0;
                    wk->seq = GURU2_RECEIPT_SEQ_CANCEL_SIGNAL_WAIT;
                } else {
                    wk->seq = GURU2_RECEIPT_SEQ_INIT;
                }
            }
        }
        wk->yesNo = NULL;
    }
    func_ov137_021dd924(wk);
    return procSeq;
}

// pokebw2's Guru2ReceiptSeq_CancelSignalWait
static int func_ov137_021ddaec(Guru2ReceiptWork *wk, int procSeq) {
    wk->signalTimer = 0;
    func_ov137_021dd924(wk);
    return procSeq;
}

// pokebw2's Guru2ReceiptSeq_CancelOk
static int func_ov137_021ddb04(Guru2ReceiptWork *wk, int procSeq) {
    if (wk->g2c->signalCount != func_ov022_021841a4(func_ov137_021dcf50(wk))
        || wk->g2c->signalCount != func_ov137_021de354(wk)) {
        wk->signalTimer = 0;
        wk->seq = GURU2_RECEIPT_SEQ_CANCEL_REFUSED;
        func_ov137_021dd924(wk);
        return procSeq;
    }
    wk->signalTimer++;
    if (wk->signalTimer > 30) {
        Guru2CommSignal signal;

        MI_CpuFill8(&signal, 0, sizeof(signal));
        signal.type = GURU2_SIGNAL_LEAVE;
        signal.netId = func_02039338();
        func_ov137_021dcbc4(wk->g2c, GURU2_NET_CMD_SIGNAL, &signal, sizeof(signal));
        wk->signalTimer = 0;
        wk->seq = GURU2_RECEIPT_SEQ_END_MSG;
    }
    func_ov137_021dd924(wk);
    return procSeq;
}

// pokebw2's Guru2ReceiptSeq_CancelRefused
static int func_ov137_021ddb90(Guru2ReceiptWork *wk, int procSeq) {
    wk->cancelSent = FALSE;
    func_ov137_021dd93c(wk, GURU2_RECEIPT_SEQ_INIT);
    func_ov137_021dd924(wk);
    return procSeq;
}

// pokebw2's Guru2ReceiptSeq_StartYesNoInit
static int func_ov137_021ddbb0(Guru2ReceiptWork *wk, int procSeq) {
    wk->yesNo = ShopUI_CreateConfirmDialog(&data_ov137_021e1ae8, 31, 11, 0, HEAPID_GURU2);
    wk->seq = GURU2_RECEIPT_SEQ_START_YESNO;
    func_ov137_021dd924(wk);
    return procSeq;
}

// pokebw2's Guru2ReceiptSeq_StartYesNo
static int func_ov137_021ddbe8(Guru2ReceiptWork *wk, int procSeq) {
    Guru2CommWork *g2c = wk->g2c;
    u32 result;

    if (g2c->joinCount != func_ov137_021de354(wk) || g2c->leavingBits != 0) {
        if (func_ov137_021de7f4() & (PAD_BUTTON_A | PAD_BUTTON_B | PAD_KEY_UP | PAD_KEY_DOWN)) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
        }
        func_ov137_021dd924(wk);
        return procSeq;
    }
    result = func_0201febc(wk->yesNo);
    if (result != BMPMENU_NULL) {
        if (result == BMPMENU_CANCEL) {
            func_ov022_02184134(func_ov137_021dcf50(wk));
            wk->seq = GURU2_RECEIPT_SEQ_INIT;
        } else if (func_02039338() == 0) {
            wk->seq = GURU2_RECEIPT_SEQ_START_SEND;
            wk->startCount = func_ov137_021de354(wk);
        }
        wk->yesNo = NULL;
    }
    func_ov137_021dd924(wk);
    return procSeq;
}

// pokebw2's Guru2ReceiptSeq_StartSend
static int func_ov137_021ddc7c(Guru2ReceiptWork *wk, int procSeq) {
    if (wk->unk4813 == 0) {
        if (func_ov137_021dcbc4(wk->g2c, GURU2_NET_CMD_START, NULL, 0) == TRUE) {
            wk->seq = GURU2_RECEIPT_SEQ_START;
            func_0203d7bc(TRUE);
        }
    }
    return procSeq;
}

// pokebw2's Guru2ReceiptSeq_StartWait
static int func_ov137_021ddcb4(Guru2ReceiptWork *wk, int procSeq) {
    if (func_02039338() == 0) {
        func_ov022_021841a4(func_ov137_021dcf50(wk));
    }
    if (wk->g2c->startCount == func_ov022_021841a4(func_ov137_021dcf50(wk))) {
        wk->seq = GURU2_RECEIPT_SEQ_START;
    }
    return procSeq;
}

// pokebw2's Guru2ReceiptSeq_EndMsg
static int func_ov137_021ddcec(Guru2ReceiptWork *wk, int procSeq) {
    func_ov137_021de4b4(wk, BtlPokeparam_Text_005, 0);
    func_ov137_021dd93c(wk, GURU2_RECEIPT_SEQ_END_WAIT);
    func_ov137_021dd924(wk);
    return procSeq;
}

// pokebw2's Guru2ReceiptSeq_EndWait
static int func_ov137_021ddd0c(Guru2ReceiptWork *wk, int procSeq) {
    wk->wait++;
    if (wk->wait > 60) {
        GFL_WipeSet(WIPE_MODE_BOTH, WIPE_TYPE_FADE_OUT, WIPE_TYPE_FADE_OUT, WIPE_COLOR_BLACK, 16, 1,
                    HEAPID_GURU2);
        procSeq = GURU2_RECEIPT_PROC_FADE_OUT;
    }
    func_ov137_021dd924(wk);
    return procSeq;
}

// pokebw2's Guru2ReceiptSeq_ParentCancelYesNoInit
static int func_ov137_021ddd48(Guru2ReceiptWork *wk, int procSeq) {
    wk->yesNo = ShopUI_CreateConfirmDialog(&data_ov137_021e1ae8, 31, 11, 0, HEAPID_GURU2);
    wk->seq = GURU2_RECEIPT_SEQ_PARENT_CANCEL_YESNO;
    func_ov137_021dd924(wk);
    return procSeq;
}

// pokebw2's Guru2ReceiptSeq_ParentCancelYesNo
static int func_ov137_021ddd80(Guru2ReceiptWork *wk, int procSeq) {
    Guru2CommWork *g2c = wk->g2c;
    u32 result;

    if (g2c->joinCount != func_ov137_021de354(wk) || g2c->leavingBits != 0) {
        if (func_ov137_021de7f4() & (PAD_BUTTON_A | PAD_BUTTON_B | PAD_KEY_UP | PAD_KEY_DOWN)) {
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
        }
        func_ov137_021dd924(wk);
        return procSeq;
    }
    result = func_0201febc(wk->yesNo);
    if (result != BMPMENU_NULL) {
        if (result == BMPMENU_CANCEL) {
            wk->seq = GURU2_RECEIPT_SEQ_INIT;
            func_ov022_02184134(func_ov137_021dcf50(wk));
        } else {
            wk->seq = GURU2_RECEIPT_SEQ_PARENT_CANCEL_MSG;
            func_ov137_021dcbc4(wk->g2c, GURU2_NET_CMD_CANCEL, NULL, 0);
            copyVarForText(wk->wordSet, 0, &func_ov022_02184180(func_ov137_021dcf50(wk), 0)->info);
        }
        wk->yesNo = NULL;
    }
    func_ov137_021dd924(wk);
    return procSeq;
}

// pokebw2's Guru2ReceiptSeq_ParentCancelMsg
static int func_ov137_021dde24(Guru2ReceiptWork *wk, int procSeq) {
    if (func_02039338() == 0) {
        func_ov137_021de4b4(wk, BtlPokeparam_Text_005, 0);
    } else {
        copyVarForText(wk->wordSet, 0, &func_ov022_02184180(func_ov137_021dcf50(wk), 0)->info);
        func_ov137_021de4b4(wk, BtlPokeparam_Text_016, 0);
    }
    func_ov137_021dd93c(wk, GURU2_RECEIPT_SEQ_PARENT_CANCEL_WAIT);
    wk->wait = 0;
    func_ov137_021dd924(wk);
    return procSeq;
}

// pokebw2's Guru2ReceiptSeq_ParentCancelWait
static int func_ov137_021dde74(Guru2ReceiptWork *wk, int procSeq) {
    wk->wait++;
    if (wk->wait > 45) {
        wk->seq = GURU2_RECEIPT_SEQ_PARENT_CANCEL_SYNC;
    }
    func_ov137_021dd924(wk);
    return procSeq;
}

// pokebw2's Guru2ReceiptSeq_ParentCancelSync
static int func_ov137_021dde94(Guru2ReceiptWork *wk, int procSeq) {
    func_020399a8();
    wk->syncBits = wk->unk480C ^ func_ov137_021de37c(wk);
    func_0203a424(func_020399a8(), 0xcf, 40, (u16)wk->syncBits);
    wk->seq = GURU2_RECEIPT_SEQ_PARENT_CANCEL_SYNC_WAIT;
    func_ov137_021dd924(wk);
    return procSeq;
}

// pokebw2's Guru2ReceiptSeq_ParentCancelSyncWait
static int func_ov137_021ddedc(Guru2ReceiptWork *wk, int procSeq) {
    if (func_02039cb4(func_020399a8(), 0xcf, 40)
        || wk->syncBits != (func_ov137_021de37c(wk) ^ wk->unk480C)) {
        func_0203d7b0(FALSE);
        GFL_WipeSet(WIPE_MODE_BOTH, WIPE_TYPE_FADE_OUT, WIPE_TYPE_FADE_OUT, WIPE_COLOR_BLACK, 16, 1,
                    HEAPID_GURU2);
        procSeq = GURU2_RECEIPT_PROC_FADE_OUT;
    }
    func_ov137_021dd924(wk);
    return procSeq;
}

// pokebw2's Guru2ReceiptSeq_NoMembersMsgWait
static int func_ov137_021ddf38(Guru2ReceiptWork *wk, int procSeq) {
    if (func_ov137_021de55c(wk)) {
        func_ov137_021de4b4(wk, BtlPokeparam_Text_007, 0);
        wk->seq = GURU2_RECEIPT_SEQ_NO_MEMBERS_END;
    }
    func_ov137_021dd924(wk);
    return procSeq;
}

// pokebw2's Guru2ReceiptSeq_NoMembersEnd
static int func_ov137_021ddf64(Guru2ReceiptWork *wk, int procSeq) {
    if (func_ov137_021de55c(wk)) {
        wk->seq = GURU2_RECEIPT_SEQ_END_WAIT;
    }
    func_ov137_021dd924(wk);
    return procSeq;
}

// pokebw2's Guru2ReceiptSeq_MemberLeft
static int func_ov137_021ddf84(Guru2ReceiptWork *wk, int procSeq) {
    func_ov137_021de4b4(wk, BtlPokeparam_Text_015, 1);
    wk->seq = GURU2_RECEIPT_SEQ_MEMBER_LEFT_2;
    func_02039338();
    wk->leftCount = func_ov022_021841a4(func_ov137_021dcf50(wk));
    wk->leftTimer = 300;
    func_ov137_021dd924(wk);
    return procSeq;
}

// pokebw2's Guru2ReceiptSeq_MemberLeft2
static int func_ov137_021ddfc0(Guru2ReceiptWork *wk, int procSeq) {
    if (wk->leftCount != 0 && wk->leftCount != func_ov022_021841a4(func_ov137_021dcf50(wk))) {
        wk->leftCount = 0;
    }
    wk->seq = GURU2_RECEIPT_SEQ_MEMBER_LEFT_WAIT;
    wk->wait = 0;
    func_ov137_021dd924(wk);
    return procSeq;
}

// pokebw2's Guru2ReceiptSeq_MemberLeftWait
static int func_ov137_021ddffc(Guru2ReceiptWork *wk, int procSeq) {
    if (wk->leftCount != 0 && wk->leftCount != func_ov022_021841a4(func_ov137_021dcf50(wk))) {
        wk->leftCount = 0;
    }
    wk->leftTimer--;
    if (wk->leftTimer < 0) {
        wk->leftCount = 0;
    }
    wk->wait++;
    if (wk->wait > 60 && wk->leftCount == 0) {
        wk->seq = GURU2_RECEIPT_SEQ_INIT;
        func_02039338();
    }
    func_ov137_021dd924(wk);
    return procSeq;
}

// pokebw2's Guru2ReceiptSeq_Start
static int func_ov137_021de060(Guru2ReceiptWork *wk, int procSeq) {
    GFL_WipeSet(WIPE_MODE_BOTH, WIPE_TYPE_FADE_OUT, WIPE_TYPE_FADE_OUT, WIPE_COLOR_BLACK, 16, 1, HEAPID_GURU2);
    wk->receiptDone = TRUE;
    func_ov137_021dd924(wk);
    return GURU2_RECEIPT_PROC_FADE_OUT;
}

// pokebw2's Guru2Receipt_SetSeq
void func_ov137_021de08c(Guru2ReceiptWork *wk, int seq, u8 netId) {
    switch (seq) {
    case GURU2_RECEIPT_SEQ_2:
        func_ov137_021de5c0(wk);
        break;
    case GURU2_RECEIPT_SEQ_PARENT_CANCEL_MSG:
        if (wk->yesNo != NULL) {
            func_0201fef4(wk->yesNo);
            wk->yesNo = NULL;
        }
        break;
    case GURU2_RECEIPT_SEQ_START_WAIT:
        func_ov137_021de4b4(wk, BtlPokeparam_Text_012, 0);
        if (wk->yesNo != NULL) {
            func_0201fef4(wk->yesNo);
            wk->yesNo = NULL;
        }
        break;
    case GURU2_RECEIPT_SEQ_MEMBER_LEFT:
        if (wk->cancelSent == TRUE) {
            return;
        }
        copyVarForText(wk->wordSet, 0, &func_ov022_02184180(func_ov137_021dcf50(wk), netId)->info);
        if (netId == func_02039338()) {
            return;
        }
        if (wk->yesNo != NULL) {
            func_0201fef4(wk->yesNo);
            wk->yesNo = NULL;
        }
        if (func_02039338() == 0) {
            wk->g2c->leavingBits &= 0xffff ^ netId;
        }
        break;
    case GURU2_RECEIPT_SEQ_START:
        if (!func_ov137_021de55c(wk)) {
            func_0201ce0c(wk->stream);
        }
        if (wk->yesNo != NULL) {
            func_0201fef4(wk->yesNo);
            wk->yesNo = NULL;
        }
        break;
    case GURU2_RECEIPT_SEQ_CANCEL_OK:
    case GURU2_RECEIPT_SEQ_CANCEL_REFUSED:
        break;
    default:
        return;
    }
    wk->seq = seq;
}

// pokebw2's Guru2Receipt_SetSeqOnJoin
void func_ov137_021de17c(Guru2ReceiptWork *wk, int seq, u8 netId) {
    if (wk->seq == GURU2_RECEIPT_SEQ_MAIN) {
        switch (seq) {
        case GURU2_RECEIPT_SEQ_1: {
            UnionAppMember *member = func_ov022_02184180(func_ov137_021dcf50(wk), netId);

            if (member != NULL) {
                copyVarForText(wk->wordSet, 0, &member->info);
            }
            wk->seq = seq;
            wk->g2c->joinCount = func_ov022_021841a4(func_ov137_021dcf50(wk));
            wk->g2c->leavingBits = 0;
            break;
        }
        case GURU2_RECEIPT_SEQ_MEMBER_LEFT:
            copyVarForText(wk->wordSet, 0, &func_ov022_02184180(func_ov137_021dcf50(wk), netId)->info);
            if (netId == func_02039338()) {
                return;
            }
            if (func_02039338() == 0) {
                wk->g2c->leavingBits &= 0xffff ^ netId;
            }
            wk->seq = seq;
            break;
        }
    } else if (seq == GURU2_RECEIPT_SEQ_1) {
        wk->g2c->leavingBits = 0;
        wk->g2c->joinCount = func_ov022_021841a4(func_ov137_021dcf50(wk));
    }
}

// pokebw2's Guru2Receipt_CountMembers
static int func_ov137_021de220(Guru2ReceiptWork *wk) {
    int i;
    int count = 0;

    for (i = 0; i < GURU2_MEMBER_MAX; i++) {
        if (func_ov022_02184180(func_ov137_021dcf50(wk), i) != NULL) {
            count++;
        }
    }
    return count;
}

// Draws the members' names and IDs again if the members changed, and returns whether they did
// pokebw2's Guru2Receipt_DrawNames
static BOOL func_ov137_021de248(BmpWin *window, int unused, u16 color, Guru2ReceiptWork *wk) {
    int self = func_02039338();
    int i;

    if (!func_ov137_021de388(wk)) {
        return FALSE;
    }
    GFL_BitmapFill(BmpWin_GetBitmap(window), 0);
    for (i = 0; i < GURU2_MEMBER_MAX; i++) {
        if (wk->members[i].member != NULL) {
            u16 id = getTrainerID(&wk->members[i].member->info);
            StrBuf *idBuf = GFL_StrBufCreate(16, HEAPID_GURU2);
            StrBuf *format;
            u16 nameColor;

            textCopy(wk->members[i].member->info.name, wk->names[i]);
            WordSetNumber(wk->wordSet, 0, id, 5, 2, TRUE);
            format = GFL_MsgDataLoadStrbufNew(wk->msgData, BtlPokeparam_Text_001);
            GFL_WordSetFormatStrbuf(wk->wordSet, idBuf, format);
            if (self == i) {
                nameColor = GURU2_RECEIPT_COLOR_SELF;
            } else {
                nameColor = color;
            }
            GFL_TextRendererDrawToBitmapEx(BmpWin_GetBitmap(window), 5, i * 18 + 1, wk->names[i], wk->font, nameColor);
            GFL_TextRendererDrawToBitmapEx(BmpWin_GetBitmap(window), 70, i * 18 + 1, idBuf, wk->font, nameColor);
            GFL_StrBufFree(idBuf);
            GFL_StrBufFree(format);
        }
    }
    BmpWin_TransferNow(window);
    return TRUE;
}

// pokebw2's Guru2Receipt_Dummy
static void func_ov137_021de350(Guru2ReceiptWork *wk) {
}

// pokebw2's Guru2Receipt_CountMembers2
static int func_ov137_021de354(Guru2ReceiptWork *wk) {
    int i;
    int count = 0;

    for (i = 0; i < GURU2_MEMBER_MAX; i++) {
        if (func_ov022_02184180(func_ov137_021dcf50(wk), i) != NULL) {
            count++;
        }
    }
    return count;
}

// pokebw2's Guru2Receipt_GetJoined
static u32 func_ov137_021de37c(Guru2ReceiptWork *wk) {
    return func_ov022_021841a0(func_ov137_021dcf50(wk));
}

// Reads the members, sets what the trainer of each one that changed does, and returns whether any changed
// pokebw2's Guru2Receipt_UpdateMembers
static BOOL func_ov137_021de388(Guru2ReceiptWork *wk) {
    BOOL changed = FALSE;
    int i;

    for (i = 0; i < GURU2_MEMBER_MAX; i++) {
        wk->members[i].prev = wk->members[i].member;
        wk->members[i].member = func_ov022_02184180(func_ov137_021dcf50(wk), i);
        wk->ids[i].prev.parts = wk->ids[i].id.parts;
        if (wk->members[i].member != NULL) {
            wk->ids[i].id.parts.id = getIDAsUInt(&wk->members[i].member->info);
            wk->ids[i].id.parts.present = TRUE;
        } else {
            wk->ids[i].id.value = 0;
        }
    }
    for (i = 0; i < GURU2_MEMBER_MAX; i++) {
        if (wk->members[i].prev != wk->members[i].member) {
            changed = TRUE;
            if (wk->members[i].member == NULL) {
                wk->actorStates[i] = GURU2_RECEIPT_ACTOR_LEAVE;
            } else {
                wk->actorStates[i] = GURU2_RECEIPT_ACTOR_APPEAR;
            }
        } else if (wk->ids[i].id.value != wk->ids[i].prev.value) {
            changed = TRUE;
            if (!wk->ids[i].id.parts.present) {
                wk->actorStates[i] = GURU2_RECEIPT_ACTOR_LEAVE;
            } else {
                wk->actorStates[i] = GURU2_RECEIPT_ACTOR_APPEAR;
            }
        }
    }
    return changed;
}

// pokebw2's Guru2Receipt_PrintMessage
static void func_ov137_021de4b4(Guru2ReceiptWork *wk, u32 messageId, int unused) {
    StrBuf *format = GFL_StrBufCreate(180, HEAPID_GURU2);

    GFL_MsgDataLoadStrbuf(wk->msgData, messageId, format);
    GFL_WordSetFormatStrbuf(wk->wordSet, wk->msgBuf, format);
    GFL_StrBufFree(format);
    GFL_BitmapFill(BmpWin_GetBitmap(wk->msgWindow), 15);
    BmpWin_DrawFrame(wk->msgWindow, FALSE, 1, 10);
    if (wk->stream != NULL) {
        func_0201ce48(wk->stream);
    }
    wk->stream = func_0201cce4(wk->msgWindow, 0, 0, wk->msgBuf, wk->font, 0, wk->tcbManager, 0,
                               HEAPID_GURU2, 15);
    BmpWin_TransferNow(wk->msgWindow);
}

// pokebw2's Guru2Receipt_IsMessageDone
static BOOL func_ov137_021de55c(Guru2ReceiptWork *wk) {
    u32 state;

    if (wk->stream == NULL) {
        return TRUE;
    }
    state = func_0201ce30(wk->stream);
    if (state == PRINT_STREAM_RUNNING) {
        if (GCTX_HIDGetHeldKeys() & PAD_BUTTON_A) {
            func_0201ce5c(wk->stream, 0);
        }
    } else if (state == PRINT_STREAM_PAUSED) {
        if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_A) {
            func_0201ce38(wk->stream);
        }
    } else if (state == PRINT_STREAM_DONE) {
        func_0201ce48(wk->stream);
        wk->stream = NULL;
        return TRUE;
    }
    return FALSE;
}

// pokebw2's Guru2Receipt_ClearMessage
static void func_ov137_021de5c0(Guru2ReceiptWork *wk) {
    func_0201f7a8(wk->msgWindow, FALSE);
}

// pokebw2's Guru2Receipt_UpdateActors
static void func_ov137_021de5d0(Guru2ReceiptWork *wk) {
    int i;
    BOOL appeared = FALSE;

    for (i = 0; i < GURU2_MEMBER_MAX; i++) {
        switch (wk->actorStates[i]) {
        case GURU2_RECEIPT_ACTOR_APPEAR: {
            int gender = getTrainerGender(&wk->members[i].member->info);
            int look = func_02008554(&wk->members[i].member->info);

            if (i == func_02039338()) {
                func_0204b878(wk->actors[i], 4);
            } else {
                func_ov137_021de6ec(wk, wk->trainerPalette, i, look, gender);
                func_0204b878(wk->actors[i], 4);
            }
            func_0204b3dc(wk->actors[i], TRUE);
            wk->actorStates[i] = GURU2_RECEIPT_ACTOR_SHOWN;
            appeared = TRUE;
            break;
        }
        case GURU2_RECEIPT_ACTOR_LEAVE:
            if (i == func_02039338()) {
                getTrainerGender(&wk->members[i].member->info);
                func_0204b878(wk->actors[i], 5);
            } else {
                func_0204b878(wk->actors[i], 5);
            }
            wk->actorStates[i] = GURU2_RECEIPT_ACTOR_NONE;
            break;
        case GURU2_RECEIPT_ACTOR_NONE:
        case GURU2_RECEIPT_ACTOR_SHOWN:
            break;
        }
    }
    if (appeared) {
        GFL_SndSEPlay(SEQ_SE_FLD_05);
    }
}

// pokebw2's Guru2Receipt_LoadTrainerPalette
static void func_ov137_021de6c0(Guru2ReceiptWork *wk, ArcTool *arc) {
    wk->trainerPaletteFile =
        GFL_G2DIOReadNCLR(GURU2_RECEIPT_TRAINER_ARCID, 0, &wk->trainerPalette, HEAPID_GURU2);
}

// pokebw2's Guru2Receipt_FreeTrainerPalette
static void func_ov137_021de6dc(Guru2ReceiptWork *wk) {
    GFL_HeapFree(wk->trainerPaletteFile);
}

// The row of the trainer palette file for each appearance (func_02008554)
// pokebw2's sLookPalettes
static const u8 data_ov137_021e1af8[16] = { 2, 3, 6, 5, 4, 5, 2, 0, 1, 3, 6, 5, 4, 7, 7, 0 };

// Gives a member's actor the palette and characters of the member's appearance
// pokebw2's Guru2Receipt_SetActorLook
static void func_ov137_021de6ec(Guru2ReceiptWork *wk, NNSG2dPaletteData *palette, int id, int look, int gender) {
    NNSG2dCharacterData *character;
    void *file;

    GX_LoadOBJPltt((u8 *)palette->pRawData + data_ov137_021e1af8[look] * 0x20, id * 0x20, 0x20);
    file = GFL_G2DIOReadOBJNCGR(GURU2_RECEIPT_TRAINER_ARCID, look + 1, FALSE, &character, HEAPID_GURU2);
    func_0204a9d0(wk->resources[GURU2_RECEIPT_RES_CHAR + id], character);
    GFL_HeapFree(file);
}

// The parent's check of the members each frame, which returns the proc's next sequence
// pokebw2's Guru2Receipt_CheckMembers
static inline int Guru2Receipt_GetMemberCount(const Guru2ReceiptWork *wk) {
    return wk->memberCount;
}
static int func_ov137_021de738(Guru2ReceiptWork *wk, int unused) {
    int count = func_ov137_021de354(wk);

    if (count > wk->g2c->joinCount) {
        if (func_ov022_02184114(func_ov137_021dcf50(wk))) {
            wk->entryClosed = TRUE;
        }
    } else {
        wk->entryClosed = FALSE;
    }
    if (count == wk->memberCount) {
        return GURU2_RECEIPT_PROC_MAIN;
    }
    switch (count) {
    case 1:
        if (func_ov022_021841a4(func_ov137_021dcf50(wk)) > 1 ||
            func_ov022_021841a0(func_ov137_021dcf50(wk)) > 1) {
            return GURU2_RECEIPT_PROC_MAIN;
        }
        wk->seq = GURU2_RECEIPT_SEQ_NO_MEMBERS_MSG_WAIT;
        if (wk->yesNo != NULL) {
            func_0201fef4(wk->yesNo);
            wk->yesNo = NULL;
        }
        return GURU2_RECEIPT_PROC_MAIN_NO_UPDATE;
    case 2:
    case 3:
    case 4:
        if (count < Guru2Receipt_GetMemberCount(wk)) {
            wk->g2c->joinCount = func_ov022_021841a4(func_ov137_021dcf50(wk));
        }
        break;
    case 0:
    case 5:
        break;
    }
    wk->memberCount = func_ov137_021de354(wk);
    return GURU2_RECEIPT_PROC_MAIN;
}

// pokebw2's Guru2Receipt_GetPressedKeys
static u32 func_ov137_021de7f4(void) {
    return GCTX_HIDGetPressedKeys();
}
