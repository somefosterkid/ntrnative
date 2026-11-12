// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov177/irc_sync_check.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/compatible_irc_sys.h"
#include "app/irc_appbar.h"
#include "app/irc_compatible.h"
#include "app/irc_sync_check.h"
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
#include "gfl/g3d.h"
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
#include "nitro/fx/fx_vec.h"
#include "nitro/fx/fx_mtx.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "nnsys/g2d.h"
#include "save/player_info.h"
#include "save/save_control.h"
#include "save/trainer_log.h"
#include "system/bmp_winframe.h"
#include "system/game_data.h"
#include "system/game_system.h"
#include "system/gf_font.h"
#include "system/printsys.h"
#include "system/wordset.h"
#include "text/system/btl_main_3.h"

// pokebw2's prototypes of what Black's headers don't declare, under pokebw2's names until tools/port_file.py
// gives them Black's from the relocations
int func_ov101_021b7250(IrcAppBar *bar);
BOOL func_ov101_021b727c(IrcAppBar *bar);

// The Sync Check of the compatibility checker: its graphics, its windows, the two 3D thumbs that follow the stylus, the
// traces of the thumbs that the two players compare, the link that sends them and the steps of the screen. The ROM has
// no string for the file, so the name is a guess. Names are ours

#define HEAP_ID 0x37
#define BG_COUNT 7
#define CLACT_COUNT 32
#define SPARKLE_COUNT 32
#define THUMB_COUNT 2
#define TRACE_POINT_COUNT 10

// The command that sends a player's traces to the other
#define NET_COMMAND_BASE 0xe00

// The frames that a thumb is traced for, and how often a point of the trace is taken
#define TRACE_FRAMES 150
#define TRACE_INTERVAL 16
#define CONNECTED_FRAMES 60
#define HINT_FRAMES 180
#define DONE_FRAMES 30

// Which of the two outlines func_ov103_021b9060 shows
#define OUTLINE_FIRST 1
#define OUTLINE_SECOND 2

// The steps of func_ov103_021b8828
enum {
    RESULT_EXCHANGE_IDS_FIRST,
    RESULT_PRINT_CHECKING,
    RESULT_SEND_TRACES,
    RESULT_SEND_COUNT,
    RESULT_EXCHANGE_IDS_LAST,
    RESULT_WAIT_CONNECTED,
    RESULT_WAIT_SCORE,
    RESULT_FINISH,
};

typedef struct {
    int x;
    int y;
} IrcSyncPoint;

// The points that one thumb passes through
typedef struct {
    IrcSyncPoint points[TRACE_POINT_COUNT];
    u16 index;
    u32 frames;
    u32 phase;
    BOOL done;
} IrcSyncTrace;

// What a player sends the other
typedef struct {
    IrcSyncPoint first;
    IrcSyncPoint second;
    IrcSyncTrace traceA;
    IrcSyncTrace traceB;
    int retries;
    BOOL isNew;
} IrcSyncData;

typedef struct {
    int left;
    int top;
    int right;
    int bottom;
} IrcSyncRect;

typedef struct {
    CompatibleIrcSys *sys;
    int state;
    IrcSyncData peer;
    IrcSyncData own;
    IrcSyncData send;
    BOOL received;
} IrcSyncNet;

typedef struct {
    ClActUnit *unit;
    u32 palette;
    u32 chars;
    u32 cellAnims;
    ClActor *actors[CLACT_COUNT + 1];
} IrcSyncCheckClAct;

typedef struct {
    // The characters of the window frame
    u32 frameChars;
} IrcSyncCheckBG;

// A 3D model with two animations
typedef struct {
    void *resource;
    void *animationResources[2];
    G3DModel *model;
    G3DActor *actor;
    void *animations[2];
    SRTMatrix transform;
    int animation;
    BOOL animationDone;
} IrcSyncCheckModel;

typedef struct {
    G3DCamera *camera;
    IrcSyncCheckModel models[THUMB_COUNT];
} IrcSyncCheckG3d;

// The thumbs, which stay where the stylus was last put
typedef struct {
    IrcSyncCheckModel *models[THUMB_COUNT];
    int lastX;
    int lastY;
} IrcSyncCheckThumbs;

// Black 2's; twinned code confirms the offsets of bg, g3d, clact, thumbs and vblankTask in Black
typedef struct {
    IrcSyncCheckBG bg;
    IrcSyncCheckG3d g3d;
    u32 unkC8;
    IrcSyncCheckClAct clact;
    IrcSyncCheckThumbs thumbs;
    TCB *vblankTask;
} IrcSyncCheckGraphic;

// Black 2's, not checked against Black
typedef struct {
    Font *font;
    MsgData *msgData;
    PrintQueue *queue;
    WordSet *wordSet;
} IrcSyncCheckText;

typedef struct {
    BmpWin *window;
    PrintWindow printWindow;
    StrBuf *strbuf;
    u16 color;
} IrcSyncCheckWindow;

// Black 2's; twinned code confirms the offsets of actor and value in Black
typedef struct {
    ClActor *actor;
    u32 value;
    u8 unk8[0x24];
} IrcSyncCheckSparkle;

typedef struct {
    IrcSyncCheckSparkle sparkles[SPARKLE_COUNT];
    int unk580;
    int palette;
    int timer;
    int interval;
} IrcSyncCheckSparkles;

typedef struct IrcSyncCheckWork IrcSyncCheckWork;
typedef void (*IrcSyncCheckStepFunc)(IrcSyncCheckWork *wk, u16 *state);

// Black 2's; twinned code confirms the offsets of graphic, text, messageWindow, sparkles, step, stepState, timer,
// stepDone, retries, counter, hintTimer, hintToggle, titleWindow, net, button, first, second, traceA, traceB and param,
// and its size in Black
struct IrcSyncCheckWork {
    IrcSyncCheckGraphic graphic;
    IrcSyncCheckText text;
    // The message
    IrcSyncCheckWindow messageWindow;
    IrcSyncCheckSparkles sparkles;
    IrcSyncCheckStepFunc step;
    u16 stepState;
    u16 timer;
    BOOL stepDone;
    int unk734;
    // How many times a thumb was lifted too early
    int retries;
    u32 counter;
    u32 hintTimer;
    BOOL hintToggle;
    // The title
    IrcSyncCheckWindow titleWindow;
    IrcSyncNet net;
    u32 unk9F0[18];
    IrcAppBar *button;
    IrcSyncPoint first;
    IrcSyncPoint second;
    IrcSyncTrace traceA;
    IrcSyncTrace traceB;
    IrcCompatibleParam1 *param;
};

