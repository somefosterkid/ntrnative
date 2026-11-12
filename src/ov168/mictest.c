// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov253/mictest.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/mictest.h"
#include "app/title.h"
#include "constants/arc.h"
#include "constants/sound.h"
#include "constants/text_banks.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/graphics.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/math_util.h"
#include "gfl/msg.h"
#include "gfl/net.h"
#include "gfl/overlay.h"
#include "gfl/proc.h"
#include "gfl/random.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/tcb.h"
#include "gfl/touchpanel.h"
#include "gfl/ui.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_vec.h"
#include "nitro/gx.h"
#include "nitro/spi/ARM9/mic.h"
#include "nitro/os.h"
#include "nnsys/gfd.h"
#include "system/app_menu_common.h"
#include "system/gf_font.h"
#include "system/printsys.h"
#include "system/wipe.h"
#include "gfl/bmp.h"
#include "gfl/str.h"

// The microphone test, which the title screen starts by holding Down, X and Y: the player speaks into the
// microphone, and the louder, the more the speaker on the screen shakes and the more notes it sends up. The Back
// button on the lower screen returns to the title screen

// The microphone test's graphics, archive 107
#define ARCID_MIC_TEST 106

#define BG_COUNT 5
#define WINDOW_COUNT 3
#define ACTOR_COUNT 7
#define NOTE_COUNT 3
// The first of the actors that are notes
#define ACTOR_NOTE_FIRST 4
#define ACTOR_BACK_BUTTON 3
#define ACTOR_CENTER 2

// The frames between the notes that a sound sends up
#define NOTE_INTERVAL 10

// The first resource of each group of an actor's resources in MicTestWork.resources: its characters, its palette and
// its cell animations
#define RES_SPEAKER 0
#define RES_CENTER 3
#define RES_NOTE 6
#define RES_BACK_BUTTON 9

typedef struct MicTestSeq MicTestSeq;
typedef void (*MicTestSeqFunc)(MicTestSeq *seq, u32 *state);

// Runs a function once a frame with its state, until it halts
struct MicTestSeq {
    void *work;
    MicTestSeqFunc func;
    u32 state;
    BOOL halted;
};

typedef struct {
    BmpWin *windows[WINDOW_COUNT];
    MsgData *msgData;
    Font *font;
} MicTestBG;

// A MICAutoParam with what the test keeps of the microphone's samples
typedef struct {
    MICAutoParam param;
    u8 *buffer;
    // The loudest sample that the speaker shows, which falls with time
    int peak;
    BOOL newPeak;
    BOOL sampling;
    // Whether to start again when the console wakes up
    BOOL resume;
    BOOL asleep;
    BOOL wasOpen;
} MicTestMic;

// A note, which flies from the speaker, swaying
typedef struct {
    ClActor *actor;
    u32 delay;
    VecFx32 position;
    VecFx32 velocity;
    fx32 phase;
    fx32 phaseStep;
    int swayRadius;
    TCB *task;
} MicTestNote;

typedef struct {
    ClActor *actors[ACTOR_COUNT];
    ClActUnit *unit;
    u32 resources[12];
    MicTestNote notes[NOTE_COUNT];
    u32 spawnTimer;
    MicTestBG bg;
    MicTestMic mic;
    MicTestSeq seq;
    void *tcbMemory;
    TCBManager *tcbManager;
    TCB *vblankTask;
    u32 heapId;
} MicTestWork;

typedef struct {
    u8 bg;
    u8 x;
    u8 y;
    u8 width;
    u8 height;
    u8 palette;
    u16 charPos;
    u8 textX;
    u8 unused[3];
} MicTestWindowSetup;

typedef struct {
    BGSetup setup;
    u8 bg;
    u8 mode;
} MicTestBGSetup;

typedef struct {
    u32 resource;
    u32 surface;
    u32 setup;
} MicTestActorSetup;

