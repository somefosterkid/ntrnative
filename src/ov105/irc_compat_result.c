// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov179/irc_compat_result.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/compatible_irc_sys.h"
#include "app/irc_appbar.h"
#include "app/irc_compatible.h"
#include "constants/arc.h"
#include "constants/items.h"
#include "constants/sound.h"
#include "constants/text_banks.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/fade.h"
#include "gfl/graphics.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/proc.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "gfl/touchpanel.h"
#include "nitro/gx.h"
#include "nnsys/g2d.h"
#include "nnsys/gfd.h"
#include "save/bag.h"
#include "save/player_info.h"
#include "save/records.h"
#include "save/save_control.h"
#include "save/trainer_log.h"
#include "system/bmp_winframe.h"
#include "system/game_data.h"
#include "system/game_system.h"
#include "system/gf_font.h"
#include "system/printsys.h"
#include "system/wordset.h"

// The compatibility result of the infrared link: its graphics, its windows, the three digits of the score, the flash of
// the background and the steps of the screen. The ROM has no string for the file, so the name is a guess. Names are
// ours

#define BG_COUNT 8
#define DIGIT_COUNT 3
#define CLACT_COUNT 38
#define FLASH_COLOR_COUNT 16
#define SWEET_HEART_MAX 999

// The actor of the sprite behind the digits that is not one of the three
#define ACTOR_BACKGROUND (CLACT_COUNT - 1)

// The medal for a score of 80 and over
#define MEDAL_COMPAT 180

// The record that counts the checks
#define RECORD_COMPAT_CHECKS 0x71

// The steps of func_ov105_021b80e8
enum {
    STEP_WAIT_START,
    STEP_START_FLASH,
    STEP_WAIT_FLASH,
    STEP_WAIT_DIGITS,
    STEP_PRINT_SCORE,
    STEP_JINGLE,
    STEP_WAIT_JINGLE,
    STEP_REWARD,
    STEP_WAIT_BUTTON_DELAY,
    STEP_SHOW_BUTTON,
    STEP_WAIT_SAVE,
};

// What func_ov105_021b8860 did
enum {
    REWARD_GIVEN,
    // Not given: the player was logged today, or there is no link
    REWARD_NONE,
    // Not given: nowhere to put the item
    REWARD_NO_ROOM,
};

typedef struct {
    ClActUnit *unit;
    u32 palette;
    u32 chars;
    u32 cellAnims;
    ClActor *actors[CLACT_COUNT];
} IrcCompatClAct;

typedef struct {
    // The characters of the window frame
    u32 frameChars;
    IrcCompatClAct clact;
    TCB *vblankTask;
} IrcCompatGraphic;

typedef struct {
    Font *font;
    MsgData *msgData;
    PrintQueue *queue;
    WordSet *wordSet;
} IrcCompatText;

typedef struct {
    BmpWin *window;
    PrintWindow printWindow;
    StrBuf *strbuf;
    u16 color;
} IrcCompatWindow;

// The score in three digit sprites, which appear one after the other
typedef struct {
    ClActor *actors[DIGIT_COUNT];
    u16 unkC[DIGIT_COUNT];
    u16 unk12[DIGIT_COUNT];
    int unk18[DIGIT_COUNT];
    u8 unk24[12];
    u8 digits[DIGIT_COUNT];
    u8 unk33;
    // Whether each digit has appeared
    int shown[DIGIT_COUNT];
} IrcCompatDigits;

// The flash of colors of palette 8 of BG 3, a tier at a time, and how it fades back
typedef struct {
    u16 unk0;
    u8 unk2[6];
    u16 target[FLASH_COLOR_COUNT];
    u16 colors[FLASH_COLOR_COUNT];
    u32 frame;
    BOOL done;
    BOOL active;
    u16 lastStep;
    u16 step;
    int phase;
    // Whether the score is close enough over its tier to flash one more
    BOOL carry;
} IrcCompatFlash;

// The colors that flash at each step: the step's colors and how many
typedef struct {
    u8 colors[2];
    u8 count;
} IrcCompatFlashStep;

typedef struct IrcCompatResultWork IrcCompatResultWork;
typedef void (*IrcCompatStepFunc)(IrcCompatResultWork *wk, u16 *state);

struct IrcCompatResultWork {
    IrcCompatGraphic graphic;
    IrcCompatText text;
    // The message of the players, then of the result
    IrcCompatWindow windowA;
    // The title
    IrcCompatWindow windowB;
    IrcCompatDigits digits;
    IrcCompatFlash flash;
    IrcAppBar *button;
    IrcCompatStepFunc step;
    u16 stepState;
    BOOL stepDone;
    u32 counter;
    // Whether both players have logs
    BOOL bothLogs;
    // How many Sweet Hearts the score earns
    int reward;
    IrcCompatibleParam3 *param;
};