static BOOL func_ov103_021b7560(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov103_021b7748(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov103_021b77b0(GameProc *proc, u32 *state, void *param, void *work);
static void func_ov103_021b787c(IrcSyncCheckGraphic *graphic, HeapID heapId);
static void func_ov103_021b78ec(IrcSyncCheckGraphic *graphic);
static void func_ov103_021b7918(IrcSyncCheckGraphic *graphic);
static ClActor *func_ov103_021b793c(IrcSyncCheckGraphic *graphic, int index);
static ClActUnit *func_ov103_021b7948(IrcSyncCheckGraphic *graphic);
static IrcSyncCheckThumbs *func_ov103_021b7954(IrcSyncCheckGraphic *graphic);
static void func_ov103_021b795c(TCB *tcb, void *data);
static void func_ov103_021b7970(IrcSyncCheckBG *bg, HeapID heapId);
static void func_ov103_021b7ae8(IrcSyncCheckBG *bg);
static void func_ov103_021b7b34(IrcSyncCheckBG *bg);
static void func_ov103_021b7b3c(IrcSyncCheckClAct *clact, const BGSysVRAMConfig *vramConfig, HeapID heapId);
static void func_ov103_021b7c08(IrcSyncCheckClAct *clact);
static void func_ov103_021b7c4c(IrcSyncCheckClAct *clact);
static void func_ov103_021b7c54(IrcSyncCheckClAct *clact);
static ClActor *func_ov103_021b7c5c(IrcSyncCheckClAct *clact, int index);
static ClActUnit *func_ov103_021b7c64(IrcSyncCheckClAct *clact);
static void func_ov103_021b7c68(IrcSyncCheckG3d *g3d, HeapID heapId);
static void func_ov103_021b7d08(IrcSyncCheckG3d *g3d);
static void func_ov103_021b7d30(IrcSyncCheckG3d *g3d);
static void func_ov103_021b7d44(IrcSyncCheckG3d *g3d);
static IrcSyncCheckModel *func_ov103_021b7d4c(IrcSyncCheckG3d *g3d, u16 index);
static void func_ov103_021b7d58(void);
static void func_ov103_021b7dfc(IrcSyncCheckModel *model, ArcTool *arc, const u16 *fileIds, u32 polygonId,
                                   HeapID heapId);
static void func_ov103_021b7eb8(IrcSyncCheckModel *model);
static void func_ov103_021b7f04(IrcSyncCheckModel *model);
static BOOL func_ov103_021b7f4c(IrcSyncCheckModel *model);
static void func_ov103_021b7f50(IrcSyncCheckModel *model, const VecFx32 *position);
static void func_ov103_021b7f60(IrcSyncCheckModel *model, int animation);
static int func_ov103_021b7f78(IrcSyncCheckModel *model);
static void func_ov103_021b7f7c(IrcSyncCheckText *text, BOOL keepFont, HeapID heapId);
static void func_ov103_021b7fc8(IrcSyncCheckText *text);
static BOOL func_ov103_021b7ff0(IrcSyncCheckText *text);
static Font *func_ov103_021b7ffc(IrcSyncCheckText *text);
static PrintQueue *func_ov103_021b8000(IrcSyncCheckText *text);
static MsgData *func_ov103_021b8004(IrcSyncCheckText *text);
static WordSet *func_ov103_021b8008(IrcSyncCheckText *text);
static void func_ov103_021b800c(IrcSyncCheckWindow *win, u8 bg, u8 x, u8 y, u8 width, u8 height, u8 palette,
                                    HeapID heapId);
static void func_ov103_021b8074(IrcSyncCheckWindow *win, u8 bg, u8 x, u8 y, u8 width, u8 height,
                                             u8 palette, u16 color, u8 fromEnd, HeapID heapId);
static void func_ov103_021b80e4(IrcSyncCheckWindow *win);
static BOOL func_ov103_021b8100(IrcSyncCheckWindow *win, IrcSyncCheckText *text);
static void func_ov103_021b8138(IrcSyncCheckWindow *win, IrcSyncCheckText *text, u32 msgId, int x, s16 y);
static void func_ov103_021b81a4(IrcSyncCheckWindow *win, IrcSyncCheckText *text, u32 msgId);
static void func_ov103_021b824c(IrcSyncCheckWindow *win, IrcSyncCheckText *text, u32 msgId,
                                              CompatibleIrcProfile *other, s16 x, s16 y, HeapID heapId);
static void func_ov103_021b831c(IrcSyncCheckWindow *win);
static void func_ov103_021b82dc(IrcSyncCheckWindow *win, IrcSyncCheckWork *wk, u32 msgId);
static void func_ov103_021b8338(IrcSyncCheckWork *wk, IrcSyncCheckStepFunc step);
static void func_ov103_021b8348(IrcSyncCheckWork *wk, IrcSyncCheckStepFunc step, u16 state);
static void func_ov103_021b8358(IrcSyncCheckWork *wk);
static void func_ov103_021b8364(IrcSyncCheckWork *wk, u16 *state);
static void func_ov103_021b856c(IrcSyncCheckWork *wk, u16 *state);
static void func_ov103_021b8728(IrcSyncCheckWork *wk, u16 *state);
static void func_ov103_021b8828(IrcSyncCheckWork *wk, u16 *state);
static void func_ov103_021b8a44(IrcSyncCheckWork *wk, u16 *state);
static void func_ov103_021b8af4(IrcSyncNet *net, CompatibleIrcSys *sys);
static void func_ov103_021b8b1c(IrcSyncNet *net);
static BOOL func_ov103_021b8b38(IrcSyncNet *net, const IrcSyncData *data);
static void func_ov103_021b8bc0(IrcSyncNet *net, IrcSyncData *dest);
static void func_ov103_021b8bd8(int netId, int size, const void *data, void *work, NetHandle *handle);
static void *func_ov103_021b8bfc(int netId, void *work, int size);
static void func_ov103_021b8c14(IrcSyncCheckThumbs *thumbs, IrcSyncCheckG3d *g3d, HeapID heapId);
static void func_ov103_021b8c40(IrcSyncCheckThumbs *thumbs);
static void func_ov103_021b8c4c(IrcSyncCheckThumbs *thumbs);
static void func_ov103_021b8ca0(IrcSyncCheckThumbs *thumbs, int index, int animation);
static void func_ov103_021b8cb0(IrcSyncCheckThumbs *thumbs, int index, int x, int y);
static BOOL func_ov103_021b8d54(const IrcSyncRect *rect, IrcSyncPoint *point);
static BOOL func_ov103_021b8d9c(const IrcSyncRect *rect, IrcSyncPoint *point);
static u8 func_ov103_021b8de4(IrcSyncCheckWork *wk);
static u8 func_ov103_021b8f00(const IrcSyncPoint *ownStart, const IrcSyncPoint *own,
                                     const IrcSyncPoint *peerStart, const IrcSyncPoint *peer);
static void func_ov103_021b8fa0(ArcTool *arc, u32 fileId, u32 bg, u32 tileOffset, u8 srcX, u8 srcY,
                                        u8 srcWidth, u8 srcHeight, u8 x, u8 y, u8 width, u8 height, u8 palette,
                                        BOOL compressed, HeapID heapId);
static void func_ov103_021b9060(u32 outlines, HeapID heapId);
static void func_ov103_021b9110(IrcSyncCheckSparkles *sparkles, IrcSyncCheckGraphic *graphic, int unk580,
                                      u32 palette, int firstActor, u32 value);
static void func_ov103_021b9174(IrcSyncCheckSparkles *sparkles);
static void func_ov103_021b9184(IrcSyncCheckSparkle *sparkle, ClActor *actor, u32 palette, u32 value);
static void func_ov103_021b91a8(IrcSyncTrace *trace);
static BOOL func_ov103_021b91b4(IrcSyncTrace *trace, const IrcSyncRect *rect);
static BOOL func_ov103_021b9214(const IrcSyncTrace *trace);

// pokebw2's data_ov103_021b9304
const GameProcFunctions data_ov103_021b9304 = {
    func_ov103_021b7560,
    func_ov103_021b77b0,
    func_ov103_021b7748,
};

// pokebw2's sIrcSyncCheckModelFileIds
static const u16 data_ov103_021b9218[3] = { 52, 50, 51 };

// pokebw2's sIrcSyncCheckCommands
static const NetCommand data_ov103_021b9220[1] = {
    { func_ov103_021b8bd8, func_ov103_021b8bfc },
};

// pokebw2's sIrcSyncCheckCameraTarget
static const VecFx32 data_ov103_021b9240 = { 0, 0, 0 };
// pokebw2's sIrcSyncCheckCameraUpVector
static const VecFx32 data_ov103_021b9234 = { 0, FX32_ONE, 0 };

// Where the player puts a thumb: the first outline and the second
// pokebw2's sIrcSyncCheckRectFirst
static const IrcSyncRect data_ov103_021b925c = { 0, 16, 88, 160 };
// pokebw2's sIrcSyncCheckRectSecond
static const IrcSyncRect data_ov103_021b926c = { 89, 16, 176, 160 };

// pokebw2's sIrcSyncCheckLCDConfig
static const BGSysLCDConfig data_ov103_021b924c = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_1, GX_BG0_AS_3D };

// pokebw2's sIrcSyncCheckCameraPosition
static const VecFx32 data_ov103_021b9228 = { 0, 0, FX32_CONST(70) };

// pokebw2's sIrcSyncCheckBGs
static const u32 data_ov103_021b927c[BG_COUNT] = { 1, 2, 3, 4, 5, 6, 7 };

// pokebw2's sIrcSyncCheckBGModes
static const u32 data_ov103_021b9298[BG_COUNT] = { BGMODE_TEXT, BGMODE_TEXT, BGMODE_TEXT, BGMODE_TEXT,
                                                    BGMODE_TEXT, BGMODE_TEXT, BGMODE_AFFINE };

// pokebw2's sIrcSyncCheckLights
static const Light data_ov103_021b92b4[4] = {
    { { 0, -4096, 0 }, GX_RGB(16, 16, 16) },
    { { 0, 4096, 0 }, GX_RGB(16, 16, 16) },
    { { 0, -4096, 0 }, GX_RGB(16, 16, 16) },
    { { 0, -4096, 0 }, GX_RGB(16, 16, 16) },
};

// pokebw2's sIrcSyncCheckVRAMConfig
static const BGSysVRAMConfig data_ov103_021b92d4 = {
    GX_VRAM_BG_128_A,  GX_VRAM_BGEXTPLTT_NONE,  GX_VRAM_SUB_BG_128_C,        GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_64_E,  GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_128_D,       GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_NONE,  GX_VRAM_TEXPLTT_NONE,    GX_OBJVRAMMODE_CHAR_1D_128K, GX_OBJVRAMMODE_CHAR_1D_128K,
};

// pokebw2's sIrcSyncCheckBGSetups
static const BGSetup data_ov103_021b9310[BG_COUNT] = {
    { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x04000, 0x8000,
      GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE },
    { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x1000, GX_BG_CHARBASE_0x08000, 0x8000,
      GX_BG_EXTPLTT_01, 1, GX_BG_AREAOVER_XLU, FALSE },
    { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x1800, GX_BG_CHARBASE_0x10000, 0x8000,
      GX_BG_EXTPLTT_01, 2, GX_BG_AREAOVER_XLU, FALSE },
    { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x04000, 0x8000,
      GX_BG_EXTPLTT_01, 1, GX_BG_AREAOVER_XLU, FALSE },
    { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x0800, GX_BG_CHARBASE_0x08000, 0x8000,
      GX_BG_EXTPLTT_01, 2, GX_BG_AREAOVER_XLU, FALSE },
    { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x1000, GX_BG_CHARBASE_0x10000, 0x8000,
      GX_BG_EXTPLTT_01, 3, GX_BG_AREAOVER_XLU, FALSE },
    { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_256, GX_BG_SCRBASE_0x1800, GX_BG_CHARBASE_0x14000, 0x4000,
      GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE },
};

// pokebw2's IrcSyncCheck_Init
static BOOL func_ov103_021b7560(GameProc *proc, u32 *state, void *param, void *work) {
    IrcSyncCheckWork *wk;
    IrcCompatibleParam1 *syncParam = param;
    ClActUnit *unit;
    Font *font;
    PrintQueue *queue;

    GFL_HeapCreateChild(HEAPID_USER, HEAP_ID, 0x25000);
    wk = GFL_ProcInitSubsystem(proc, sizeof(IrcSyncCheckWork), HEAP_ID);
    MI_CpuFill8(wk, 0, sizeof(IrcSyncCheckWork));
    wk->param = syncParam;
    wk->param->unk11 = 0;
    func_ov103_021b8af4(&wk->net, wk->param->sys);
    func_ov103_021b787c(&wk->graphic, HEAP_ID);
    func_ov103_021b7f7c(&wk->text, FALSE, HEAP_ID);
    if (wk->param->gsys != NULL) {
        GSYS_GetGameCommSystem(wk->param->gsys);
    }
    func_ov103_021b800c(&wk->messageWindow, 5, 1, 18, 30, 5, 8, HEAP_ID);
    BmpWin_DrawFrame(wk->messageWindow.window, 0, (u16)wk->graphic.bg.frameChars, 6);
    func_ov103_021b8074(&wk->titleWindow, 7, 6, 3, 20, 5, 8, 1, TRUE, HEAP_ID);
    func_ov103_021b81a4(&wk->titleWindow, &wk->text, BtlMain3_Text_016);
    GFL_BGSysScaleBGReq(7, 3, 0x1000);
    GFL_BGSysScaleBGReq(7, 6, 0x1000);
    GFL_BGSysAdjustBGOriginReq(7, 9, 0x80);
    GFL_BGSysAdjustBGOriginReq(7, 12, 0x28);
    func_ov103_021b91a8(&wk->traceA);
    func_ov103_021b91a8(&wk->traceB);
    func_ov103_021b9110(&wk->sparkles, &wk->graphic, 0, 1, 0, 1);
    unit = func_ov103_021b7948(&wk->graphic);
    font = func_ov103_021b7ffc(&wk->text);
    queue = func_ov103_021b8000(&wk->text);
    wk->button = func_ov101_021b70dc(2, unit, 1, 13, 13, 2, font, queue, HEAP_ID);
    func_ov101_021b63cc(wk->param->sys, 2);
    func_ov103_021b8338(wk, func_ov103_021b8364);
    GX_SetVisibleWnd(GX_WNDMASK_W0 | GX_WNDMASK_W1);
    G2_SetWnd0InsidePlane(GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3 | GX_WND_PLANEMASK_OBJ,
                          FALSE);
    G2_SetWnd1InsidePlane(GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3 | GX_WND_PLANEMASK_OBJ,
                          FALSE);
    G2_SetWndOutsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3 |
                              GX_WND_PLANEMASK_OBJ,
                          FALSE);
    G2_SetWnd0Position(255, 168, 44, 192);
    G2_SetWnd1Position(0, 168, 255, 192);
    return TRUE;
}

