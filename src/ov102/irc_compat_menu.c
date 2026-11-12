// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov176/irc_compat_menu.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/compatible_irc_sys.h"
#include "app/irc_appbar.h"
#include "app/irc_compat_menu.h"
#include "app/irc_compatible.h"
#include "constants/arc.h"
#include "constants/sound.h"
#include "constants/text_banks.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/button_man.h"
#include "gfl/clact.h"
#include "gfl/fade.h"
#include "gfl/graphics.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/proc.h"
#include "gfl/random.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "gfl/touchpanel.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/gx.h"
#include "nitro/os.h"
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
#include "text/system/btl_main_4.h"

// The first screen of the compatibility checker: its graphics, its windows, the two buttons, the hearts that float up
// the background and the steps of the screen, which also make the link with the other player. The ROM has no string for
// the file, so the name is a guess. Names are ours

#define HEAP_ID 0x3b
#define BG_COUNT 8
#define CLACT_COUNT 32
#define HEART_COUNT 30
#define BUTTON_COUNT_MAX 4

// The actor of the sprite behind the hearts
#define ACTOR_BACKGROUND (CLACT_COUNT - 1)

// The frames that the title scrolls in for, and the frames the connection is shown for
#define SCROLL_FRAMES 120
#define SCROLL_DISTANCE 0x58
#define CONNECTED_FRAMES 60
#define FINISH_FRAMES 30

// The steps of func_ov102_021b7f24
enum {
    CONNECT_RESET,
    CONNECT_START_NET,
    CONNECT_START_NEGOTIATION,
    CONNECT_PRINT_CHECKING,
    CONNECT_SYNC,
    CONNECT_EXCHANGE_PROFILES,
    CONNECT_EXCHANGE_IDS,
    CONNECT_UNUSED,
    CONNECT_SEND_SELECTION,
    CONNECT_POLL_SELECTION,
    CONNECT_SEND_PING,
    CONNECT_POLL_PING,
    CONNECT_SEND_COUNT,
    CONNECT_EXCHANGE_IDS_LAST,
    CONNECT_WAIT_CONNECTED,
    CONNECT_WAIT_FINISH,
};

typedef struct {
    ClActUnit *unit;
    u32 palette;
    u32 chars;
    u32 cellAnims;
    ClActor *actors[CLACT_COUNT];
} IrcCompatMenuClAct;

typedef struct {
    IrcCompatMenuClAct clact;
    TCB *vblankTask;
} IrcCompatMenuGraphic;

typedef struct {
    // The characters of the window frame
    u32 frameChars;
    TCB *vblankTask;
} IrcCompatMenuBG;

typedef struct {
    Font *font;
    MsgData *msgData;
    PrintQueue *queue;
    WordSet *wordSet;
} IrcCompatMenuText;

typedef struct {
    BmpWin *window;
    PrintWindow printWindow;
    StrBuf *strbuf;
    u16 color;
} IrcCompatMenuWindow;

// A button: the message on it, and its place in tiles
typedef struct {
    u16 msgId;
    u16 x;
    u16 y;
    u16 width;
    u16 height;
    u16 palette;
} IrcCompatMenuButtonDef;

typedef struct {
    ButtonMan *manager;
    BmpWin *windows[BUTTON_COUNT_MAX];
    TouchRect rects[BUTTON_COUNT_MAX + 1];
    const IrcCompatMenuButtonDef *defs;
    u16 count;
    u16 bg;
    // The button that was touched
    u16 touched;
    // Whether the touched button has flashed
    u16 pressed;
    u16 state;
    u16 frame;
    BOOL enabled;
} IrcCompatMenuButtons;

// A value that goes from one number to another in steps, in fixed point
typedef struct {
    int current;
    int start;
    int end;
    int step;
    int frame;
    int duration;
} IrcCompatMenuEase;

// How the title scrolls in
typedef struct {
    IrcCompatMenuEase ease;
    BOOL active;
} IrcCompatMenuScroll;

// A heart sprite that floats up from below the screen and starts again once it is out of sight
typedef struct {
    int x;
    int y;
    int startX;
    int startY;
    u32 frame;
    u32 duration;
    ClActor *actor;
} IrcCompatMenuHeart;

// A heart for each player the trainer has logged
typedef struct {
    IrcCompatMenuHeart hearts[HEART_COUNT];
    u32 timer;
    u16 activeCount;
    u16 count;
} IrcCompatMenuHearts;

typedef struct IrcCompatMenuWork IrcCompatMenuWork;
typedef void (*IrcCompatMenuStepFunc)(IrcCompatMenuWork *wk, u16 *state);

struct IrcCompatMenuWork {
    IrcCompatMenuGraphic graphic;
    IrcCompatMenuBG bg;
    IrcCompatMenuText text;
    // The message of the step
    IrcCompatMenuWindow messageWindow;
    IrcCompatMenuButtons buttons;
    IrcCompatMenuScroll scroll;
    IrcCompatMenuHearts hearts;
    IrcAppBar *button;
    // The title
    IrcCompatMenuWindow titleWindow;
    IrcCompatMenuStepFunc step;
    u16 stepState;
    u16 timer;
    BOOL stepDone;
    IrcCompatibleParam0 *param;
    u32 startTime;
    // The button that was chosen
    int choice;
    // How long the choice took
    int choiceTime;
    u32 counter;
    BOOL choiceSent;
};