static BOOL func_ov105_021b7560(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov105_021b7730(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov105_021b7778(GameProc *proc, u32 *state, void *param, void *work);
void func_ov105_021b784c(IrcCompatGraphic *graphic, HeapID heapId);
void func_ov105_021b78a0(IrcCompatGraphic *graphic);
static void func_ov105_021b78bc(IrcCompatGraphic *graphic);
ClActor *func_ov105_021b78c8(IrcCompatGraphic *graphic, int index);
ClActUnit *func_ov105_021b78d4(IrcCompatGraphic *graphic);
void func_ov105_021b78e0(TCB *tcb, void *data);
void func_ov105_021b78f4(IrcCompatGraphic *graphic, HeapID heapId);
void func_ov105_021b7a3c(IrcCompatGraphic *graphic);
void func_ov105_021b7a80(IrcCompatGraphic *graphic);
void func_ov105_021b7a88(IrcCompatClAct *clact, const BGSysVRAMConfig *vramConfig, HeapID heapId);
void func_ov105_021b7bc0(IrcCompatClAct *clact);
static void func_ov105_021b7c04(IrcCompatClAct *clact);
void func_ov105_021b7c0c(IrcCompatClAct *clact);
ClActor *func_ov105_021b7c14(IrcCompatClAct *clact, int index);
ClActUnit *func_ov105_021b7c1c(IrcCompatClAct *clact);
void func_ov105_021b7c20(IrcCompatText *text, BOOL keepFont, HeapID heapId);
void func_ov105_021b7c6c(IrcCompatText *text);
static BOOL func_ov105_021b7c94(IrcCompatText *text);
Font *func_ov105_021b7ca0(IrcCompatText *text);
PrintQueue *func_ov105_021b7ca4(IrcCompatText *text);
MsgData *func_ov105_021b7ca8(IrcCompatText *text);
WordSet *func_ov105_021b7cac(IrcCompatText *text);
void func_ov105_021b7cb0(IrcCompatWindow *win, u8 bg, u8 x, u8 y, u8 width, u8 height, u8 palette,
                                 HeapID heapId);
void func_ov105_021b7d20(IrcCompatWindow *win, u8 bg, u8 x, u8 y, u8 width, u8 height, u8 palette,
                                          u16 color, u8 fromEnd, HeapID heapId);
void func_ov105_021b7d90(IrcCompatWindow *win);
static BOOL func_ov105_021b7dac(IrcCompatWindow *win, IrcCompatText *text);
void func_ov105_021b7de4(IrcCompatWindow *win, IrcCompatText *text, u32 msgId, int x, s16 y);
void func_ov105_021b7e48(IrcCompatWindow *win, IrcCompatText *text, u32 msgId);
void func_ov105_021b7eec(IrcCompatWindow *win, IrcCompatText *text, u8 msgId, u16 number, u16 index);
void func_ov105_021b7f78(IrcCompatWindow *win, IrcCompatText *text, u32 msgId,
                                            const CompatibleIrcProfile *own, const CompatibleIrcProfile *other,
                                            HeapID heapId);
void func_ov105_021b8008(IrcCompatWindow *win, IrcCompatText *text, const CompatibleIrcProfile *own,
                                           u32 count, u32 msgId);
void func_ov105_021b80bc(IrcCompatResultWork *wk, IrcCompatStepFunc step);
void func_ov105_021b80cc(IrcCompatResultWork *wk);
void func_ov105_021b80d8(IrcCompatResultWork *wk, u16 *state);
static void func_ov105_021b80e8(IrcCompatResultWork *wk, u16 *state);
void func_ov105_021b8340(IrcCompatResultWork *wk, u16 *state);
void func_ov105_021b8348(IrcCompatDigits *digits, IrcCompatGraphic *graphic, int score);
void func_ov105_021b83e0(IrcCompatDigits *digits, int index);
BOOL func_ov105_021b8434(IrcCompatDigits *digits);
int func_ov105_021b8438(IrcCompatDigits *digits, int index);
void func_ov105_021b8440(IrcCompatDigits *digits, u32 value);
void func_ov105_021b84b4(IrcCompatFlash *flash, u16 unk0, u8 score, HeapID heapId);
void func_ov105_021b8598(IrcCompatFlash *flash);
static void func_ov105_021b85a4(IrcCompatFlash *flash);
void func_ov105_021b871c(IrcCompatFlash *flash);
BOOL func_ov105_021b8730(IrcCompatFlash *flash);
static void func_ov105_021b8734(u16 *dest, u16 frame, u16 duration, u8 palette, u8 index, u16 from, u16 to);
u32 func_ov105_021b87e8(u8 score, BOOL *carry);
u32 func_ov105_021b8840(u32 score, BOOL bothLogs);
u32 func_ov105_021b8860(IrcCompatResultWork *wk);

// pokebw2's data_ov105_021b8958
const GameProcFunctions data_ov105_021b8958 = {
    func_ov105_021b7560,
    func_ov105_021b7778,
    func_ov105_021b7730,
};

// pokebw2's sIrcCompatLCDConfig
static const BGSysLCDConfig data_ov105_021b8964 = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_1, GX_BG0_AS_2D };

// The divisors that cut a score into digits
// pokebw2's sIrcCompatDigitDivisors
static const u32 data_ov105_021b8974[4] = { 1, 10, 100, 1000 };

// pokebw2's sIrcCompatBGModes
static const u32 data_ov105_021b8998[BG_COUNT] = { BGMODE_TEXT, BGMODE_TEXT, BGMODE_TEXT, BGMODE_TEXT,
                                                 BGMODE_TEXT, BGMODE_TEXT, BGMODE_TEXT, BGMODE_AFFINE };

// pokebw2's sIrcCompatBGs
static const u32 data_ov105_021b89b8[BG_COUNT] = { 0, 1, 2, 3, 4, 5, 6, 7 };

// pokebw2's sIrcCompatFlashSteps
static const IrcCompatFlashStep data_ov105_021b8984[6] = {
    { { 0, 0 }, 0 }, { { 12, 11 }, 2 }, { { 9, 10 }, 2 }, { { 8, 0 }, 1 }, { { 7, 6 }, 2 }, { { 3, 5 }, 2 },
};

// pokebw2's sIrcCompatVRAMConfig
static const BGSysVRAMConfig data_ov105_021b89d8 = {
    GX_VRAM_BG_128_A,  GX_VRAM_BGEXTPLTT_NONE,  GX_VRAM_SUB_BG_128_C,        GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_128_B, GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_16_I,        GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_NONE,  GX_VRAM_TEXPLTT_NONE,    GX_OBJVRAMMODE_CHAR_1D_128K, GX_OBJVRAMMODE_CHAR_1D_128K,
};

// pokebw2's sIrcCompatBGSetups
static const BGSetup data_ov105_021b8a08[BG_COUNT] = {
    { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x04000, 0x8000,
      GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE },
    { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x0800, GX_BG_CHARBASE_0x0c000, 0x8000,
      GX_BG_EXTPLTT_01, 2, GX_BG_AREAOVER_XLU, FALSE },
    { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x1000, GX_BG_CHARBASE_0x10000, 0x8000,
      GX_BG_EXTPLTT_01, 3, GX_BG_AREAOVER_XLU, FALSE },
    { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x1800, GX_BG_CHARBASE_0x14000, 0x8000,
      GX_BG_EXTPLTT_01, 1, GX_BG_AREAOVER_XLU, FALSE },
    { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x04000, 0x8000,
      GX_BG_EXTPLTT_01, 1, GX_BG_AREAOVER_XLU, FALSE },
    { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x0800, GX_BG_CHARBASE_0x0c000, 0x8000,
      GX_BG_EXTPLTT_01, 2, GX_BG_AREAOVER_XLU, FALSE },
    { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x1000, GX_BG_CHARBASE_0x10000, 0x8000,
      GX_BG_EXTPLTT_01, 3, GX_BG_AREAOVER_XLU, FALSE },
    { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_256, GX_BG_SCRBASE_0x1800, GX_BG_CHARBASE_0x14000, 0x4000,
      GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE },
};

// pokebw2's IrcCompatResult_Main


static BOOL func_ov105_021b7560(GameProc *proc, u32 *state, void *param, void *work) {
    IrcCompatResultWork *wk;
    CompatibleIrcProfile own;
    CompatibleIrcProfile profile;
    ClActUnit *unit;
    Font *font;
    PrintQueue *queue;

    GFL_HeapCreateChild(HEAPID_USER, HEAPID_IRC_COMPAT, 0x20000);
    wk = GFL_ProcInitSubsystem(proc, sizeof(IrcCompatResultWork), HEAPID_IRC_COMPAT);
    MI_CpuFill8(wk, 0, sizeof(IrcCompatResultWork));
    wk->param = param;
    func_ov105_021b784c(&wk->graphic, HEAPID_IRC_COMPAT);
    func_ov105_021b7c20(&wk->text, FALSE, HEAPID_IRC_COMPAT);
    if (wk->param->gsys != NULL) {
        GSYS_GetGameCommSystem(wk->param->gsys);
    }
    func_ov105_021b7cb0(&wk->windowA, 5, 1, 18, 30, 5, 5, HEAPID_IRC_COMPAT);
    BmpWin_DrawFrame(wk->windowA.window, 0, wk->graphic.frameChars, 6);
    func_ov105_021b7d20(&wk->windowB, 7, 6, 3, 20, 5, 8, 1, 1, HEAPID_IRC_COMPAT);
    func_ov105_021b7e48(&wk->windowB, &wk->text, 1);
    GFL_BGSysScaleBGReq(7, 3, 0x1000);
    GFL_BGSysScaleBGReq(7, 6, 0x1000);
    GFL_BGSysAdjustBGOriginReq(7, 9, 0x80);
    GFL_BGSysAdjustBGOriginReq(7, 12, 0x28);
    func_ov105_021b8348(&wk->digits, &wk->graphic, wk->param->result);
    if (wk->param->gsys != NULL) {
        func_ov101_021b658c(wk->param->gsys, &own);
        func_ov105_021b7f78(&wk->windowA, &wk->text, 0, &own, wk->param->profile, HEAPID_IRC_COMPAT);
    }
    unit = func_ov105_021b78d4(&wk->graphic);
    font = func_ov105_021b7ca0(&wk->text);
    queue = func_ov105_021b7ca4(&wk->text);
    wk->button = func_ov101_021b70dc(2, unit, 0, 13, 13, 2, font, queue, HEAPID_IRC_COMPAT);
    func_ov101_021b728c(wk->button, FALSE);
    func_ov105_021b84b4(&wk->flash, 3, wk->param->result, HEAPID_IRC_COMPAT);
    if (wk->param->gsys != NULL) {
        func_ov101_021b658c(wk->param->gsys, &profile);
        if (profile.isFourteenth != 0 && wk->param->profile->isFourteenth != 0) {
            wk->bothLogs = TRUE;
        } else {
            wk->bothLogs = FALSE;
        }
        wk->reward = func_ov105_021b8840(wk->param->result, wk->bothLogs);
    }
    func_ov105_021b80bc(wk, func_ov105_021b80d8);
    return TRUE;
}

static BOOL func_ov105_021b7730(GameProc *proc, u32 *state, void *param, void *work) {
    IrcCompatResultWork *wk = work;

    func_ov105_021b8598(&wk->flash);
    func_ov105_021b7d90(&wk->windowB);
    func_ov101_021b71c0(wk->button);
    func_ov105_021b7d90(&wk->windowA);
    func_ov105_021b78a0(&wk->graphic);
    func_ov105_021b7c6c(&wk->text);
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(HEAPID_IRC_COMPAT);
    return TRUE;
}

static BOOL func_ov105_021b7778(GameProc *proc, u32 *state, void *param, void *work) {
    IrcCompatResultWork *wk = work;

    switch (*state) {
    case 0:
        *state = 1;
        break;
    case 1:
        GFL_FadeSet(3, 16, 0, 0);
        *state = 2;
        break;
    case 2:
        if (GFL_FadeIsRunning()) {
            break;
        }
        *state = 3;
        break;
    case 3:
        wk->step(wk, &wk->stepState);
        if (wk->stepDone) {
            *state = 4;
        }
        break;
    case 4:
        GFL_FadeSet(3, 0, 16, 0);
        *state = 5;
        break;
    case 5:
        if (GFL_FadeIsRunning()) {
            break;
        }
        *state = 6;
        break;
    case 6:
        return TRUE;
    }
    if (func_ov105_021b7c94(&wk->text)) {
        func_ov105_021b7dac(&wk->windowA, &wk->text);
        func_ov105_021b7dac(&wk->windowB, &wk->text);
    }
    func_ov105_021b78bc(&wk->graphic);
    func_ov105_021b85a4(&wk->flash);
    func_ov101_021b71e4(wk->button);
    if (wk->param->sys != NULL) {
        func_ov101_021b63d0(wk->param->sys);
    }
    return FALSE;
}

// pokebw2's IrcCompatGraphic_Create
void func_ov105_021b784c(IrcCompatGraphic *graphic, HeapID heapId) {
    MI_CpuFill8(graphic, 0, sizeof(IrcCompatGraphic));
    GFL_BGSysInitVRAM(GX_VRAM_LCDC_NONE);
    GFL_BGSysSetVRAMBanks(&data_ov105_021b89d8);
    GFL_BGSysSetDisplayLayout(0);
    GFL_BGSysEnableEngines();
    GFL_BGSysDisableAllA();
    func_ov105_021b7a88(&graphic->clact, &data_ov105_021b89d8, heapId);
    func_ov105_021b78f4(graphic, heapId);
    graphic->vblankTask = GFL_VBlankTCBAdd(func_ov105_021b78e0, graphic, 0);
}

// pokebw2's IrcCompatGraphic_Free
void func_ov105_021b78a0(IrcCompatGraphic *graphic) {
    GFL_TCBRemove(graphic->vblankTask);
    func_ov105_021b7a3c(graphic);
    func_ov105_021b7bc0(&graphic->clact);
}

// pokebw2's IrcCompatGraphic_Update
static void func_ov105_021b78bc(IrcCompatGraphic *graphic) {
    func_ov105_021b7c04(&graphic->clact);
}

// pokebw2's IrcCompatGraphic_GetActor
ClActor *func_ov105_021b78c8(IrcCompatGraphic *graphic, int index) {
    return func_ov105_021b7c14(&graphic->clact, index);
}

// pokebw2's IrcCompatGraphic_GetUnit
ClActUnit *func_ov105_021b78d4(IrcCompatGraphic *graphic) {
    return func_ov105_021b7c1c(&graphic->clact);
}

// pokebw2's IrcCompatGraphic_VBlank
void func_ov105_021b78e0(TCB *tcb, void *data) {
    IrcCompatGraphic *graphic = data;

    func_ov105_021b7a80(graphic);
    func_ov105_021b7c0c(&graphic->clact);
}

// pokebw2's IrcCompatGraphic_InitBG
void func_ov105_021b78f4(IrcCompatGraphic *graphic, HeapID heapId) {
    ArcTool *arc;
    int i;

    GFL_BGSysCreate(heapId);
    BmpWin_InitAllocator(heapId);
    GFL_BGSysSetLCDConfig(&data_ov105_021b8964);
    for (i = 0; i < BG_COUNT; i++) {
        GFL_BGSysCreateBG(data_ov105_021b89b8[i], &data_ov105_021b8a08[i], data_ov105_021b8998[i]);
        GFL_BGSysClearBG(data_ov105_021b89b8[i]);
        GFL_BGSysSetBGEnabled(data_ov105_021b89b8[i], TRUE);
    }
    arc = GFL_ArcSysCreateFileHandle(ARCID_IRC_COMPAT, heapId);
    GFL_G2DIOLoadArcNCLRDefault(arc, 16, PALTYPE_MAIN_BG, 0, 0, heapId);
    GFL_G2DIOLoadArcNCLRDefault(arc, 16, PALTYPE_SUB_BG, 0, 0, heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 0, 2, 0, 0, FALSE, heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 0, 6, 0, 0, FALSE, heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 48, 4, 0, 0, FALSE, heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 13, 1, 0, 0, FALSE, heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 4, 2, 0, 0, FALSE, heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 4, 6, 0, 0, FALSE, heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 46, 4, 0, 0, FALSE, heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 14, 1, 0, 0, FALSE, heapId);
    GFL_BGSysFillChar(5, 0, 1, 0);
    graphic->frameChars = GFL_BGSysLoadArcNCGRDynamic(arc, 49, 5, 0, FALSE, heapId);
    GFL_ArcToolFree(arc);
}

// pokebw2's IrcCompatGraphic_FreeBG
void func_ov105_021b7a3c(IrcCompatGraphic *graphic) {
    int i;

    GFL_BGSysFreeCharMemory(5, (u16)graphic->frameChars, (u16)(graphic->frameChars >> 16));
    GFL_BGSysFreeFilledChar(5, 1, 0);
    for (i = 0; i < BG_COUNT; i++) {
        GFL_BGSysReleaseBG(data_ov105_021b89b8[i]);
    }
    BmpWin_FreeAllocator();
    GFL_BGSysFree();
}

// pokebw2's IrcCompatGraphic_VBlankBG
void func_ov105_021b7a80(IrcCompatGraphic *graphic) {
    GFL_BGSysUpdate();
}

// pokebw2's IrcCompatClAct_Init
void func_ov105_021b7a88(IrcCompatClAct *clact, const BGSysVRAMConfig *vramConfig, HeapID heapId) {
    ArcTool *arc;
    ClActorSetup setup;
    ClActorSetup background;
    int i;

    MI_CpuFill8(clact, 0, sizeof(IrcCompatClAct));
    ClActSys_Create(&data_020a1448, vramConfig, heapId);
    clact->unit = func_0204b100(128, 0, heapId);
    func_0204b270(clact->unit);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_OBJ, TRUE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
    arc = GFL_ArcSysCreateFileHandle(ARCID_IRC_COMPAT, heapId);
    clact->palette = func_0204abf0(arc, 12, CLACT_VRAM_MAIN, 0, heapId);
    clact->chars = func_0204a6c8(arc, 11, FALSE, CLACT_VRAM_MAIN, heapId);
    clact->cellAnims = func_0204af28(arc, 10, 9, heapId);
    GFL_ArcToolFree(arc);
    MI_CpuFill8(&setup, 0, sizeof(ClActorSetup));
    for (i = 0; i < 3; i++) {
        clact->actors[i] = func_0204b294(clact->unit, clact->chars, clact->palette, clact->cellAnims, &setup,
                                         CLACT_SURFACE_MAIN, heapId);
    }
    MI_CpuFill8(&background, 0, sizeof(ClActorSetup));
    background.x = 128;
    background.y = 96;
    clact->actors[ACTOR_BACKGROUND] = func_0204b294(clact->unit, clact->chars, clact->palette, clact->cellAnims,
                                                    &background, CLACT_SURFACE_MAIN, heapId);
    func_0204b3dc(clact->actors[ACTOR_BACKGROUND], FALSE);
    func_0204b84c(clact->actors[ACTOR_BACKGROUND], 0);
    func_0204b800(clact->actors[ACTOR_BACKGROUND], 0);
    func_0204b92c(clact->actors[ACTOR_BACKGROUND], TRUE);
    func_0204b878(clact->actors[ACTOR_BACKGROUND], 11);
    for (i = 0; i < 3; i++) {
        func_0204b3dc(clact->actors[i], FALSE);
        func_0204b84c(clact->actors[i], 0);
        func_0204b800(clact->actors[i], 0);
    }
}

// pokebw2's IrcCompatClAct_Free
void func_ov105_021b7bc0(IrcCompatClAct *clact) {
    int i;

    for (i = 0; i < CLACT_COUNT; i++) {
        if (clact->actors[i] != NULL) {
            func_0204b3b4(clact->actors[i]);
        }
    }
    func_0204ada4(clact->palette);
    func_0204a8d4(clact->chars);
    func_0204afd8(clact->cellAnims);
    func_0204b1cc(clact->unit);
    func_0204a5ac();
    MI_CpuFill8(clact, 0, sizeof(IrcCompatClAct));
}

// pokebw2's IrcCompatClAct_Update
static void func_ov105_021b7c04(IrcCompatClAct *clact) {
    func_0204a600();
}

// pokebw2's IrcCompatClAct_VBlank
void func_ov105_021b7c0c(IrcCompatClAct *clact) {
    func_0204a648();
}

// pokebw2's IrcCompatClAct_GetActor
ClActor *func_ov105_021b7c14(IrcCompatClAct *clact, int index) {
    return clact->actors[index];
}

// pokebw2's IrcCompatClAct_GetUnit
ClActUnit *func_ov105_021b7c1c(IrcCompatClAct *clact) {
    return clact->unit;
}

// pokebw2's IrcCompatText_Init
void func_ov105_021b7c20(IrcCompatText *text, BOOL keepFont, HeapID heapId) {
    MI_CpuFill8(text, 0, sizeof(IrcCompatText));
    func_0201dd60();
    if (!keepFont) {
        text->font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, heapId);
    }
    text->queue = func_0201c440(heapId);
    text->msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_BTL_MAIN_6, heapId);
    text->wordSet = GFL_WordSetSystemCreateDefault(heapId);
}