BOOL func_ov168_02183b60(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov168_02183ccc(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov168_02183c50(GameProc *proc, u32 *state, void *param, void *work);
void func_ov168_02183d0c(MicTestSeq *seq, void *work, MicTestSeqFunc func);
static void func_ov168_02183d1c(MicTestSeq *seq);
void func_ov168_02183d34(MicTestSeq *seq, MicTestSeqFunc func);
void *func_ov168_02183d3c(MicTestSeq *seq);
static BOOL func_ov168_02183d40(MicTestSeq *seq);
void func_ov168_02183d44(MicTestSeq *seq);
void func_ov168_02183d4c(MicTestSeq *seq, u32 *state);
void func_ov168_02183dac(MicTestSeq *seq, u32 *state);
void func_ov168_02183e18(MicTestSeq *seq, u32 *state);
void func_ov168_02183e60(MicTestSeq *seq, u32 *state);
void func_ov168_02183ec0(MicTestSeq *seq, u32 *state);
void func_ov168_02183ed4(HeapID heapId);
void func_ov168_02183f08(void);
void func_ov168_02183f30(void);
void func_ov168_02183f40(TCB *tcb, void *data);
void func_ov168_02183f6c(MicTestWork *wk, u32 heapId);
void func_ov168_02183f98(MicTestWork *wk);
static void func_ov168_02183fa0(MicTestWork *wk);
void func_ov168_02183fc4(MicTestWork *wk, HeapID heapId);
void func_ov168_02184138(MicTestWork *wk);
void func_ov168_02184144(MicTestWork *wk);
void func_ov168_0218414c(MicTestBG *bg, u32 heapId);
void func_ov168_021841c8(MicTestBG *bg);
static void func_ov168_021841f4(MicTestBG *bg);
void func_ov168_021841f8(MicTestBG *bg, u32 heapId);
void func_ov168_021842b4(MicTestBG *bg);
void func_ov168_021842bc(MicTestBG *bg, u32 heapId);
void func_ov168_02184360(MicTestBG *bg);
void func_ov168_02184398(MicTestMic *mic, u32 heapId, MICCallback callback, void *arg);
void func_ov168_02184408(MicTestMic *mic);
static void func_ov168_0218441c(MicTestMic *mic);
static void func_ov168_02184470(MicTestMic *mic);
static void func_ov168_0218448c(MicTestMic *mic);
int func_ov168_0218449c(MicTestMic *mic);
void func_ov168_021844c8(MICResult result, void *arg);
u32 func_ov168_021845ac(u8 peak);
BOOL func_ov168_021845e0(MicTestWork *wk);
BOOL func_ov168_02184600(MicTestWork *wk, u32 heapId, s16 x, s16 y, TCBManager *tcbManager);
void func_ov168_02184680(TCBManager *tcbManager, ClActor *actor, MicTestNote *note, s16 x, s16 y, fx32 speed,
                              int angle, int swayRadius, u32 delay);
void func_ov168_02184780(TCB *tcb, void *data);
void func_ov168_02184818(MicTestWork *wk);
s32 func_ov168_0218483c(void);
BOOL func_ov168_0218484c(void);

// pokebw2's MIC_TEST_PROC_FUNCTIONS
// pokebw2's MicTest_Exit
// pokebw2's MicTest_Init
// pokebw2's MicTest_Main
const GameProcFunctions data_ov168_02184904 = { func_ov168_02183b60, func_ov168_02183ccc, func_ov168_02183c50 };

// pokebw2's sMicTestBufferFullCount
static u32 data_ov168_02184a40;

// pokebw2's sMicTestWindowColors
static const u16 data_ov168_02184868[WINDOW_COUNT] = { 0x044f, 0x044f, 0x3c40 };

// The Back button
// pokebw2's sMicTestExitRects
static const TouchRect data_ov168_0218486e[2] = {
    { 160, 192, 176, 0 },
    { TOUCH_RECT_END, 0, 0, 0 },
};

// pokebw2's sMicTestLCDConfig
static const BGSysLCDConfig data_ov168_02184878 = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_2D };

// pokebw2's sMicTestWindowSetups
static const MicTestWindowSetup data_ov168_02184888[WINDOW_COUNT] = {
    { 0, 0, 1, 10, 3, 13, 1, 0, { 0 } },
    { 4, 5, 6, 22, 8, 13, 1, 0, { 0 } },
    { 4, 23, 21, 6, 2, 13, 177, 0, { 0 } },
};

// pokebw2's sMicTestActorSetups
static const ClActorSetup data_ov168_021848ac[5] = {
    { 64, 96, 0, 0, 0 }, { 192, 96, 0, 0, 0 }, { 128, 96, 0, 0, 0 }, { 216, 176, 0, 0, 1 }, { 0, 0, 0, 0, 0 },
};

// pokebw2's sMicTestVRAMConfig
static const BGSysVRAMConfig data_ov168_021848d4 = {
    GX_VRAM_BG_128_A,  GX_VRAM_BGEXTPLTT_NONE,  GX_VRAM_SUB_BG_128_C,       GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_128_B, GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_16_I,       GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_NONE,  GX_VRAM_TEXPLTT_NONE,    GX_OBJVRAMMODE_CHAR_1D_32K, GX_OBJVRAMMODE_CHAR_1D_32K,
};

// pokebw2's sMicTestActors
static const MicTestActorSetup data_ov168_02184910[ACTOR_COUNT] = {
    { RES_SPEAKER, CLACT_SURFACE_MAIN, 0 }, { RES_SPEAKER, CLACT_SURFACE_MAIN, 1 },
    { RES_CENTER, CLACT_SURFACE_MAIN, 2 },  { RES_BACK_BUTTON, CLACT_SURFACE_SUB, 3 },
    { RES_NOTE, CLACT_SURFACE_MAIN, 4 },    { RES_NOTE, CLACT_SURFACE_MAIN, 4 },
    { RES_NOTE, CLACT_SURFACE_MAIN, 4 },
};

// pokebw2's sMicTestBGSetups
static const MicTestBGSetup data_ov168_02184964[BG_COUNT] = {
    { { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xf800, GX_BG_CHARBASE_0x00000, 0x4000,
        GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE },
      0,
      BGMODE_TEXT },
    { { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xf000, GX_BG_CHARBASE_0x04000, 0x4000,
        GX_BG_EXTPLTT_01, 1, GX_BG_AREAOVER_XLU, FALSE },
      1,
      BGMODE_TEXT },
    { { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xe800, GX_BG_CHARBASE_0x08000, 0x4000,
        GX_BG_EXTPLTT_01, 2, GX_BG_AREAOVER_XLU, FALSE },
      2,
      BGMODE_TEXT },
    { { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xf800, GX_BG_CHARBASE_0x00000, 0x4000,
        GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE },
      4,
      BGMODE_TEXT },
    { { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xf000, GX_BG_CHARBASE_0x04000, 0x4000,
        GX_BG_EXTPLTT_01, 1, GX_BG_AREAOVER_XLU, FALSE },
      5,
      BGMODE_TEXT },
};




#define HEAPID_MICTEST 0x49

BOOL func_ov168_02183b60(GameProc *proc, u32 *state, void *param, void *work) {
    MicTestWork *wk;

    GFL_HeapCreateChild(HEAPID_USER, HEAPID_MICTEST, 0x70000);
    wk = GFL_ProcInitSubsystem(proc, sizeof(MicTestWork), HEAPID_MICTEST);
    MI_CpuFill8(wk, 0, sizeof(MicTestWork));
    wk->heapId = HEAPID_MICTEST;
    func_ov168_02183ed4(HEAPID_MICTEST);
    func_ov168_02183f30();
    wk->tcbMemory = GFL_HeapAllocate(wk->heapId, GFL_TCBMgrCalcAllocSize(5), FALSE, "mictest.c", 541);
    wk->tcbManager = GFL_TCBMgrCreate(5, wk->tcbMemory);
    wk->vblankTask = GFL_VBlankTCBAdd(func_ov168_02183f40, wk, 1);
    func_ov168_0218414c(&wk->bg, wk->heapId);
    func_ov168_021841f8(&wk->bg, wk->heapId);
    func_ov168_021842bc(&wk->bg, wk->heapId);
    func_ov168_02183f6c(wk, wk->heapId);
    func_ov168_02183fc4(wk, wk->heapId);
    func_ov168_02184398(&wk->mic, wk->heapId, func_ov168_021844c8, wk);
    func_ov168_02183d0c(&wk->seq, wk, func_ov168_02183d4c);
    GFL_SndBGMFadeOut(10);
    return TRUE;
}

BOOL func_ov168_02183c50(GameProc *proc, u32 *state, void *param, void *work) {
    MicTestWork *wk = work;
    HeapID heapId;

    func_ov168_02184408(&wk->mic);
    GFL_TCBRemove(wk->vblankTask);
    func_02030da4(wk->tcbManager);
    GFL_HeapFree(wk->tcbMemory);
    func_ov168_02184138(wk);
    func_ov168_02183f98(wk);
    func_ov168_02184360(&wk->bg);
    func_ov168_021841c8(&wk->bg);
    func_ov168_02183f08();
    heapId = wk->heapId;
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(heapId);
    GFL_SndBGMFadeIn(10);
    GCTX_ProcMgrReplaceProc(OVERLAY_ID(88), &data_ov088_021bd574, NULL);
    return TRUE;
}

static BOOL func_ov168_02183ccc(GameProc *proc, u32 *state, void *param, void *work) {
    MicTestWork *wk = work;

    if (func_ov168_02183d40(&wk->seq)) {
        return TRUE;
    }
    func_ov168_02183d1c(&wk->seq);
    func_ov168_02183fa0(wk);
    func_ov168_021841f4(&wk->bg);
    func_ov168_0218441c(&wk->mic);
    GFL_TCBMgrUpdate(wk->tcbManager);
    return FALSE;
}

// pokebw2's MicTestSeq_Init
void func_ov168_02183d0c(MicTestSeq *seq, void *work, MicTestSeqFunc func) {
    seq->work = work;
    seq->halted = FALSE;
    func_ov168_02183d34(seq, func);
}

// pokebw2's MicTestSeq_Run
static void func_ov168_02183d1c(MicTestSeq *seq) {
    if (!func_ov168_02183d40(seq)) {
        seq->func(seq, &seq->state);
    }
}

// pokebw2's MicTestSeq_SetFunc
void func_ov168_02183d34(MicTestSeq *seq, MicTestSeqFunc func) {
    seq->func = func;
    seq->state = 0;
}

// pokebw2's MicTestSeq_GetWork
void *func_ov168_02183d3c(MicTestSeq *seq) {
    return seq->work;
}

// pokebw2's MicTestSeq_IsHalted
static BOOL func_ov168_02183d40(MicTestSeq *seq) {
    return seq->halted;
}

// pokebw2's MicTestSeq_Halt
void func_ov168_02183d44(MicTestSeq *seq) {
    seq->halted = TRUE;
}

// pokebw2's MicTest_SeqWipeIn
void func_ov168_02183d4c(MicTestSeq *seq, u32 *state) {
    MicTestWork *wk = func_ov168_02183d3c(seq);

    switch (*state) {
    case 0:
        GFL_WipeSet(0, 1, 1, 0, 6, 1, wk->heapId);
        *state = 1;
        break;
    case 1:
        if (GFL_WipeIsFinished()) {
            *state = 2;
        }
        break;
    case 2:
        func_ov168_02183d34(seq, func_ov168_02183dac);
        break;
    }
}

// pokebw2's MicTest_SeqListen
void func_ov168_02183dac(MicTestSeq *seq, u32 *state) {
    MicTestWork *wk = func_ov168_02183d3c(seq);

    switch (*state) {
    case 0:
        func_ov168_02184470(&wk->mic);
        *state = 1;
        break;
    case 1:
        if (func_ov168_0218484c()) {
            if (func_02006d94()) {
                GFL_SndSEPlay(SEQ_SE_CANCEL1);
                *state = 2;
            }
        }
        break;
    case 2:
        func_ov168_0218448c(&wk->mic);
        *state = 3;
        break;
    case 3:
        func_ov168_02183d34(seq, func_ov168_02183e18);
        break;
    }
}

// pokebw2's MicTest_SeqPressBack
void func_ov168_02183e18(MicTestSeq *seq, u32 *state) {
    MicTestWork *wk = func_ov168_02183d3c(seq);

    switch (*state) {
    case 0:
        func_0204b878(wk->actors[ACTOR_BACK_BUTTON], 1);
        *state = 1;
        break;
    case 1:
        if (!func_0204b98c(wk->actors[ACTOR_BACK_BUTTON])) {
            *state = 2;
        }
        break;
    case 2:
        func_ov168_02183d34(seq, func_ov168_02183e60);
        break;
    }
}

// pokebw2's MicTest_SeqWipeOut
void func_ov168_02183e60(MicTestSeq *seq, u32 *state) {
    MicTestWork *wk = func_ov168_02183d3c(seq);

    switch (*state) {
    case 0:
        GFL_WipeSet(0, 0, 0, 0, 6, 1, wk->heapId);
        *state = 1;
        break;
    case 1:
        if (GFL_WipeIsFinished()) {
            *state = 2;
        }
        break;
    case 2:
        func_ov168_02183d34(seq, func_ov168_02183ec0);
        break;
    }
}

// pokebw2's MicTest_SeqEnd
void func_ov168_02183ec0(MicTestSeq *seq, u32 *state) {
    func_ov168_02184818(func_ov168_02183d3c(seq));
    func_ov168_02183d44(seq);
}

// pokebw2's MicTest_InitDisplay
void func_ov168_02183ed4(HeapID heapId) {
    GFL_BGSysDisableAllA();
    GFL_BGSysDisableAllB();
    GX_SetVisiblePlane(0);
    GXS_SetVisiblePlane(0);
    func_02022224(0);
    func_02022224(1);
}

// pokebw2's MicTest_ExitDisplay
void func_ov168_02183f08(void) {
    GFL_BGSysDisableAllA();
    GFL_BGSysDisableAllB();
    GX_SetVisiblePlane(0);
    GXS_SetVisiblePlane(0);
}

// pokebw2's MicTest_SetVRAM
void func_ov168_02183f30(void) {
    GFL_BGSysSetVRAMBanks(&data_ov168_021848d4);
}

// pokebw2's MicTest_VBlank
void func_ov168_02183f40(TCB *tcb, void *data) {
    MicTestWork *wk = data;

    NNS_GfdDoVramTransfer();
    func_ov168_02184144(wk);
    func_ov168_021842b4(&wk->bg);
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

// pokebw2's MicTest_InitClAct
void func_ov168_02183f6c(MicTestWork *wk, u32 heapId) {
    ClActSys_Create(&data_020a1448, &data_ov168_021848d4, heapId);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_OBJ, TRUE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
}

// pokebw2's MicTest_FreeClAct
void func_ov168_02183f98(MicTestWork *wk) {
    func_0204a5ac();
}

// pokebw2's MicTest_UpdateClAct
static void func_ov168_02183fa0(MicTestWork *wk) {
    u16 i;

    for (i = 0; i < ACTOR_COUNT; i++) {
        func_0204b8d4(wk->actors[i], FX32_ONE);
    }
    func_0204a600();
}

// pokebw2's MicTest_CreateActors
void func_ov168_02183fc4(MicTestWork *wk, HeapID heapId) {
    u16 i;
    ArcTool *arc;
    ArcTool *commonArc;

    wk->unit = func_0204b100(32, 0, heapId);
    arc = GFL_ArcSysCreateFileHandle(ARCID_MIC_TEST, heapId);
    commonArc = GFL_ArcSysCreateFileHandle(getUINarcIdx(), heapId);
    wk->resources[RES_SPEAKER + 1] = func_0204abf0(arc, 6, 0, 0, heapId);
    wk->resources[RES_SPEAKER] = func_0204a6c8(arc, 7, 0, 0, heapId);
    wk->resources[RES_SPEAKER + 2] = func_0204af28(arc, 8, 9, heapId);
    wk->resources[RES_CENTER + 1] = func_0204abf0(arc, 10, 0, 32, heapId);
    wk->resources[RES_CENTER] = func_0204a6c8(arc, 11, 0, 0, heapId);
    wk->resources[RES_CENTER + 2] = func_0204af28(arc, 12, 13, heapId);
    wk->resources[RES_NOTE + 1] = func_0204abf0(arc, 14, 0, 64, heapId);
    wk->resources[RES_NOTE] = func_0204a6c8(arc, 15, 0, 0, heapId);
    wk->resources[RES_NOTE + 2] = func_0204af28(arc, 16, 17, heapId);
    wk->resources[RES_BACK_BUTTON + 1] = func_0204abf0(commonArc, 31, 1, 0, heapId);
    wk->resources[RES_BACK_BUTTON] = func_0204a6c8(commonArc, 32, 0, 1, heapId);
    wk->resources[RES_BACK_BUTTON + 2] = func_0204af28(arc, 18, 19, heapId);
    GFL_ArcToolFree(commonArc);
    GFL_ArcToolFree(arc);
    for (i = 0; i < ACTOR_COUNT; i++) {
        const MicTestActorSetup *actor = &data_ov168_02184910[i];

        wk->actors[i] = func_0204b294(wk->unit, wk->resources[actor->resource], wk->resources[actor->resource + 1],
                                      wk->resources[actor->resource + 2], &data_ov168_021848ac[actor->setup],
                                      (u16)actor->surface, heapId);
        func_0204b92c(wk->actors[i], TRUE);
    }
    for (i = 0; i < NOTE_COUNT; i++) {
        func_0204b3dc(wk->actors[ACTOR_NOTE_FIRST + i], FALSE);
    }
    func_0204b570(wk->actors[0], 0);
    func_0204b604(wk->actors[0], 1, 1);
    wk->spawnTimer = NOTE_INTERVAL;
}

// pokebw2's MicTest_FreeActors
void func_ov168_02184138(MicTestWork *wk) {
    func_0204b1cc(wk->unit);
}

// pokebw2's MicTest_VBlankClAct
void func_ov168_02184144(MicTestWork *wk) {
    func_0204a648();
}

// pokebw2's MicTest_InitBG
void func_ov168_0218414c(MicTestBG *bg, u32 heapId) {
    u32 i;

    GFL_BGSysCreate(heapId);
    BmpWin_InitAllocator(heapId);
    GFL_BGSysSetLCDConfig(&data_ov168_02184878);
    bg->font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, heapId);
    for (i = 0; i < BG_COUNT; i++) {
        u8 bgNum = data_ov168_02184964[i].bg;

        GFL_BGSysCreateBG(bgNum, &data_ov168_02184964[i].setup, data_ov168_02184964[i].mode);
        GFL_BGSysClearScr(bgNum);
        GFL_BGSysClearCharCore(bgNum, 0x20, 0, heapId);
        GFL_BGSysSetBGEnabled(bgNum, TRUE);
    }
}

// pokebw2's MicTest_FreeBG
void func_ov168_021841c8(MicTestBG *bg) {
    u32 i;

    for (i = 0; i < BG_COUNT; i++) {
        GFL_BGSysReleaseBG(data_ov168_02184964[i].bg);
    }
    BmpWin_FreeAllocator();
    GFL_BGSysFree();
}

// pokebw2's MicTest_UpdateBG
static void func_ov168_021841f4(MicTestBG *bg) {
}

// pokebw2's MicTest_LoadGraphics
void func_ov168_021841f8(MicTestBG *bg, u32 heapId) {
    GFL_BGSysLoadNCLRDefault(ARCID_MIC_TEST, 0, 0, 0, 0x20, heapId);
    GFL_BGSysLoadNCLRDefault(ARCID_MIC_TEST, 1, 4, 0, 0x20, heapId);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, 0, 0x1a0, 0x20, heapId);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, 4, 0x1a0, 0x20, heapId);
    GFL_BGSysLoadNCGRStatic(ARCID_MIC_TEST, 4, 5, 0, 0, FALSE, heapId);
    loadBGScrToVramByNarcNoReserveNegAlign(ARCID_MIC_TEST, 5, 5, 0, 0, FALSE, heapId);
    GFL_BGSysLoadNCGRStatic(ARCID_MIC_TEST, 2, 1, 0, 0, FALSE, heapId);
    loadBGScrToVramByNarcNoReserveNegAlign(ARCID_MIC_TEST, 3, 1, 0, 0, FALSE, heapId);
}

