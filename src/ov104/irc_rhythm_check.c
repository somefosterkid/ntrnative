// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov178/irc_rhythm_check.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/compatible_irc_sys.h"
#include "app/irc_appbar.h"
#include "app/irc_compatible.h"
#include "app/irc_rhythm_check.h"
#include "constants/arc.h"
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
#include "gfl/net_command.h"
#include "gfl/net_handle.h"
#include "gfl/net_system.h"
#include "gfl/net.h"
#include "gfl/proc.h"
#include "gfl/random.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "gfl/touchpanel.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_trig.h"
#include "nitro/fx/fx_vec.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "nitro/math.h"
#include "nitro/os.h"
#include "nnsys/g2d.h"
#include "nnsys/gfd.h"
#include "save/player_info.h"
#include "save/save_control.h"
#include "save/trainer_log.h"
#include "system/bmp_winframe.h"
#include "system/game_data.h"
#include "system/game_system.h"
#include "system/gf_font.h"
#include "system/printsys.h"
#include "system/wordset.h"
#include "text/system/btl_main_7.h"

// The Rhythm Check of the compatibility checker: its graphics, its windows, the countdown, the sparkles of the taps,
// the flash of the background, the taps that the two players compare, the link that sends them and the steps of the
// screen. The ROM has no string for the file, so the name is a guess. Names are ours

#define HEAP_ID 0x38
#define BG_COUNT 7
#define CLACT_COUNT 35
#define PARTICLE_COUNT 30
#define COUNTDOWN_ACTOR 31

// The command that sends a player's taps to the other
#define NET_COMMAND_BASE 0xf00

// The taps of a player: ten are kept, and the taps after them are only counted
#define TAP_CAPACITY 15
#define TAP_COUNT 10
#define EXTRA_TAP_MAX 99

// The area of the touch screen that counts as a tap
#define TOUCH_MAX_X 256
#define TOUCH_MAX_Y 168

#define COUNTDOWN_FRAME_COUNT 60
#define COUNTDOWN_COUNT 3
#define CONNECTED_FRAMES 60
#define SCORE_FRAMES 30

// How well a tap kept the rhythm of the two before it, which picks the color of its sparkles
enum {
    TAP_GRADE_EARLY_FAR,
    TAP_GRADE_EARLY,
    TAP_GRADE_UNUSED,
    TAP_GRADE_ON_BEAT,
    TAP_GRADE_UNUSED_4,
    TAP_GRADE_LATE,
    TAP_GRADE_LATE_FAR,
};

// What the sparkles of the system do by themselves
enum {
    PARTICLES_RISE,
    PARTICLES_TAPS_ONLY,
    PARTICLES_CONVERGE,
};

// The steps of func_ov104_021b8198
enum {
    START_BEGIN,
    START_HIDE_BG,
    START_UNUSED,
    START_COUNTDOWN,
};

// The steps of func_ov104_021b8384
enum {
    RESULT_EXCHANGE_IDS_FIRST,
    RESULT_PRINT_CHECKING,
    RESULT_SEND_TAPS,
    RESULT_SEND_COUNT,
    RESULT_EXCHANGE_IDS_LAST,
    RESULT_WAIT_CONNECTED,
    RESULT_WAIT_SCORE,
    RESULT_FINISH,
};

typedef struct {
    int x;
    int y;
} IrcRhythmPoint;

static inline void IrcRhythmPoint_Set(IrcRhythmPoint *point, int x, int y) {
    point->x = x;
    point->y = y;
}

// A tap: when it was, and how long after the one before
typedef struct {
    int time;
    int interval;
} IrcRhythmTap;

// What a player sends the other
typedef struct {
    IrcRhythmTap taps[TAP_CAPACITY];
    u16 count;
    u16 extraTaps;
    u64 start;
} IrcRhythmData;

typedef struct {
    CompatibleIrcSys *sys;
    int state;
    IrcRhythmData peer;
    IrcRhythmData own;
    IrcRhythmData send;
    BOOL received;
} IrcRhythmNet;

typedef struct {
    ClActUnit *unit;
    u32 palette;
    u32 chars;
    u32 cellAnims;
    u32 subPalette;
    u32 subChars;
    u32 subCellAnims;
    u32 countdownPalette;
    u32 countdownChars;
    u32 countdownCellAnims;
    ClActor *actors[CLACT_COUNT];
} IrcRhythmClAct;

typedef struct {
    // The characters of the window frame
    u32 frameChars;
} IrcRhythmBG;

typedef struct {
    IrcRhythmBG bg;
    IrcRhythmClAct clact;
    TCB *vblankTask;
} IrcRhythmGraphic;

typedef struct {
    Font *font;
    MsgData *msgData;
    PrintQueue *queue;
    WordSet *wordSet;
} IrcRhythmText;

typedef struct {
    BmpWin *window;
    PrintWindow printWindow;
    StrBuf *strbuf;
    u16 color;
} IrcRhythmWindow;

// A sparkle that flies from a point to another
typedef struct {
    ClActor *actor;
    u32 surface;
    BOOL active;
    VecFx32 start;
    VecFx32 end;
    u32 frame;
    u32 duration;
} IrcRhythmParticle;

typedef struct {
    IrcRhythmParticle particles[PARTICLE_COUNT];
    int mode;
    u32 timer;
    u32 interval;
} IrcRhythmParticles;

// The 3, 2, 1 before the taps
typedef struct {
    ClActor *actor;
    u32 unk4;
    u32 unk8;
    u32 frame;
    u32 count;
    BOOL active;
    BOOL done;
} IrcRhythmCountdown;

// The color of palette 5 of BG 0 that swings between two colors while the taps last
typedef struct {
    u16 color;
    u16 angle;
    BOOL enabled;
    u32 unk8;
    BOOL active;
} IrcRhythmFlash;

typedef struct IrcRhythmCheckWork IrcRhythmCheckWork;
typedef void (*IrcRhythmStepFunc)(IrcRhythmCheckWork *wk, u16 *state);

struct IrcRhythmCheckWork {
    IrcRhythmGraphic graphic;
    IrcRhythmText text;
    // The message
    IrcRhythmWindow messageWindow;
    IrcRhythmNet net;
    IrcRhythmParticles particles;
    IrcAppBar *button;
    IrcRhythmCountdown countdown;
    IrcRhythmFlash flash;
    // The title
    IrcRhythmWindow titleWindow;
    IrcRhythmWindow hintWindow;
    IrcRhythmStepFunc step;
    u16 stepState;
    u16 timer;
    BOOL stepDone;
    u32 unk810;
    IrcRhythmData data;
    u32 unk898;
    // Whether the taps are done and are being scored
    BOOL scoring;
    u16 resultState;
    u16 resultTimer;
    TCB *vblankTask;
    BOOL clearBlend;
    // Whether a tap was made this frame
    BOOL tapped;
    IrcCompatibleParam2 *param;
};