// pokebw2's IrcCompatText_Free
void func_ov105_021b7c6c(IrcCompatText *text) {
    GFL_WordSetSystemFree(text->wordSet);
    GFL_MsgDataFree(text->msgData);
    func_0201c4c0(text->queue);
    GFL_FontFree(text->font);
    MI_CpuFill8(text, 0, sizeof(IrcCompatText));
}

// pokebw2's IrcCompatText_Update
static BOOL func_ov105_021b7c94(IrcCompatText *text) {
    return func_0201c4e4(text->queue);
}

// pokebw2's IrcCompatText_GetFont
Font *func_ov105_021b7ca0(IrcCompatText *text) {
    return text->font;
}

// pokebw2's IrcCompatText_GetQueue
PrintQueue *func_ov105_021b7ca4(IrcCompatText *text) {
    return text->queue;
}

// pokebw2's IrcCompatText_GetMsgData
MsgData *func_ov105_021b7ca8(IrcCompatText *text) {
    return text->msgData;
}

// pokebw2's IrcCompatText_GetWordSet
WordSet *func_ov105_021b7cac(IrcCompatText *text) {
    return text->wordSet;
}

void func_ov105_021b7cb0(IrcCompatWindow *win, u8 bg, u8 x, u8 y, u8 width, u8 height, u8 palette,
                                 HeapID heapId) {
    MI_CpuFill8(win, 0, sizeof(IrcCompatWindow));
    win->color = 15;
    win->window = BmpWin_CreateDynamic(bg, x, y, width, height, palette, TRUE);
    win->strbuf = GFL_StrBufCreate(255, heapId);
    PrintWindow_Init(&win->printWindow, win->window);
    GFL_BitmapFill(BmpWin_GetBitmap(win->window), win->color);
    BmpWin_TransferNow(win->window);
}

