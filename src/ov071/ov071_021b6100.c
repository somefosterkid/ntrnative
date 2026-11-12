// Black's code at ov071 0x021b6100-0x021b6a0c. No name is known: the module and address.

#include "types.h"
#include "app/monolith/monolith_main.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "gfl/msg.h"
#include "gfl/proc.h"
#include "gfl/tcb.h"
#include "gfl/tcbl.h"
#include "nitro/fs.h"
#include "nitro/fx/fx.h"
#include "nitro/gx.h"
#include "nitro/mi.h"
#include "nnsys/g2d.h"
#include "system/wipe.h"
#include "system/game_comm.h"
#include "system/game_system.h"
#include "field/intrude_comm.h"
#include "save/high_link.h"
#include "struct_decls.h"
#include "system/actor_tool.h"
#include "system/app_printsys_common.h"
#include "system/app_menu_common.h"
#include "system/bmp_oam.h"
#include "system/gf_font.h"
#include "system/palanm.h"
#include "system/printsys.h"
#include "system/wordset.h"
#include "gfl/net.h"

struct GameProc {
    const GameProcFunctions *functions;
    s32 overlayId;
    u32 state;
    u32 subState;
    void *param;
    void *work;
    GameProc *parent;
    GameProc *child;
};
struct GameProcManager {
    HeapID heapId;
    s32 unk4;
    GameProc *proc;
    BOOL queued;
    BOOL replace;
    s32 nextOverlayId;
    const GameProcFunctions *nextFunctions;
    void *nextParam;
};
struct ArcTool {
    FSFile file;
    u32 fimgOffset;
    u16 fatbOffset;
    u16 fileCount;
};
struct TCB {
    TCBManager *manager;
    TCB *prev;
    TCB *next;
    u32 priority;
    void *data;
    TCBFunc func;
    BOOL addedDuringUpdate;
};
struct TCBManager {
    u32 count;
    u32 used;
    TCB head;
    TCB **ready;
    TCB *tasks;
    BOOL adding;
    TCB *current;
    TCB *next;
};
typedef struct {
    u16 paletteMask;
    u16 delay : 6;
    u16 cur : 5;
    u16 end : 5;
    u16 color : 15;
    u16 direction : 1;
    u16 step : 5;
    u16 delayCounter : 6;
} FadeControl;
typedef struct {
    u16 *unfaded;
    u16 *faded;
    u32 size;
    FadeControl fade;
} PaletteBuffer;
struct PaletteFade {
    PaletteBuffer buffers[PALFADE_BUFFER_COUNT];
    u16 state : 2;
    u16 activeMask : 14;
    u16 transferMask : 14;
    u16 taskActive : 1;
    u16 transferAll : 1;
    u8 stopRequested;
};
typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3_0 : 7;
    u8 unk3_7 : 1;
    u8 unk4;
} WordSetAttr;
typedef struct {
    WordSetAttr attr;
    StrBuf *strbuf;
} WordSetBuf;
struct WordSet {
    u32 count;
    u32 heapId;
    WordSetBuf *bufs;
    StrBuf *tmp;
};
struct StrBuf {
    u16 size;
    u16 length;
    u32 magic;
    u16 chars[1];
};
struct TCBEx {
    TCBExManager *manager;
    TCBEx *prev;
    TCBEx *next;
    u32 priority;
    void *data;
    TCBExFunc func;
};
struct TCBExManager {
    u32 count;
    u32 dataSize;
    HeapID dataHeapId;
    u32 unkC;
    BOOL removeRequested;
    TCBEx *tasks;
    TCBEx head;
    TCBEx *current;
    TCBEx *ready;
};
typedef struct {
    u16 langCount;
    u16 lineCount;
    u32 maxBlockSize;
    u32 unk8;
    u32 langOffsets[0];
} MsgFileHeader;
typedef struct {
    u32 offset;
    u16 length;
    u16 unk6;
} MsgEntry;
struct MsgData {
    MsgFileHeader *header;
    void *block;
    MsgEntry entry;
    u32 fileOffset;
    ArcTool *arc;
    HeapID heapId;
    u8 lang;
    u8 preloaded;
};
struct ActorPalSlots {
    u32 unk0;
    u8 count[2];
    u8 slots[2][16];
};
struct ClActRenderer {
    NNSG2dRendererInstance renderer;
    NNSG2dRenderSurface *surfaces;
    u32 surfaceCount;
};
struct ClActUnit {
    ClActor *actors;
    ClActor *actorList;
    ClActRenderer *renderer;
    u16 count;
    union {
        struct {
            u8 rendererType : 4;
            u8 unk4 : 4;
        };
        struct {
            u8 : 4;
            u8 active : 1;
            u8 unk5 : 1;
            u8 unk6 : 1;
        };
    };
    u8 priority;
    ClActUnit *next;
    ClActUnit *prev;
};
typedef struct {
    u32 type;
    NNSG2dCellDataBank *cells;
    NNSG2dAnimBankData *anims;
    union {
        struct {
            NNSG2dCellAnimation cellAnim;
            u32 transferHandle;
        };
        struct {
            NNSG2dMultiCellAnimation mcAnim;
            NNSG2dMultiCellDataBank *mcBank;
            NNSG2dAnimBankData *mcAnims;
            void *mcWork;
        };
    };
} ClActAnim;
struct ClActor {
    ClActor *next;
    ClActor *prev;
    ClActUnit *unit;
    ClActorPos pos;
    ClActorPos affineCenter;
    ClActorScale scale;
    NNSG2dImageProxy imgProxy;
    NNSG2dImagePaletteProxy pltProxy;
    fx32 unk54;
    u16 rotation;
    u16 sequence;
    u16 charIdx;
    u16 plttIdx;
    u32 priority : 8;
    u32 unk60_8 : 4;
    u32 palette : 4;
    u32 bgPriority : 2;
    u32 affineMode : 2;
    u32 flip : 2;
    u32 objMode : 2;
    u32 unk60_24 : 1;
    u32 unk60_25 : 1;
    u32 visible : 1;
    u32 playMode : 4;
    u32 unk60_31 : 1;
    u32 unk64_0 : 1;
    ClActAnim anim;
};