// pokebw2's IrcSyncCheck_Exit
static BOOL func_ov103_021b7748(GameProc *proc, u32 *state, void *param, void *work) {
    IrcSyncCheckWork *wk = work;

    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    func_ov101_021b71c0(wk->button);
    func_ov103_021b9174(&wk->sparkles);
    func_ov103_021b80e4(&wk->titleWindow);
    func_ov103_021b80e4(&wk->messageWindow);
    func_ov103_021b78ec(&wk->graphic);
    func_ov103_021b7fc8(&wk->text);
    func_ov103_021b8b1c(&wk->net);
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(HEAP_ID);
    return TRUE;
}

// pokebw2's IrcSyncCheck_Main
static BOOL func_ov103_021b77b0(GameProc *proc, u32 *state, void *param, void *work) {
    IrcSyncCheckWork *wk = work;

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
    if (func_ov103_021b7ff0(&wk->text)) {
        func_ov103_021b8100(&wk->messageWindow, &wk->text);
        func_ov103_021b8100(&wk->titleWindow, &wk->text);
    }
    func_ov103_021b7918(&wk->graphic);
    func_ov101_021b63d0(wk->param->sys);
    func_ov101_021b71e4(wk->button);
    return FALSE;
}

// pokebw2's IrcSyncCheckGraphic_Create
static void func_ov103_021b787c(IrcSyncCheckGraphic *graphic, HeapID heapId) {
    MI_CpuFill8(graphic, 0, sizeof(IrcSyncCheckGraphic));
    GFL_BGSysInitVRAM(GX_VRAM_LCDC_NONE);
    GFL_BGSysSetVRAMBanks(&data_ov103_021b92d4);
    GFL_BGSysSetDisplayLayout(0);
    GFL_BGSysEnableEngines();
    GFL_BGSysDisableAllA();
    func_ov103_021b7b3c(&graphic->clact, &data_ov103_021b92d4, heapId);
    func_ov103_021b7970(&graphic->bg, heapId);
    func_ov103_021b7c68(&graphic->g3d, heapId);
    func_ov103_021b8c14(&graphic->thumbs, &graphic->g3d, heapId);
    graphic->vblankTask = GFL_VBlankTCBAdd(func_ov103_021b795c, graphic, 0);
}

// pokebw2's IrcSyncCheckGraphic_Free
static void func_ov103_021b78ec(IrcSyncCheckGraphic *graphic) {
    GFL_TCBRemove(graphic->vblankTask);
    func_ov103_021b8c40(&graphic->thumbs);
    func_ov103_021b7d08(&graphic->g3d);
    func_ov103_021b7ae8(&graphic->bg);
    func_ov103_021b7c08(&graphic->clact);
}

// pokebw2's IrcSyncCheckGraphic_Update
static void func_ov103_021b7918(IrcSyncCheckGraphic *graphic) {
    func_ov103_021b7c4c(&graphic->clact);
    func_ov103_021b7d30(&graphic->g3d);
    func_ov103_021b8c4c(&graphic->thumbs);
    func_ov103_021b7d44(&graphic->g3d);
}

// pokebw2's IrcSyncCheckGraphic_GetActor
static ClActor *func_ov103_021b793c(IrcSyncCheckGraphic *graphic, int index) {
    return func_ov103_021b7c5c(&graphic->clact, index);
}

// pokebw2's IrcSyncCheckGraphic_GetUnit
static ClActUnit *func_ov103_021b7948(IrcSyncCheckGraphic *graphic) {
    return func_ov103_021b7c64(&graphic->clact);
}

// pokebw2's IrcSyncCheckGraphic_GetThumbs
static IrcSyncCheckThumbs *func_ov103_021b7954(IrcSyncCheckGraphic *graphic) {
    return &graphic->thumbs;
}

// pokebw2's IrcSyncCheckGraphic_VBlank
static void func_ov103_021b795c(TCB *tcb, void *data) {
    IrcSyncCheckGraphic *graphic = data;

    func_ov103_021b7b34(&graphic->bg);
    func_ov103_021b7c54(&graphic->clact);
}

// pokebw2's IrcSyncCheckBG_Init
static void func_ov103_021b7970(IrcSyncCheckBG *bg, HeapID heapId) {
    ArcTool *arc;
    int i;

    GFL_BGSysCreate(heapId);
    BmpWin_InitAllocator(heapId);
    GFL_BGSysSetLCDConfig(&data_ov103_021b924c);
    for (i = 0; i < BG_COUNT; i++) {
        GFL_BGSysCreateBG(data_ov103_021b927c[i], &data_ov103_021b9310[i], data_ov103_021b9298[i]);
        GFL_BGSysClearBG(data_ov103_021b927c[i]);
        GFL_BGSysSetBGEnabled(data_ov103_021b927c[i], TRUE);
    }
    arc = GFL_ArcSysCreateFileHandle(ARCID_IRC_COMPAT, heapId);
    GFL_G2DIOLoadArcNCLRDefault(arc, 1, PALTYPE_MAIN_BG, 0, 0, heapId);
    GFL_G2DIOLoadArcNCLRDefault(arc, 1, PALTYPE_SUB_BG, 0, 0, heapId);
    GFL_G2DIOLoadArcNCLRDefault(arc, 6, PALTYPE_MAIN_BG, 0x120, 0, heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 2, 3, 0, 0, FALSE, heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 0, 6, 0, 0, FALSE, heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 48, 4, 0, 0, FALSE, heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 5, 2, 0, 0, FALSE, heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 3, 3, 0, 0, FALSE, heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 4, 6, 0, 0, FALSE, heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 45, 4, 0, 0, FALSE, heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 7, 2, 0, 0, FALSE, heapId);
    func_ov103_021b9060(OUTLINE_FIRST, heapId);
    GFL_BGSysFillChar(5, 0, 1, 0);
    bg->frameChars = GFL_BGSysLoadArcNCGRDynamic(arc, 49, 5, 0, FALSE, heapId);
    GFL_ArcToolFree(arc);
    G2_SetBlendAlpha(5, 12, 13, 8);
}