static BOOL func_ov102_021b7560(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov102_021b7734(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov102_021b7798(GameProc *proc, u32 *state, void *param, void *work);
static void func_ov102_021b7860(IrcCompatMenuGraphic *graphic, HeapID heapId);
static void func_ov102_021b78d0(IrcCompatMenuGraphic *graphic);
static void func_ov102_021b78e4(IrcCompatMenuGraphic *graphic);
static ClActor *func_ov102_021b78ec(IrcCompatMenuGraphic *graphic, int index);
static ClActUnit *func_ov102_021b78f4(IrcCompatMenuGraphic *graphic);
static void func_ov102_021b78fc(TCB *tcb, void *data);
static void func_ov102_021b7908(IrcCompatMenuBG *bg, HeapID heapId);
static void func_ov102_021b7a60(IrcCompatMenuBG *bg);
static void func_ov102_021b7aac(TCB *tcb, void *data);
static void func_ov102_021b7ab4(IrcCompatMenuClAct *clact, const BGSysVRAMConfig *vramConfig, HeapID heapId);
static void func_ov102_021b7bc0(IrcCompatMenuClAct *clact);
static void func_ov102_021b7c04(IrcCompatMenuClAct *clact);
static void func_ov102_021b7c0c(IrcCompatMenuClAct *clact);
static ClActor *func_ov102_021b7c14(IrcCompatMenuClAct *clact, int index);
static ClActUnit *func_ov102_021b7c1c(IrcCompatMenuClAct *clact);
static void func_ov102_021b7c20(IrcCompatMenuText *text, HeapID heapId);
static void func_ov102_021b7c88(IrcCompatMenuText *text);
static BOOL func_ov102_021b7cb0(IrcCompatMenuText *text);
static Font *func_ov102_021b7cbc(IrcCompatMenuText *text);
static PrintQueue *func_ov102_021b7cc0(IrcCompatMenuText *text);
static MsgData *func_ov102_021b7cc4(IrcCompatMenuText *text);
static void func_ov102_021b7cc8(IrcCompatMenuWindow *win, u8 bg, u8 x, u8 y, u8 width, u8 height, u8 palette,
                                     HeapID heapId);
static void func_ov102_021b7d38(IrcCompatMenuWindow *win, u8 bg, u8 x, u8 y, u8 width, u8 height,
                                              u8 palette, u16 color, u8 fromEnd, HeapID heapId);
static void func_ov102_021b7da8(IrcCompatMenuWindow *win);
static BOOL func_ov102_021b7dc4(IrcCompatMenuWindow *win, IrcCompatMenuText *text);
static void func_ov102_021b7dfc(IrcCompatMenuWindow *win, IrcCompatMenuText *text, u32 msgId, int x, s16 y);
void func_ov102_021b7e60(IrcCompatMenuWindow *win, IrcCompatMenuText *text, u32 msgId);
static void func_ov102_021b7f08(IrcCompatMenuWork *wk, IrcCompatMenuStepFunc step);
void func_ov102_021b7f18(IrcCompatMenuWork *wk);
void func_ov102_021b7f24(IrcCompatMenuWork *wk, u16 *state);
void func_ov102_021b81b0(IrcCompatMenuWork *wk, u16 *state);
void func_0200ed54(RivalDataSave *rivalData, u32 id);
void func_ov102_021b82dc(IrcCompatMenuWork *wk, u16 *state);
void func_ov102_021b8320(IrcCompatMenuWork *wk, u16 *state);
void func_ov102_021b838c(IrcCompatMenuWork *wk, u16 *state);
void func_ov102_021b83e0(IrcCompatMenuWork *wk, u16 *state);
static void func_ov102_021b842c(IrcCompatMenuWork *wk);
static void func_ov102_021b8464(IrcCompatMenuButtons *buttons, u8 bg, const IrcCompatMenuButtonDef *defs,
                                      u16 count, IrcCompatMenuText *text, HeapID heapId);
static void func_ov102_021b85f4(IrcCompatMenuButtons *buttons);
void func_ov102_021b8624(IrcCompatMenuButtons *buttons);
void func_ov102_021b86f8(IrcCompatMenuButtons *buttons, BOOL enabled);
BOOL func_ov102_021b86fc(IrcCompatMenuButtons *buttons);
BOOL func_ov102_021b870c(IrcCompatMenuButtons *buttons, int *result);
void func_ov102_021b8724(IrcCompatMenuButtons *buttons);
static void func_ov102_021b8794(u32 button, u32 event, void *work);
static void func_ov102_021b879c(IrcCompatMenuScroll *scroll, HeapID heapId);
static void func_ov102_021b87bc(IrcCompatMenuScroll *scroll);
static void func_ov102_021b87c8(IrcCompatMenuScroll *scroll);
static void func_ov102_021b87fc(IrcCompatMenuScroll *scroll);
static void func_ov102_021b8810(IrcCompatMenuHeart *heart, ClActor *actor, BOOL isSpecial, HeapID heapId);
static void func_ov102_021b8868(IrcCompatMenuHeart *heart);
static void func_ov102_021b8874(IrcCompatMenuHeart *heart);
static void func_ov102_021b88f8(IrcCompatMenuHeart *heart);
static void func_ov102_021b8934(IrcCompatMenuHearts *hearts, IrcCompatMenuGraphic *graphic, TrainerLogSave *log,
                                     HeapID heapId);
static void func_ov102_021b89b8(IrcCompatMenuHearts *hearts);
static void func_ov102_021b89f0(IrcCompatMenuHearts *hearts);
static void func_ov102_021b8a40(IrcCompatMenuEase *ease, int from, int to, int frames);
static BOOL func_ov102_021b8a90(IrcCompatMenuEase *ease);

// pokebw2's IRC_COMPAT_MENU_PROC_FUNCTIONS
const GameProcFunctions data_ov102_021b8ab8 = {
    func_ov102_021b7560,
    func_ov102_021b7798,
    func_ov102_021b7734,
};

// pokebw2's sIrcCompatMenuLCDConfig
static const BGSysLCDConfig data_ov102_021b8ac4 = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_1, GX_BG0_AS_2D };

// pokebw2's sIrcCompatMenuButtonDefs
static const IrcCompatMenuButtonDef data_ov102_021b8ad4[2] = {
    { BtlMain4_Text_006, 3, 2, 26, 5, 11 },
    { BtlMain4_Text_007, 3, 14, 26, 5, 4 },
};

// pokebw2's sIrcCompatMenuBGModes
static const u32 data_ov102_021b8b0c[BG_COUNT] = { BGMODE_TEXT, BGMODE_TEXT, BGMODE_TEXT, BGMODE_TEXT,
                                                     BGMODE_TEXT, BGMODE_TEXT, BGMODE_TEXT, BGMODE_AFFINE };

// pokebw2's sIrcCompatMenuBGs
static const u32 data_ov102_021b8aec[BG_COUNT] = { 0, 1, 2, 3, 4, 5, 6, 7 };

// pokebw2's sIrcCompatMenuVRAMConfig
static const BGSysVRAMConfig data_ov102_021b8b2c = {
    GX_VRAM_BG_128_A,  GX_VRAM_BGEXTPLTT_NONE,  GX_VRAM_SUB_BG_128_C,        GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_128_B, GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_128_D,       GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_NONE,  GX_VRAM_TEXPLTT_NONE,    GX_OBJVRAMMODE_CHAR_1D_128K, GX_OBJVRAMMODE_CHAR_1D_128K,
};

// pokebw2's sIrcCompatMenuBGSetups
static const BGSetup data_ov102_021b8b5c[BG_COUNT] = {
    { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x04000, 0x8000,
      GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE },
    { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x0800, GX_BG_CHARBASE_0x08000, 0x8000,
      GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE },
    { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x1000, GX_BG_CHARBASE_0x0c000, 0x8000,
      GX_BG_EXTPLTT_01, 1, GX_BG_AREAOVER_XLU, FALSE },
    { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x1800, GX_BG_CHARBASE_0x10000, 0x8000,
      GX_BG_EXTPLTT_01, 2, GX_BG_AREAOVER_XLU, FALSE },
    { 0, 0, 0x1000, 0, BGRES_256x512, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x04000, 0x8000,
      GX_BG_EXTPLTT_01, 2, GX_BG_AREAOVER_XLU, FALSE },
    { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x1000, GX_BG_CHARBASE_0x08000, 0x8000,
      GX_BG_EXTPLTT_01, 1, GX_BG_AREAOVER_XLU, FALSE },
    { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x2000, GX_BG_CHARBASE_0x0c000, 0x8000,
      GX_BG_EXTPLTT_01, 3, GX_BG_AREAOVER_XLU, FALSE },
    { 0, 0, 0x1000, 0, BGRES_256x256, GX_BG_COLORMODE_256, GX_BG_SCRBASE_0x3000, GX_BG_CHARBASE_0x14000, 0x4000,
      GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE },
};

// pokebw2's IrcCompatMenu_Init
static BOOL func_ov102_021b7560(GameProc *proc, u32 *state, void *param, void *work) {
    IrcCompatMenuWork *wk;
    IrcCompatibleParam0 *menuParam = param;
    SaveControl *save;
    TrainerLogSave *log;
    ClActUnit *unit;
    Font *font;
    PrintQueue *queue;

    GFL_HeapCreateChild(HEAPID_USER, HEAP_ID, 0x40000);
    wk = GFL_ProcInitSubsystem(proc, sizeof(IrcCompatMenuWork), HEAP_ID);
    MI_CpuFill8(wk, 0, sizeof(IrcCompatMenuWork));
    wk->param = menuParam;
    func_ov102_021b7c20(&wk->text, HEAP_ID);
    func_ov102_021b7860(&wk->graphic, HEAP_ID);
    func_ov102_021b7908(&wk->bg, HEAP_ID);
    if (wk->param->gsys != NULL) {
        GSYS_GetGameCommSystem(wk->param->gsys);
    }
    func_ov102_021b8464(&wk->buttons, 2, data_ov102_021b8ad4, 2, &wk->text, HEAP_ID);
    func_ov102_021b7cc8(&wk->messageWindow, 5, 1, 18, 30, 5, 8, HEAP_ID);
    BmpWin_DrawFrame(wk->messageWindow.window, 0, (u16)wk->bg.frameChars, 6);
    func_ov102_021b7dfc(&wk->messageWindow, &wk->text, BtlMain4_Text_005, 0, 0);
    func_ov102_021b7d38(&wk->titleWindow, 7, 6, 3, 20, 5, 8, 1, TRUE, HEAP_ID);
    func_ov102_021b7e60(&wk->titleWindow, &wk->text, BtlMain4_Text_010);
    GFL_BGSysScaleBGReq(7, 3, 0x1000);
    GFL_BGSysScaleBGReq(7, 6, 0x1000);
    GFL_BGSysAdjustBGOriginReq(7, 9, 0x80);
    GFL_BGSysAdjustBGOriginReq(7, 12, 0x28);
    func_ov102_021b879c(&wk->scroll, HEAP_ID);
    if (wk->param->gsys != NULL) {
        save = GameData_GetSaveControl(GSYS_GetGameData(wk->param->gsys));
    } else {
        save = SaveControl_GetInstance();
    }
    log = func_0200a8a0(save);
    func_ov102_021b8934(&wk->hearts, &wk->graphic, log, HEAP_ID);
    unit = func_ov102_021b78f4(&wk->graphic);
    font = func_ov102_021b7cbc(&wk->text);
    queue = func_ov102_021b7cc0(&wk->text);
    wk->button = func_ov101_021b70dc(2, unit, 0, 14, 0, 2, font, queue, HEAP_ID);
    switch (wk->param->visit) {
    case IRC_COMPAT_VISIT_FIRST:
        func_ov102_021b7f08(wk, func_ov102_021b81b0);
        break;
    case IRC_COMPAT_VISIT_AGAIN:
        func_ov102_021b7f08(wk, func_ov102_021b8320);
        break;
    case IRC_COMPAT_VISIT_AFTER_RESULT:
        func_ov102_021b7f08(wk, func_ov102_021b81b0);
        break;
    }
    func_02005d28();
    func_ov101_021b63cc(wk->param->sys, 0);
    return TRUE;
}

// pokebw2's IrcCompatMenu_Exit
static BOOL func_ov102_021b7734(GameProc *proc, u32 *state, void *param, void *work) {
    IrcCompatMenuWork *wk = work;

    func_ov101_021b71c0(wk->button);
    func_ov102_021b89b8(&wk->hearts);
    func_ov102_021b87bc(&wk->scroll);
    func_ov102_021b85f4(&wk->buttons);
    func_ov102_021b7da8(&wk->titleWindow);
    func_ov102_021b7da8(&wk->messageWindow);
    func_ov102_021b7a60(&wk->bg);
    func_ov102_021b78d0(&wk->graphic);
    func_ov102_021b7c88(&wk->text);
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(HEAP_ID);
    return TRUE;
}

// pokebw2's IrcCompatMenu_Main
static BOOL func_ov102_021b7798(GameProc *proc, u32 *state, void *param, void *work) {
    IrcCompatMenuWork *wk = work;

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
        func_0204b92c(func_ov102_021b78ec(&wk->graphic, ACTOR_BACKGROUND), TRUE);
        func_ov102_021b87fc(&wk->scroll);
        GFL_SndBGMPlay(SEQ_ME_AISHOU_KIDOU, SND_CHANNEL_MASK_ALL);
        *state = 3;
        break;
    case 3:
        func_ov102_021b89f0(&wk->hearts);
        func_ov102_021b87c8(&wk->scroll);
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
    func_ov102_021b842c(wk);
    return FALSE;
}

// pokebw2's IrcCompatMenuGraphic_Create
static void func_ov102_021b7860(IrcCompatMenuGraphic *graphic, HeapID heapId) {
    MI_CpuFill8(graphic, 0, sizeof(IrcCompatMenuGraphic));
    G2_BlendNone();
    G2S_BlendNone();
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    GFL_BGSysInitVRAM(GX_VRAM_LCDC_NONE);
    GFL_BGSysSetDisplayLayout(0);
    GFL_BGSysEnableEngines();
    GFL_BGSysSetVRAMBanks(&data_ov102_021b8b2c);
    func_ov102_021b7ab4(&graphic->clact, &data_ov102_021b8b2c, heapId);
    graphic->vblankTask = GFL_VBlankTCBAdd(func_ov102_021b78fc, graphic, 0);
}

// pokebw2's IrcCompatMenuGraphic_Free
static void func_ov102_021b78d0(IrcCompatMenuGraphic *graphic) {
    GFL_TCBRemove(graphic->vblankTask);
    func_ov102_021b7bc0(&graphic->clact);
}

// pokebw2's IrcCompatMenuGraphic_Update
static void func_ov102_021b78e4(IrcCompatMenuGraphic *graphic) {
    func_ov102_021b7c04(&graphic->clact);
}

// pokebw2's IrcCompatMenuGraphic_GetActor
static ClActor *func_ov102_021b78ec(IrcCompatMenuGraphic *graphic, int index) {
    return func_ov102_021b7c14(&graphic->clact, index);
}

// pokebw2's IrcCompatMenuGraphic_GetUnit
static ClActUnit *func_ov102_021b78f4(IrcCompatMenuGraphic *graphic) {
    return func_ov102_021b7c1c(&graphic->clact);
}

// pokebw2's IrcCompatMenuGraphic_VBlank
static void func_ov102_021b78fc(TCB *tcb, void *data) {
    IrcCompatMenuGraphic *graphic = data;

    func_ov102_021b7c0c(&graphic->clact);
}

// pokebw2's IrcCompatMenuBG_Init
static void func_ov102_021b7908(IrcCompatMenuBG *bg, HeapID heapId) {
    ArcTool *arc;
    int i;

    GFL_BGSysCreate(heapId);
    BmpWin_InitAllocator(heapId);
    GFL_BGSysSetLCDConfig(&data_ov102_021b8ac4);
    for (i = 0; i < BG_COUNT; i++) {
        GFL_BGSysCreateBG(data_ov102_021b8aec[i], &data_ov102_021b8b5c[i], data_ov102_021b8b0c[i]);
        GFL_BGSysClearBG(data_ov102_021b8aec[i]);
        GFL_BGSysSetBGEnabled(data_ov102_021b8aec[i], TRUE);
    }
    arc = GFL_ArcSysCreateFileHandle(ARCID_IRC_COMPAT, heapId);
    GFL_G2DIOLoadArcNCLRDefault(arc, 1, PALTYPE_MAIN_BG, 0, 0, heapId);
    GFL_G2DIOLoadArcNCLRDefault(arc, 1, PALTYPE_SUB_BG, 0, 0, heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 0, 3, 0, 0, FALSE, heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 0, 6, 0, 0, FALSE, heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 48, 4, 0, 0, FALSE, heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 42, 2, 0, 0, FALSE, heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 4, 3, 0, 0, FALSE, heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 4, 6, 0, 0, FALSE, heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 44, 4, 0, 0, FALSE, heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 43, 2, 0, 0, FALSE, heapId);
    GFL_BGSysFillChar(5, 0, 1, 0);
    bg->frameChars = GFL_BGSysLoadArcNCGRDynamic(arc, 49, 5, 0, FALSE, heapId);
    GFL_ArcToolFree(arc);
    bg->vblankTask = GFL_VBlankTCBAdd(func_ov102_021b7aac, bg, 0);
}

// pokebw2's IrcCompatMenuBG_Free
static void func_ov102_021b7a60(IrcCompatMenuBG *bg) {
    int i;

    GFL_TCBRemove(bg->vblankTask);
    GFL_BGSysFreeCharMemory(5, (u16)bg->frameChars, (u16)(bg->frameChars >> 16));
    GFL_BGSysFreeFilledChar(5, 1, 0);
    for (i = 0; i < BG_COUNT; i++) {
        GFL_BGSysReleaseBG(data_ov102_021b8aec[i]);
    }
    BmpWin_FreeAllocator();
    GFL_BGSysFree();
}

// pokebw2's IrcCompatMenuBG_VBlank
static void func_ov102_021b7aac(TCB *tcb, void *data) {
    GFL_BGSysUpdate();
}

// pokebw2's IrcCompatMenuClAct_Init
static void func_ov102_021b7ab4(IrcCompatMenuClAct *clact, const BGSysVRAMConfig *vramConfig, HeapID heapId) {
    ArcTool *arc;
    ClActorSetup setup;
    int i;

    MI_CpuFill8(clact, 0, sizeof(IrcCompatMenuClAct));
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
    for (i = 0; i < HEART_COUNT; i++) {
        clact->actors[i] = func_0204b294(clact->unit, clact->chars, clact->palette, clact->cellAnims, &setup,
                                         CLACT_SURFACE_SUB, heapId);
        func_0204b3dc(clact->actors[i], FALSE);
        func_0204b84c(clact->actors[i], 3);
        func_0204b92c(clact->actors[i], TRUE);
    }
    setup.x = 129;
    setup.y = 104;
    setup.sequence = 15;
    clact->actors[ACTOR_BACKGROUND] = func_0204b294(clact->unit, clact->chars, clact->palette, clact->cellAnims,
                                                    &setup, CLACT_SURFACE_SUB, heapId);
    func_0204b3dc(clact->actors[ACTOR_BACKGROUND], TRUE);
    func_0204b84c(clact->actors[ACTOR_BACKGROUND], 2);
    func_0204b92c(clact->actors[ACTOR_BACKGROUND], FALSE);
}

// pokebw2's IrcCompatMenuClAct_Free
static void func_ov102_021b7bc0(IrcCompatMenuClAct *clact) {
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
    MI_CpuFill8(clact, 0, sizeof(IrcCompatMenuClAct));
}

// pokebw2's IrcCompatMenuClAct_Update
static void func_ov102_021b7c04(IrcCompatMenuClAct *clact) {
    func_0204a600();
}

// pokebw2's IrcCompatMenuClAct_VBlank
static void func_ov102_021b7c0c(IrcCompatMenuClAct *clact) {
    func_0204a648();
}

// pokebw2's IrcCompatMenuClAct_GetActor
static ClActor *func_ov102_021b7c14(IrcCompatMenuClAct *clact, int index) {
    return clact->actors[index];
}

// pokebw2's IrcCompatMenuClAct_GetUnit
static ClActUnit *func_ov102_021b7c1c(IrcCompatMenuClAct *clact) {
    return clact->unit;
}

// pokebw2's IrcCompatMenuText_Init
static void func_ov102_021b7c20(IrcCompatMenuText *text, HeapID heapId) {
    MI_CpuFill8(text, 0, sizeof(IrcCompatMenuText));
    func_0201dd60();
    text->font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, heapId);
    text->queue = func_0201c440(heapId);
    text->msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_BTL_MAIN_4, heapId);
    text->wordSet = GFL_WordSetSystemCreateDefault(heapId);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, PALTYPE_SUB_BG, 0, 0x20, heapId);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, PALTYPE_MAIN_BG, 0, 0x20, heapId);
}