static BOOL func_ov104_021b7560(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov104_021b7768(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov104_021b77ec(GameProc *proc, u32 *state, void *param, void *work);
static void func_ov104_021b78ec(IrcRhythmGraphic *graphic, HeapID heapId);
static void func_ov104_021b7940(IrcRhythmGraphic *graphic);
static void func_ov104_021b795c(IrcRhythmGraphic *graphic);
static ClActor *func_ov104_021b7968(IrcRhythmGraphic *graphic, int index);
static ClActUnit *func_ov104_021b7974(IrcRhythmGraphic *graphic);
static void func_ov104_021b7980(TCB *tcb, void *data);
static void func_ov104_021b7994(IrcRhythmBG *bg, HeapID heapId);
static void func_ov104_021b7ad4(IrcRhythmBG *bg);
static void func_ov104_021b7b24(IrcRhythmBG *bg);
static void func_ov104_021b7b2c(IrcRhythmClAct *clact, const BGSysVRAMConfig *vramConfig, HeapID heapId);
static void func_ov104_021b7ca4(IrcRhythmClAct *clact);
static void func_ov104_021b7d0c(IrcRhythmClAct *clact);
static void func_ov104_021b7d14(IrcRhythmClAct *clact);
static ClActor *func_ov104_021b7d1c(IrcRhythmClAct *clact, int index);
static ClActUnit *func_ov104_021b7d24(IrcRhythmClAct *clact);
static void func_ov104_021b7d28(IrcRhythmText *text, int fontType, HeapID heapId);
static void func_ov104_021b7dac(IrcRhythmText *text);
static BOOL func_ov104_021b7dd4(IrcRhythmText *text);
static Font *func_ov104_021b7de0(IrcRhythmText *text);
static PrintQueue *func_ov104_021b7de4(IrcRhythmText *text);
static MsgData *func_ov104_021b7de8(IrcRhythmText *text);
static WordSet *func_ov104_021b7dec(IrcRhythmText *text);
static void func_ov104_021b7df0(IrcRhythmWindow *win, u8 bg, u8 x, u8 y, u8 width, u8 height, u8 palette,
                                 HeapID heapId);
static void func_ov104_021b7e58(IrcRhythmWindow *win, u8 bg, u8 x, u8 y, u8 width, u8 height, u8 palette,
                                          u16 color, u8 fromEnd, HeapID heapId);
static void func_ov104_021b7ec8(IrcRhythmWindow *win);
static BOOL func_ov104_021b7ee4(IrcRhythmWindow *win, IrcRhythmText *text);
static void func_ov104_021b7f1c(IrcRhythmWindow *win, IrcRhythmText *text, u32 msgId, int x, s16 y);
static void func_ov104_021b7f80(IrcRhythmWindow *win, IrcRhythmText *text, u32 msgId);
void func_ov104_021b7f90(IrcRhythmWindow *win, IrcRhythmText *text, u32 msgId, u16 color);
static void func_ov104_021b8030(IrcRhythmWindow *win, IrcRhythmText *text, u32 msgId,
                                           CompatibleIrcProfile *other, s16 x, s16 y, HeapID heapId);
static void func_ov104_021b80b8(IrcRhythmWindow *win);
static void func_ov104_021b80d4(IrcRhythmCountdown *countdown, IrcRhythmGraphic *graphic, HeapID heapId);
static void func_ov104_021b80f8(IrcRhythmCountdown *countdown);
void func_ov104_021b8104(IrcRhythmCountdown *countdown);
void func_ov104_021b816c(IrcRhythmCountdown *countdown);
BOOL func_ov104_021b8178(IrcRhythmCountdown *countdown);
static void func_ov104_021b817c(IrcRhythmCheckWork *wk, IrcRhythmStepFunc step);
void func_ov104_021b818c(IrcRhythmCheckWork *wk);
void func_ov104_021b8198(IrcRhythmCheckWork *wk, u16 *state);
void func_ov104_021b8220(IrcRhythmCheckWork *wk, u16 *state);
static void func_ov104_021b829c(IrcRhythmCheckWork *wk, u16 *state);
void func_ov104_021b8384(IrcRhythmCheckWork *wk, u16 *state);
void func_ov104_021b8528(IrcRhythmCheckWork *wk, u16 *state);
static void func_ov104_021b85d8(IrcRhythmData *data);
void func_ov104_021b85e4(IrcRhythmData *data);
BOOL func_ov104_021b8614(IrcRhythmData *data);
void func_ov104_021b8624(IrcRhythmData *data);
int func_ov104_021b86a0(IrcRhythmData *data);
static void func_ov104_021b8724(IrcRhythmNet *net, CompatibleIrcSys *sys);
static void func_ov104_021b874c(IrcRhythmNet *net);
BOOL func_ov104_021b8768(IrcRhythmNet *net, const IrcRhythmData *data);
void func_ov104_021b87f4(IrcRhythmNet *net, IrcRhythmData *dest);
static void func_ov104_021b8810(int netId, int size, const void *data, void *work, NetHandle *handle);
static void *func_ov104_021b8834(int netId, void *work, int size);
BOOL func_ov104_021b884c(IrcRhythmPoint *point);
u8 func_ov104_021b8880(IrcRhythmCheckWork *wk);
static void func_ov104_021b89dc(NNS_GFD_DST_TYPE type, u16 *dest, int angle, u32 palette, u8 index, u16 from, u16 to);
static void func_ov104_021b8a88(IrcRhythmParticles *particles, IrcRhythmGraphic *graphic, int mode, int firstActor,
                                    u32 surface);
static void func_ov104_021b8adc(IrcRhythmParticles *particles);
static void func_ov104_021b8aec(IrcRhythmParticles *particles);
void func_ov104_021b8c3c(IrcRhythmParticles *particles, const IrcRhythmPoint *origin, int palette);
static void func_ov104_021b8d08(IrcRhythmParticle *particle, ClActor *actor, u32 surface);
static void func_ov104_021b8d20(IrcRhythmParticle *particle);
static void func_ov104_021b8da4(IrcRhythmParticle *particle, const IrcRhythmPoint *from, const IrcRhythmPoint *to,
                                    int palette, u32 duration);
static BOOL func_ov104_021b8e8c(IrcRhythmParticle *particle);
static void func_ov104_021b8e90(IrcRhythmFlash *flash, HeapID heapId);
static void func_ov104_021b8ea4(IrcRhythmFlash *flash);
static void func_ov104_021b8eb0(IrcRhythmFlash *flash);
void func_ov104_021b8f00(IrcRhythmFlash *flash);
static void func_ov104_021b8f08(TCB *tcb, void *data);

// Nothing in the overlay or in another refers to it, and it is the first of the overlay's read-only data
const u32 IRC_RHYTHM_CHECK_UNUSED_400 = 0x400;

static const NetCommand data_ov104_021b8f28[1] = {
    { func_ov104_021b8810, func_ov104_021b8834 },
};

static const BGSysLCDConfig data_ov104_021b8f30 = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_1, GX_BG0_AS_2D };

static const u32 data_ov104_021b8f40[BG_COUNT] = { BGMODE_TEXT, BGMODE_TEXT, BGMODE_TEXT, BGMODE_TEXT,
                                                 BGMODE_TEXT, BGMODE_TEXT, BGMODE_AFFINE };

static const u32 data_ov104_021b8f5c[BG_COUNT] = { 0, 1, 2, 4, 5, 6, 7 };

static const BGSysVRAMConfig data_ov104_021b8f78 = {
    GX_VRAM_BG_128_A,  GX_VRAM_BGEXTPLTT_NONE,  GX_VRAM_SUB_BG_128_C,        GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_128_B, GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_16_I,        GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_NONE,  GX_VRAM_TEXPLTT_NONE,    GX_OBJVRAMMODE_CHAR_1D_128K, GX_OBJVRAMMODE_CHAR_1D_128K,
};

const GameProcFunctions data_ov104_021b8fa8 = {
    func_ov104_021b7560,
    func_ov104_021b77ec,
    func_ov104_021b7768,
};

static const BGSetup data_ov104_021b8fb4[BG_COUNT] = {
    { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x04000, 0x8000,
      GX_BG_EXTPLTT_01, 1, GX_BG_AREAOVER_XLU, FALSE },
    { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x0800, GX_BG_CHARBASE_0x10000, 0x8000,
      GX_BG_EXTPLTT_01, 2, GX_BG_AREAOVER_XLU, FALSE },
    { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x1000, GX_BG_CHARBASE_0x18000, 0x8000,
      GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE },
    { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x04000, 0x8000,
      GX_BG_EXTPLTT_01, 1, GX_BG_AREAOVER_XLU, FALSE },
    { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x0800, GX_BG_CHARBASE_0x0c000, 0x8000,
      GX_BG_EXTPLTT_01, 2, GX_BG_AREAOVER_XLU, FALSE },
    { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x1000, GX_BG_CHARBASE_0x10000, 0x8000,
      GX_BG_EXTPLTT_01, 3, GX_BG_AREAOVER_XLU, FALSE },
    { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_256, GX_BG_SCRBASE_0x1800, GX_BG_CHARBASE_0x14000, 0x4000,
      GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE },
};

// pokebw2's IrcRhythmCheck_Init
static BOOL func_ov104_021b7560(GameProc *proc, u32 *state, void *param, void *work) {
    IrcRhythmCheckWork *wk;
    IrcCompatibleParam2 *rhythmParam = param;
    ClActUnit *unit;
    Font *font;
    PrintQueue *queue;

    GFL_HeapCreateChild(HEAPID_USER, HEAP_ID, 0x20000);
    wk = GFL_ProcInitSubsystem(proc, sizeof(IrcRhythmCheckWork), HEAP_ID);
    MI_CpuFill8(wk, 0, sizeof(IrcRhythmCheckWork));
    wk->param = rhythmParam;
    func_ov104_021b78ec(&wk->graphic, HEAP_ID);
    func_ov104_021b7d28(&wk->text, 0, HEAP_ID);
    if (wk->param->gsys != NULL) {
        GSYS_GetGameCommSystem(wk->param->gsys);
    }
    func_ov104_021b7df0(&wk->messageWindow, 5, 1, 18, 30, 5, 8, HEAP_ID);
    BmpWin_DrawFrame(wk->messageWindow.window, 0, (u16)wk->graphic.bg.frameChars, 6);
    func_ov104_021b8e90(&wk->flash, HEAP_ID);
    if (wk->param->gsys != NULL) {
        func_ov104_021b8030(&wk->messageWindow, &wk->text, BtlMain7_Text_000,
                                       wk->param->profile, 0, 0, HEAP_ID);
    } else {
        func_ov104_021b7f1c(&wk->messageWindow, &wk->text, BtlMain7_Text_000, 0, 0);
    }
    func_ov104_021b80d4(&wk->countdown, &wk->graphic, HEAP_ID);
    func_ov104_021b7e58(&wk->titleWindow, 7, 6, 3, 20, 5, 8, 1, TRUE, HEAP_ID);
    func_ov104_021b7f80(&wk->titleWindow, &wk->text, BtlMain7_Text_007);
    GFL_BGSysScaleBGReq(7, 3, 0x1000);
    GFL_BGSysScaleBGReq(7, 6, 0x1000);
    GFL_BGSysAdjustBGOriginReq(7, 9, 0x80);
    GFL_BGSysAdjustBGOriginReq(7, 12, 0x28);
    func_ov104_021b7e58(&wk->hintWindow, 2, 8, 10, 16, 2, 13, 0, FALSE, HEAP_ID);
    func_ov104_021b8724(&wk->net, wk->param->sys);
    func_ov104_021b85d8(&wk->data);
    func_ov104_021b8a88(&wk->particles, &wk->graphic, PARTICLES_TAPS_ONLY, 0, 0);
    unit = func_ov104_021b7974(&wk->graphic);
    font = func_ov104_021b7de0(&wk->text);
    queue = func_ov104_021b7de4(&wk->text);
    wk->button = func_ov101_021b70dc(2, unit, 0, 14, 13, 2, font, queue, HEAP_ID);
    func_ov101_021b63cc(wk->param->sys, 1);
    G2_SetBlendBrightness(0x22, -8);
    wk->vblankTask = GFL_VBlankTCBAdd(func_ov104_021b8f08, wk, 0);
    func_ov104_021b817c(wk, func_ov104_021b8198);
    return TRUE;
}

// pokebw2's IrcRhythmCheck_Exit
static BOOL func_ov104_021b7768(GameProc *proc, u32 *state, void *param, void *work) {
    IrcRhythmCheckWork *wk = work;

    GFL_TCBRemove(wk->vblankTask);
    func_ov104_021b8adc(&wk->particles);
    G2_BlendNone();
    func_ov101_021b71c0(wk->button);
    func_ov104_021b874c(&wk->net);
    func_ov104_021b80f8(&wk->countdown);
    func_ov104_021b8ea4(&wk->flash);
    func_ov104_021b7ec8(&wk->hintWindow);
    func_ov104_021b7ec8(&wk->titleWindow);
    func_ov104_021b7ec8(&wk->messageWindow);
    func_ov104_021b7940(&wk->graphic);
    func_ov104_021b7dac(&wk->text);
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(HEAP_ID);
    return TRUE;
}

// pokebw2's IrcRhythmCheck_Main
static BOOL func_ov104_021b77ec(GameProc *proc, u32 *state, void *param, void *work) {
    IrcRhythmCheckWork *wk = work;

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
        func_ov104_021b8eb0(&wk->flash);
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
    if (func_ov104_021b7dd4(&wk->text)) {
        func_ov104_021b7ee4(&wk->messageWindow, &wk->text);
        func_ov104_021b7ee4(&wk->titleWindow, &wk->text);
        func_ov104_021b7ee4(&wk->hintWindow, &wk->text);
    }
    func_ov104_021b795c(&wk->graphic);
    func_ov104_021b8aec(&wk->particles);
    if (!wk->tapped) {
        func_ov101_021b71e4(wk->button);
    }
    func_ov101_021b63d0(wk->param->sys);
    return FALSE;
}

// pokebw2's IrcRhythmGraphic_Create
static void func_ov104_021b78ec(IrcRhythmGraphic *graphic, HeapID heapId) {
    MI_CpuFill8(graphic, 0, sizeof(IrcRhythmGraphic));
    GFL_BGSysInitVRAM(GX_VRAM_LCDC_NONE);
    GFL_BGSysSetVRAMBanks(&data_ov104_021b8f78);
    GFL_BGSysSetDisplayLayout(0);
    GFL_BGSysEnableEngines();
    GFL_BGSysDisableAllA();
    func_ov104_021b7b2c(&graphic->clact, &data_ov104_021b8f78, heapId);
    func_ov104_021b7994(&graphic->bg, heapId);
    graphic->vblankTask = GFL_VBlankTCBAdd(func_ov104_021b7980, graphic, 0);
}

// pokebw2's IrcRhythmGraphic_Free
static void func_ov104_021b7940(IrcRhythmGraphic *graphic) {
    GFL_TCBRemove(graphic->vblankTask);
    func_ov104_021b7ad4(&graphic->bg);
    func_ov104_021b7ca4(&graphic->clact);
}

// pokebw2's IrcRhythmGraphic_Update
static void func_ov104_021b795c(IrcRhythmGraphic *graphic) {
    func_ov104_021b7d0c(&graphic->clact);
}

// pokebw2's IrcRhythmGraphic_GetActor
static ClActor *func_ov104_021b7968(IrcRhythmGraphic *graphic, int index) {
    return func_ov104_021b7d1c(&graphic->clact, index);
}

// pokebw2's IrcRhythmGraphic_GetUnit
static ClActUnit *func_ov104_021b7974(IrcRhythmGraphic *graphic) {
    return func_ov104_021b7d24(&graphic->clact);
}

// pokebw2's IrcRhythmGraphic_VBlank
static void func_ov104_021b7980(TCB *tcb, void *data) {
    IrcRhythmGraphic *graphic = data;

    func_ov104_021b7b24(&graphic->bg);
    func_ov104_021b7d14(&graphic->clact);
}

// pokebw2's IrcRhythmBG_Init
static void func_ov104_021b7994(IrcRhythmBG *bg, HeapID heapId) {
    ArcTool *arc;
    int i;

    GFL_BGSysCreate(heapId);
    BmpWin_InitAllocator(heapId);
    GFL_BGSysSetLCDConfig(&data_ov104_021b8f30);
    for (i = 0; i < BG_COUNT; i++) {
        GFL_BGSysCreateBG(data_ov104_021b8f5c[i], &data_ov104_021b8fb4[i], data_ov104_021b8f40[i]);
        GFL_BGSysClearBG(data_ov104_021b8f5c[i]);
        GFL_BGSysSetBGEnabled(data_ov104_021b8f5c[i], TRUE);
    }
    arc = GFL_ArcSysCreateFileHandle(ARCID_IRC_COMPAT, heapId);
    GFL_G2DIOLoadArcNCLRDefault(arc, 1, PALTYPE_MAIN_BG, 0, 0, heapId);
    GFL_G2DIOLoadArcNCLRDefault(arc, 1, PALTYPE_SUB_BG, 0, 0, heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 36, 1, 0, 0, FALSE, heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 0, 6, 0, 0, FALSE, heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 48, 4, 0, 0, FALSE, heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 37, 1, 0, 0, FALSE, heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 4, 6, 0, 0, FALSE, heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 47, 4, 0, 0, FALSE, heapId);
    GFL_BGSysFillChar(5, 0, 1, 0);
    bg->frameChars = GFL_BGSysLoadArcNCGRDynamic(arc, 49, 5, 0, FALSE, heapId);
    GFL_ArcToolFree(arc);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, PALTYPE_MAIN_BG, 0x1a0, 0x20, heapId);
    GFL_BGSysFillChar(2, 0, 1, 0);
}