// pokebw2's IrcSyncCheckBG_Free
static void func_ov103_021b7ae8(IrcSyncCheckBG *bg) {
    int i;

    G2_BlendNone();
    GFL_BGSysFreeCharMemory(5, (u16)bg->frameChars, (u16)(bg->frameChars >> 16));
    GFL_BGSysFreeFilledChar(5, 1, 0);
    for (i = 0; i < BG_COUNT; i++) {
        GFL_BGSysReleaseBG(data_ov103_021b927c[i]);
    }
    BmpWin_FreeAllocator();
    GFL_BGSysFree();
}

// pokebw2's IrcSyncCheckBG_VBlank
static void func_ov103_021b7b34(IrcSyncCheckBG *bg) {
    GFL_BGSysUpdate();
}

// pokebw2's IrcSyncCheckClAct_Init
static void func_ov103_021b7b3c(IrcSyncCheckClAct *clact, const BGSysVRAMConfig *vramConfig, HeapID heapId) {
    ArcTool *arc;
    ClActorSetup setup;
    int i;

    MI_CpuFill8(clact, 0, sizeof(IrcSyncCheckClAct));
    ClActSys_Create(&data_020a1448, vramConfig, heapId);
    clact->unit = func_0204b100(128, 0, heapId);
    func_0204b270(clact->unit);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_OBJ, TRUE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
    arc = GFL_ArcSysCreateFileHandle(ARCID_IRC_COMPAT, heapId);
    clact->palette = func_0204abf0(arc, 12, CLACT_VRAM_SUB, 0, heapId);
    clact->chars = func_0204a6c8(arc, 11, FALSE, CLACT_VRAM_SUB, heapId);
    clact->cellAnims = func_0204af28(arc, 10, 9, heapId);
    GFL_ArcToolFree(arc);
    MI_CpuFill8(&setup, 0, sizeof(ClActorSetup));
    for (i = 0; i < CLACT_COUNT; i++) {
        clact->actors[i] = func_0204b294(clact->unit, clact->chars, clact->palette, clact->cellAnims, &setup,
                                         CLACT_SURFACE_SUB, heapId);
        func_0204b3dc(clact->actors[i], FALSE);
        func_0204b84c(clact->actors[i], 3);
        func_0204b92c(clact->actors[i], TRUE);
        func_0204b878(clact->actors[i], 0);
    }
}

// pokebw2's IrcSyncCheckClAct_Free
static void func_ov103_021b7c08(IrcSyncCheckClAct *clact) {
    int i;

    for (i = 0; i < CLACT_COUNT + 1; i++) {
        if (clact->actors[i] != NULL) {
            func_0204b3b4(clact->actors[i]);
        }
    }
    func_0204ada4(clact->palette);
    func_0204a8d4(clact->chars);
    func_0204afd8(clact->cellAnims);
    func_0204b1cc(clact->unit);
    func_0204a5ac();
    MI_CpuFill8(clact, 0, sizeof(IrcSyncCheckClAct));
}

// pokebw2's IrcSyncCheckClAct_Update
static void func_ov103_021b7c4c(IrcSyncCheckClAct *clact) {
    func_0204a600();
}

// pokebw2's IrcSyncCheckClAct_VBlank
static void func_ov103_021b7c54(IrcSyncCheckClAct *clact) {
    func_0204a648();
}

// pokebw2's IrcSyncCheckClAct_GetActor
static ClActor *func_ov103_021b7c5c(IrcSyncCheckClAct *clact, int index) {
    return clact->actors[index];
}

// pokebw2's IrcSyncCheckClAct_GetUnit
static ClActUnit *func_ov103_021b7c64(IrcSyncCheckClAct *clact) {
    return clact->unit;
}

// pokebw2's IrcSyncCheckG3d_Init
static void func_ov103_021b7c68(IrcSyncCheckG3d *g3d, HeapID heapId) {
    ArcTool *arc;

    GFL_G3DSysCreate(FALSE, 1, FALSE, 1, 0, heapId, func_ov103_021b7d58);
    g3d->camera = GFL_G3DCameraCreate(G3DCAM_PROJECTION_ORTHO, FX32_CONST(24), FX32_CONST(-24), FX32_CONST(-32),
                                      FX32_CONST(32), FX32_ONE, FX32_CONST(1000), 0, &data_ov103_021b9228,
                                      &data_ov103_021b9234, &data_ov103_021b9240, heapId);
    arc = GFL_ArcSysCreateFileHandle(ARCID_IRC_COMPAT, heapId);
    func_ov103_021b7dfc(&g3d->models[0], arc, data_ov103_021b9218, 1, heapId);
    func_ov103_021b7dfc(&g3d->models[1], arc, data_ov103_021b9218, 2, heapId);
    GFL_ArcToolFree(arc);
}

// pokebw2's IrcSyncCheckG3d_Free
static void func_ov103_021b7d08(IrcSyncCheckG3d *g3d) {
    int i;

    for (i = 0; i < THUMB_COUNT; i++) {
        func_ov103_021b7eb8(&g3d->models[i]);
    }
    GFL_G3DCameraFree(g3d->camera);
    GFL_G3DSysFree();
}

// pokebw2's IrcSyncCheckG3d_Begin
static void func_ov103_021b7d30(IrcSyncCheckG3d *g3d) {
    GFL_G3DSysReset();
    GFL_G3DCameraFlush(g3d->camera);
    GFL_G3DSysMtxViewFlush();
}

// pokebw2's IrcSyncCheckG3d_End
static void func_ov103_021b7d44(IrcSyncCheckG3d *g3d) {
    GFL_G3DSysReqSwapBuffers();
}

// pokebw2's IrcSyncCheckG3d_GetModel
static IrcSyncCheckModel *func_ov103_021b7d4c(IrcSyncCheckG3d *g3d, u16 index) {
    return &g3d->models[index];
}

// The 3D system calls this once it is set up
// pokebw2's IrcSyncCheck_G3DInit
static void func_ov103_021b7d58(void) {
    u32 i;

    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG0, TRUE);
    G2_SetBG0Priority(0);
    G3X_SetShading(GX_SHADING_HIGHLIGHT);
    G3X_AntiAlias(FALSE);
    G3X_AlphaTest(FALSE, 0);
    G3X_AlphaBlend(TRUE);
    G3X_EdgeMarking(FALSE);
    G3X_SetFog(FALSE, 0, 0, 0);
    G3X_SetClearColor(GX_RGB(0, 0, 0), 0, 0x7fff, 63, FALSE);
    G3_ViewPort(0, 0, 255, 191);
    for (i = 0; i < NELEMS(data_ov103_021b92b4); i++) {
        GFL_G3DSysLightSet(i, &data_ov103_021b92b4[i]);
    }
    GFL_G3DSysSetSwapBufferParams(GX_SORTMODE_MANUAL, GX_BUFFERMODE_Z);
}

static void func_ov103_021b7dfc(IrcSyncCheckModel *model, ArcTool *arc, const u16 *fileIds, u32 polygonId,
                                   HeapID heapId) {
    void *texture;
    NNSG3dRenderObj *renderObj;
    int i;

    MI_CpuFill8(model, 0, sizeof(IrcSyncCheckModel));
    texture = NULL;
    model->resource = GFL_G3DSysReadArcToolResource(arc, fileIds[0]);
    if (GFL_G3DResCheckType(model->resource, G3D_RES_CHECK_TEX)) {
        texture = model->resource;
        GFL_G3DResUploadTexData(model->resource);
    }
    if (GFL_G3DResCheckType(model->resource, G3D_RES_CHECK_MDL)) {
        model->model = GFL_G3DMdlCreate(model->resource, 0, texture);
        renderObj = GFL_G3DMdlGetEngineModel(model->model);
        for (i = 0; i < 2; i++) {
            model->animationResources[i] = GFL_G3DSysReadArcToolResource(arc, fileIds[1 + i]);
            model->animations[i] = GFL_G3DAnmCreate(model->model, model->animationResources[i], 0);
        }
        model->actor = GFL_G3DActorCreate(model->model, model->animations, 2);
        for (i = 0; i < 2; i++) {
            GFL_G3DActorBindAnm(model->actor, i);
        }
        NNS_G3dMdlSetMdlPolygonIDAll(renderObj->resMdl, polygonId);
    }
    MTX_Identity33_(&model->transform.rotation);
    model->transform.scale.x = FX32_ONE;
    model->transform.scale.y = FX32_ONE;
    model->transform.scale.z = FX32_ONE;
}