// pokebw2's MicTest_VBlankBG
void func_ov168_021842b4(MicTestBG *bg) {
    GFL_BGSysUpdate();
}

// pokebw2's MicTest_CreateWindows
void func_ov168_021842bc(MicTestBG *bg, u32 heapId) {
    int i;

    bg->msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_INTRO_BTL_SERVER_FLOW, heapId);
    for (i = 0; i < WINDOW_COUNT; i++) {
        const MicTestWindowSetup *setup = &data_ov168_02184888[i];
        GFLBitmap *bitmap;
        StrBuf *str;
        BmpWin *window;

        bg->windows[i] = BmpWin_CreateStatic(setup->bg, setup->x, setup->y, setup->width, setup->height, setup->palette,
                                             setup->charPos);
        bitmap = BmpWin_GetBitmap(bg->windows[i]);
        GFL_BitmapFill(bitmap, 0);
        func_020456f8(bg->windows[i]);
        str = GFL_MsgDataLoadStrbufNew(bg->msgData, i);
        GFL_TextRendererDrawToBitmapEx(bitmap, setup->textX, 0, str, bg->font, data_ov168_02184868[i]);
        GFL_StrBufFree(str);
        window = bg->windows[i];
        BmpWin_FlushChar(window);
        BmpWin_FlushMap(window);
        GFL_BGSysLoadScr(BmpWin_GetBGIndex(window));
    }
}