// pokebw2's IrcRhythmBG_Free
static void func_ov104_021b7ad4(IrcRhythmBG *bg) {
    int i;

    GFL_BGSysFreeFilledChar(2, 1, 0);
    GFL_BGSysFreeCharMemory(5, (u16)bg->frameChars, (u16)(bg->frameChars >> 16));
    GFL_BGSysFreeFilledChar(5, 1, 0);
    for (i = 0; i < BG_COUNT; i++) {
        GFL_BGSysReleaseBG(data_ov104_021b8f5c[i]);
    }
    BmpWin_FreeAllocator();
    GFL_BGSysFree();
}

// pokebw2's IrcRhythmBG_VBlank
static void func_ov104_021b7b24(IrcRhythmBG *bg) {
    GFL_BGSysUpdate();
}

// pokebw2's IrcRhythmClAct_Init
static void func_ov104_021b7b2c(IrcRhythmClAct *clact, const BGSysVRAMConfig *vramConfig, HeapID heapId) {
    ArcTool *arc;
    ClActorSetup setup;
    int i;

    MI_CpuFill8(clact, 0, sizeof(IrcRhythmClAct));
    ClActSys_Create(&data_020a1448, vramConfig, heapId);
    clact->unit = func_0204b100(128, 0, heapId);
    func_0204b270(clact->unit);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_OBJ, TRUE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
    arc = GFL_ArcSysCreateFileHandle(ARCID_IRC_COMPAT, heapId);
    clact->palette = func_0204abf0(arc, 35, CLACT_VRAM_MAIN, 0, heapId);
    clact->chars = func_0204a6c8(arc, 34, FALSE, CLACT_VRAM_MAIN, heapId);
    clact->cellAnims = func_0204af28(arc, 33, 32, heapId);
    clact->countdownPalette = func_0204ac18(arc, 12, CLACT_VRAM_MAIN, 0xe0, 12, 1, heapId);
    clact->countdownChars = func_0204a6c8(arc, 11, FALSE, CLACT_VRAM_MAIN, heapId);
    clact->countdownCellAnims = func_0204af28(arc, 10, 9, heapId);
    clact->subPalette = func_0204abf0(arc, 35, CLACT_VRAM_SUB, 0, heapId);
    clact->subChars = func_0204a6c8(arc, 34, FALSE, CLACT_VRAM_SUB, heapId);
    clact->subCellAnims = func_0204af28(arc, 33, 32, heapId);
    GFL_ArcToolFree(arc);
    MI_CpuFill8(&setup, 0, sizeof(ClActorSetup));
    for (i = 0; i < PARTICLE_COUNT; i++) {
        clact->actors[i] = func_0204b294(clact->unit, clact->chars, clact->palette, clact->cellAnims, &setup,
                                         CLACT_SURFACE_MAIN, heapId);
        func_0204b3dc(clact->actors[i], FALSE);
        func_0204b84c(clact->actors[i], 3);
        func_0204b92c(clact->actors[i], TRUE);
        func_0204b878(clact->actors[i], 0);
    }
    setup.x = 128;
    setup.y = 96;
    for (i = COUNTDOWN_ACTOR; i < COUNTDOWN_ACTOR + 3; i++) {
        setup.sequence = i - 30;
        clact->actors[i] = func_0204b294(clact->unit, clact->countdownChars, clact->countdownPalette,
                                         clact->countdownCellAnims, &setup, CLACT_SURFACE_MAIN, heapId);
        func_0204b3dc(clact->actors[i], FALSE);
        func_0204b6f4(clact->actors[i], 0, TRUE);
    }
}