// pokebw2's IrcSyncCheckModel_Free
static void func_ov103_021b7eb8(IrcSyncCheckModel *model) {
    int i;

    GFL_G3DActorFree(model->actor);
    for (i = 0; i < 2; i++) {
        if (model->animations[i] != NULL) {
            GFL_G3DAnmFree(model->animations[i]);
        }
    }
    GFL_G3DMdlFree(model->model);
    for (i = 0; i < 2; i++) {
        GFL_G3DResFree(model->animationResources[i]);
    }
    GFL_G3DResFree(model->resource);
    MI_CpuFill8(model, 0, sizeof(IrcSyncCheckModel));
}

// pokebw2's IrcSyncCheckModel_Update
static void func_ov103_021b7f04(IrcSyncCheckModel *model) {
    int i;
    fx32 frame;
    BOOL done;

    if (model->animation != 0) {
        for (i = 0; i < 2; i++) {
            GFL_G3DActorStepAnmFrameLoop(model->actor, i, FX32_ONE);
        }
        GFL_G3DActorGetAnmFrame(model->actor, 0, &frame);
        done = FALSE;
        if (frame == 0) {
            done = TRUE;
        }
        model->animationDone = done;
        GFL_G3DSysDrawObj(model->actor, &model->transform);
    }
}

// pokebw2's IrcSyncCheckModel_IsAnimationDone
static BOOL func_ov103_021b7f4c(IrcSyncCheckModel *model) {
    return model->animationDone;
}

// pokebw2's IrcSyncCheckModel_SetPosition
static void func_ov103_021b7f50(IrcSyncCheckModel *model, const VecFx32 *position) {
    model->transform.translation = *position;
}

// pokebw2's IrcSyncCheckModel_SetAnimation
static void func_ov103_021b7f60(IrcSyncCheckModel *model, int animation) {
    if (model->animation != animation) {
        GFL_G3DActorResetAnmFrame(model->actor, 0);
        model->animation = animation;
    }
}

// pokebw2's IrcSyncCheckModel_GetAnimation
static int func_ov103_021b7f78(IrcSyncCheckModel *model) {
    return model->animation;
}

// pokebw2's IrcSyncCheckText_Init
static void func_ov103_021b7f7c(IrcSyncCheckText *text, BOOL keepFont, HeapID heapId) {
    MI_CpuFill8(text, 0, sizeof(IrcSyncCheckText));
    func_0201dd60();
    if (!keepFont) {
        text->font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, heapId);
    }
    text->queue = func_0201c440(heapId);
    text->msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_BTL_MAIN_3, heapId);
    text->wordSet = GFL_WordSetSystemCreateDefault(heapId);
}

// pokebw2's IrcSyncCheckText_Free
static void func_ov103_021b7fc8(IrcSyncCheckText *text) {
    GFL_WordSetSystemFree(text->wordSet);
    GFL_MsgDataFree(text->msgData);
    func_0201c4c0(text->queue);
    GFL_FontFree(text->font);
    MI_CpuFill8(text, 0, sizeof(IrcSyncCheckText));
}

// pokebw2's IrcSyncCheckText_Update
static BOOL func_ov103_021b7ff0(IrcSyncCheckText *text) {
    return func_0201c4e4(text->queue);
}

// pokebw2's IrcSyncCheckText_GetFont
static Font *func_ov103_021b7ffc(IrcSyncCheckText *text) {
    return text->font;
}

// pokebw2's IrcSyncCheckText_GetQueue
static PrintQueue *func_ov103_021b8000(IrcSyncCheckText *text) {
    return text->queue;
}

// pokebw2's IrcSyncCheckText_GetMsgData
static MsgData *func_ov103_021b8004(IrcSyncCheckText *text) {
    return text->msgData;
}

// pokebw2's IrcSyncCheckText_GetWordSet
static WordSet *func_ov103_021b8008(IrcSyncCheckText *text) {
    return text->wordSet;
}

static void func_ov103_021b800c(IrcSyncCheckWindow *win, u8 bg, u8 x, u8 y, u8 width, u8 height, u8 palette,
                                    HeapID heapId) {
    MI_CpuFill8(win, 0, sizeof(IrcSyncCheckWindow));
    win->color = 15;
    win->window = BmpWin_CreateDynamic(bg, x, y, width, height, palette, TRUE);
    win->strbuf = GFL_StrBufCreate(255, heapId);
    PrintWindow_Init(&win->printWindow, win->window);
    func_ov103_021b831c(win);
    BmpWin_TransferNow(win->window);
}

static void func_ov103_021b8074(IrcSyncCheckWindow *win, u8 bg, u8 x, u8 y, u8 width, u8 height,
                                             u8 palette, u16 color, u8 fromEnd, HeapID heapId) {
    MI_CpuFill8(win, 0, sizeof(IrcSyncCheckWindow));
    win->color = color;
    win->window = BmpWin_CreateDynamic(bg, x, y, width, height, palette, fromEnd);
    win->strbuf = GFL_StrBufCreate(255, heapId);
    PrintWindow_Init(&win->printWindow, win->window);
    GFL_BitmapFill(BmpWin_GetBitmap(win->window), win->color);
    BmpWin_TransferNow(win->window);
}

// pokebw2's IrcSyncCheckWindow_Free
static void func_ov103_021b80e4(IrcSyncCheckWindow *win) {
    GFL_StrBufFree(win->strbuf);
    BmpWin_Free(win->window);
    MI_CpuFill8(win, 0, sizeof(IrcSyncCheckWindow));
}

// pokebw2's IrcSyncCheckWindow_Flush
static BOOL func_ov103_021b8100(IrcSyncCheckWindow *win, IrcSyncCheckText *text) {
    PrintWindow_Flush(&win->printWindow, text->queue);
    return PrintWindow_IsPrinted(&win->printWindow);
}

// pokebw2's IrcSyncCheckWindow_PrintMsg
static void func_ov103_021b8138(IrcSyncCheckWindow *win, IrcSyncCheckText *text, u32 msgId, int x, s16 y) {
    MsgData *msgData = func_ov103_021b8004(text);
    PrintQueue *queue = func_ov103_021b8000(text);
    Font *font = func_ov103_021b7ffc(text);

    GFL_BitmapFill(BmpWin_GetBitmap(win->window), win->color);
    GFL_MsgDataLoadStrbuf(msgData, msgId, win->strbuf);
    func_0201c6ec(queue);
    PrintWindow_PrintNoColor(&win->printWindow, queue, x, y, win->strbuf, font);
}

// pokebw2's IrcSyncCheckWindow_PrintMsgCentered
static void func_ov103_021b81a4(IrcSyncCheckWindow *win, IrcSyncCheckText *text, u32 msgId) {
    MsgData *msgData = func_ov103_021b8004(text);
    PrintQueue *queue = func_ov103_021b8000(text);
    Font *font = func_ov103_021b7ffc(text);
    u16 x;
    u16 y;

    GFL_BitmapFill(BmpWin_GetBitmap(win->window), win->color);
    GFL_MsgDataLoadStrbuf(msgData, msgId, win->strbuf);
    x = func_02045738(win->window) * 4;
    y = func_02045740(win->window) * 4;
    x -= (u16)(GFL_FontGetBlockWidth(win->strbuf, font, 0) / 2);
    y -= (u16)(GFL_FontGetBlockHeight(win->strbuf, font) / 2);
    func_0201c6ec(queue);
    PrintWindow_Print(&win->printWindow, queue, x, y, win->strbuf, font, PRINT_COLOR(15, 14, 4));
}

static void func_ov103_021b824c(IrcSyncCheckWindow *win, IrcSyncCheckText *text, u32 msgId,
                                              CompatibleIrcProfile *other, s16 x, s16 y, HeapID heapId) {
    WordSet *wordSet;
    PrintQueue *queue;
    Font *font;
    MsgData *msgData;
    StrBuf *format;

    GFL_BitmapFill(BmpWin_GetBitmap(win->window), win->color);
    wordSet = func_ov103_021b8008(text);
    msgData = func_ov103_021b8004(text);
    copyVarForText(wordSet, 0, &other->trainer);
    format = GFL_MsgDataLoadStrbufNew(msgData, msgId);
    GFL_WordSetFormatStrbuf(wordSet, win->strbuf, format);
    GFL_StrBufFree(format);
    queue = func_ov103_021b8000(text);
    font = func_ov103_021b7ffc(text);
    func_0201c6ec(queue);
    PrintWindow_PrintNoColor(&win->printWindow, queue, x, y, win->strbuf, font);
}

// Prints a message with the other player's name in it, if there is a link
// pokebw2's IrcSyncCheck_PrintMsgNamed
static void func_ov103_021b82dc(IrcSyncCheckWindow *win, IrcSyncCheckWork *wk, u32 msgId) {
    if (wk->param->gsys != NULL) {
        func_ov103_021b824c(win, &wk->text, msgId, wk->param->profile, 0, 0, HEAP_ID);
    } else {
        func_ov103_021b8138(win, &wk->text, msgId, 0, 0);
    }
}

// pokebw2's IrcSyncCheckWindow_Clear
static void func_ov103_021b831c(IrcSyncCheckWindow *win) {
    GFL_BitmapFill(BmpWin_GetBitmap(win->window), win->color);
    BmpWin_FlushChar(win->window);
}