// pokebw2's IrcCompatMenuText_Free
static void func_ov102_021b7c88(IrcCompatMenuText *text) {
    GFL_WordSetSystemFree(text->wordSet);
    GFL_MsgDataFree(text->msgData);
    func_0201c4c0(text->queue);
    GFL_FontFree(text->font);
    MI_CpuFill8(text, 0, sizeof(IrcCompatMenuText));
}

// pokebw2's IrcCompatMenuText_Update
static BOOL func_ov102_021b7cb0(IrcCompatMenuText *text) {
    return func_0201c4e4(text->queue);
}

// pokebw2's IrcCompatMenuText_GetFont
static Font *func_ov102_021b7cbc(IrcCompatMenuText *text) {
    return text->font;
}

// pokebw2's IrcCompatMenuText_GetQueue
static PrintQueue *func_ov102_021b7cc0(IrcCompatMenuText *text) {
    return text->queue;
}

// pokebw2's IrcCompatMenuText_GetMsgData
static MsgData *func_ov102_021b7cc4(IrcCompatMenuText *text) {
    return text->msgData;
}

static void func_ov102_021b7cc8(IrcCompatMenuWindow *win, u8 bg, u8 x, u8 y, u8 width, u8 height, u8 palette,
                                     HeapID heapId) {
    MI_CpuFill8(win, 0, sizeof(IrcCompatMenuWindow));
    win->color = 15;
    win->window = BmpWin_CreateDynamic(bg, x, y, width, height, palette, TRUE);
    win->strbuf = GFL_StrBufCreate(255, heapId);
    PrintWindow_Init(&win->printWindow, win->window);
    GFL_BitmapFill(BmpWin_GetBitmap(win->window), win->color);
    BmpWin_TransferNow(win->window);
}