void func_ov105_021b7d20(IrcCompatWindow *win, u8 bg, u8 x, u8 y, u8 width, u8 height, u8 palette,
                                          u16 color, u8 fromEnd, HeapID heapId) {
    MI_CpuFill8(win, 0, sizeof(IrcCompatWindow));
    win->color = color;
    win->window = BmpWin_CreateDynamic(bg, x, y, width, height, palette, fromEnd);
    win->strbuf = GFL_StrBufCreate(255, heapId);
    PrintWindow_Init(&win->printWindow, win->window);
    GFL_BitmapFill(BmpWin_GetBitmap(win->window), win->color);
    BmpWin_TransferNow(win->window);
}

// pokebw2's IrcCompatWindow_Free
void func_ov105_021b7d90(IrcCompatWindow *win) {
    GFL_StrBufFree(win->strbuf);
    BmpWin_Free(win->window);
    MI_CpuFill8(win, 0, sizeof(IrcCompatWindow));
}

// pokebw2's IrcCompatWindow_Flush
static BOOL func_ov105_021b7dac(IrcCompatWindow *win, IrcCompatText *text) {
    PrintWindow_Flush(&win->printWindow, text->queue);
    return PrintWindow_IsPrinted(&win->printWindow);
}

// pokebw2's IrcCompatWindow_PrintMsg
void func_ov105_021b7de4(IrcCompatWindow *win, IrcCompatText *text, u32 msgId, int x, s16 y) {
    MsgData *msgData = func_ov105_021b7ca8(text);
    PrintQueue *queue = func_ov105_021b7ca4(text);
    Font *font = func_ov105_021b7ca0(text);

    GFL_BitmapFill(BmpWin_GetBitmap(win->window), win->color);
    GFL_MsgDataLoadStrbuf(msgData, msgId, win->strbuf);
    PrintWindow_PrintNoColor(&win->printWindow, queue, x, y, win->strbuf, font);
}