// pokebw2's IrcSyncCheck_SetStep
static void func_ov103_021b8338(IrcSyncCheckWork *wk, IrcSyncCheckStepFunc step) {
    wk->step = step;
    wk->stepState = 0;
}

// pokebw2's IrcSyncCheck_SetStepAt
static void func_ov103_021b8348(IrcSyncCheckWork *wk, IrcSyncCheckStepFunc step, u16 state) {
    wk->step = step;
    wk->stepState = state;
}

// pokebw2's IrcSyncCheck_SetStepDone
static void func_ov103_021b8358(IrcSyncCheckWork *wk) {
    wk->stepDone = TRUE;
}

// Waits for a thumb on the first outline
// pokebw2's IrcSyncCheck_StepWaitFirst
static void func_ov103_021b8364(IrcSyncCheckWork *wk, u16 *state) {
    IrcSyncCheckThumbs *thumbs;
    IrcSyncPoint *first = &wk->first;
    int i;

    if (func_ov101_021b7250(wk->button) == 1) {
        wk->param->result = IRC_SYNC_CHECK_RESULT_CANCELLED;
        func_ov103_021b8358(wk);
    } else if (!func_ov101_021b727c(wk->button)) {
        switch (*state) {
        case 0:
            if (func_ov103_021b9214(&wk->traceA)) {
                func_ov103_021b8138(&wk->messageWindow, &wk->text, BtlMain3_Text_017, 0, 0);
            } else {
                if (wk->param->gsys != NULL) {
                    func_ov103_021b824c(&wk->messageWindow, &wk->text, BtlMain3_Text_000, wk->param->profile, 0, 0,
                                                      HEAP_ID);
                } else {
                    func_ov103_021b8138(&wk->messageWindow, &wk->text, BtlMain3_Text_000, 0, 0);
                }
            }
            thumbs = func_ov103_021b7954(&wk->graphic);
            for (i = 0; i < THUMB_COUNT; i++) {
                func_ov103_021b8ca0(thumbs, i, 0);
            }
            GFL_SndStop();
            func_ov103_021b9060(OUTLINE_FIRST, HEAP_ID);
            wk->hintTimer = 0;
            *state = 1;
            break;
        case 1:
            if (func_ov103_021b8d54(&data_ov103_021b925c, first)) {
                func_ov103_021b9060(0, HEAP_ID);
                wk->hintTimer = 0;
                wk->hintToggle = FALSE;
                thumbs = func_ov103_021b7954(&wk->graphic);
                func_ov103_021b8ca0(thumbs, 0, 1);
                func_ov103_021b8cb0(thumbs, 0, first->x, first->y);
                GFL_SndSEPlay(SEQ_SE_SYS_18);
                if (func_ov103_021b9214(&wk->traceA)) {
                    func_ov103_021b8138(&wk->messageWindow, &wk->text, BtlMain3_Text_002, 0, 0);
                    func_ov103_021b9060(OUTLINE_FIRST | OUTLINE_SECOND, HEAP_ID);
                    func_ov103_021b8348(wk, func_ov103_021b856c, 1);
                } else {
                    if (wk->param->gsys != NULL) {
                        func_ov103_021b824c(&wk->messageWindow, &wk->text, BtlMain3_Text_001, wk->param->profile, 0, 0,
                                                          HEAP_ID);
                    } else {
                        func_ov103_021b8138(&wk->messageWindow, &wk->text, BtlMain3_Text_001, 0, 0);
                    }
                    func_ov103_021b8338(wk, func_ov103_021b856c);
                }
            } else if (!func_ov103_021b9214(&wk->traceA)) {
                if (wk->hintTimer++ >= HINT_FRAMES) {
                    wk->hintTimer = 0;
                    wk->hintToggle ^= 1;
                    if (wk->hintToggle) {
                        func_ov103_021b82dc(&wk->messageWindow, wk, BtlMain3_Text_011);
                    } else {
                        func_ov103_021b82dc(&wk->messageWindow, wk, BtlMain3_Text_000);
                    }
                }
            }
            break;
        }
    }
    if (func_ov101_021b6480(wk->param->sys) != 0) {
        func_ov103_021b8338(wk, func_ov103_021b8a44);
    }
}

// Traces the first thumb while it is held
// pokebw2's IrcSyncCheck_StepTraceA
static void func_ov103_021b856c(IrcSyncCheckWork *wk, u16 *state) {
    IrcSyncPoint point;
    IrcSyncCheckThumbs *thumbs;
    IrcSyncPoint *second = &wk->second;
    IrcSyncTrace *trace = &wk->traceA;

    if (func_ov101_021b7250(wk->button) == 1) {
        wk->param->result = IRC_SYNC_CHECK_RESULT_CANCELLED;
        func_ov103_021b8358(wk);
    } else if (!func_ov101_021b727c(wk->button)) {
        switch (*state) {
        case 0:
            if (func_ov103_021b8d9c(&data_ov103_021b925c, &point)) {
                if (func_ov103_021b91b4(trace, &data_ov103_021b925c)) {
                    func_ov103_021b8138(&wk->messageWindow, &wk->text, BtlMain3_Text_002, 0, 0);
                    func_ov103_021b9060(OUTLINE_FIRST | OUTLINE_SECOND, HEAP_ID);
                    *state = 1;
                }
                thumbs = func_ov103_021b7954(&wk->graphic);
                func_ov103_021b8ca0(thumbs, 0, 1);
                func_ov103_021b8cb0(thumbs, 0, point.x, point.y);
            } else {
                if (func_ov103_021b9214(trace)) {
                    func_ov103_021b8138(&wk->messageWindow, &wk->text, BtlMain3_Text_002, 0, 0);
                    func_ov103_021b9060(OUTLINE_FIRST | OUTLINE_SECOND, HEAP_ID);
                    *state = 1;
                } else {
                    func_ov103_021b91a8(trace);
                }
                *state = 2;
            }
            break;
        case 1:
            if (func_ov103_021b8d9c(&data_ov103_021b926c, second)) {
                func_ov103_021b9060(0, HEAP_ID);
                if (wk->param->gsys != NULL) {
                    func_ov103_021b824c(&wk->messageWindow, &wk->text, BtlMain3_Text_001, wk->param->profile, 0, 0,
                                                      HEAP_ID);
                } else {
                    func_ov103_021b8138(&wk->messageWindow, &wk->text, BtlMain3_Text_001, 0, 0);
                }
                func_ov103_021b8338(wk, func_ov103_021b8728);
                thumbs = func_ov103_021b7954(&wk->graphic);
                func_ov103_021b8ca0(thumbs, 1, 1);
                func_ov103_021b8cb0(thumbs, 1, second->x, second->y);
                GFL_SndSEPlay(SEQ_SE_SYS_23);
            } else if (!func_ov103_021b8d9c(&data_ov103_021b925c, &point)) {
                *state = 2;
            }
            break;
        case 2:
            wk->retries++;
            func_ov103_021b8338(wk, func_ov103_021b8364);
            break;
        }
    }
    if (func_ov101_021b6480(wk->param->sys) != 0) {
        func_ov103_021b8338(wk, func_ov103_021b8a44);
    }
}

// Traces the second thumb while it is held
// pokebw2's IrcSyncCheck_StepTraceB
static void func_ov103_021b8728(IrcSyncCheckWork *wk, u16 *state) {
    IrcSyncPoint point;
    IrcSyncCheckThumbs *thumbs;
    IrcSyncTrace *trace = &wk->traceB;

    if (func_ov101_021b7250(wk->button) == 1) {
        wk->param->result = IRC_SYNC_CHECK_RESULT_CANCELLED;
        func_ov103_021b8358(wk);
    } else if (!func_ov101_021b727c(wk->button)) {
        switch (*state) {
        case 0:
            if (func_ov103_021b8d9c(&data_ov103_021b926c, &point)) {
                if (func_ov103_021b91b4(trace, &data_ov103_021b926c)) {
                    func_ov103_021b8138(&wk->messageWindow, &wk->text, BtlMain3_Text_003,
                                                0, 0);
                    GFL_SndSEPlay(SEQ_SE_SYS_21);
                    func_ov103_021b8338(wk, func_ov103_021b8828);
                }
                thumbs = func_ov103_021b7954(&wk->graphic);
                func_ov103_021b8ca0(thumbs, 1, 1);
                func_ov103_021b8cb0(thumbs, 1, point.x, point.y);
            } else {
                thumbs = func_ov103_021b7954(&wk->graphic);
                func_ov103_021b8ca0(thumbs, 1, 0);
                func_ov103_021b91a8(trace);
                *state = 1;
            }
            break;
        case 1:
            wk->retries++;
            func_ov103_021b8338(wk, func_ov103_021b8364);
            break;
        }
    }
    if (func_ov101_021b6480(wk->param->sys) != 0) {
        func_ov103_021b8338(wk, func_ov103_021b8a44);
    }
}