static void func_ov102_021b7d38(IrcCompatMenuWindow *win, u8 bg, u8 x, u8 y, u8 width, u8 height,
                                              u8 palette, u16 color, u8 fromEnd, HeapID heapId) {
    MI_CpuFill8(win, 0, sizeof(IrcCompatMenuWindow));
    win->color = color;
    win->window = BmpWin_CreateDynamic(bg, x, y, width, height, palette, fromEnd);
    win->strbuf = GFL_StrBufCreate(255, heapId);
    PrintWindow_Init(&win->printWindow, win->window);
    GFL_BitmapFill(BmpWin_GetBitmap(win->window), win->color);
    BmpWin_TransferNow(win->window);
}

// pokebw2's IrcCompatMenuWindow_Free
static void func_ov102_021b7da8(IrcCompatMenuWindow *win) {
    GFL_StrBufFree(win->strbuf);
    BmpWin_Free(win->window);
    MI_CpuFill8(win, 0, sizeof(IrcCompatMenuWindow));
}

// pokebw2's IrcCompatMenuWindow_Flush
static BOOL func_ov102_021b7dc4(IrcCompatMenuWindow *win, IrcCompatMenuText *text) {
    PrintWindow_Flush(&win->printWindow, text->queue);
    return PrintWindow_IsPrinted(&win->printWindow);
}