typedef struct Ov071Work Ov071Work;
typedef struct Ov071FileList Ov071FileList;

typedef struct {
    u32 palette;
    u32 chars;
    u32 cellAnims;
    u32 fontPalette;
} Ov071ActorRes;

typedef struct {
    u32 unk[6];
} Ov071Entry;

typedef enum Ov071EntrySlot {
    OV071_ENTRY_SLOT_FIRST = 0,
} Ov071EntrySlot;

// What the screens' procs get as their param
typedef struct {
    Ov071FileList *param;
    Ov071Work *work;
    u8 *state;
    u8 unk0c[0x20];
    // The screen to show next
    u8 next;
    // Asks the top screen's proc to end
    u8 endTop;
    u8 exiting;
    u8 unk27;
} Ov071ScreenParam;

// The work of this module: fields only as far as the code reaches them
struct Ov071Work {
    PaletteFade *fade;
    ActorPalSlots *palSlots;
    PrintQueue *printQueue;
    Font *font;
    WordSet *wordSet;
    ClActUnit *clactUnit;
    TCBExManager *tcbMgr;
    MsgData *msgData[4];
    BmpOamSys *bmpOam;
    ArcTool *arc;
    void *passPowerData;
    AppPrintsysCommon printsys;
    Ov071ActorRes actorRes[2];
    Ov071Entry entries[6];
    u8 unkF0;
    u8 unkF1;
    u8 unkF2[6];
    Ov071ScreenParam screen;
    TCB *vblankTcb;
    GameProcManager *topProcMgr;
    GameProcManager *bottomProcMgr;
    u8 current;
    u8 topStarted;
    u8 bottomStarted;
    u8 unk137;
    // The first of the menu bar's characters on BG 7
    u32 barChar;
};

// A list of six file IDs at +4, and a flag at +0x3d, as the caller passes them
struct Ov071FileList {
    u8 unk0[4];
    u8 fileIds[6];
    u8 unkA[0x33];
    u8 enabled;
};