// pokebw2's IrcRhythmClAct_Free
static void func_ov104_021b7ca4(IrcRhythmClAct *clact) {
    int i;

    for (i = 0; i < CLACT_COUNT; i++) {
        if (clact->actors[i] != NULL) {
            func_0204b3b4(clact->actors[i]);
        }
    }
    func_0204ada4(clact->palette);
    func_0204a8d4(clact->chars);
    func_0204afd8(clact->cellAnims);
    func_0204ada4(clact->subPalette);
    func_0204a8d4(clact->subChars);
    func_0204afd8(clact->subCellAnims);
    func_0204ada4(clact->countdownPalette);
    func_0204a8d4(clact->countdownChars);
    func_0204afd8(clact->countdownCellAnims);
    func_0204b1cc(clact->unit);
    func_0204a5ac();
    MI_CpuFill8(clact, 0, sizeof(IrcRhythmClAct));
}

// pokebw2's IrcRhythmClAct_Update
static void func_ov104_021b7d0c(IrcRhythmClAct *clact) {
    func_0204a600();
}

// pokebw2's IrcRhythmClAct_VBlank
static void func_ov104_021b7d14(IrcRhythmClAct *clact) {
    func_0204a648();
}

// pokebw2's IrcRhythmClAct_GetActor
static ClActor *func_ov104_021b7d1c(IrcRhythmClAct *clact, int index) {
    return clact->actors[index];
}

// pokebw2's IrcRhythmClAct_GetUnit
static ClActUnit *func_ov104_021b7d24(IrcRhythmClAct *clact) {
    return clact->unit;
}

// pokebw2's IrcRhythmText_Init
static void func_ov104_021b7d28(IrcRhythmText *text, int fontType, HeapID heapId) {
    MI_CpuFill8(text, 0, sizeof(IrcRhythmText));
    func_0201dd60();
    switch (fontType) {
    case 0:
        text->font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, heapId);
        break;
    case 1:
        text->font = GFL_FontCreate(ARCID_FONT, 1, 0, FALSE, heapId);
        break;
    }
    text->queue = func_0201c440(heapId);
    text->msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_BTL_MAIN_7, heapId);
    text->wordSet = GFL_WordSetSystemCreateDefault(heapId);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, PALTYPE_SUB_BG, 0x1c0, 0x20, heapId);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, PALTYPE_MAIN_BG, 0x1c0, 0x20, heapId);
}

// pokebw2's IrcRhythmText_Free
static void func_ov104_021b7dac(IrcRhythmText *text) {
    GFL_WordSetSystemFree(text->wordSet);
    GFL_MsgDataFree(text->msgData);
    func_0201c4c0(text->queue);
    GFL_FontFree(text->font);
    MI_CpuFill8(text, 0, sizeof(IrcRhythmText));
}

// pokebw2's IrcRhythmText_Update
static BOOL func_ov104_021b7dd4(IrcRhythmText *text) {
    return func_0201c4e4(text->queue);
}

// pokebw2's IrcRhythmText_GetFont
static Font *func_ov104_021b7de0(IrcRhythmText *text) {
    return text->font;
}