// pokebw2's MicTest_FreeWindows
void func_ov168_02184360(MicTestBG *bg) {
    int i;

    for (i = 0; i < WINDOW_COUNT; i++) {
        GFL_BitmapFill(BmpWin_GetBitmap(bg->windows[i]), 0);
        func_020456f8(bg->windows[i]);
        BmpWin_Free(bg->windows[i]);
    }
    GFL_MsgDataFree(bg->msgData);
    GFL_FontFree(bg->font);
}

// pokebw2's MicTestMic_Init
enum MicBufferSize {
    MIC_BUFFER_SIZE = 0x120,
};

void func_ov168_02184398(MicTestMic *mic, u32 heapId, MICCallback callback, void *arg) {
    u8 *buffer;
    enum MicBufferSize bufferSize;

    setupMic(heapId);
    func_02034f84(8);
    buffer = GFL_HeapAllocate(heapId, MIC_BUFFER_SIZE, FALSE, "mictest.c", 1539);
    bufferSize = MIC_BUFFER_SIZE;
    MI_CpuFill8(buffer, 0, bufferSize);
    mic->buffer = buffer;
    mic->param.type = MIC_SAMPLING_TYPE_8BIT;
    mic->param.buffer = (void *)(((uintptr_t)buffer + 0x1f) & ~0x1f);
    mic->param.rate = MIC_SAMPLING_RATE_8180;
    mic->param.loop_enable = TRUE;
    mic->param.size = bufferSize - 0x20;
    mic->param.full_callback = callback;
    mic->param.full_arg = arg;
    mic->wasOpen = func_02006d94();
}