void func_ov105_021b7e48(IrcCompatWindow *win, IrcCompatText *text, u32 msgId) {
    MsgData *msgData = func_ov105_021b7ca8(text);
    PrintQueue *queue = func_ov105_021b7ca4(text);
    Font *font = func_ov105_021b7ca0(text);
    u16 x;
    u16 y;

    GFL_BitmapFill(BmpWin_GetBitmap(win->window), win->color);
    GFL_MsgDataLoadStrbuf(msgData, msgId, win->strbuf);
    x = func_02045738(win->window) * 4;
    y = func_02045740(win->window) * 4;
    x -= GFL_FontGetBlockWidth(win->strbuf, font, 0) / 2;
    y -= GFL_FontGetBlockHeight(win->strbuf, font) / 2;
    func_0201c724(queue, BmpWin_GetBitmap(win->printWindow.window), x, y, win->strbuf, font, PRINT_COLOR(15, 14, 4));
    win->printWindow.flushPending = 1;
}


void func_ov105_021b7eec(IrcCompatWindow *win, IrcCompatText *text, u8 msgId, u16 number, u16 index) {
    WordSet *wordSet;
    PrintQueue *queue;
    Font *font;
    MsgData *msgData;
    StrBuf *format;

    GFL_BitmapFill(BmpWin_GetBitmap(win->window), win->color);
    wordSet = func_ov105_021b7cac(text);
    msgData = func_ov105_021b7ca8(text);
    WordSetNumber(wordSet, index, number, 3, 0, TRUE);
    format = GFL_MsgDataLoadStrbufNew(msgData, msgId);
    GFL_WordSetFormatStrbuf(wordSet, win->strbuf, format);
    GFL_StrBufFree(format);
    queue = func_ov105_021b7ca4(text);
    font = func_ov105_021b7ca0(text);
    PrintWindow_PrintNoColor(&win->printWindow, queue, 0, 0, win->strbuf, font);
}