// pokebw2's IrcRhythmText_GetQueue
static PrintQueue *func_ov104_021b7de4(IrcRhythmText *text) {
    return text->queue;
}

// pokebw2's IrcRhythmText_GetMsgData
static MsgData *func_ov104_021b7de8(IrcRhythmText *text) {
    return text->msgData;
}

// pokebw2's IrcRhythmText_GetWordSet
static WordSet *func_ov104_021b7dec(IrcRhythmText *text) {
    return text->wordSet;
}

static void func_ov104_021b7df0(IrcRhythmWindow *win, u8 bg, u8 x, u8 y, u8 width, u8 height, u8 palette,
                                 HeapID heapId) {
    MI_CpuFill8(win, 0, sizeof(IrcRhythmWindow));
    win->color = 15;
    win->window = BmpWin_CreateDynamic(bg, x, y, width, height, palette, TRUE);
    win->strbuf = GFL_StrBufCreate(255, heapId);
    PrintWindow_Init(&win->printWindow, win->window);
    func_ov104_021b80b8(win);
    BmpWin_TransferNow(win->window);
}

static void func_ov104_021b7e58(IrcRhythmWindow *win, u8 bg, u8 x, u8 y, u8 width, u8 height, u8 palette,
                                          u16 color, u8 fromEnd, HeapID heapId) {
    MI_CpuFill8(win, 0, sizeof(IrcRhythmWindow));
    win->color = color;
    win->window = BmpWin_CreateDynamic(bg, x, y, width, height, palette, fromEnd);
    win->strbuf = GFL_StrBufCreate(255, heapId);
    PrintWindow_Init(&win->printWindow, win->window);
    GFL_BitmapFill(BmpWin_GetBitmap(win->window), win->color);
    BmpWin_TransferNow(win->window);
}

// pokebw2's IrcRhythmWindow_Free
static void func_ov104_021b7ec8(IrcRhythmWindow *win) {
    GFL_StrBufFree(win->strbuf);
    BmpWin_Free(win->window);
    MI_CpuFill8(win, 0, sizeof(IrcRhythmWindow));
}

// pokebw2's IrcRhythmWindow_Flush
static BOOL func_ov104_021b7ee4(IrcRhythmWindow *win, IrcRhythmText *text) {
    PrintWindow_Flush(&win->printWindow, text->queue);
    return PrintWindow_IsPrinted(&win->printWindow);
}

// pokebw2's IrcRhythmWindow_PrintMsg
static void func_ov104_021b7f1c(IrcRhythmWindow *win, IrcRhythmText *text, u32 msgId, int x, s16 y) {
    MsgData *msgData = func_ov104_021b7de8(text);
    PrintQueue *queue = func_ov104_021b7de4(text);
    Font *font = func_ov104_021b7de0(text);

    GFL_BitmapFill(BmpWin_GetBitmap(win->window), win->color);
    GFL_MsgDataLoadStrbuf(msgData, msgId, win->strbuf);
    PrintWindow_PrintNoColor(&win->printWindow, queue, x, y, win->strbuf, font);
}

// pokebw2's IrcRhythmWindow_PrintMsgCentered
static void func_ov104_021b7f80(IrcRhythmWindow *win, IrcRhythmText *text, u32 msgId) {
    func_ov104_021b7f90(win, text, msgId, PRINT_COLOR(15, 14, 4));
}

// pokebw2's IrcRhythmWindow_PrintMsgCenteredColor
void func_ov104_021b7f90(IrcRhythmWindow *win, IrcRhythmText *text, u32 msgId, u16 color) {
    MsgData *msgData = func_ov104_021b7de8(text);
    PrintQueue *queue = func_ov104_021b7de4(text);
    Font *font = func_ov104_021b7de0(text);
    u16 x;
    u16 y;

    GFL_BitmapFill(BmpWin_GetBitmap(win->window), win->color);
    GFL_MsgDataLoadStrbuf(msgData, msgId, win->strbuf);
    x = func_02045738(win->window) * 4;
    y = func_02045740(win->window) * 4;
    x -= GFL_FontGetBlockWidth(win->strbuf, font, 0) / 2;
    y -= GFL_FontGetBlockHeight(win->strbuf, font) / 2;
    func_0201c724(queue, BmpWin_GetBitmap(win->printWindow.window), x, y, win->strbuf, font, color);
    win->printWindow.flushPending = TRUE;
}


static void func_ov104_021b8030(IrcRhythmWindow *win, IrcRhythmText *text, u32 msgId,
                                           CompatibleIrcProfile *other, s16 x, s16 y, HeapID heapId) {
    WordSet *wordSet;
    PrintQueue *queue;
    Font *font;
    MsgData *msgData;
    StrBuf *format;

    GFL_BitmapFill(BmpWin_GetBitmap(win->window), win->color);
    wordSet = func_ov104_021b7dec(text);
    msgData = func_ov104_021b7de8(text);
    copyVarForText(wordSet, 0, &other->trainer);
    format = GFL_MsgDataLoadStrbufNew(msgData, msgId);
    GFL_WordSetFormatStrbuf(wordSet, win->strbuf, format);
    GFL_StrBufFree(format);
    queue = func_ov104_021b7de4(text);
    font = func_ov104_021b7de0(text);
    PrintWindow_PrintNoColor(&win->printWindow, queue, x, y, win->strbuf, font);
}

// pokebw2's IrcRhythmWindow_Clear
static void func_ov104_021b80b8(IrcRhythmWindow *win) {
    GFL_BitmapFill(BmpWin_GetBitmap(win->window), win->color);
    BmpWin_FlushChar(win->window);
}

// pokebw2's IrcRhythmCountdown_Init
static void func_ov104_021b80d4(IrcRhythmCountdown *countdown, IrcRhythmGraphic *graphic, HeapID heapId) {
    int i;

    MI_CpuFill8(countdown, 0, sizeof(IrcRhythmCountdown));
    for (i = 0; i < COUNTDOWN_COUNT; i++) {
        countdown->actor = func_ov104_021b7968(graphic, COUNTDOWN_ACTOR + i);
    }
}

// pokebw2's IrcRhythmCountdown_Free
static void func_ov104_021b80f8(IrcRhythmCountdown *countdown) {
    MI_CpuFill8(countdown, 0, sizeof(IrcRhythmCountdown));
}

// pokebw2's IrcRhythmCountdown_Update
void func_ov104_021b8104(IrcRhythmCountdown *countdown) {
    if (countdown->active) {
        if (countdown->frame == 0) {
            GFL_SndSEPlay(SEQ_SE_SYS_43);
            func_0204b3dc(countdown->actor, TRUE);
            func_0204b878(countdown->actor, 4 - countdown->count);
        }
        if (countdown->frame++ >= COUNTDOWN_FRAME_COUNT) {
            countdown->frame = 0;
            countdown->count++;
        }
        if (countdown->count == COUNTDOWN_COUNT) {
            GFL_SndSEPlay(SEQ_SE_SYS_44);
            countdown->done = TRUE;
            countdown->active = FALSE;
            func_0204b3dc(countdown->actor, FALSE);
        }
    }
}

// pokebw2's IrcRhythmCountdown_Start
void func_ov104_021b816c(IrcRhythmCountdown *countdown) {
    countdown->frame = 0;
    countdown->count = 0;
    countdown->active = TRUE;
}

// pokebw2's IrcRhythmCountdown_IsDone
BOOL func_ov104_021b8178(IrcRhythmCountdown *countdown) {
    return countdown->done;
}

// pokebw2's IrcRhythmCheck_SetStep
static void func_ov104_021b817c(IrcRhythmCheckWork *wk, IrcRhythmStepFunc step) {
    wk->step = step;
    wk->stepState = 0;
}

// pokebw2's IrcRhythmCheck_SetStepDone
void func_ov104_021b818c(IrcRhythmCheckWork *wk) {
    wk->stepDone = TRUE;
}