void ampOffFreeBlocks(void);


void func_ov168_02184408(MicTestMic *mic) {
    GFL_HeapFree(mic->buffer);
    ampOffFreeBlocks();
    func_02034fb8(8);
}


// pokebw2's MicTestMic_Update
static void func_ov168_0218441c(MicTestMic *mic) {
    BOOL open;

    func_02006d64(1);
    open = func_02006d94();
    if (!open || !mic->wasOpen) {
        if (!mic->asleep) {
            if (mic->sampling) {
                func_ov168_0218448c(mic);
                mic->resume = TRUE;
            }
            mic->asleep = TRUE;
        }
        if (open && !mic->wasOpen) {
            mic->asleep = FALSE;
            if (mic->resume) {
                func_ov168_02184470(mic);
                mic->resume = FALSE;
            }
        }
    }
    mic->wasOpen = open;
}

// pokebw2's MicTestMic_Start
static void func_ov168_02184470(MicTestMic *mic) {
    if (!mic->asleep) {
        MIC_StartLimitedSampling(&mic->param);
        mic->sampling = TRUE;
    } else {
        mic->resume = TRUE;
    }
}

// pokebw2's MicTestMic_Stop
static void func_ov168_0218448c(MicTestMic *mic) {
    MIC_StopLimitedSampling();
    mic->sampling = FALSE;
}