// pokebw2's IrcCompatMenuWindow_PrintMsg
static void func_ov102_021b7dfc(IrcCompatMenuWindow *win, IrcCompatMenuText *text, u32 msgId, int x, s16 y) {
    MsgData *msgData = func_ov102_021b7cc4(text);
    PrintQueue *queue = func_ov102_021b7cc0(text);
    Font *font = func_ov102_021b7cbc(text);

    GFL_BitmapFill(BmpWin_GetBitmap(win->window), win->color);
    GFL_MsgDataLoadStrbuf(msgData, msgId, win->strbuf);
    PrintWindow_PrintNoColor(&win->printWindow, queue, x, y, win->strbuf, font);
}

void func_ov102_021b7e60(IrcCompatMenuWindow *win, IrcCompatMenuText *text, u32 msgId) {
    MsgData *msgData = func_ov102_021b7cc4(text);
    PrintQueue *queue = func_ov102_021b7cc0(text);
    Font *font = func_ov102_021b7cbc(text);
    u16 x;
    u16 y;

    GFL_BitmapFill(BmpWin_GetBitmap(win->window), win->color);
    GFL_MsgDataLoadStrbuf(msgData, msgId, win->strbuf);
    x = func_02045738(win->window) * 4;
    y = func_02045740(win->window) * 4;
    x -= GFL_FontGetBlockWidth(win->strbuf, font, 0) / 2;
    y -= GFL_FontGetBlockHeight(win->strbuf, font) / 2;
    func_0201c6ec(queue);
    func_0201c724(queue, BmpWin_GetBitmap(win->printWindow.window), x, y, win->strbuf, font, PRINT_COLOR(15, 14, 1));
    win->printWindow.flushPending = TRUE;
}


// pokebw2's IrcCompatMenu_SetStep
static void func_ov102_021b7f08(IrcCompatMenuWork *wk, IrcCompatMenuStepFunc step) {
    wk->step = step;
    wk->stepState = 0;
}

// pokebw2's IrcCompatMenu_SetStepDone
void func_ov102_021b7f18(IrcCompatMenuWork *wk) {
    wk->stepDone = TRUE;
}