// pokebw2's IrcRhythmCheck_StepStart
void func_ov104_021b8198(IrcRhythmCheckWork *wk, u16 *state) {
    switch (*state) {
    case 0:
        wk->timer = 0;
        *state = 1;
        break;
    case 1:
        GFL_BGSysSetBGEnabled(2, FALSE);
        *state = 3;
        break;
    case 3:
        func_ov104_021b816c(&wk->countdown);
        func_ov104_021b817c(wk, func_ov104_021b8220);
        break;
    }
    if (func_ov101_021b7250(wk->button) == 1) {
        wk->param->result = IRC_RHYTHM_CHECK_RESULT_CANCELLED;
        func_ov104_021b818c(wk);
    }
    if (func_ov101_021b6480(wk->param->sys) != 0) {
        func_ov104_021b817c(wk, func_ov104_021b8528);
    }
}


// pokebw2's IrcRhythmCheck_StepCountdown
void func_ov104_021b8220(IrcRhythmCheckWork *wk, u16 *state) {
    func_ov104_021b8104(&wk->countdown);
    if (func_ov104_021b8178(&wk->countdown)) {
        wk->clearBlend = TRUE;
        func_ov104_021b85e4(&wk->data);
        func_ov104_021b8f00(&wk->flash);
        func_ov104_021b817c(wk, func_ov104_021b829c);
    }
    if (func_ov101_021b7250(wk->button) == 1) {
        wk->param->result = IRC_RHYTHM_CHECK_RESULT_CANCELLED;
        func_ov104_021b818c(wk);
    }
    if (func_ov101_021b6480(wk->param->sys) != 0) {
        func_ov104_021b817c(wk, func_ov104_021b8528);
    }
}

// pokebw2's IrcRhythmCheck_StepTap
// pokebw2's IrcRhythmCheck_StepTap
// pokebw2's IrcRhythmCheck_StepTap
static void func_ov104_021b829c(IrcRhythmCheckWork *wk, u16 *state) {
    IrcRhythmData *data = &wk->data;
    IrcRhythmPoint touch;

    wk->tapped = FALSE;
    if (!func_ov101_021b727c(wk->button)) {
        if (func_ov104_021b884c(&touch)) {
            int grade = func_ov104_021b86a0(data);

            func_ov104_021b8c3c(&wk->particles, &touch, grade);
            if (wk->resultState == 0) {
                func_ov104_021b8624(data);
            }
            wk->tapped = TRUE;
        }
    }
    if (func_ov104_021b8614(data)) {
        if (!wk->scoring) {
            func_ov104_021b7f1c(&wk->messageWindow, &wk->text, BtlMain7_Text_001, 0, 0);
            GFL_SndSEPlay(SEQ_SE_SYS_21);
            wk->scoring = TRUE;
        }
        if (wk->scoring) {
            func_ov104_021b8384(wk, &wk->resultState);
        }
    }
    if (!wk->scoring) {
        if (func_ov101_021b7250(wk->button) == 1) {
            wk->param->result = IRC_RHYTHM_CHECK_RESULT_CANCELLED;
            func_ov104_021b818c(wk);
        }
        if (func_ov101_021b6480(wk->param->sys) != 0) {
            func_ov104_021b817c(wk, func_ov104_021b8528);
        }
    }
}

// pokebw2's IrcRhythmCheck_StepResult
void func_ov104_021b8384(IrcRhythmCheckWork *wk, u16 *state) {
    if (*state < 5) {
        if (func_ov101_021b7250(wk->button) == 1) {
            func_ov101_021b63bc(wk->param->sys);
            wk->param->result = IRC_RHYTHM_CHECK_RESULT_CANCELLED;
            func_ov104_021b818c(wk);
            return;
        }
    }
    switch (*state) {
    case 0:
        if (func_ov101_021b630c(wk->param->sys, 2)) {
            *state = 1;
        }
        break;
    case 1:
        func_ov104_021b7f1c(&wk->messageWindow, &wk->text, BtlMain7_Text_002, 0, 0);
        *state = 2;
        break;
    case 2:
        if (func_ov104_021b8768(&wk->net, &wk->data)) {
            *state = 3;
        }
        break;
    case 3:
        func_ov101_021b63cc(wk->param->sys, 2);
        if (func_ov101_021b6424(wk->param->sys)) {
            *state = 4;
        }
        break;
    case 4:
        if (func_ov101_021b630c(wk->param->sys, 3)) {
            func_ov101_021b72a0(wk->button, FALSE);
            *state = 5;
        }
        break;
    case 5:
        if (wk->timer >= 60) {
            GFL_SndSEPlay(SEQ_SE_SYS_12);
            func_ov104_021b7f1c(&wk->messageWindow, &wk->text, BtlMain7_Text_003, 0, 0);
            *state = 6;
        }
        break;
    case 6:
        if (wk->resultTimer++ >= 30) {
            IrcRhythmData received;
            int diff;

            wk->param->unk10 = func_ov104_021b8880(wk);
            func_ov104_021b87f4(&wk->net, &received);
            diff = wk->data.extraTaps - received.extraTaps;
            if (diff < 0) {
                diff = -diff;
            }
            wk->param->unk14 = diff;
            wk->param->result = IRC_RHYTHM_CHECK_RESULT_DONE;
            *state = 7;
        }
        return;
    case 7:
        func_ov104_021b818c(wk);
        return;
    }
    if (*state >= 1 && *state <= 5 && wk->timer <= 60) {
        wk->timer++;
    }
    if (*state < 3) {
        if (func_ov101_021b6480(wk->param->sys) != 0) {
            func_ov104_021b817c(wk, func_ov104_021b8528);
        }
    }
}




// The link broke: ends the screen
// pokebw2's IrcRhythmCheck_StepInterrupted
void func_ov104_021b8528(IrcRhythmCheckWork *wk, u16 *state) {
    switch (*state) {
    case 0:
        if (func_ov101_021b6480(wk->param->sys) > 0) {
            wk->param->unk10 = 0;
            wk->param->result = IRC_RHYTHM_CHECK_RESULT_CANCELLED;
            func_ov104_021b818c(wk);
            return;
        }
        if (func_ov101_021b6480(wk->param->sys) < 0) {
            *state = 1;
            return;
        }
        break;
    case 1:
        wk->param->unk10 = 0;
        wk->param->result = IRC_RHYTHM_CHECK_RESULT_INTERRUPTED;
        *state = 2;
        break;
    case 2:
        func_ov104_021b7f1c(&wk->messageWindow, &wk->text, BtlMain7_Text_004, 0,
                                 0);
        *state = 3;
        break;
    case 3:
        if (func_ov101_021b630c(wk->param->sys, 6)) {
            *state = 4;
        }
        break;
    case 4:
        func_ov104_021b818c(wk);
        break;
    }
}

// pokebw2's IrcRhythmData_Clear
static void func_ov104_021b85d8(IrcRhythmData *data) {
    MI_CpuFill8(data, 0, sizeof(IrcRhythmData));
}