// pokebw2's MicTestMic_GetPeak
int func_ov168_0218449c(MicTestMic *mic) {
    int peak = 0;
    int i;
    int last = mic->param.size - 1;
    u8 *samples = mic->param.buffer;

    for (i = 0; i < last; i++) {
        u8 sample = samples[i];

        if (sample < 0x80) {
            sample = 0xff - sample;
        }
        if (sample > peak) {
            peak = sample;
        }
    }
    return peak;
}

// pokebw2's MicTest_OnBufferFull
void func_ov168_021844c8(MICResult result, void *arg) {
    MicTestWork *wk = arg;
    MicTestMic *mic;
    int peak;
    u32 level;

    if (result != MIC_RESULT_SUCCESS) {
        return;
    }
    mic = &wk->mic;
    peak = func_ov168_0218449c(mic);
    if (mic->peak < peak) {
        level = func_ov168_021845ac(peak);
        mic->peak = peak;
        mic->newPeak = TRUE;
    } else {
        level = func_ov168_021845ac(mic->peak);
        mic->peak -= 2;
    }
    if (level != 0) {
        if (mic->newPeak) {
            func_0204b878(wk->actors[ACTOR_CENTER], 1);
            mic->newPeak = FALSE;
        }
        if (func_ov168_021845e0(wk)) {
            func_ov168_02184600(wk, wk->heapId, 128, 64, wk->tcbManager);
        } else {
            wk->spawnTimer--;
            if (wk->spawnTimer == 0) {
                func_ov168_02184600(wk, wk->heapId, 128, 64, wk->tcbManager);
                wk->spawnTimer = NOTE_INTERVAL;
            }
        }
        func_0204b878(wk->actors[0], level);
        func_0204b878(wk->actors[1], level);
    } else if (level == 0) {
        func_0204b878(wk->actors[ACTOR_CENTER], 0);
        func_0204b878(wk->actors[0], 0);
        func_0204b878(wk->actors[1], 0);
    }
    data_ov168_02184a40++;
}