void func_ov102_021b7f24(IrcCompatMenuWork *wk, u16 *state) {
    if (*state < CONNECT_WAIT_CONNECTED && func_ov101_021b7250(wk->button) == 1) {
        func_ov101_021b63bc(wk->param->sys);
        func_ov102_021b7f08(wk, func_ov102_021b8320);
        func_ov101_021b7264(wk->button);
        func_ov101_021b72a8(wk->button, IRC_APPBAR_LABEL_CANCEL);
        GFL_BGSysLoadScr(0);
        func_ov102_021b7dfc(&wk->messageWindow, &wk->text, 5, 0, 0);
        func_ov102_021b8724(&wk->buttons);
        return;
    }
    switch (*state) {
    case CONNECT_RESET:
        func_ov101_021b63bc(wk->param->sys);
        *state = CONNECT_START_NET;
        break;
    case CONNECT_START_NET:
        if (func_ov101_021b6150(wk->param->sys)) {
            GFL_SndSEPlay(SEQ_SE_SYS_21);
            func_ov101_021b72a0(wk->button, TRUE);
            *state = CONNECT_START_NEGOTIATION;
        }
        break;
    case CONNECT_START_NEGOTIATION:
        if (func_ov101_021b6254(wk->param->sys)) {
            *state = CONNECT_PRINT_CHECKING;
        }
        break;
    case CONNECT_PRINT_CHECKING:
        func_ov102_021b7dfc(&wk->messageWindow, &wk->text, 4, 0, 0);
        *state = CONNECT_SYNC;
        break;
    case CONNECT_SYNC:
        if (func_ov101_021b62ac(wk->param->sys)) {
            *state = CONNECT_EXCHANGE_PROFILES;
        }
        break;
    case CONNECT_EXCHANGE_PROFILES:
        if (func_ov101_021b6568(wk->param->sys, wk->param->gsys)) {
            func_ov101_021b657c(wk->param->sys, wk->param->profile);
            if (wk->param->gsys != NULL) {
                RivalDataSave *rivalData = getHollow_RivalData(GameData_GetSaveControl(GSYS_GetGameData(wk->param->gsys)));
                func_0200ed54(rivalData, getIDAsUInt(&wk->param->profile->trainer));
            }
            *state = CONNECT_EXCHANGE_IDS;
        }
        break;
    case CONNECT_EXCHANGE_IDS:
        if (func_ov101_021b630c(wk->param->sys, 0)) {
            *state = CONNECT_SEND_SELECTION;
        }
        break;
    case CONNECT_SEND_SELECTION:
        wk->choiceSent = TRUE;
        if (func_ov101_021b6518(wk->param->sys, wk->choiceTime, wk->choice)) {
            *state = CONNECT_POLL_SELECTION;
        }
        break;
    case CONNECT_POLL_SELECTION:
        if (func_ov101_021b652c(wk->param->sys)) {
            *state = CONNECT_SEND_PING;
        }
        break;
    case CONNECT_SEND_PING:
        if (func_ov101_021b6540(wk->param->sys)) {
            *state = CONNECT_POLL_PING;
        }
        break;
    case CONNECT_POLL_PING:
        if (func_ov101_021b6554(wk->param->sys)) {
            *state = CONNECT_SEND_COUNT;
        }
        break;
    case CONNECT_SEND_COUNT:
        func_ov101_021b63cc(wk->param->sys, 1);
        if (func_ov101_021b6424(wk->param->sys)) {
            *state = CONNECT_EXCHANGE_IDS_LAST;
        }
        break;
    case CONNECT_EXCHANGE_IDS_LAST:
        if (func_ov101_021b630c(wk->param->sys, 1)) {
            func_ov101_021b72a0(wk->button, FALSE);
            *state = CONNECT_WAIT_CONNECTED;
        }
        break;
    case CONNECT_WAIT_CONNECTED:
        if (wk->timer >= CONNECTED_FRAMES) {
            GFL_SndSEPlay(SEQ_SE_SYS_12);
            func_ov102_021b7dfc(&wk->messageWindow, &wk->text, 2, 0, 0);
            *state = CONNECT_WAIT_FINISH;
        }
        break;
    case CONNECT_WAIT_FINISH:
        if (wk->counter++ >= FINISH_FRAMES) {
            wk->param->result = IRC_COMPAT_MENU_RESULT_CHECK;
            func_ov102_021b7f08(wk, func_ov102_021b82dc);
        }
        return;
    }
    if (*state >= CONNECT_PRINT_CHECKING && *state <= CONNECT_WAIT_CONNECTED && wk->timer <= CONNECTED_FRAMES) {
        wk->timer++;
    }
    if (*state < CONNECT_SEND_COUNT && func_ov101_021b6480(wk->param->sys)) {
        func_ov102_021b7f08(wk, func_ov102_021b83e0);
    }
}


void func_ov102_021b81b0(IrcCompatMenuWork *wk, u16 *state) {
    switch (*state) {
    case 0:
        wk->choice = 2;
        wk->choiceSent = FALSE;
        wk->startTime = OS_TicksToMilliSeconds((u32)OS_GetTick());
        *state = 1;
        break;
    case 1:
        func_ov102_021b8624(&wk->buttons);
        if (func_ov102_021b86fc(&wk->buttons)) {
            func_ov101_021b72a0(wk->button, FALSE);
        }
        if (func_ov102_021b870c(&wk->buttons, &wk->choice)) {
            switch (wk->choice) {
            case 0:
                *state = 2;
                break;
            case 1:
                wk->param->result = IRC_COMPAT_MENU_RESULT_RANKING;
                func_ov102_021b7f08(wk, func_ov102_021b82dc);
                break;
            }
        }
        wk->choiceTime = OS_TicksToMilliSeconds((u32)OS_GetTick()) - wk->startTime;
        if (func_ov101_021b7254(wk->button)) {
            func_ov102_021b86f8(&wk->buttons, FALSE);
        }
        if (func_ov101_021b7250(wk->button) == 1) {
            func_ov101_021b63bc(wk->param->sys);
            func_ov102_021b7f08(wk, func_ov102_021b838c);
            return;
        }
        break;
    case 2:
        func_ov102_021b7dfc(&wk->messageWindow, &wk->text, 3, 0, 0);
        func_ov101_021b72a8(wk->button, IRC_APPBAR_LABEL_BACK);
        func_ov101_021b72a0(wk->button, FALSE);
        func_ov102_021b7f08(wk, func_ov102_021b7f24);
        break;
    }
}


// Waits a moment, and then ends the screen
// pokebw2's IrcCompatMenu_StepFinish
void func_ov102_021b82dc(IrcCompatMenuWork *wk, u16 *state) {
    switch (*state) {
    case 0:
        if (wk->param->result == IRC_COMPAT_MENU_RESULT_RANKING) {
            if (wk->timer++ > 10) {
                wk->timer = 0;
                *state = 1;
            }
        } else {
            *state = 1;
        }
        break;
    case 1:
        func_ov102_021b7f18(wk);
        break;
    }
}

void func_ov102_021b8320(IrcCompatMenuWork *wk, u16 *state) {
    switch (*state) {
    case 0:
        func_ov102_021b7dfc(&wk->messageWindow, &wk->text, 5, 0, 0);
        func_ov101_021b63c4(wk->param->sys);
        func_ov101_021b63bc(wk->param->sys);
        *state = 1;
        break;
    case 1:
        if (func_ov101_021b61e8(wk->param->sys)) {
            *state = 2;
        }
        break;
    case 2:
        func_ov102_021b7f08(wk, func_ov102_021b81b0);
        break;
    }
}


void func_ov102_021b838c(IrcCompatMenuWork *wk, u16 *state) {
    switch (*state) {
    case 0:
        *state = 1;
        func_ov101_021b63c4(wk->param->sys);
        func_ov101_021b63bc(wk->param->sys);
        break;
    case 1:
        if (func_ov101_021b61e8(wk->param->sys)) {
            *state = 2;
        }
        break;
    case 2:
        wk->param->result = IRC_COMPAT_MENU_RESULT_QUIT;
        func_ov102_021b7f18(wk);
        break;
    }
}


// The other player started first: ends the screen if the link is made, and goes back to the menu if not
// pokebw2's IrcCompatMenu_StepPeerStarted
void func_ov102_021b83e0(IrcCompatMenuWork *wk, u16 *state) {
    func_ov101_021b6480(wk->param->sys);
    if (func_ov101_021b62fc(wk->param->sys)) {
        func_ov101_021b63cc(wk->param->sys, 1);
        wk->param->result = IRC_COMPAT_MENU_RESULT_CHECK;
        func_ov102_021b7f08(wk, func_ov102_021b82dc);
    } else {
        func_ov102_021b7f08(wk, func_ov102_021b81b0);
    }
}