// Sends the traces to the other player, and scores the two
// pokebw2's IrcSyncCheck_StepResult
static void func_ov103_021b8828(IrcSyncCheckWork *wk, u16 *state) {
    IrcSyncData data;
    IrcSyncData received;
    SaveControl *save;
    TrainerLogSave *log;

    if (*state < RESULT_WAIT_CONNECTED && func_ov101_021b7250(wk->button) == 1) {
        wk->param->result = IRC_SYNC_CHECK_RESULT_CANCELLED;
        func_ov103_021b8358(wk);
        return;
    }
    switch (*state) {
    case RESULT_EXCHANGE_IDS_FIRST:
        if (!func_ov101_021b630c(wk->param->sys, 4)) {
            break;
        }
        *state = RESULT_PRINT_CHECKING;
        break;
    case RESULT_PRINT_CHECKING:
        func_ov103_021b8138(&wk->messageWindow, &wk->text, BtlMain3_Text_004, 0, 0);
        *state = RESULT_SEND_TRACES;
        break;
    case RESULT_SEND_TRACES:
        data.first = wk->first;
        data.second = wk->second;
        data.traceA = wk->traceA;
        data.traceB = wk->traceB;
        data.retries = wk->retries;
        if (wk->param->gsys != NULL) {
            save = GameData_GetSaveControl(GSYS_GetGameData(wk->param->gsys));
            log = func_0200a8a0(save);
            data.isNew = func_0200a9d4(log, getIDAsUInt(&wk->param->profile->trainer)) ? FALSE : TRUE;
        }
        if (!func_ov103_021b8b38(&wk->net, &data)) {
            break;
        }
        *state = RESULT_SEND_COUNT;
        break;
    case RESULT_SEND_COUNT:
        func_ov101_021b63cc(wk->param->sys, 3);
        if (!func_ov101_021b6424(wk->param->sys)) {
            break;
        }
        *state = RESULT_EXCHANGE_IDS_LAST;
        break;
    case RESULT_EXCHANGE_IDS_LAST:
        if (!func_ov101_021b630c(wk->param->sys, 5)) {
            break;
        }
        func_ov101_021b72a0(wk->button, FALSE);
        *state = RESULT_WAIT_CONNECTED;
        break;
    case RESULT_WAIT_CONNECTED:
        if (wk->timer < CONNECTED_FRAMES) {
            break;
        }
        GFL_SndSEPlay(SEQ_SE_SYS_12);
        func_ov103_021b8138(&wk->messageWindow, &wk->text, BtlMain3_Text_005, 0, 0);
        wk->counter = 0;
        *state = RESULT_WAIT_SCORE;
        break;
    case RESULT_WAIT_SCORE:
        if (wk->counter++ < DONE_FRAMES) {
            return;
        }
        wk->param->unk10 = func_ov103_021b8de4(wk);
        func_ov103_021b8bc0(&wk->net, &received);
        wk->param->unk11 = wk->retries + received.retries;
        wk->param->unk12 = received.isNew;
        wk->param->result = IRC_SYNC_CHECK_RESULT_DONE;
        *state = RESULT_FINISH;
        return;
    case RESULT_FINISH:
        func_ov103_021b8358(wk);
        return;
    }
    if (*state < RESULT_SEND_COUNT) {
        if (func_ov101_021b6480(wk->param->sys) != 0) {
            func_ov103_021b8338(wk, func_ov103_021b8a44);
        }
    }
    if (*state >= RESULT_PRINT_CHECKING && *state <= RESULT_WAIT_CONNECTED && wk->timer <= CONNECTED_FRAMES) {
        wk->timer++;
    }
}

// The link broke: ends the screen
// pokebw2's IrcSyncCheck_StepInterrupted
static void func_ov103_021b8a44(IrcSyncCheckWork *wk, u16 *state) {
    switch (*state) {
    case 0:
        if (func_ov101_021b6480(wk->param->sys) > 0) {
            *state = 1;
            return;
        }
        if (func_ov101_021b6480(wk->param->sys) < 0) {
            wk->param->unk10 = 0;
            wk->param->result = IRC_SYNC_CHECK_RESULT_INTERRUPTED;
            func_ov103_021b8358(wk);
            return;
        }
        break;
    case 1:
        wk->param->unk10 = 0;
        wk->param->result = IRC_SYNC_CHECK_RESULT_INTERRUPTED;
        *state = 2;
        break;
    case 2:
        func_ov103_021b8138(&wk->messageWindow, &wk->text, BtlMain3_Text_006, 0,
                                    0);
        *state = 3;
        break;
    case 3:
        if (func_ov101_021b630c(wk->param->sys, 6)) {
            *state = 4;
        }
        break;
    case 4:
        func_ov103_021b8358(wk);
        break;
    }
}

// pokebw2's IrcSyncNet_Init
static void func_ov103_021b8af4(IrcSyncNet *net, CompatibleIrcSys *sys) {
    MI_CpuFill8(net, 0, sizeof(IrcSyncNet));
    net->sys = sys;
    func_ov101_021b64f0(sys, NET_COMMAND_BASE, data_ov103_021b9220, NELEMS(data_ov103_021b9220), net);
}

// pokebw2's IrcSyncNet_Free
static void func_ov103_021b8b1c(IrcSyncNet *net) {
    func_ov101_021b6508(net->sys, NET_COMMAND_BASE);
    MI_CpuFill8(net, 0, sizeof(IrcSyncNet));
}