void func_ov105_021b7f78(IrcCompatWindow *win, IrcCompatText *text, u32 msgId,
                                            const CompatibleIrcProfile *own, const CompatibleIrcProfile *other,
                                            HeapID heapId) {
    WordSet *wordSet;
    PrintQueue *queue;
    Font *font;
    MsgData *msgData;
    StrBuf *format;

    GFL_BitmapFill(BmpWin_GetBitmap(win->window), win->color);
    wordSet = func_ov105_021b7cac(text);
    msgData = func_ov105_021b7ca8(text);
    copyVarForText(wordSet, 0, (PlayerInfo *)&other->trainer);
    copyVarForText(wordSet, 1, (PlayerInfo *)&own->trainer);
    format = GFL_MsgDataLoadStrbufNew(msgData, msgId);
    GFL_WordSetFormatStrbuf(wordSet, win->strbuf, format);
    GFL_StrBufFree(format);
    queue = func_ov105_021b7ca4(text);
    font = func_ov105_021b7ca0(text);
    PrintWindow_PrintNoColor(&win->printWindow, queue, 0, 0, win->strbuf, font);
}

void func_ov105_021b8008(IrcCompatWindow *win, IrcCompatText *text, const CompatibleIrcProfile *own,
                                           u32 count, u32 msgId) {
    WordSet *wordSet;
    PrintQueue *queue;
    Font *font;
    MsgData *msgData;
    StrBuf *format;
    BOOL plural;

    GFL_BitmapFill(BmpWin_GetBitmap(win->window), win->color);
    wordSet = func_ov105_021b7cac(text);
    msgData = func_ov105_021b7ca8(text);
    copyVarForText(wordSet, 0, (PlayerInfo *)&own->trainer);
    plural = FALSE;
    if (count > 1) {
        plural = TRUE;
    }
    loadItemText(wordSet, 1, ITEM_SWEET_HEART, plural, FALSE);
    WordSetNumber(wordSet, 2, count, 1, 0, TRUE);
    format = GFL_MsgDataLoadStrbufNew(msgData, msgId);
    GFL_WordSetFormatStrbuf(wordSet, win->strbuf, format);
    GFL_StrBufFree(format);
    queue = func_ov105_021b7ca4(text);
    font = func_ov105_021b7ca0(text);
    PrintWindow_PrintNoColor(&win->printWindow, queue, 0, 0, win->strbuf, font);
}

// pokebw2's IrcCompatResult_SetStep
void func_ov105_021b80bc(IrcCompatResultWork *wk, IrcCompatStepFunc step) {
    wk->step = step;
    wk->stepState = 0;
}

// pokebw2's IrcCompatResult_SetStepDone
void func_ov105_021b80cc(IrcCompatResultWork *wk) {
    wk->stepDone = TRUE;
}

// pokebw2's IrcCompatResult_StartStep
void func_ov105_021b80d8(IrcCompatResultWork *wk, u16 *state) {
    func_ov105_021b80bc(wk, func_ov105_021b80e8);
}

static inline void PlayResultBGM(u32 rank) {
    switch (rank) {
    case 5:
        GFL_SndBGMPlay(SEQ_ME_AISHOU_H, SND_CHANNEL_MASK_ALL);
        break;
    case 4:
        GFL_SndBGMPlay(SEQ_ME_AISHOU_M, SND_CHANNEL_MASK_ALL);
        break;
    case 1:
    case 2:
    case 3:
        GFL_SndBGMPlay(SEQ_ME_AISHOU_L, SND_CHANNEL_MASK_ALL);
        break;
    case 0:
        GFL_SndBGMPlay(SEQ_ME_AISHOU_L, SND_CHANNEL_MASK_ALL);
        break;
    }
}