// The animation of the speaker for a sample as loud as peak
// pokebw2's MicTest_GetLevel
u32 func_ov168_021845ac(u8 peak) {
    if (peak <= 150) {
        return 0;
    }
    if (peak >= 151 && peak <= 162) {
        return 1;
    }
    if (peak >= 163 && peak <= 176) {
        return 2;
    }
    if (peak >= 177 && peak <= 197) {
        return 3;
    }
    if (peak >= 198) {
        return 4;
    }
    // Never reached: every peak is in a range above. Black's code falls off the end with the peak in r0
    return peak;
}

// pokebw2's MicTest_AreNotesFree
BOOL func_ov168_021845e0(MicTestWork *wk) {
    int i;

    for (i = 0; i < NOTE_COUNT; i++) {
        if (wk->notes[i].task != NULL) {
            return FALSE;
        }
    }
    return TRUE;
}

// pokebw2's MicTest_SpawnNote
BOOL func_ov168_02184600(MicTestWork *wk, u32 heapId, s16 x, s16 y, TCBManager *tcbManager) {
    ClActor *actor = NULL;
    MicTestNote *note = NULL;
    int i;

    for (i = 0; i < NOTE_COUNT; i++) {
        if (wk->notes[i].task == NULL) {
            note = &wk->notes[i];
            actor = wk->actors[ACTOR_NOTE_FIRST + i];
            break;
        }
    }
    if (note != NULL) {
        fx32 speed = GFL_RandomLC(0x1001) + 0x1333;
        int angle = GFL_RandomLC(61) + 60;
        int swayRadius = GFL_RandomLC(15) + 6;

        func_ov168_02184680(tcbManager, actor, note, x, y, speed, angle, swayRadius, 1);
        return TRUE;
    }
    return FALSE;
}