// Sends the data, and returns TRUE once the other player's has also arrived
// pokebw2's IrcSyncNet_Send
static BOOL func_ov103_021b8b38(IrcSyncNet *net, const IrcSyncData *data) {
    switch (net->state) {
    case 0:
        net->send = *data;
        net->state = 1;
        break;
    case 1:
        if (func_ov101_021b64b8(net->sys, NET_COMMAND_BASE, sizeof(IrcSyncData), &net->send,
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

// pokebw2's IrcSyncNet_GetReceived
static void func_ov103_021b8bc0(IrcSyncNet *net, IrcSyncData *dest) {
    *dest = net->peer;
}

// pokebw2's IrcSyncNet_OnReceive
static void func_ov103_021b8bd8(int netId, int size, const void *data, void *work, NetHandle *handle) {
    IrcSyncNet *net = work;

    if (handle == func_020399a8()) {
        if (netId != func_02039338()) {
            net->received = TRUE;
        }
    }
}

// pokebw2's IrcSyncNet_GetBuffer
static void *func_ov103_021b8bfc(int netId, void *work, int size) {
    IrcSyncNet *net = work;

    if (netId == func_02039338()) {
        return &net->own;
    }
    return &net->peer;
}

// pokebw2's IrcSyncCheckThumbs_Init
static void func_ov103_021b8c14(IrcSyncCheckThumbs *thumbs, IrcSyncCheckG3d *g3d, HeapID heapId) {
    int i;

    MI_CpuFill8(thumbs, 0, sizeof(IrcSyncCheckThumbs));
    for (i = 0; i < THUMB_COUNT; i++) {
        thumbs->models[i] = func_ov103_021b7d4c(g3d, i);
        func_ov103_021b7f60(thumbs->models[i], 0);
    }
}

// pokebw2's IrcSyncCheckThumbs_Free
static void func_ov103_021b8c40(IrcSyncCheckThumbs *thumbs) {
    MI_CpuFill8(thumbs, 0, sizeof(IrcSyncCheckThumbs));
}

// Draws the thumbs, with a sound when one has finished its animation
// pokebw2's IrcSyncCheckThumbs_Update
static void func_ov103_021b8c4c(IrcSyncCheckThumbs *thumbs) {
    int i;

    for (i = 0; i < THUMB_COUNT; i++) {
        func_ov103_021b7f04(thumbs->models[i]);
    }
    if (func_ov103_021b7f78(thumbs->models[0]) != 0) {
        if (func_ov103_021b7f4c(thumbs->models[0])) {
            GFL_SndSEPlay(SEQ_SE_SYS_18);
        }
    }
    if (func_ov103_021b7f78(thumbs->models[1]) != 0) {
        if (func_ov103_021b7f4c(thumbs->models[1])) {
            GFL_SndSEPlay(SEQ_SE_SYS_23);
        }
    }
}

// pokebw2's IrcSyncCheckThumbs_SetAnimation
static void func_ov103_021b8ca0(IrcSyncCheckThumbs *thumbs, int index, int animation) {
    func_ov103_021b7f60(thumbs->models[index], animation);
}

// Puts a thumb where the stylus is. The second thumb is further from the first's last place than the stylus is
// pokebw2's IrcSyncCheckThumbs_Place
static void func_ov103_021b8cb0(IrcSyncCheckThumbs *thumbs, int index, int x, int y) {
    VecFx32 nearPos;
    VecFx32 farPos;
    VecFx32 direction;

    switch (index) {
    case 0:
        NNS_G3dScrPosToWorldLine(x, y, &nearPos, &farPos);
        VEC_Subtract(&farPos, &nearPos, &direction);
        VEC_Normalize(&direction, &direction);
        VEC_MultAdd(FX32_CONST(64), &direction, &nearPos, &nearPos);
        func_ov103_021b7f50(thumbs->models[index], &nearPos);
        thumbs->lastX = x;
        thumbs->lastY = y;
        break;
    case 1:
        NNS_G3dScrPosToWorldLine(thumbs->lastX + (x - thumbs->lastX) * 2, thumbs->lastY + (y - thumbs->lastY) * 2, &nearPos,
                      &farPos);
        VEC_Subtract(&farPos, &nearPos, &direction);
        VEC_Normalize(&direction, &direction);
        VEC_MultAdd(FX32_CONST(64), &direction, &nearPos, &nearPos);
        func_ov103_021b7f50(thumbs->models[index], &nearPos);
        break;
    }
}

// Whether the stylus was put down inside a rectangle this frame, and where
// pokebw2's IrcSyncRect_IsPressed
static BOOL func_ov103_021b8d54(const IrcSyncRect *rect, IrcSyncPoint *point) {
    u32 x;
    u32 y;

    if (func_02035d20(&x, &y) && x - rect->left < rect->right - rect->left && y - rect->top < rect->bottom - rect->top) {
        if (point != NULL) {
            point->x = x;
            point->y = y;
        }
        return TRUE;
    }
    return FALSE;
}

// Whether the stylus is on the screen inside a rectangle, and where
// pokebw2's IrcSyncRect_IsHeld
static BOOL func_ov103_021b8d9c(const IrcSyncRect *rect, IrcSyncPoint *point) {
    u32 x;
    u32 y;

    if (func_02035cb8(&x, &y) && x - rect->left < rect->right - rect->left && y - rect->top < rect->bottom - rect->top) {
        if (point != NULL) {
            point->x = x;
            point->y = y;
        }
        return TRUE;
    }
    return FALSE;
}

// The score of a Sync Check: how close the first points are, and how alike the traces are
// pokebw2's IrcSyncCheck_ComputeScore
static u8 func_ov103_021b8de4(IrcSyncCheckWork *wk) {
    IrcSyncData own;
    IrcSyncData peer;
    u32 distance;
    u32 pointScore;
    u32 traceScoreA;
    u32 traceScoreB;
    IrcSyncPoint ownFirst;
    IrcSyncPoint peerFirst;
    int dx;
    int dy;

    own.first = wk->first;
    own.second = wk->second;
    own.traceA = wk->traceA;
    own.traceB = wk->traceB;
    func_ov103_021b8bc0(&wk->net, &peer);
    ownFirst = own.first;
    peerFirst = peer.first;
    dy = ownFirst.y - peerFirst.y;
    dx = ownFirst.x - peerFirst.x;
    distance = dx * dx + dy * dy;
    if (distance != 0 && distance <= 64) {
        pointScore = 100;
    } else if (distance > 64 && distance <= 256) {
        pointScore = 80;
    } else if (distance > 256 && distance <= 576) {
        pointScore = 50;
    } else if (distance > 576 && distance <= 1024) {
        pointScore = 30;
    } else if (distance > 1024 && distance <= 1600) {
        pointScore = 10;
    } else {
        pointScore = 0;
    }
    traceScoreA = func_ov103_021b8f00(own.traceA.points, own.traceA.points, peer.traceA.points,
                                             peer.traceA.points);
    traceScoreB = func_ov103_021b8f00(own.traceB.points, own.traceB.points, peer.traceB.points,
                                             peer.traceB.points);
    return traceScoreB * 20 / 100 + (pointScore * 40 / 100 + traceScoreA * 40 / 100);
}

// How alike two traces are, by how far each of their points is from the first
static u8 func_ov103_021b8f00(const IrcSyncPoint *ownStart, const IrcSyncPoint *own,
                                     const IrcSyncPoint *peerStart, const IrcSyncPoint *peer) {
    int i;
    u32 total = 0;

    for (i = 1; i < TRACE_POINT_COUNT; i++) {
        int ownDx = own[i].x - ownStart->x;
        int ownDy = own[i].y - ownStart->y;
        int peerDx = peer[i].x - peerStart->x;
        int peerDy = peer[i].y - peerStart->y;
        u32 ownDistance;
        u32 peerDistance;
        u16 difference;

        ownDistance = (ownDx < 0 ? -ownDx : ownDx) + (ownDy < 0 ? -ownDy : ownDy);
        peerDistance = (peerDx < 0 ? -peerDx : peerDx) + (peerDy < 0 ? -peerDy : peerDy);
        if (ownDistance > peerDistance) {
            difference = ownDistance - peerDistance;
        } else {
            difference = peerDistance - ownDistance;
        }
        if (difference == 0) {
            total += 100;
        } else if (difference == 1) {
            total += 80;
        } else if (difference == 2) {
            total += 50;
        } else if (difference >= 3 && difference <= 5) {
            total += 30;
        }
    }
    return total / 9;
}

static void func_ov103_021b8fa0(ArcTool *arc, u32 fileId, u32 bg, u32 tileOffset, u8 srcX, u8 srcY,
                                        u8 srcWidth, u8 srcHeight, u8 x, u8 y, u8 width, u8 height, u8 palette,
                                        BOOL compressed, HeapID heapId) {
    NNSG2dScreenData *screen;
    void *data;
    int i;

    data = GFL_ArcToolReadHeapNewLZ(arc, fileId, compressed, HEAPID_TAIL(heapId));
    NNS_G2dGetUnpackedScreenData(data, &screen);
    if (tileOffset != 0 && GFL_BGSysGetBGColorPaletteMode(bg) == 0) {
        u16 *tiles = (u16 *)screen->rawData;

        for (i = 0; i < srcWidth * srcHeight; i++) {
            tiles[i] += tileOffset;
        }
    }
    if (GFL_BGSysIsScrHeapExists(bg)) {
        GFL_BGSysLoadScrArea(bg, x, y, width, height, screen->rawData, srcX, srcY, srcWidth, srcHeight);
        GFL_BGSysSetScrPaletteNo(bg, x, y, width, height, palette);
        GFL_BGSysLoadScr(bg);
    }
    GFL_HeapFree(data);
}

// Shows the outlines that a mask has: the first and the second
// pokebw2's IrcSyncCheck_ShowOutlines
static void func_ov103_021b9060(u32 outlines, HeapID heapId) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_IRC_COMPAT, heapId);

    if (outlines & OUTLINE_FIRST) {
        func_ov103_021b8fa0(arc, 7, 2, 0, 0, 0, 16, 24, 0, 0, 16, 24, 9, FALSE, heapId);
    } else {
        GFL_BGSysFillScrArea(2, 0, 0, 0, 16, 24, 9);
    }
    if (outlines & OUTLINE_SECOND) {
        func_ov103_021b8fa0(arc, 8, 2, 0, 0, 0, 16, 24, 16, 0, 16, 24, 9, FALSE, heapId);
    } else {
        GFL_BGSysFillScrArea(2, 0, 16, 0, 16, 24, 9);
    }
    GFL_BGSysQueueScrLoad(2);
    GFL_ArcToolFree(arc);
}

static void func_ov103_021b9110(IrcSyncCheckSparkles *sparkles, IrcSyncCheckGraphic *graphic, int unk580,
                                      u32 palette, int firstActor, u32 value) {
    int i;

    MI_CpuFill8(sparkles, 0, sizeof(IrcSyncCheckSparkles));
    sparkles->unk580 = unk580;
    sparkles->palette = palette;
    sparkles->timer = 0;
    sparkles->interval = 10;
    for (i = 0; i < SPARKLE_COUNT; i++) {
        func_ov103_021b9184(&sparkles->sparkles[i], func_ov103_021b793c(graphic, firstActor + i),
                                 palette, value);
    }
}

// pokebw2's IrcSyncCheckSparkles_Free
static void func_ov103_021b9174(IrcSyncCheckSparkles *sparkles) {
    MI_CpuFill8(sparkles, 0, sizeof(IrcSyncCheckSparkles));
}

// pokebw2's IrcSyncCheckSparkle_Init
static void func_ov103_021b9184(IrcSyncCheckSparkle *sparkle, ClActor *actor, u32 palette, u32 value) {
    MI_CpuFill8(sparkle, 0, sizeof(IrcSyncCheckSparkle));
    sparkle->actor = actor;
    sparkle->value = value;
    func_0204b6f4(actor, (u8)palette, TRUE);
}

// pokebw2's IrcSyncTrace_Clear
static void func_ov103_021b91a8(IrcSyncTrace *trace) {
    MI_CpuFill8(trace, 0, sizeof(IrcSyncTrace));
}

// Takes a point of the trace every few frames while a thumb is held, and returns TRUE once it is done
// pokebw2's IrcSyncTrace_Update
static BOOL func_ov103_021b91b4(IrcSyncTrace *trace, const IrcSyncRect *rect) {
    if (trace->done) {
        return TRUE;
    }
    if (trace->frames++ > TRACE_FRAMES) {
        func_ov103_021b8d9c(rect, &trace->points[TRACE_POINT_COUNT - 1]);
        trace->done = TRUE;
        return TRUE;
    }
    if (trace->phase == 0) {
        func_ov103_021b8d9c(rect, &trace->points[trace->index++]);
    }
    if (trace->phase++ > TRACE_INTERVAL - 1) {
        trace->phase = 0;
    }
    return FALSE;
}

// pokebw2's IrcSyncTrace_IsDone
static BOOL func_ov103_021b9214(const IrcSyncTrace *trace) {
    return trace->done;
}