// Starts the OS_GetTick that the taps are timed from
// pokebw2's IrcRhythmData_Start
void func_ov104_021b85e4(IrcRhythmData *data) {
    // BUG: the OS_GetTick is only started if it already was
#ifdef BUGFIX
    if (!OS_IsTickAvailable()) {
#else
    if (OS_IsTickAvailable()) {
#endif
        OS_InitTick();
    }
    data->start = OS_TicksToMilliSeconds(OS_GetTick());
}

// pokebw2's IrcRhythmData_IsFull
BOOL func_ov104_021b8614(IrcRhythmData *data) {
    if (data->count >= TAP_COUNT) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's IrcRhythmData_AddTap
void func_ov104_021b8624(IrcRhythmData *data) {
    if (data->count < TAP_COUNT) {
        data->taps[data->count].time = OS_TicksToMilliSeconds(OS_GetTick()) - data->start;
        if (data->count == 0) {
            data->taps[data->count].interval = 0;
        } else {
            data->taps[data->count].interval = data->taps[data->count].time - data->taps[data->count - 1].time;
        }
        data->count++;
    } else if (data->extraTaps < EXTRA_TAP_MAX) {
        data->extraTaps++;
    }
}

// Compares the last two intervals, plays a sound for how they differ and returns the tap's grade
// pokebw2's IrcRhythmData_GetTapGrade
int func_ov104_021b86a0(IrcRhythmData *data) {
    u32 sound;
    int grade;
    int diff;

    if (data->count < 2) {
        sound = SEQ_SE_SYS_13;
        grade = TAP_GRADE_ON_BEAT;
    } else {
        diff = data->taps[data->count - 1].interval - data->taps[data->count - 2].interval;
        if (MATH_ABS(diff) >= 0 && MATH_ABS(diff) < 50) {
            sound = SEQ_SE_SYS_13;
            grade = TAP_GRADE_ON_BEAT;
        } else if (diff < 0) {
            if (MATH_ABS(diff) < 200) {
                sound = SEQ_SE_SYS_16;
                grade = TAP_GRADE_EARLY;
            } else {
                sound = SEQ_SE_SYS_17;
                grade = TAP_GRADE_EARLY_FAR;
            }
        } else {
            if (MATH_ABS(diff) < 200) {
                sound = SEQ_SE_SYS_14;
                grade = TAP_GRADE_LATE;
            } else {
                sound = SEQ_SE_SYS_15;
                grade = TAP_GRADE_LATE_FAR;
            }
        }
    }
    GFL_SndSEPlay(sound);
    return grade;
}

// pokebw2's IrcRhythmNet_Init
static void func_ov104_021b8724(IrcRhythmNet *net, CompatibleIrcSys *sys) {
    MI_CpuFill8(net, 0, sizeof(IrcRhythmNet));
    net->sys = sys;
    func_ov101_021b64f0(sys, NET_COMMAND_BASE, data_ov104_021b8f28, 1, net);
}

// pokebw2's IrcRhythmNet_Free
static void func_ov104_021b874c(IrcRhythmNet *net) {
    func_ov101_021b6508(net->sys, NET_COMMAND_BASE);
    MI_CpuFill8(net, 0, sizeof(IrcRhythmNet));
}

// Sends the data, and returns TRUE once the other player's has also arrived
// pokebw2's IrcRhythmNet_Send
BOOL func_ov104_021b8768(IrcRhythmNet *net, const IrcRhythmData *data) {
    switch (net->state) {
    case 0:
        net->send = *data;
        net->state = 1;
        break;
    case 1:
        if (func_ov101_021b64b8(net->sys, NET_COMMAND_BASE, sizeof(IrcRhythmData), &net->send,
                                    !func_0203d1a4(func_020399a8()), 0, FALSE, TRUE)) {
            net->state = 2;
        }
        break;
    case 2:
        if (net->received) {
            net->received = FALSE;
            net->state = 0;
            return TRUE;
        }
        break;
    }
    return FALSE;
}

// pokebw2's IrcRhythmNet_GetReceived
void func_ov104_021b87f4(IrcRhythmNet *net, IrcRhythmData *dest) {
    *dest = net->peer;
}

// pokebw2's IrcRhythmNet_OnReceive
static void func_ov104_021b8810(int netId, int size, const void *data, void *work, NetHandle *handle) {
    IrcRhythmNet *net = work;

    if (handle == func_020399a8()) {
        if (netId != func_02039338()) {
            net->received = TRUE;
        }
    }
}

// pokebw2's IrcRhythmNet_GetBuffer
static void *func_ov104_021b8834(int netId, void *work, int size) {
    IrcRhythmNet *net = work;

    if (netId == func_02039338()) {
        return &net->own;
    }
    return &net->peer;
}

// Reads a touch inside the area of the screen that counts
// pokebw2's IrcRhythm_GetTouch
BOOL func_ov104_021b884c(IrcRhythmPoint *point) {
    u32 x;
    u32 y;

    if (func_02035d20(&x, &y) && x <= TOUCH_MAX_X && y <= TOUCH_MAX_Y) {
        point->x = x;
        point->y = y;
        return TRUE;
    }
    return FALSE;
}

// Scores how close the two players' rhythms were, from how long all the taps took and how each interval differed
// pokebw2's IrcRhythmCheck_ComputeScore
u8 func_ov104_021b8880(IrcRhythmCheckWork *wk) {
    IrcRhythmData own = wk->data;
    IrcRhythmData peer;
    // BUG: nothing sets this if the two durations differ by 30000 ms or more
#ifdef BUGFIX
    u32 base = 0;
#else
    u32 base;
#endif
    u32 sum;
    u32 diff;
    int i;

    func_ov104_021b87f4(&wk->net, &peer);
    diff = (own.taps[own.count - 1].time - own.taps[0].time) - (peer.taps[peer.count - 1].time - peer.taps[0].time);
    if ((int)diff < 0) {
        diff = -diff;
    }
    if (diff == 0) {
        base = 100;
    } else if (diff < 500) {
        base = 90;
    } else if (diff < 1000) {
        base = 80;
    } else if (diff < 3000) {
        base = 70;
    } else if (diff < 5000) {
        base = 60;
    } else if (diff < 10000) {
        base = 50;
    } else if (diff < 15000) {
        base = 40;
    } else if (diff < 20000) {
        base = 30;
    } else if (diff < 25000) {
        base = 20;
    } else if (diff < 30000) {
        base = 10;
    }
    sum = 0;
    for (i = 1; i < TAP_COUNT; i++) {
        diff = own.taps[i].interval - peer.taps[i].interval;
        if ((int)diff < 0) {
            diff = -diff;
        }
        if (diff == 0) {
            sum += 100;
        } else if (diff < 50) {
            sum += 90;
        } else if (diff < 100) {
            sum += 80;
        } else if (diff < 200) {
            sum += 70;
        } else if (diff < 300) {
            sum += 60;
        } else if (diff < 400) {
            sum += 50;
        } else if (diff < 500) {
            sum += 40;
        } else if (diff < 600) {
            sum += 30;
        } else if (diff < 1000) {
            sum += 20;
        } else if (diff < 15000) {
            sum += 10;
        }
    }
    return (base + sum / 9) / 2;
}

// Sets a color of a palette to a blend of two colors, by how far the angle has gone around a circle
// pokebw2's IrcRhythm_BlendColor
static void func_ov104_021b89dc(NNS_GFD_DST_TYPE type, u16 *dest, int angle, u32 palette, u8 index, u16 from, u16 to) {
    u8 fromR = from & GX_RGB_R_MASK;
    u8 fromG = (from & GX_RGB_G_MASK) >> GX_RGB_G_SHIFT;
    u8 fromB = (from & GX_RGB_B_MASK) >> GX_RGB_B_SHIFT;
    u8 toR = to & GX_RGB_R_MASK;
    u8 toG = (to & GX_RGB_G_MASK) >> GX_RGB_G_SHIFT;
    u8 toB = (to & GX_RGB_B_MASK) >> GX_RGB_B_SHIFT;
    s16 weight = (FX_CosIdx(angle) + FX32_ONE) / 2;
    int stepB = (toB - fromB) * weight >> FX32_SHIFT;
    int stepR = (toR - fromR) * weight >> FX32_SHIFT;
    int stepG = (toG - fromG) * weight >> FX32_SHIFT;

    *dest = GX_RGB((u8)(fromR + stepR), (u8)(fromG + stepG), (u8)(fromB + stepB));
    NNS_GfdRegisterNewVramTransferTask(type, palette * 0x20 + index * 2, dest, 2);
}

static void func_ov104_021b8a88(IrcRhythmParticles *particles, IrcRhythmGraphic *graphic, int mode, int firstActor,
                                    u32 surface) {
    int i;

    MI_CpuFill8(particles, 0, sizeof(IrcRhythmParticles));
    particles->mode = mode;
    particles->timer = 0;
    particles->interval = 10;
    for (i = 0; i < PARTICLE_COUNT; i++) {
        func_ov104_021b8d08(&particles->particles[i], func_ov104_021b7968(graphic, firstActor + i), surface);
    }
}

// pokebw2's IrcRhythmParticles_Free
static void func_ov104_021b8adc(IrcRhythmParticles *particles) {
    MI_CpuFill8(particles, 0, sizeof(IrcRhythmParticles));
}

// pokebw2's IrcRhythmParticles_Update
static void func_ov104_021b8aec(IrcRhythmParticles *particles) {
    IrcRhythmPoint b;
    IrcRhythmPoint a;
    u32 duration;
    u8 side;
    int palette;
    u16 angle;
    int i;
    int x;

    switch (particles->mode) {
    case PARTICLES_RISE:
        if (particles->timer++ > particles->interval) {
            particles->timer = 0;
            for (i = 0; i < PARTICLE_COUNT; i++) {
                if (!func_ov104_021b8e8c(&particles->particles[i])) {
                    side = GFL_RandomLC(2);
                    x = GFL_RandomLC(256);
                    IrcRhythmPoint_Set(&b, x, -36);
                    IrcRhythmPoint_Set(&a, x, 228);
                    duration = GFL_RandomLC(50) + 110;
                    palette = 2;
                    if (GFL_RandomLC(2)) {
                        palette = 5;
                    }
                    if (side) {
                        func_ov104_021b8da4(&particles->particles[i], &a, &b, palette, duration);
                    } else {
                        func_ov104_021b8da4(&particles->particles[i], &b, &a, palette, duration);
                    }
                    break;
                }
            }
        }
        break;
    case PARTICLES_TAPS_ONLY:
        break;
    case PARTICLES_CONVERGE:
        if (particles->timer++ > particles->interval) {
            particles->timer = 0;
            for (i = 0; i < PARTICLE_COUNT; i++) {
                if (!func_ov104_021b8e8c(&particles->particles[i])) {
                    angle = GFL_RandomLC(0xffff);
                    IrcRhythmPoint_Set(&b, FX_Whole(FX_SinIdx(angle) << 8), FX_Whole(FX_CosIdx(angle) << 8));
                    IrcRhythmPoint_Set(&a, 128, 96);
                    duration = GFL_RandomLC(50) + 110;
                    palette = 2;
                    if (GFL_RandomLC(2)) {
                        palette = 5;
                    }
                    func_ov104_021b8da4(&particles->particles[i], &b, &a, palette, duration);
                    break;
                }
            }
        }
        break;
    }
    for (i = 0; i < PARTICLE_COUNT; i++) {
        func_ov104_021b8d20(&particles->particles[i]);
    }
}

// Sends sparkles out from a tap, in the color of its grade
// pokebw2's IrcRhythmParticles_Burst
void func_ov104_021b8c3c(IrcRhythmParticles *particles, const IrcRhythmPoint *origin, int palette) {
    VecFx32 center;
    VecFx32 ring;
    VecFx32 dir;
    IrcRhythmPoint target;
    u16 angle;
    int burst;
    int i;

    center.x = origin->x << FX32_SHIFT;
    center.y = origin->y << FX32_SHIFT;
    center.z = 0;
    for (burst = 0; burst < 12; burst++) {
        for (i = 0; i < PARTICLE_COUNT; i++) {
            if (!func_ov104_021b8e8c(&particles->particles[i])) {
                angle = GFL_RandomLC(0xffff);
                target.x = FX_Whole(FX_SinIdx(angle) << 8);
                target.y = FX_Whole(FX_CosIdx(angle) << 8);
                ring.x = target.x << FX32_SHIFT;
                ring.y = target.y << FX32_SHIFT;
                ring.z = 0;
                VEC_Subtract(&ring, &center, &dir);
                VEC_Normalize(&dir, &dir);
                target.x = FX_Whole(dir.x << 8);
                target.y = FX_Whole(dir.y << 8);
                target.x += origin->x;
                target.y += origin->y;
                func_ov104_021b8da4(&particles->particles[i], origin, &target, palette, GFL_RandomLC(5) + 40);
                break;
            }
        }
    }
}

// pokebw2's IrcRhythmParticle_Init
static void func_ov104_021b8d08(IrcRhythmParticle *particle, ClActor *actor, u32 surface) {
    MI_CpuFill8(particle, 0, sizeof(IrcRhythmParticle));
    particle->actor = actor;
    particle->surface = surface;
}

// pokebw2's IrcRhythmParticle_Update
static void func_ov104_021b8d20(IrcRhythmParticle *particle) {
    VecFx32 dir;
    VecFx32 current;
    ClActorPos pos;
    fx32 length;

    if (particle->active) {
        VEC_Subtract(&particle->end, &particle->start, &dir);
        length = VEC_Mag(&dir);
        VEC_Normalize(&dir, &dir);
        VEC_MultAdd(length * particle->frame / particle->duration, &dir, &particle->start, &current);
        pos.x = current.x >> FX32_SHIFT;
        pos.y = current.y >> FX32_SHIFT;
        if (particle->frame++ > particle->duration) {
            // BUG: the end is in fixed point, and the position is not
#ifdef BUGFIX
            pos.x = particle->end.x >> FX32_SHIFT;
            pos.y = particle->end.y >> FX32_SHIFT;
#else
            pos.x = particle->end.x;
            pos.y = particle->end.y;
#endif
            func_0204b3dc(particle->actor, FALSE);
            particle->active = FALSE;
        }
        func_0204b404(particle->actor, &pos, particle->surface);
    }
}

static void func_ov104_021b8da4(IrcRhythmParticle *particle, const IrcRhythmPoint *from, const IrcRhythmPoint *to,
                                    int palette, u32 duration) {
    particle->active = TRUE;
    VEC_Set(&particle->start, FX32_CONST(from->x), FX32_CONST(from->y), 0);
    VEC_Set(&particle->end, FX32_CONST(to->x), FX32_CONST(to->y), 0);
    particle->frame = 0;
    particle->duration = duration;
    func_0204b3dc(particle->actor, TRUE);
    func_0204b6f4(particle->actor, (u8)palette, TRUE);
}

// pokebw2's IrcRhythmParticle_IsActive
static BOOL func_ov104_021b8e8c(IrcRhythmParticle *particle) {
    return particle->active;
}

// pokebw2's IrcRhythmFlash_Init
static void func_ov104_021b8e90(IrcRhythmFlash *flash, HeapID heapId) {
    MI_CpuFill8(flash, 0, sizeof(IrcRhythmFlash));
    flash->enabled = TRUE;
}

// pokebw2's IrcRhythmFlash_Free
static void func_ov104_021b8ea4(IrcRhythmFlash *flash) {
    MI_CpuFill8(flash, 0, sizeof(IrcRhythmFlash));
}

// pokebw2's IrcRhythmFlash_Update
static void func_ov104_021b8eb0(IrcRhythmFlash *flash) {
    if (flash->active && flash->enabled) {
        u16 angle = flash->angle;

        if (angle + 0x400 >= 0x10000) {
            flash->angle = angle - 0xfc00;
        } else {
            flash->angle = angle + 0x400;
        }
        func_ov104_021b89dc(NNS_GFD_DST_2D_BG_PLTT_MAIN, &flash->color, flash->angle, 5, 3, 0x6adf, 0x55fe);
    }
}

// pokebw2's IrcRhythmFlash_Start
void func_ov104_021b8f00(IrcRhythmFlash *flash) {
    flash->active = TRUE;
}

// Puts the blend back to none once the countdown is done
// pokebw2's IrcRhythmCheck_VBlank
static void func_ov104_021b8f08(TCB *tcb, void *data) {
    IrcRhythmCheckWork *wk = data;

    if (wk->clearBlend) {
        wk->clearBlend = FALSE;
        G2_BlendNone();
    }
}