// pokebw2's IrcCompatMenu_UpdateAll
static void func_ov102_021b842c(IrcCompatMenuWork *wk) {
    func_ov102_021b7cb0(&wk->text);
    func_ov102_021b7dc4(&wk->messageWindow, &wk->text);
    func_ov102_021b7dc4(&wk->titleWindow, &wk->text);
    func_ov101_021b71e4(wk->button);
    func_ov102_021b78e4(&wk->graphic);
}

static void func_ov102_021b8464(IrcCompatMenuButtons *buttons, u8 bg, const IrcCompatMenuButtonDef *defs,
                                      u16 count, IrcCompatMenuText *text, HeapID heapId) {
    int i;

    MI_CpuFill8(buttons, 0, sizeof(IrcCompatMenuButtons));
    buttons->count = count;
    buttons->defs = defs;
    buttons->bg = bg;
    buttons->enabled = TRUE;
    for (i = 0; i < buttons->count; i++) {
        const IrcCompatMenuButtonDef *def = &defs[i];

        buttons->rects[i].top = def->y * 8 + 1;
        buttons->rects[i].bottom = (def->y + def->height) * 8 - 1;
        buttons->rects[i].left = def->x * 8 + 1;
        buttons->rects[i].right = (def->x + def->width) * 8 - 1;
    }
    buttons->rects[i].top = TOUCH_RECT_END;
    buttons->manager = func_02051b84(buttons->rects, func_ov102_021b8794, buttons, heapId);
    for (i = 0; i < buttons->count; i++) {
        const IrcCompatMenuButtonDef *def = &defs[i];
        StrBuf *label;
        u32 width;
        u32 height;
        GFLBitmap *bitmap;

        buttons->windows[i] = BmpWin_CreateDynamic(bg, def->x + 4, def->y + 1, def->width - 8, def->height - 2,
                                                   def->palette, TRUE);
        GFL_BitmapFill(BmpWin_GetBitmap(buttons->windows[i]), 4);
        BmpWin_TransferNow(buttons->windows[i]);
        label = GFL_MsgDataLoadStrbufNew(func_ov102_021b7cc4(text), def->msgId);
        GFL_TextRndUpdateColorIndexLUT(15, 14, 4);
        width = GFL_FontGetBlockWidth(label, func_ov102_021b7cbc(text), 0);
        height = GFL_FontGetBlockHeight(label, func_ov102_021b7cbc(text));
        bitmap = BmpWin_GetBitmap(buttons->windows[i]);
        GFL_TextRendererDrawToBitmap(bitmap, (def->width - 8) * 4 - width / 2, (def->height - 2) * 4 - height / 2,
                                     label, func_ov102_021b7cbc(text));
        GFL_StrBufFree(label);
        BmpWin_FlushChar(buttons->windows[i]);
    }
    func_0201dd68();
}

// pokebw2's IrcCompatMenuButtons_Free
static void func_ov102_021b85f4(IrcCompatMenuButtons *buttons) {
    int i;

    for (i = 0; i < buttons->count; i++) {
        BmpWin_Free(buttons->windows[i]);
    }
    func_02051c6c(buttons->manager);
    MI_CpuFill8(buttons, 0, sizeof(IrcCompatMenuButtons));
}

static inline u8 IrcCompatMenuButtons_GetBG(const IrcCompatMenuButtons *buttons, u32 index) {
    return BmpWin_GetBGIndex(buttons->windows[index]);
}

// Runs the buttons: a touched button flashes for a few frames, and then counts as pressed
// pokebw2's IrcCompatMenuButtons_Update
void func_ov102_021b8624(IrcCompatMenuButtons *buttons) {
    switch (buttons->state) {
    case 0:
        buttons->pressed = 0;
        if (func_02051c88(buttons->manager) && buttons->enabled) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            buttons->frame = 0;
            buttons->state = 1;
        }
        break;
    case 1: {
        u8 flash = (buttons->frame / 4) % 2;
        u8 bg = IrcCompatMenuButtons_GetBG(buttons, buttons->touched);
        u8 x = buttons->defs[buttons->touched].x;
        u8 y = buttons->defs[buttons->touched].y;
        u8 width = buttons->defs[buttons->touched].width;
        u8 height = buttons->defs[buttons->touched].height;
        u8 normalPalette;
        u8 flashPalette;

        switch (buttons->touched) {
        case 0:
            flashPalette = 12;
            normalPalette = 11;
            break;
        case 1:
            flashPalette = 9;
            normalPalette = 4;
            break;
        }
        if (flash == 0) {
            GFL_BGSysSetScrPaletteNo(bg, x, y, width, height, normalPalette);
            GFL_BGSysLoadScr(bg);
        } else {
            GFL_BGSysSetScrPaletteNo(bg, x, y, width, height, flashPalette);
            GFL_BGSysLoadScr(bg);
        }
        buttons->frame++;
        if (buttons->frame >= 16) {
            buttons->pressed = 1;
            buttons->state = 0;
        }
        break;
    }
    }
}

// pokebw2's IrcCompatMenuButtons_SetEnabled
void func_ov102_021b86f8(IrcCompatMenuButtons *buttons, BOOL enabled) {
    buttons->enabled = enabled;
}