static void func_ov105_021b80e8(IrcCompatResultWork *wk, u16 *state) {
    CompatibleIrcProfile profile;

    switch (*state) {
    case 0:
        if (wk->counter++ > 30) {
            wk->counter = 0;
            *state = 1;
        }
        break;
    case 1:
        func_ov105_021b871c(&wk->flash);
        *state = 2;
        break;
    case 2:
        if (!func_ov105_021b8730(&wk->flash)) {
            break;
        }
        GFL_SndSEPlay(SEQ_SE_SYS_20);
        func_ov105_021b83e0(&wk->digits, 0);
        func_ov105_021b83e0(&wk->digits, 1);
        func_ov105_021b83e0(&wk->digits, 2);
        *state = 3;
        break;
    case 3:
        if (!func_ov105_021b8438(&wk->digits, 0)) {
            break;
        }
        if (!func_ov105_021b8438(&wk->digits, 1)) {
            break;
        }
        if (!func_ov105_021b8438(&wk->digits, 2)) {
            break;
        }
        *state = 4;
        break;
    case 4: {
        u32 msgId = (u8)(wk->param->result / 10 + 2);

        if (msgId > 12) {
            msgId = 12;
        } else if (msgId < 2) {
            msgId = 2;
        }
        func_ov105_021b7eec(&wk->windowA, &wk->text, msgId, wk->param->result, 2);
        *state = 5;
        break;
    }
    case 5:
        if (wk->counter++ > 50) {
            wk->counter = 0;
            PlayResultBGM(func_ov105_021b87e8(wk->param->result, NULL));
            *state = 6;
        }
        break;
    case 6:
        if (GFL_SndBGMIsPlaying()) {
            break;
        }
        if (wk->counter++ > 30) {
            wk->counter = 0;
            *state = 7;
        }
        break;
    case 7: {
        u32 result;

        if (!func_02035c60()) {
            break;
        }
        if (wk->param->gsys != NULL) {
            func_ov101_021b658c(wk->param->gsys, &profile);
        }
        result = func_ov105_021b8860(wk);
        if (result == 0) {
            if (!wk->bothLogs) {
                func_ov105_021b8008(&wk->windowA, &wk->text, &profile, (u8)wk->reward, 13);
            } else {
                func_ov105_021b8008(&wk->windowA, &wk->text, &profile, (u8)wk->reward, 14);
            }
            *state = 8;
        } else if (result == 2) {
            func_ov105_021b7de4(&wk->windowA, &wk->text, 15, 0, 0);
            *state = 8;
        } else {
            *state = 9;
        }
        break;
    }
    case 8:
        if (wk->counter++ > 30) {
            func_ov101_021b728c(wk->button, TRUE);
            wk->counter = 0;
            *state = 9;
        }
        break;
    case 9:
        func_ov101_021b728c(wk->button, TRUE);
        *state = 10;
        break;
    case 10:
        if (func_02035c60()) {
            func_ov105_021b80bc(wk, func_ov105_021b8340);
        }
        break;
    }
    if (func_ov101_021b7250(wk->button) == 1) {
        func_ov105_021b80cc(wk);
    }
    func_ov105_021b8434(&wk->digits);
}


// pokebw2's IrcCompatResult_FinishStep
void func_ov105_021b8340(IrcCompatResultWork *wk, u16 *state) {
    func_ov105_021b80cc(wk);
}

// pokebw2's IrcCompatDigits_Init
void func_ov105_021b8348(IrcCompatDigits *digits, IrcCompatGraphic *graphic, int score) {
    int i;

    MI_CpuFill8(digits, 0, sizeof(IrcCompatDigits));
    for (i = 0; i < DIGIT_COUNT; i++) {
        ClActorPos pos;

        digits->actors[i] = func_ov105_021b78c8(graphic, i);
        if (score < 100) {
            pos.x = 36 * i + 74;
        } else if (score >= 0 && score < 10) {
            pos.x = 36 * i + 56;
        } else {
            pos.x = 36 * i + 92;
        }
        pos.y = 84;
        func_0204b404(digits->actors[i], &pos, 0);
        func_0204b3dc(digits->actors[i], FALSE);
        digits->unkC[i] = 0;
        digits->unk12[i] = 60;
        digits->unk18[i] = 0xfef / digits->unk12[i];
    }
    func_ov105_021b8440(digits, score);
}

// pokebw2's IrcCompatDigits_Show
void func_ov105_021b83e0(IrcCompatDigits *digits, int index) {
    if (digits->shown[index] == 0) {
        if (index == 0 && digits->digits[2 - index] == 0) {
            digits->shown[index] = TRUE;
            return;
        }
        if (index == 0 && digits->digits[2 - index] == 0 && index == 1 && digits->digits[2 - index] == 0) {
            digits->shown[index] = TRUE;
            return;
        }
        digits->shown[index] = TRUE;
        func_0204b3dc(digits->actors[index], TRUE);
    }
}

// pokebw2's IrcCompatDigits_Update
BOOL func_ov105_021b8434(IrcCompatDigits *digits) {
    return FALSE;
}

// pokebw2's IrcCompatDigits_IsShown
int func_ov105_021b8438(IrcCompatDigits *digits, int index) {
    return digits->shown[index];
}

// pokebw2's IrcCompatDigits_SetValue
void func_ov105_021b8440(IrcCompatDigits *digits, u32 value) {
    int i;
    u8 rest = value;

    // A count of the digits that nothing uses
    if (rest != 0) {
        for (;;) {
            if (rest == 0) {
                break;
            }
            rest = rest / 10;
        }
    }
    for (i = 0; i < DIGIT_COUNT; i++) {
        func_0204b878(digits->actors[i], 0);
    }
    for (i = 0; i < DIGIT_COUNT; i++) {
        digits->digits[i] = value % data_ov105_021b8974[i + 1] / data_ov105_021b8974[i];
        func_0204b878(digits->actors[2 - i], digits->digits[i] + 1);
    }
}

// pokebw2's IrcCompatFlash_Init
void func_ov105_021b84b4(IrcCompatFlash *flash, u16 unk0, u8 score, HeapID heapId) {
    ArcTool *arc;
    NNSG2dPaletteData *palette;
    void *file;
    u16 *src;
    int i;

    MI_CpuFill8(flash, 0, sizeof(IrcCompatFlash));
    flash->unk0 = unk0;
    flash->lastStep = func_ov105_021b87e8(score, &flash->carry);
    if (flash->carry) {
        flash->lastStep++;
    }
    arc = GFL_ArcSysCreateFileHandle(ARCID_IRC_COMPAT, heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 15, 3, 0, 0, FALSE, heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 17, 3, 0, 0, FALSE, heapId);
    GFL_ArcToolFree(arc);
    file = GFL_G2DIOReadNCLR(ARCID_IRC_COMPAT, 16, &palette, heapId);
    src = palette->pRawData;
    MI_CpuCopy8(src, flash->target, sizeof(flash->target));
    MI_CpuCopy8(src + 0x80, flash->target, sizeof(flash->target));
    GFL_HeapFree(file);
    MI_CpuCopy8(flash->target, flash->colors, sizeof(flash->colors));
    for (i = 0; i < 13; i++) {
        flash->colors[i] = GX_RGB(31, 31, 31);
    }
    for (i = 0; i < FLASH_COLOR_COUNT; i++) {
        NNS_GfdRegisterNewVramTransferTask(NNS_GFD_DST_2D_BG_PLTT_MAIN, 8 * 0x20 + i * 2, &flash->colors[i], 2);
    }
}

// pokebw2's IrcCompatFlash_Free
void func_ov105_021b8598(IrcCompatFlash *flash) {
    MI_CpuFill8(flash, 0, sizeof(IrcCompatFlash));
}