void func_ov168_02184680(TCBManager *tcbManager, ClActor *actor, MicTestNote *note, s16 x, s16 y, fx32 speed,
                              int angle, int swayRadius, u32 delay) {
    MicTestNote *data;

    note->task = GFL_TCBMgrAddTask(tcbManager, func_ov168_02184780, note, 0);
    data = GFL_TCBGetData(note->task);
    data->actor = actor;
    data->delay = delay;
    data->position.x = FX32_CONST(x);
    data->position.y = FX32_CONST(y);
    data->position.z = 0;
    data->velocity.x = FX_Mul(speed, func_0203f5fc(angle + 90));
    data->velocity.y = -FX_Mul(speed, func_0203f5fc(angle));
    data->velocity.z = 0;
    data->phase = 0;
    data->phaseStep = FX32_CONST(6);
    data->swayRadius = swayRadius;
    func_0204b550(data->actor, x, 0);
    func_0204b550(data->actor, y, 1);
}

// pokebw2's MicTestNote_Task
void func_ov168_02184780(TCB *tcb, void *data) {
    MicTestNote *note = data;
    s16 x;
    s16 y;
    s16 sway;

    if (note->delay != 0) {
        note->delay--;
        if (note->delay == 0) {
            func_0204b3dc(note->actor, TRUE);
        }
        return;
    }
    VEC_Add(&note->position, &note->velocity, &note->position);
    x = note->position.x >> FX32_SHIFT;
    y = note->position.y >> FX32_SHIFT;
    note->phase += note->phaseStep;
    while (note->phase > FX32_CONST(360)) {
        note->phase -= FX32_CONST(360);
    }
    sway = (func_0203f5fc(note->phase >> FX32_SHIFT) * note->swayRadius) >> FX32_SHIFT;
    x += sway;
    func_0204b550(note->actor, x, 0);
    func_0204b550(note->actor, y, 1);
    if (y < -16) {
        func_0204b3dc(note->actor, FALSE);
        GFL_TCBRemove(tcb);
        note->task = NULL;
    }
}

// pokebw2's MicTest_StopNotes
void func_ov168_02184818(MicTestWork *wk) {
    int i;

    for (i = 0; i < NOTE_COUNT; i++) {
        if (wk->notes[i].task != NULL) {
            GFL_TCBRemove(wk->notes[i].task);
            wk->notes[i].task = NULL;
        }
    }
}

// pokebw2's MicTest_GetTouchedRect
s32 func_ov168_0218483c(void) {
    return func_02035c08(data_ov168_0218486e);
}

// pokebw2's MicTest_IsExitPressed
BOOL func_ov168_0218484c(void) {
    if (func_ov168_0218483c() == 0 || (GCTX_HIDGetPressedKeys() & PAD_BUTTON_B)) {
        return TRUE;
    }
    return FALSE;
}