// pokebw2's IrcCompatMenuButtons_IsTouched
BOOL func_ov102_021b86fc(IrcCompatMenuButtons *buttons) {
    if (buttons->state != 0) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's IrcCompatMenuButtons_GetResult
BOOL func_ov102_021b870c(IrcCompatMenuButtons *buttons, int *result) {
    if (buttons->pressed != 0) {
        if (result != NULL) {
            *result = buttons->touched;
        }
        return TRUE;
    }
    return FALSE;
}

// pokebw2's IrcCompatMenuButtons_Reset
void func_ov102_021b8724(IrcCompatMenuButtons *buttons) {
    int i;

    buttons->state = 0;
    buttons->pressed = 0;
    for (i = 0; i < buttons->count; i++) {
        u8 bg = BmpWin_GetBGIndex(buttons->windows[i]);
        const IrcCompatMenuButtonDef *def = &buttons->defs[i];
        u8 x = def->x;
        u8 y = def->y;
        u8 width = def->width;
        u8 height = def->height;
        u8 palette;

        switch (i) {
        case 0:
            palette = 11;
            break;
        case 1:
            palette = 4;
            break;
        }
        GFL_BGSysSetScrPaletteNo(bg, x, y, width, height, palette);
        GFL_BGSysLoadScr(bg);
    }
}

// pokebw2's IrcCompatMenuButtons_Callback
static void func_ov102_021b8794(u32 button, u32 event, void *work) {
    IrcCompatMenuButtons *buttons = work;

    if (event == BMN_EVENT_TOUCH) {
        buttons->touched = button;
    }
}

// pokebw2's IrcCompatMenuScroll_Init
static void func_ov102_021b879c(IrcCompatMenuScroll *scroll, HeapID heapId) {
    MI_CpuFill8(scroll, 0, sizeof(IrcCompatMenuScroll));
    GFL_BGSysMoveBGReq(4, BG_MOVE_SET_Y, SCROLL_DISTANCE);
    GFL_BGSysMoveBGReq(7, BG_MOVE_SET_Y, SCROLL_DISTANCE);
}

// pokebw2's IrcCompatMenuScroll_Free
static void func_ov102_021b87bc(IrcCompatMenuScroll *scroll) {
    MI_CpuFill8(scroll, 0, sizeof(IrcCompatMenuScroll));
}

// pokebw2's IrcCompatMenuScroll_Update
static void func_ov102_021b87c8(IrcCompatMenuScroll *scroll) {
    if (scroll->active) {
        if (func_ov102_021b8a90(&scroll->ease)) {
            scroll->active = FALSE;
        }
        GFL_BGSysMoveBGReq(4, BG_MOVE_SET_Y, SCROLL_DISTANCE - scroll->ease.current);
        GFL_BGSysMoveBGReq(7, BG_MOVE_SET_Y, SCROLL_DISTANCE - scroll->ease.current);
    }
}

// pokebw2's IrcCompatMenuScroll_Start
static void func_ov102_021b87fc(IrcCompatMenuScroll *scroll) {
    scroll->active = TRUE;
    func_ov102_021b8a40(&scroll->ease, 0, SCROLL_DISTANCE, SCROLL_FRAMES);
}

// pokebw2's IrcCompatMenuHeart_Init
static void func_ov102_021b8810(IrcCompatMenuHeart *heart, ClActor *actor, BOOL isSpecial, HeapID heapId) {
    ClActorPos pos;
    u16 sequence;

    MI_CpuFill8(heart, 0, sizeof(IrcCompatMenuHeart));
    heart->actor = actor;
    if (isSpecial) {
        sequence = 13;
    } else {
        sequence = 12;
    }
    func_0204b878(heart->actor, sequence);
    func_ov102_021b88f8(heart);
    pos.x = heart->startX;
    pos.y = heart->startY;
    func_0204b404(heart->actor, &pos, TRUE);
    func_0204b3dc(heart->actor, TRUE);
    func_0204b8c4(heart->actor, GFL_RandomLCAlt(1 << 13));
}

// pokebw2's IrcCompatMenuHeart_Free
static void func_ov102_021b8868(IrcCompatMenuHeart *heart) {
    MI_CpuFill8(heart, 0, sizeof(IrcCompatMenuHeart));
}

// pokebw2's IrcCompatMenuHeart_Update
static void func_ov102_021b8874(IrcCompatMenuHeart *heart) {
    ClActorPos pos;
    int diff = heart->x - heart->startX;
    s8 sign = diff / (diff < 0 ? -diff : diff);

    diff = diff < 0 ? -diff : diff;
    pos.x = heart->startX + sign * (diff * heart->frame / heart->duration);
    diff = heart->y - heart->startY;
    sign = diff / (diff < 0 ? -diff : diff);
    diff = diff < 0 ? -diff : diff;
    pos.y = heart->startY + sign * (diff * heart->frame / heart->duration);
    func_0204b404(heart->actor, &pos, TRUE);
    if (heart->frame++ > heart->duration) {
        func_ov102_021b88f8(heart);
    }
}

// pokebw2's IrcCompatMenuHeart_Reset
static void func_ov102_021b88f8(IrcCompatMenuHeart *heart) {
    heart->startX = GFL_RandomLCAlt(256);
    heart->startY = 208;
    heart->x = heart->startX + GFL_RandomLCAlt(184) - 92;
    heart->y = 184 - 200;
    heart->frame = 0;
    heart->duration = GFL_RandomLCAlt(184 + 176) + 240;
}

static void func_ov102_021b8934(IrcCompatMenuHearts *hearts, IrcCompatMenuGraphic *graphic, TrainerLogSave *log,
                                     HeapID heapId) {
    int i;

    MI_CpuFill8(hearts, 0, sizeof(IrcCompatMenuHearts));
    if (log == NULL) {
        hearts->count = HEART_COUNT;
    } else {
        hearts->count = func_0200a8ac(log);
    }
    for (i = 0; i < hearts->count; i++) {
        ClActor *actor = func_ov102_021b78ec(graphic, i);
        BOOL isSpecial;

        if (log == NULL) {
            isSpecial = GFL_RandomLCAlt(2);
        } else {
            isSpecial = func_0200a8f8(log, i) == 1 ? TRUE : FALSE;
        }
        func_ov102_021b8810(&hearts->hearts[i], actor, isSpecial, heapId);
    }
}

// pokebw2's IrcCompatMenuHearts_Free
static void func_ov102_021b89b8(IrcCompatMenuHearts *hearts) {
    int i;

    for (i = 0; i < hearts->count; i++) {
        func_ov102_021b8868(&hearts->hearts[i]);
    }
    MI_CpuFill8(hearts, 0, sizeof(IrcCompatMenuHearts));
}

// Sets one more heart going every 30 frames
// pokebw2's IrcCompatMenuHearts_Update
static void func_ov102_021b89f0(IrcCompatMenuHearts *hearts) {
    int i;

    for (i = 0; i < hearts->activeCount; i++) {
        func_ov102_021b8874(&hearts->hearts[i]);
    }
    if (hearts->timer++ > 30) {
        hearts->timer = 0;
        if (hearts->activeCount < hearts->count) {
            hearts->activeCount++;
        }
    }
}

// pokebw2's IrcCompatMenuEase_Start
static void func_ov102_021b8a40(IrcCompatMenuEase *ease, int from, int to, int frames) {
    int diff;

    ease->current = from;
    ease->start = from;
    ease->end = to;
    ease->duration = frames;
    if (frames != 0) {
        diff = to - from;
        ease->step = FX32_CONST(diff) / frames;
        ease->frame = 0;
    } else {
        ease->frame = frames - 2;
    }
}

// pokebw2's IrcCompatMenuEase_Update
static BOOL func_ov102_021b8a90(IrcCompatMenuEase *ease) {
    if (ease->frame < ease->duration - 1) {
        ease->frame++;
        ease->current = ease->start + FX_Whole(ease->step * ease->frame);
        return FALSE;
    }
    ease->current = ease->end;
    return TRUE;
}