// pokebw2's IrcCompatFlash_Update
static void func_ov105_021b85a4(IrcCompatFlash *flash) {
    int k;

    if (!flash->active) {
        return;
    }
    switch (flash->phase) {
    case 0:
        if (flash->step > flash->lastStep) {
            if (flash->carry) {
                flash->phase = 1;
                return;
            }
            flash->done = TRUE;
            flash->active = FALSE;
            return;
        }
        if (flash->frame == 0) {
            GFL_SndSEPlay(SEQ_SE_FLD_165 + flash->step);
        }
        for (k = 0; k < data_ov105_021b8984[flash->step].count; k++) {
            u8 color = data_ov105_021b8984[flash->step].colors[k];

            func_ov105_021b8734(&flash->colors[color], flash->frame, 40, 8, color, GX_RGB(31, 31, 31),
                                      flash->target[color]);
        }
        if (flash->frame++ >= 40) {
            flash->frame = 0;
            flash->step++;
        }
        break;
    case 1:
        if (flash->frame++ >= 60) {
            flash->frame = 0;
            flash->phase = 2;
        }
        break;
    case 2:
        if (flash->frame == 0) {
            GFL_SndSEPlay(SEQ_SE_FLD_164 + flash->step);
        }
        for (k = 0; k < data_ov105_021b8984[flash->step - 1].count; k++) {
            u8 color = data_ov105_021b8984[flash->step - 1].colors[k];

            func_ov105_021b8734(&flash->colors[color], flash->frame, 40, 8, color, flash->target[color],
                                      GX_RGB(31, 31, 31));
        }
        if (flash->frame++ >= 40) {
            flash->frame = 0;
            flash->done = TRUE;
            flash->active = FALSE;
        }
        break;
    }
}

// pokebw2's IrcCompatFlash_Start
void func_ov105_021b871c(IrcCompatFlash *flash) {
    flash->active = TRUE;
    flash->done = FALSE;
    flash->step = 1;
    flash->phase = 0;
    flash->frame = 0;
}

// pokebw2's IrcCompatFlash_IsDone
BOOL func_ov105_021b8730(IrcCompatFlash *flash) {
    return flash->done;
}

// pokebw2's IrcCompatFlash_BlendColor
static void func_ov105_021b8734(u16 *dest, u16 frame, u16 duration, u8 palette, u8 index, u16 from, u16 to) {
    u8 fromR = from & GX_RGB_R_MASK;
    u8 fromG = (from & GX_RGB_G_MASK) >> GX_RGB_G_SHIFT;
    u8 fromB = (from & GX_RGB_B_MASK) >> GX_RGB_B_SHIFT;
    u8 toR = to & GX_RGB_R_MASK;
    u8 toG = (to & GX_RGB_G_MASK) >> GX_RGB_G_SHIFT;
    u8 toB = (to & GX_RGB_B_MASK) >> GX_RGB_B_SHIFT;
    int stepB = (toB - fromB) * frame / duration;
    int stepR = (toR - fromR) * frame / duration;
    int stepG = (toG - fromG) * frame / duration;

    *dest = GX_RGB((u8)(fromR + stepR), (u8)(fromG + stepG), (u8)(fromB + stepB));
    NNS_GfdRegisterNewVramTransferTask(NNS_GFD_DST_2D_BG_PLTT_MAIN, palette * 0x20 + index * 2, dest, 2);
}

// pokebw2's IrcCompat_GetTier
u32 func_ov105_021b87e8(u8 score, BOOL *carry) {
    int next;
    u32 tier;

    if (score == 100) {
        next = 0xffff;
        tier = 5;
    } else if (score >= 80) {
        next = 100;
        tier = 4;
    } else if (score >= 60) {
        next = 80;
        tier = 3;
    } else if (score >= 30) {
        next = 60;
        tier = 2;
    } else if (score != 0) {
        next = 30;
        tier = 1;
    } else if (score == 0) {
        next = 0;
        tier = 0;
    } else {
        next = 100;
        tier = 1;
    }
    if (carry != NULL) {
        *carry = score + 3 > next ? TRUE : FALSE;
    }
    return tier;
}

// pokebw2's IrcCompat_GetRewardCount
u32 func_ov105_021b8840(u32 score, BOOL bothLogs) {
    u32 count = bothLogs ? 2 : 1;

    if (score == 100) {
        return count * 3;
    }
    if (score > 79) {
        count *= 2;
    }
    return count;
}

u32 func_ov105_021b8860(IrcCompatResultWork *wk) {
    u32 result = REWARD_NONE;
    TrainerLogSave *log = func_0200a8a0(GameData_GetSaveControl(GSYS_GetGameData(wk->param->gsys)));
    CompatibleIrcProfile *profile = wk->param->profile;
    PlayerInfo *info = &profile->trainer;

    if (!func_0200a9a4(log, getIDAsUInt(info)) && wk->param->gsys != NULL) {
        BagSave *bag = GameData_GetBag(GSYS_GetGameData(wk->param->gsys));
        u32 owned = BagSave_GetItemCountByID(bag, ITEM_SWEET_HEART, HEAPID_IRC_COMPAT);

        if (owned == SWEET_HEART_MAX) {
            result = REWARD_NO_ROOM;
        } else {
            u32 count = wk->reward;

            if (SWEET_HEART_MAX - owned < count) {
                count = SWEET_HEART_MAX - owned;
            }
            if (BagSave_AddItem(bag, ITEM_SWEET_HEART, count, HEAPID_IRC_COMPAT)) {
                result = REWARD_GIVEN;
            } else {
                result = REWARD_NO_ROOM;
            }
        }
    }
    if (wk->param->profile != NULL) {
        u16 *name = GetPlayerName(info);
        u8 unk10_17 = getTrainerGender(info) != 0 ? 0 : 1;

        func_0200aa68(log, name, wk->param->result, unk10_17, profile->birthMonth, profile->birthDay, getIDAsUInt(info),
                      profile->species, profile->form, profile->sex, profile->isEgg);
    }
    if (wk->param->gsys != NULL) {
        RecordAddOne(GameData_GetRecords(GSYS_GetGameData(wk->param->gsys)), RECORD_COMPAT_CHECKS);
    }
    return result;
}