void func_ov071_021b6960(Ov071Work *work);
void func_ov071_021b699c(TCB *tcb, void *data);
void func_ov071_021b68b4(Ov071Work *work);
void func_ov071_021b6808(Ov071Work *work);
void func_ov071_021b677c(Ov071Work *work);
void func_ov071_021b6624(void);
void func_ov071_021b6588(Ov071Work *work);
void func_ov071_021b6538(void);
void func_ov071_021b6524(void);
BOOL func_ov071_021b6100(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov071_021b6250(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov071_021b6458(GameProc *proc, u32 *state, void *param, void *work);

const GameProcFunctions data_ov071_021ba758 = { func_ov071_021b6100, func_ov071_021b6250, func_ov071_021b6458 };

static const BGSysLCDConfig data_ov071_021ba764 = { 1, 0, 0, 0 };

// The procs of each screen: the top screen's and the bottom screen's
typedef struct {
    const GameProcFunctions *top;
    const GameProcFunctions *bottom;
} Ov071ScreenProcs;

const BGSetup data_ov071_021ba794 = { 0, 0, 0x800, 0, 1, 0, 0, 1, 0x8000, 0, 3, 0, 0 };

static const Ov071ScreenProcs data_ov071_021ba774[4] = {
    { &data_ov071_021ba91c, &data_ov071_021baa34 },
    { &data_ov071_021baa40, &data_ov071_021ba8d0 },
    { &data_ov071_021bab88, &data_ov071_021bab04 },
    { &data_ov071_021ba91c, &data_ov071_021baaac },
};

const BGSetup data_ov071_021ba7b4 = { 0, 0, 0x800, 0, 1, 0, 0, 1, 0x8000, 0, 3, 0, 0 };
static const BGSysVRAMConfig data_ov071_021ba7d4 = {
#include "asset/ov071_021b6100_data_ov071_021ba7d4.inc"
};

void func_ov071_021b69ac(Ov071Work *work, Ov071FileList *list);
void func_ov071_021b64d0(void);
void func_ov071_021b652c(void);
void func_ov071_021b6540(Ov071Work *work);
void func_ov071_021b65b4(void);
void func_ov071_021b6644(Ov071Work *work, Ov071Work *res);
void func_ov071_021b6790(Ov071Work *work);
void func_ov071_021b6844(Ov071Work *work);
void func_ov071_021b68e0(Ov071Work *work);

BOOL func_ov071_021b6100(GameProc *proc, u32 *state, void *param, void *work)
{
    Ov071FileList *list = param;
    Ov071Work *wk;

    GX_SetMasterBrightness(-16);
    GXS_SetMasterBrightness(-16);
    GFL_BGSysSetEnabledBGsA(0);
    GFL_BGSysSetEnabledBGsB(0);
    GX_SetVisiblePlane(0);
    GXS_SetVisiblePlane(0);
    GX_SetVisibleWnd(0);
    GXS_SetVisibleWnd(0);
    G2_SetBlendAlpha(4, 8, 15, 15);
    G2S_SetBlendAlpha(4, 8, 15, 15);
    GFL_BGSysSetDisplayLayout(1);
    GFL_HeapCreateChild(1, 0x61, 0x70000);
    wk = GFL_ProcInitSubsystem(proc, sizeof(Ov071Work), 0x61);
    MI_CpuFill8(wk, 0, sizeof(Ov071Work));
    wk->unkF1 = 0x30;
    func_ov071_021b69ac(wk, list);
    wk->arc = GFL_ArcSysCreateFileHandle(0x27, 0x61);
    func_ov071_021b64d0();
    func_ov071_021b652c();
    func_ov071_021b6540(wk);
    func_ov071_021b65b4();
    func_ov071_021b6644(wk, wk);
    func_ov071_021b6790(wk);
    func_ov071_021b6844(wk);
    func_ov071_021b68e0(wk);
    wk->passPowerData = PassPowerData_Create(0x61);
    wk->topProcMgr = CreateGameProcManager(0x61);
    wk->bottomProcMgr = CreateGameProcManager(0x61);
    func_02028470(&wk->printsys, 6);
    wk->vblankTcb = GFL_VBlankTCBAdd(func_ov071_021b699c, wk, 10);
    func_0203d34c(FALSE, 0x61);
    wk->screen.param = list;
    wk->screen.work = wk;
    wk->screen.state = &wk->unkF0;
    return TRUE;
}

BOOL func_ov071_021b6250(GameProc *proc, u32 *state, void *param, void *work)
{
    MonolithParam *monolithParam = param;
    Ov071Work *wk = work;
    GameCommSys *commSys = GSYS_GetGameCommSystem(monolithParam->gsys);
    IntrudeComm *intrude = func_ov010_02158344(commSys);
    BOOL bottom;
    BOOL top;

    switch (*state) {
    case 0:
        GFL_WipeSet(WIPE_MODE_BOTH, WIPE_TYPE_FADE_IN, WIPE_TYPE_FADE_IN, WIPE_COLOR_BLACK, 6, 1, HEAPID_TAIL(0x61));
        (*state)++;
        break;
    case 1:
        if (intrude == NULL || func_0200fb24()) {
            (*state)++;
        } else if (func_ov011_0217440c() == TRUE) {
            (*state)++;
        }
        break;
    case 2:
        if (GFL_WipeIsFinished() == TRUE) {
            (*state)++;
        }
        break;
    case 3:
        if (func_0200fb24() || (intrude == NULL && func_0202648c(commSys)) ||
            (intrude == NULL && GameCommSys_BootCheck(commSys) == 2)) {
            wk->screen.exiting = TRUE;
        }
        if (wk->screen.exiting == FALSE) {
            if (wk->topStarted == FALSE) {
                QueueGameProc(wk->topProcMgr, -1, data_ov071_021ba774[wk->current].top, &wk->screen);
                wk->topStarted = TRUE;
            }
            if (wk->bottomStarted == FALSE) {
                GFL_TextRndUpdateColorIndexLUT(15, 2, 0);
                QueueGameProc(wk->bottomProcMgr, -1, data_ov071_021ba774[wk->current].bottom, &wk->screen);
                wk->bottomStarted = TRUE;
            }
        }
        bottom = GFL_ProcMgrUpdate(wk->bottomProcMgr);
        if (wk->screen.next == 4 || (bottom != TRUE && wk->screen.exiting == TRUE)) {
            wk->screen.endTop = TRUE;
        } else if (wk->current != wk->screen.next &&
                   data_ov071_021ba774[wk->current].top != data_ov071_021ba774[wk->screen.next].top) {
            wk->screen.endTop = TRUE;
        }
        top = GFL_ProcMgrUpdate(wk->topProcMgr);
        if (top == TRUE && bottom == TRUE) {
            break;
        }
        if (wk->screen.exiting == TRUE || wk->screen.next == 4) {
            if (top == FALSE && bottom == FALSE) {
                (*state)++;
            }
            break;
        }
        wk->current = wk->screen.next;
        if (top != TRUE) {
            wk->topStarted = FALSE;
            wk->screen.endTop = FALSE;
        }
        if (bottom != TRUE) {
            wk->bottomStarted = FALSE;
        }
        break;
    case 4:
        GFL_WipeSet(WIPE_MODE_BOTH, WIPE_TYPE_FADE_OUT, WIPE_TYPE_FADE_OUT, WIPE_COLOR_BLACK, 6, 1, HEAPID_TAIL(0x61));
        (*state)++;
        break;
    case 5:
        if (intrude == NULL || func_0200fb24()) {
            (*state)++;
        } else if (func_ov011_0217446c() == TRUE) {
            (*state)++;
        }
        break;
    case 6:
        if (GFL_WipeIsFinished() == TRUE) {
            (*state)++;
        }
        break;
    case 7:
        return TRUE;
    }
    GFL_TCBExMgrUpdate(wk->tcbMgr);
    func_0201c4e4(wk->printQueue);
    func_0204a600();
    return FALSE;
}


BOOL func_ov071_021b6458(GameProc *proc, u32 *state, void *param, void *work)
{
    Ov071Work *wk = work;

    FreeGameProcManager(wk->topProcMgr);
    FreeGameProcManager(wk->bottomProcMgr);
    GFL_TCBRemove(wk->vblankTcb);
    PassPowerData_Free(wk->passPowerData);
    func_ov071_021b6960(wk);
    func_ov071_021b68b4(wk);
    func_ov071_021b6808(wk);
    func_ov071_021b677c(wk);
    func_ov071_021b6624();
    func_ov071_021b6588(wk);
    func_ov071_021b6538();
    func_ov071_021b6524();
    GFL_ArcToolFree(wk->arc);
    reg_G2_BLDCNT = 0;
    reg_G2S_DB_BLDCNT = 0;
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(0x61);
    return TRUE;
}

void func_ov071_021b64d0(void) {
    GFL_BGSysSetVRAMBanks(&data_ov071_021ba7d4);
    GFL_BGSysCreate(0x61);
    GFL_BGSysSetLCDConfig(&data_ov071_021ba764);
    MIi_CpuClear32(0, (void *)HW_BG_VRAM, 0x80000);
    MIi_CpuClear32(0, (void *)HW_DB_BG_VRAM, 0x20000);
    MIi_CpuClear32(0, (void *)HW_OBJ_VRAM, 0x40000);
    MIi_CpuClear32(0, (void *)HW_DB_OBJ_VRAM, 0x20000);
}

void func_ov071_021b6524(void)
{
    GFL_BGSysFree();
}


void func_ov071_021b652c(void) {
    BmpWin_InitAllocator(0x61);
}

void func_ov071_021b6538(void)
{
    BmpWin_FreeAllocator();
}


void func_ov071_021b6540(Ov071Work *work) {
    PaletteFade *fade;

    fade = func_02021518(0x61);
    work->fade = fade;
    func_0202155c(fade, 2, 0x1c0, 0x61);
    func_0202155c(work->fade, 3, 0x1c0, 0x61);
    func_0202155c(work->fade, 0, 0x1c0, 0x61);
    func_0202155c(work->fade, 1, 0x1c0, 0x61);
    func_02021ee4(work->fade, 1);
}

void func_ov071_021b6588(Ov071Work *work) {
    func_020215a0(work->fade, 2);
    func_020215a0(work->fade, 3);
    func_020215a0(work->fade, 0);
    func_020215a0(work->fade, 1);
    func_02021540(work->fade);
}

void func_ov071_021b65b4(void)
{
    int i;

    GFL_BGSysCreateBG(3, &data_ov071_021ba794, 0);
    GFL_BGSysCreateBG(7, &data_ov071_021ba7b4, 0);
    GFL_BGSysFillScrArea(3, 0, 0, 0, 32, 32, 17);
    GFL_BGSysFillScrArea(7, 0, 0, 0, 32, 32, 17);
    for (i = 0; i <= 7; i++) {
        GFL_BGSysSetBGEnabled(i, FALSE);
    }
    GFL_BGSysSetBGEnabled(3, TRUE);
    GFL_BGSysSetBGEnabled(7, TRUE);
}


void func_ov071_021b6624(void) {
    GFL_BGSysSetBGEnabled(3, 0);
    GFL_BGSysSetBGEnabled(7, 0);
    GFL_BGSysReleaseBG(3);
    GFL_BGSysReleaseBG(7);
}

void func_ov071_021b6644(Ov071Work *work, Ov071Work *res) {
    ArcTool *arc;

    func_02021660(res->fade, res->arc, 0x10, 0x61, 0, 0x1c0, 0, 0);
    func_02021660(res->fade, res->arc, 0x11, 0x61, 1, 0x1c0, 0, 0);
    GFL_BGSysLoadArcNCGRStatic(res->arc, 0xf, 3, 0, 0, 1, 0x61);
    loadBGScrToVramByFileNoReserveNegAlign(res->arc, 10, 3, 0, 0, 1, 0x61);
    GFL_BGSysLoadArcNCGRStatic(res->arc, 0xe, 7, 0, 0, 1, 0x61);
    loadBGScrToVramByFileNoReserveNegAlign(res->arc, 4, 7, 0, 0, 1, 0x61);
    func_020215ec(res->fade, 0x17, 5, 0x61, 0, 0, 0xd0, 0);
    func_020215ec(res->fade, 0x17, 5, 0x61, 1, 0, 0xd0, 0);
    arc = GFL_ArcSysCreateFileHandle(0x53, 0x61);
    work->barChar = GFL_BGSysAllocChar(7, 0x80, 1);
    GFL_BGSysLoadArcNCGRStatic(arc, 0x1c, 7, work->barChar, 0x80, 0, 0x61);
    func_02027634(arc, 7, 0x61, work->barChar, 0xc);
    func_020216b4(res->fade, arc, 0x1b, 0x61, 1, 0x20, 0xc0);
    GFL_ArcToolFree(arc);
    GFL_BGSysLoadScr(7);
    GFL_BGSysLoadScr(3);
    GFL_BGSysLoadScr(7);
}

void func_ov071_021b677c(Ov071Work *work) {
    GFL_BGSysFreeCharMemory(7, work->barChar, 0x80);
}

void func_ov071_021b6790(Ov071Work *work) {
    GFL_TextRndUpdateColorIndexLUT(0xf, 2, 0);
    work->tcbMgr = GFL_TCBExMgrCreate(0x61, 0x61, 4, 0x20);
    work->font = GFL_FontCreate(0x17, 0, 0, 0, 0x61);
    work->printQueue = func_0201c440(0x61);
    work->wordSet = GFL_WordSetSystemCreateDefault(0x61);
    work->msgData[0] = GFL_MsgSysLoadData(0, 2, 0x43, 0x61);
    work->msgData[1] = GFL_MsgSysLoadData(0, 2, 0xa0, 0x61);
    work->msgData[2] = GFL_MsgSysLoadData(0, 2, 0xa1, 0x61);
    work->msgData[3] = GFL_MsgSysLoadData(0, 2, 0x45, 0x61);
}

void func_ov071_021b6808(Ov071Work *work) {
    GFL_MsgDataFree(work->msgData[0]);
    GFL_MsgDataFree(work->msgData[1]);
    GFL_MsgDataFree(work->msgData[2]);
    GFL_MsgDataFree(work->msgData[3]);
    GFL_WordSetSystemFree(work->wordSet);
    func_0201c6ec(work->printQueue);
    func_0201c4c0(work->printQueue);
    GFL_FontFree(work->font);
    GFL_TCBExMgrFree(work->tcbMgr);
}

void func_ov071_021b6844(Ov071Work *work)
{
    ClActSysSetup setup = data_020a1448;

    setup.oamStartMain = 4;
    setup.oamCountMain = 124;
    setup.oamStartSub = 4;
    setup.oamCountSub = 124;
    setup.charCount = 96;
    setup.cellAnimCount = 64;
    ClActSys_Create(&setup, &data_ov071_021ba7d4, 0x61);
    work->clactUnit = func_0204b100(96, 0, 0x61);
    func_0204b270(work->clactUnit);
    work->palSlots = func_02025318(0x61, 16, 16);
    work->bmpOam = func_02025538(0x61, work->clactUnit);
}


void func_ov071_021b68b4(Ov071Work *work) {
    GFL_BGSysSetBGEnabledA(0x10, 0);
    GFL_BGSysSetBGEnabledB(0x10, 0);
    func_02025588(work->bmpOam);
    func_0204b1cc(work->clactUnit);
    func_0204a5ac();
    func_02025358(work->palSlots);
}

void func_ov071_021b68e0(Ov071Work *work)
{
    ArcTool *fontArc = GFL_ArcSysCreateFileHandle(0x17, 0x61);
    s32 i;

    for (i = 0; i < 2; i++) {
        BOOL sub = FALSE;

        if (i != 0) {
            sub = TRUE;
        }
        work->actorRes[i].palette = func_02025470(work->fade, work->palSlots, work->arc, 3, sub, 3, 0x61);
        work->actorRes[i].chars = func_0204a6c8(work->arc, 0, TRUE, sub, 0x61);
        work->actorRes[i].cellAnims = func_0204af28(work->arc, 1, 2, 0x61);
        work->actorRes[i].fontPalette = func_020254bc(work->fade, work->palSlots, fontArc, 5, sub, 1, 0x61);
    }
    GFL_ArcToolFree(fontArc);
}

void func_ov071_021b6960(Ov071Work *work)
{
    s32 i;

    for (i = 0; i < 2; i++) {
        BOOL sub = FALSE;

        if (i != 0) {
            sub = TRUE;
        }
        func_02025518(work->palSlots, work->actorRes[i].palette, sub);
        func_0204a8d4(work->actorRes[i].chars);
        func_0204afd8(work->actorRes[i].cellAnims);
        func_02025518(work->palSlots, work->actorRes[i].fontPalette, sub);
    }
}



void func_ov071_021b699c(TCB *tcb, void *data) {
    Ov071Work *work = data;

    func_0204a648();
    func_02021d50(work->fade);
}

void func_ov071_021b69ac(Ov071Work *work, Ov071FileList *list)
{
    Ov071EntrySlot i;

    if (list->enabled != 0) {
        Ov071Entry *src = GFL_ArcSysReadHeapNew(0x25, 0, 0x61);
        u32 length = GFL_ArcSysGetDataLength(0x25, 0);

        for (i = 0; i < 6; i++) {
            u8 fileId = list->fileIds[i];

            if (fileId >= length) {
                fileId = 0;
            }
            work->entries[i] = src[fileId];
        }
        GFL_HeapFree(src);
    }
}

