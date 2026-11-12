// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov198/comm_tvt_field.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/comm_tvt_field.h"
#include "constants/arc.h"
#include "constants/sound.h"
#include "field/field_sound.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/button_man.h"
#include "gfl/clact.h"
#include "gfl/graphics.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/net.h"
#include "gfl/net_whpipe.h"
#include "gfl/proc.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "gfl/tcbl.h"
#include "gfl/touchpanel.h"
#include "gfl/ui.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "nitro/wm.h"
#include "nnsys/g2d.h"
#include "save/wifi_list.h"
#include "system/app_taskmenu.h"
#include "system/bmp_winframe.h"
#include "system/ctvt_beacon.h"
#include "system/game_beacon.h"
#include "system/game_comm.h"
#include "system/game_data.h"
#include "system/game_system.h"
#include "system/gf_font.h"
#include "system/printsys.h"
#include "system/wipe.h"
#include "system/wordset.h"
#include "field/intrude_work.h"

// The Xtransceiver's screen in the field: the player picks to call a friend, to answer one that calls or to quit, and
// the screen leaves to the video chat of overlay 257. The ROM has no string for the file, so the name is a guess.
// Function names are ours

#define WINDOW_COUNT 11
#define CALLER_COUNT 16
#define ACTOR_COUNT 6
#define PALETTE_COUNT 7
#define TEXT_COLOR 0x39e3

// The frames a call lasts without a new beacon of its caller
#define CALLER_FRAMES 600

// The heap of the screen
#define HEAPID_COMM_TVT_FIELD 0x1f

// Where a window goes: its position and size in tiles
typedef struct {
    u32 x;
    u32 y;
    u32 width;
    u32 height;
} CommTvtFieldWindowParam;

// A call in the neighbourhood, from the beacon of its caller
typedef struct {
    u8 mac[6];
    u8 unk6[2];
    // The frames until the call is forgotten
    u16 timer;
    // How many children the caller's beacon says it has
    u16 count;
} CommTvtFieldCaller;

typedef struct CommTvtFieldWork CommTvtFieldWork;
typedef void (*CommTvtFieldFunc)(CommTvtFieldWork *wk);
typedef BOOL (*CommTvtFieldButtonFunc)(u32 button, CommTvtFieldWork *wk);

struct CommTvtFieldWork {
    // The function that runs each frame, none to leave
    CommTvtFieldFunc func;
    CommTvtFieldButtonFunc buttonFunc;
    // What the screen leaves to do: 0 to wait, 1 to start the video chat
    int mode;
    HeapID heapId;
    BmpWin *windows[WINDOW_COUNT];
    BmpWin *nameWindow;
    ButtonMan *buttonMan;
    MsgData *msgData;
    WordSet *wordSet;
    Font *font;
    StrBuf *strbuf50;
    StrBuf *strbuf54;
    StrBuf *strbuf58;
    u32 frameChars;
    u32 backgroundChars;
    CommTvtFieldCaller callers[CALLER_COUNT];
    // The status of the calls that the screen shows, and the number of the caller it shows
    u32 shownState;
    u32 shownCaller;
    u16 palettes[PALETTE_COUNT][16];
    u16 adjustedPalettes[PALETTE_COUNT][16];
    u8 unk2EC[0x20];
    ClActUnit *unit;
    TCB *vblankTask;
    BmpWin *unk314;
    PrintStream *stream;
    TCBExManager *tcbManager;
    PrintQueue *queue;
    AppTaskMenu *taskMenu;
    StrBuf *unk328;
    u16 unk32C;
    int unk330;
    u8 unk334[0xc];
    AppTaskMenuRes *taskMenuRes;
    CommTvtFieldParam *param;
    int windowCount;
    u8 unk34C[4];
    GameData *gameData;
    GameSystem *gsys;
    int scroll;
    int selected;
    u8 unk360[4];
    u32 wifiState;
    u32 prevWifiState;
    u32 callCount;
    u32 prevCallCount;
    int unk374;
    u32 palette;
    u32 chars;
    u32 cellAnims;
    u8 unk384[8];
    BOOL leaving;
    ClActor *actors[ACTOR_COUNT];
};

static BOOL func_ov122_021d5370(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov122_021d5514(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov122_021d55fc(GameProc *proc, u32 *state, void *param, void *work);
static void func_ov122_021d4240(CommTvtFieldWork *wk, CommTvtFieldFunc func);
static void func_ov122_021d4244(CommTvtFieldWork *wk, CommTvtFieldFunc func, int line);
static void func_ov122_021d424c(CommTvtFieldWork *wk);
static void func_ov122_021d4384(int count, const u32 *msgIds, CommTvtFieldWork *wk,
                                       CommTvtFieldWindowParam *params, const int *centersX, const int *centersY);
static void func_ov122_021d44bc(u32 index, CommTvtFieldWork *wk, CommTvtFieldWindowParam *param,
                                      const int *centersX, const int *centersY);
static void func_ov122_021d4590(u32 index, CommTvtFieldWork *wk);
static void func_ov122_021d45bc(u32 index, CommTvtFieldWork *wk);
static void func_ov122_021d45e8(CommTvtFieldWork *wk);
static void func_ov122_021d4630(u32 button, u32 event, void *work);
static void func_ov122_021d4644(u16 (*palettes)[16], const u16 *offsets, int count);
static void func_ov122_021d46cc(CommTvtFieldWork *wk);
static void func_ov122_021d48d8(CommTvtFieldWork *wk);
static void func_ov122_021d4a64(CommTvtFieldWork *wk);
static void func_ov122_021d4ad4(CommTvtFieldWork *wk);
static void func_ov122_021d4b30(CommTvtFieldWork *wk);
static void func_ov122_021d4b74(CommTvtFieldWork *wk);
static void func_ov122_021d4bdc(CommTvtFieldWork *wk);
static void func_ov122_021d4bf0(CommTvtFieldWork *wk);
static void func_ov122_021d4bf4(CommTvtFieldWork *wk);
static BOOL func_ov122_021d4c50(u32 button, CommTvtFieldWork *wk);
static void func_ov122_021d4cd8(u32 index, u32 msgId, BOOL redraw, BOOL format, CommTvtFieldWork *wk);
static void func_ov122_021d4d98(CommTvtFieldWork *wk);
static void func_ov122_021d4fe4(CommTvtFieldWork *wk);
static void func_ov122_021d5004(CommTvtFieldWork *wk);
static void func_ov122_021d503c(CommTvtFieldWork *wk);
static void func_ov122_021d50b4(CommTvtFieldWork *wk, int index);
static void func_ov122_021d50dc(CommTvtFieldWork *wk);
static void func_ov122_021d5110(CommTvtFieldWork *wk);
static void func_ov122_021d5278(void *arg, const void *beacon, int gameCommandBase, u8 childCount);
static void func_ov122_021d5310(CommTvtFieldWork *wk);
static u32 func_ov122_021d533c(CommTvtFieldWork *wk);
static void func_ov122_021d5368(TCB *tcb, void *data);

// pokebw2's sTitleCenterX
static const int data_ov122_021d5710[1] = { 40 };

// pokebw2's sButtonCenterX
static const int data_ov122_021d5720[2] = { 40, 40 };

// pokebw2's sButtonCenterY
static const union { u8 raw[8]; int t[2]; } data_ov122_021d5718_raw = { {
#include "asset/comm_tvt_field_data_ov122_021d5718.inc"
} };
typedef char data_ov122_021d5718_fits[sizeof(data_ov122_021d5718_raw.t) <= sizeof(data_ov122_021d5718_raw.raw) ? 1 : -1];   // the ROM's bytes are its layout
#define data_ov122_021d5718 (data_ov122_021d5718_raw.t)

// pokebw2's sMessageWindow
static CommTvtFieldWindowParam data_ov122_021d5840 = {
#include "asset/comm_tvt_field_data_ov122_021d5840.inc"
};

// pokebw2's sNoticeWindow
static CommTvtFieldWindowParam data_ov122_021d5850 = {
#include "asset/comm_tvt_field_data_ov122_021d5850.inc"
};

// pokebw2's sCallWindow
static CommTvtFieldWindowParam data_ov122_021d5860 = {
#include "asset/comm_tvt_field_data_ov122_021d5860.inc"
};

// pokebw2's sCommTvtFieldVRAMConfig
static BGSysVRAMConfig data_ov122_021d58b0 = {
    GX_VRAM_BG_128_A,  GX_VRAM_BGEXTPLTT_NONE,  GX_VRAM_SUB_BG_128_C,        GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_128_B, GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_128_D,       GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_NONE,  GX_VRAM_TEXPLTT_NONE,    GX_OBJVRAMMODE_CHAR_1D_128K, GX_OBJVRAMMODE_CHAR_1D_128K,
};

// The colors of the palettes that the screen brightens
// pokebw2's sPaletteOffsets
static u16 data_ov122_021d5870[16] = {
#include "asset/comm_tvt_field_data_ov122_021d5870.inc"
};

// pokebw2's sButtonWindows
static CommTvtFieldWindowParam data_ov122_021d5890[2] = { { 5, 5, 10, 4 }, { 17, 5, 11, 4 } };

// The windows of the status texts, from index 4
// pokebw2's sStatusWindows
static CommTvtFieldWindowParam data_ov122_021d58e0[10] = {
#include "asset/comm_tvt_field_data_ov122_021d58e0.inc"
};

// pokebw2's CommTvtField_SetFunc
static void func_ov122_021d4240(CommTvtFieldWork *wk, CommTvtFieldFunc func) {
    wk->func = func;
}

// pokebw2's CommTvtField_SetFuncLine
static void func_ov122_021d4244(CommTvtFieldWork *wk, CommTvtFieldFunc func, int line) {
    func_ov122_021d4240(wk, func);
}

// pokebw2's CommTvtField_InitBG
static void func_ov122_021d424c(CommTvtFieldWork *wk) {
    {
        BGSetup setup = { 0,
                          0,
                          0x800,
                          0,
                          BGRES_256x256,
                          GX_BG_COLORMODE_16,
                          GX_BG_SCRBASE_0x6800,
                          GX_BG_CHARBASE_0x00000,
                          0x8000,
                          GX_BG_EXTPLTT_01,
                          0,
                          GX_BG_AREAOVER_XLU,
                          FALSE };

        GFL_BGSysCreateBG(0, &setup, BGMODE_TEXT);
        GFL_BGSysClearBG(0);
        GFL_BGSysLoadScr(0);
        GFL_BGSysSetBGPriority(0, 0);
        GFL_BGSysSetBGEnabled(0, TRUE);
    }
    {
        BGSetup setup = { 0,
                          0,
                          0x800,
                          0,
                          BGRES_256x256,
                          GX_BG_COLORMODE_16,
                          GX_BG_SCRBASE_0xf800,
                          GX_BG_CHARBASE_0x00000,
                          0x8000,
                          GX_BG_EXTPLTT_01,
                          3,
                          GX_BG_AREAOVER_XLU,
                          FALSE };

        GFL_BGSysCreateBG(4, &setup, BGMODE_TEXT);
        GFL_BGSysSetBGEnabled(4, TRUE);
        GFL_BGSysLoadScr(4);
    }
    {
        BGSetup setup = { 0,
                          0,
                          0x800,
                          0,
                          BGRES_256x256,
                          GX_BG_COLORMODE_16,
                          GX_BG_SCRBASE_0xf000,
                          GX_BG_CHARBASE_0x08000,
                          0x8000,
                          GX_BG_EXTPLTT_01,
                          0,
                          GX_BG_AREAOVER_XLU,
                          FALSE };

        GFL_BGSysCreateBG(5, &setup, BGMODE_TEXT);
        GFL_BGSysFillChar(5, 0, 1, 0);
        GFL_BGSysFillScrArea(5, 0, 0, 0, 32, 32, 17);
        GFL_BGSysLoadScr(5);
        GFL_BGSysSetBGEnabled(5, TRUE);
    }
    {
        BGSetup setup = { 0,
                          0,
                          0x800,
                          0,
                          BGRES_256x256,
                          GX_BG_COLORMODE_16,
                          GX_BG_SCRBASE_0xe000,
                          GX_BG_CHARBASE_0x00000,
                          0x8000,
                          GX_BG_EXTPLTT_01,
                          2,
                          GX_BG_AREAOVER_XLU,
                          FALSE };

        GFL_BGSysCreateBG(6, &setup, BGMODE_TEXT);
        GFL_BGSysSetBGEnabled(6, TRUE);
        GFL_BGSysLoadScr(6);
    }
    {
        BGSetup setup = { 0,
                          0,
                          0x800,
                          0,
                          BGRES_256x256,
                          GX_BG_COLORMODE_16,
                          GX_BG_SCRBASE_0xe800,
                          GX_BG_CHARBASE_0x08000,
                          0x8000,
                          GX_BG_EXTPLTT_01,
                          1,
                          GX_BG_AREAOVER_XLU,
                          FALSE };

        GFL_BGSysCreateBG(7, &setup, BGMODE_TEXT);
        GFL_BGSysSetBGEnabled(7, TRUE);
        GFL_BGSysLoadScr(7);
    }
}

static void func_ov122_021d4384(int count, const u32 *msgIds, CommTvtFieldWork *wk,
                                       CommTvtFieldWindowParam *params, const int *centersX,
                                       const int *centersY) {
    int i;
    int j;
    wk->windowCount = count;
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, PALTYPE_SUB_BG, 0x180, 0x20, wk->heapId);
    for (i = 0; i < WINDOW_COUNT; i++) {
        if (wk->windows[i] != NULL) {
            func_020456f8(wk->windows[i]);
            func_0201f7a8(wk->windows[i], 2);
            BmpWin_Free(wk->windows[i]);
        }
        wk->windows[i] = NULL;
    }
    for (j = 0; j < count; j++) {
        s32 x;
        s32 y;
        wk->windows[j] =
            BmpWin_CreateDynamic(7, params[j].x, params[j].y, params[j].width, params[j].height, 13, FALSE);
        GFL_BitmapFill(BmpWin_GetBitmap(wk->windows[j]), 0);
        GFL_MsgDataLoadStrbuf(wk->msgData, msgIds[j], wk->strbuf54);
        if (centersX != NULL) {
            x = centersX[j] - GFL_FontGetBlockWidth(wk->strbuf54, wk->font, 0) / 2;
        } else {
            x = 0;
        }
        if (centersY != NULL) {
            y = centersY[j] - GFL_FontGetBlockHeight(wk->strbuf54, wk->font) / 2;
        } else {
            y = 0;
        }
        GFL_TextRendererDrawToBitmapEx(BmpWin_GetBitmap(wk->windows[j]), x, y, wk->strbuf54, wk->font, TEXT_COLOR);
        BmpWin_FlushChar(wk->windows[j]);
        BmpWin_FlushMap(wk->windows[j]);
    }
    if (wk->buttonMan != NULL) {
        func_02051c6c(wk->buttonMan);
    }
    wk->buttonMan = NULL;
    GFL_BGSysQueueScrLoad(7);
}

static void func_ov122_021d44bc(u32 index, CommTvtFieldWork *wk, CommTvtFieldWindowParam *param,
                                      const int *centersX, const int *centersY) {
    s32 x;
    s32 y;

    if (wk->windows[index] != NULL) {
        func_020456f8(wk->windows[index]);
        func_0201f7a8(wk->windows[index], 2);
        BmpWin_Free(wk->windows[index]);
        wk->windows[index] = NULL;
    }
    wk->windows[index] = BmpWin_CreateDynamic(7, param->x, param->y, param->width, param->height, 13, FALSE);
    x = 0;
    GFL_BitmapFill(BmpWin_GetBitmap(wk->windows[index]), 0);
    if (centersX != NULL) {
        x = centersX[index] - GFL_FontGetBlockWidth(wk->strbuf54, wk->font, x) / 2;
    }
    if (centersY != NULL) {
        y = centersY[index] - GFL_FontGetBlockHeight(wk->strbuf54, wk->font) / 2;
    } else {
        y = 0;
    }
    GFL_TextRendererDrawToBitmapEx(BmpWin_GetBitmap(wk->windows[index]), x, y, wk->strbuf54, wk->font, TEXT_COLOR);
    BmpWin_FlushChar(wk->windows[index]);
    BmpWin_FlushMap(wk->windows[index]);
    GFL_BGSysQueueScrLoad(7);
}

// pokebw2's CommTvtField_FreeWindow5
static void func_ov122_021d4590(u32 index, CommTvtFieldWork *wk) {
    if (wk->windows[index] != NULL) {
        func_020456f8(wk->windows[index]);
        func_0201f7a8(wk->windows[index], 2);
        BmpWin_Free(wk->windows[index]);
    }
    wk->windows[index] = NULL;
    GFL_BGSysQueueScrLoad(5);
}

// pokebw2's CommTvtField_FreeWindow7
static void func_ov122_021d45bc(u32 index, CommTvtFieldWork *wk) {
    if (wk->windows[index] != NULL) {
        func_020456f8(wk->windows[index]);
        func_0201f7a8(wk->windows[index], 2);
        BmpWin_Free(wk->windows[index]);
    }
    wk->windows[index] = NULL;
    GFL_BGSysQueueScrLoad(7);
}

// pokebw2's CommTvtField_FreeWindows
static void func_ov122_021d45e8(CommTvtFieldWork *wk) {
    int i;

    if (wk->buttonMan != NULL) {
        func_02051c6c(wk->buttonMan);
    }
    wk->buttonMan = NULL;
    for (i = 0; i < WINDOW_COUNT; i++) {
        if (wk->windows[i] != NULL) {
            func_020456f8(wk->windows[i]);
            GFL_BGSysQueueScrLoad(5);
            func_0201f7a8(wk->windows[i], 2);
            BmpWin_Free(wk->windows[i]);
        }
        wk->windows[i] = NULL;
    }
    wk->windowCount = 0;
}

// pokebw2's CommTvtField_ButtonCallback
static void func_ov122_021d4630(u32 button, u32 event, void *work) {
    CommTvtFieldWork *wk = work;

    if (event == BMN_EVENT_TOUCH && wk->buttonFunc != NULL) {
        if (!wk->buttonFunc(button, wk)) {
            return;
        }
    }
}

// pokebw2's CommTvtField_AddPaletteOffsets
static void func_ov122_021d4644(u16 (*palettes)[16], const u16 *offsets, int count) {
    int i;
    int j;

    for (i = 0; i < count; i++) {
        for (j = 0; j < 16; j++) {
            u16 color = palettes[i][j];
            u16 r = color & GX_RGB_R_MASK;
            u16 g = (color & GX_RGB_G_MASK) >> GX_RGB_G_SHIFT;
            u16 b = (color & GX_RGB_B_MASK) >> GX_RGB_B_SHIFT;

            r += offsets[j];
            g += offsets[j];
            b += offsets[j];
            if (r > 31) {
                r = 31;
            }
            if (g > 31) {
                g = 31;
            }
            if (b > 31) {
                b = 31;
            }
            palettes[i][j] = r + (g << GX_RGB_G_SHIFT) + (b << GX_RGB_B_SHIFT);
        }
    }
}

// pokebw2's CommTvtField_InitActors
static void func_ov122_021d46cc(CommTvtFieldWork *wk) {
    u8 xs[3] = { 0x80, 0x80, 0xe0 };
    u8 ys[3] = { 0x60, 0x60, 0xb1 };
    u8 sequences[3] = { 12, 11, 0 };
    ClActorSetup setup;
    int i;

    for (i = 0; i < 3; i++) {
        setup.x = xs[i];
        setup.y = ys[i];
        setup.sequence = sequences[i];
        setup.priority = 1;
        setup.bgPriority = 2;
        wk->actors[i] =
            func_0204b294(wk->unit, wk->chars, wk->palette, wk->cellAnims, &setup, CLACT_SURFACE_SUB, wk->heapId);
        func_0204b92c(wk->actors[i], FALSE);
        func_0204b3dc(wk->actors[i], TRUE);
        func_0204b688(wk->actors[i], TRUE);
    }
    setup.x = 128;
    setup.y = 96;
    setup.sequence = 15;
    setup.priority = 0;
    setup.bgPriority = 2;
    wk->actors[3] =
        func_0204b294(wk->unit, wk->chars, wk->palette, wk->cellAnims, &setup, CLACT_SURFACE_SUB, wk->heapId);
    func_0204b92c(wk->actors[3], TRUE);
    func_0204b3dc(wk->actors[3], TRUE);
    func_0204b688(wk->actors[3], TRUE);
    setup.x = 128;
    setup.y = 96;
    setup.sequence = 18;
    setup.priority = 0;
    setup.bgPriority = 2;
    wk->actors[4] =
        func_0204b294(wk->unit, wk->chars, wk->palette, wk->cellAnims, &setup, CLACT_SURFACE_SUB, wk->heapId);
    func_0204b92c(wk->actors[4], TRUE);
    func_0204b3dc(wk->actors[4], FALSE);
    func_0204b688(wk->actors[4], TRUE);
    setup.x = 128;
    setup.y = 96;
    setup.sequence = 25;
    setup.priority = 0;
    setup.bgPriority = 2;
    wk->actors[5] =
        func_0204b294(wk->unit, wk->chars, wk->palette, wk->cellAnims, &setup, CLACT_SURFACE_SUB, wk->heapId);
    func_0204b92c(wk->actors[5], TRUE);
    func_0204b3dc(wk->actors[5], FALSE);
    func_0204b688(wk->actors[5], TRUE);
}

// pokebw2's CommTvtField_InitGraphics
static void func_ov122_021d48d8(CommTvtFieldWork *wk) {
    ArcTool *arc;
    NNSG2dPaletteData *palette;
    void *file;

    wk->strbuf54 = GFL_StrBufCreate(200, wk->heapId);
    wk->strbuf58 = GFL_StrBufCreate(200, wk->heapId);
    wk->strbuf50 = GFL_StrBufCreate(200, wk->heapId);
    wk->nameWindow = BmpWin_CreateDynamic(5, 16, 10, 10, 2, 13, FALSE);
    wk->font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, wk->heapId);
    wk->msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, 23, wk->heapId);
    arc = GFL_ArcSysCreateFileHandle(29, wk->heapId);
    GFL_G2DIOLoadArcNCLRDefault(arc, 0, PALTYPE_MAIN_BG, 0, 0, wk->heapId);
    GFL_G2DIOLoadArcNCLRDefault(arc, 1, PALTYPE_SUB_BG, 0, 0, wk->heapId);
    file = GFL_G2DIOReadNCLRArc(arc, 1, &palette, wk->heapId);
    MI_CpuCopy8(palette->pRawData, wk->palettes, sizeof(wk->palettes));
    MI_CpuCopy8(palette->pRawData, wk->adjustedPalettes, sizeof(wk->palettes));
    func_ov122_021d4644(wk->adjustedPalettes, data_ov122_021d5870, PALETTE_COUNT);
    GFL_HeapFree(file);
    wk->backgroundChars = GFL_BGSysLoadArcNCGRDynamic(arc, 4, 4, 0, FALSE, wk->heapId);
    GFL_G2DIOLoadNSCRSync(arc, 9, 4, 0, (u16)wk->backgroundChars, 0, FALSE, wk->heapId);
    GFL_G2DIOLoadNSCRSync(arc, 15, 6, 0, (u16)wk->backgroundChars, 0, FALSE, wk->heapId);
    wk->chars = func_0204a6c8(arc, 5, FALSE, CLACT_VRAM_SUB, wk->heapId);
    wk->palette = func_0204ac18(arc, 3, CLACT_VRAM_SUB, 0, 0, 10, wk->heapId);
    wk->cellAnims = func_0204af28(arc, 7, 16, wk->heapId);
    GFL_ArcToolFree(arc);
    wk->frameChars = LoadCursorImageEndOfHeap(5, 11, 0, wk->heapId);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, PALTYPE_SUB_BG, 0x180, 0x20, wk->heapId);
    func_ov122_021d46cc(wk);
}

// pokebw2's sTitleCenterY
static const int data_ov122_021d5714[1] = { 16 };

// pokebw2's sCommTvtFieldTouchRects
static const TouchRect data_ov122_021d5728[6] = {
#include "asset/comm_tvt_field_data_ov122_021d5728.inc"
};

// pokebw2's CommTvtField_InitUI
static void func_ov122_021d4a64(CommTvtFieldWork *wk) {
    u32 msgIds[2] = { 0, 1 };

    func_ov122_021d4384(2, msgIds, wk, data_ov122_021d5890, data_ov122_021d5720, data_ov122_021d5718);
    wk->buttonMan = func_02051b84(data_ov122_021d5728, func_ov122_021d4630, wk, wk->heapId);
    wk->buttonFunc = func_ov122_021d4c50;
    func_ov122_021d5004(wk);
    func_ov122_021d4244(wk, func_ov122_021d4fe4, 795);
}

// pokebw2's CommTvtField_FreeGraphics
static void func_ov122_021d4ad4(CommTvtFieldWork *wk) {
    func_0201dd68();
    func_ov122_021d45e8(wk);
    GFL_BGSysFreeFilledChar(5, 1, 0);
    GFL_BGSysFreeCharMemory(5, (u16)wk->frameChars, (u16)(wk->frameChars >> 16));
    GFL_BGSysReleaseBG(5);
    GFL_BGSysReleaseBG(4);
    GFL_MsgDataFree(wk->msgData);
    GFL_FontFree(wk->font);
    GFL_StrBufFree(wk->strbuf54);
    GFL_StrBufFree(wk->strbuf50);
    GFL_StrBufFree(wk->strbuf58);
    BmpWin_Free(wk->nameWindow);
}

// pokebw2's CommTvtField_WaitExitWipe
static void func_ov122_021d4b30(CommTvtFieldWork *wk) {
    if (GFL_WipeIsFinished()) {
        if (wk->mode == 1) {
            if (!GameCommSys_BootCheck(GSYS_GetGameCommSystem(wk->gsys))) {
                func_ov122_021d4244(wk, NULL, 833);
            }
        } else {
            func_ov122_021d4244(wk, NULL, 837);
        }
    }
}

// pokebw2's CommTvtField_StartExit
static void func_ov122_021d4b74(CommTvtFieldWork *wk) {
    wk->leaving = TRUE;
    GFL_WipeSet(WIPE_MODE_BOTH, WIPE_TYPE_FADE_OUT, WIPE_TYPE_FADE_OUT, 0, 6, 1, wk->heapId);
    func_ov014_02178118(NULL, NULL);
    if (wk->mode == 1) {
        if (GameCommSys_BootCheck(GSYS_GetGameCommSystem(wk->gsys))) {
            func_02026400(GSYS_GetGameCommSystem(wk->gsys));
        }
    }
    func_ov122_021d4244(wk, func_ov122_021d4b30, 862);
}

// pokebw2's CommTvtField_QueueStartExit
static void func_ov122_021d4bdc(CommTvtFieldWork *wk) {
    func_ov122_021d4244(wk, func_ov122_021d4b74, 879);
}

// pokebw2's CommTvtField_Idle
static void func_ov122_021d4bf0(CommTvtFieldWork *wk) {
}

// pokebw2's CommTvtField_StartPress
static void func_ov122_021d4bf4(CommTvtFieldWork *wk) {
    ClActorCallback callback;
    int index = wk->selected;

    callback.type = CLACT_CALLBACK_LAST_FRAME;
    callback.param = (uintptr_t)wk;
    callback.func = (ClActorAnimCallback)func_ov122_021d4bdc;
    if (index == 3) {
        index = 0;
    } else if (index == 4) {
        index = 1;
    }
    func_0204b92c(wk->actors[index], TRUE);
    func_0204ba00(wk->actors[index], &callback);
    func_0204b9a0(wk->actors[index]);
    func_ov122_021d4244(wk, func_ov122_021d4bf0, 912);
}

// pokebw2's CommTvtField_OnButton
static BOOL func_ov122_021d4c50(u32 button, CommTvtFieldWork *wk) {
    GameCommSys *comm = GSYS_GetGameCommSystem(wk->gsys);

    wk->selected = button;
    switch (button) {
    case 0:
    case 3:
        wk->param->exitArgument = func_ov010_02158630(comm);
        wk->mode = 0;
        GFL_SndSEPlay(SEQ_SE_SYS_69);
        func_ov122_021d4244(wk, func_ov122_021d4bf4, 964);
        break;
    case 1:
    case 4:
        wk->mode = 1;
        GFL_SndSEPlay(SEQ_SE_SYS_69);
        func_ov122_021d4244(wk, func_ov122_021d4bf4, 973);
        break;
    case 2:
        GFL_SndSEPlay(SEQ_SE_SYS_70);
        wk->mode = 2;
        func_ov122_021d4244(wk, func_ov122_021d4bf4, 979);
        break;
    }
    return TRUE;
}

// pokebw2's CommTvtField_DrawWindowText
static void func_ov122_021d4cd8(u32 index, u32 msgId, BOOL redraw, BOOL format, CommTvtFieldWork *wk) {
    if (wk->windows[index] != NULL && redraw == 0) {
        return;
    }
    if (wk->windows[index] == NULL) {
        wk->windows[index] = BmpWin_CreateDynamic(7, data_ov122_021d58e0[index].x, data_ov122_021d58e0[index].y,
                                                  data_ov122_021d58e0[index].width, data_ov122_021d58e0[index].height, 13, FALSE);
    }
    GFL_BitmapFill(BmpWin_GetBitmap(wk->windows[index]), 0);
    if (format != 0) {
        GFL_MsgDataLoadStrbuf(wk->msgData, msgId, wk->strbuf50);
        GFL_WordSetFormatStrbuf(wk->wordSet, wk->strbuf54, wk->strbuf50);
    } else {
        GFL_MsgDataLoadStrbuf(wk->msgData, msgId, wk->strbuf54);
    }
    GFL_TextRendererDrawToBitmapEx(BmpWin_GetBitmap(wk->windows[index]), 0, 0, wk->strbuf54, wk->font, TEXT_COLOR);
    BmpWin_FlushChar(wk->windows[index]);
    BmpWin_FlushMap(wk->windows[index]);
}

// pokebw2's CommTvtField_UpdateStatus
static void func_ov122_021d4d98(CommTvtFieldWork *wk) {
    BOOL changed = FALSE;
    int newIndex = 0;
    GameCommSys *comm = GSYS_GetGameCommSystem(wk->gsys);
    u32 state = func_ov010_02158630(comm);

    if (wk->unk374 == 0) {
        newIndex = 0;
        if (func_02026ca0(&newIndex) != 30) {
            wk->unk374 = 1;
        }
    }
    if (wk->callCount != wk->prevCallCount) {
        changed = TRUE;
    }
    if (wk->prevWifiState != wk->wifiState) {
        changed = TRUE;
    }
    func_ov122_021d4cd8(4, 2, FALSE, FALSE, wk);
    func_ov122_021d4cd8(6, 4, FALSE, FALSE, wk);
    func_ov122_021d4cd8(8, 12, FALSE, FALSE, wk);
    if (state == 0) {
        WordSetNumber(wk->wordSet, 0, wk->callCount, 2, 0, TRUE);
        func_ov122_021d4cd8(5, 3, changed, TRUE, wk);
        if (wk->wifiState & 0x20) {
            func_ov122_021d4cd8(7, 5, changed, TRUE, wk);
        } else {
            func_ov122_021d4cd8(7, 6, changed, TRUE, wk);
        }
        if (wk->unk374 != 0) {
            func_ov122_021d4cd8(9, 5, changed, TRUE, wk);
        } else {
            func_ov122_021d4cd8(9, 6, changed, TRUE, wk);
        }
    } else {
        func_ov122_021d4590(5, wk);
        func_ov122_021d4590(7, wk);
        func_ov122_021d4590(9, wk);
    }
    if (state == 2) {
        if (wk->shownCaller != func_ov010_02158678(comm)) {
            wk->shownState = 0;
        }
    }
    if (wk->shownState != state) {
        switch (state) {
        case 0:
            func_0204b3dc(wk->actors[5], FALSE);
            func_ov122_021d45bc(10, wk);
            GFL_MsgDataLoadStrbuf(wk->msgData, 0, wk->strbuf54);
            func_ov122_021d44bc(0, wk, &data_ov122_021d5840, data_ov122_021d5710, data_ov122_021d5714);
            break;
        case 1:
            func_0204b3dc(wk->actors[5], FALSE);
            func_ov122_021d45bc(10, wk);
            GFL_MsgDataLoadStrbuf(wk->msgData, 15, wk->strbuf54);
            func_ov122_021d44bc(0, wk, &data_ov122_021d5860, data_ov122_021d5710, data_ov122_021d5714);
            break;
        case 2: {
            u32 caller = func_ov010_02158678(comm);

            wk->shownCaller = caller;
            func_0204b3dc(wk->actors[5], FALSE);
            func_ov122_021d45bc(10, wk);
            WordSetNumber(wk->wordSet, 0, caller, 1, 0, TRUE);
            GFL_MsgDataLoadStrbuf(wk->msgData, 16, wk->strbuf50);
            GFL_WordSetFormatStrbuf(wk->wordSet, wk->strbuf54, wk->strbuf50);
            func_ov122_021d44bc(0, wk, &data_ov122_021d5860, data_ov122_021d5710, data_ov122_021d5714);
            break;
        }
        case 3:
            GFL_MsgDataLoadStrbuf(wk->msgData, 17, wk->strbuf54);
            func_ov122_021d44bc(0, wk, &data_ov122_021d5860, data_ov122_021d5710, data_ov122_021d5714);
            if (func_ov010_021586c8(comm, wk->strbuf54)) {
                func_ov122_021d44bc(10, wk, &data_ov122_021d5850, NULL, NULL);
                func_0204b3dc(wk->actors[5], TRUE);
            }
            break;
        }
        wk->shownState = state;
    }
    GFL_BGSysQueueScrLoad(7);
}

// pokebw2's CommTvtField_Run
static void func_ov122_021d4fe4(CommTvtFieldWork *wk) {
    if (GFL_WipeIsFinished()) {
        func_02051c88(wk->buttonMan);
    }
    func_ov122_021d503c(wk);
    func_ov122_021d4d98(wk);
}

// pokebw2's CommTvtField_InitMessage
static void func_ov122_021d5004(CommTvtFieldWork *wk) {
    wk->unk328 = GFL_StrBufCreate(100, wk->heapId);
    GFL_MsgDataLoadStrbuf(wk->msgData, 9, wk->unk328);
    wk->unk32C = TEXT_COLOR;
    wk->unk330 = 1;
    GFL_StrBufFree(wk->unk328);
}

// pokebw2's CommTvtField_AnimateIcon
static void func_ov122_021d503c(CommTvtFieldWork *wk) {
    switch (func_ov010_02158630(GSYS_GetGameCommSystem(wk->gsys))) {
    case 0:
    case 1:
        if (func_0204b898(wk->actors[0]) == 20) {
            func_0204b92c(wk->actors[0], FALSE);
            func_0204b878(wk->actors[0], 12);
        }
        break;
    case 2:
    case 3:
        if (func_0204b898(wk->actors[0]) == 12) {
            func_0204b92c(wk->actors[0], TRUE);
            func_0204b878(wk->actors[0], 20);
        }
        break;
    }
}

// pokebw2's CommTvtField_SetAnswerMode
static void func_ov122_021d50b4(CommTvtFieldWork *wk, int index) {
    u8 *mac;

    func_0203d798();
    mac = func_0203cf3c(index);
    wk->param->commParam.unk4 = 2;
    MI_CpuCopy8(mac, wk->param->commParam.parentMac, 6);
}

// pokebw2's CommTvtField_SetCallMode
static void func_ov122_021d50dc(CommTvtFieldWork *wk) {
    u8 mac[6] = { 0, 0, 0, 0, 0, 0 };

    func_0203d798();
    wk->param->commParam.unk4 = 1;
    MI_CpuCopy8(mac, wk->param->commParam.parentMac, 6);
}

// pokebw2's CommTvtField_UpdateCalls
static void func_ov122_021d5110(CommTvtFieldWork *wk) {
    int found = func_020107fc(func_0201283c(wk->gameData));

    if (found != -1) {
        void *beacon = func_0203cf10(found);
        StrBuf *name = GFL_StrBufCreate(200, wk->heapId);

        GFL_StrBufLoadFixedString(name, func_0202e930(beacon), 8);
        if (!func_0204b3f4(wk->actors[4])) {
            GFL_StrBufLoadFixedString(wk->strbuf58, func_0202e930(beacon), 8);
            GFL_BitmapFill(BmpWin_GetBitmap(wk->nameWindow), 0);
            GFL_TextRendererDrawToBitmapEx(BmpWin_GetBitmap(wk->nameWindow), 0, 0, wk->strbuf58, wk->font, TEXT_COLOR);
            BmpWin_FlushChar(wk->nameWindow);
            BmpWin_FlushMap(wk->nameWindow);
            func_0204b3dc(wk->actors[4], TRUE);
            GFL_BGSysQueueScrLoad(5);
            func_ov122_021d50b4(wk, found);
        }
        GFL_StrBufFree(name);
        func_02028a64(GameData_GetFieldSoundSystem(wk->gameData));
        if (func_0204b898(wk->actors[3]) == 15) {
            func_0204b878(wk->actors[3], 16);
        }
        if (func_0204b898(wk->actors[1]) == 11) {
            func_0204b92c(wk->actors[1], TRUE);
            func_0204b878(wk->actors[1], 19);
        }
    } else {
        func_02028a70(GameData_GetFieldSoundSystem(wk->gameData));
        func_0204b3dc(wk->actors[4], FALSE);
        GFL_BitmapFill(BmpWin_GetBitmap(wk->nameWindow), 0);
        BmpWin_FlushChar(wk->nameWindow);
        func_ov122_021d50dc(wk);
        if (func_0204b898(wk->actors[1]) == 19) {
            func_0204b92c(wk->actors[1], FALSE);
            func_0204b878(wk->actors[1], 11);
        }
        if (func_0204b898(wk->actors[3]) == 16) {
            func_0204b878(wk->actors[3], 15);
        }
    }
}

// pokebw2's CommTvtField_OnBeacon
static void func_ov122_021d5278(void *arg, const void *beacon, int gameCommandBase, u8 childCount) {
    CommTvtFieldWork *wk = arg;
    int i;
    int j;

    if (gameCommandBase >= 20 && gameCommandBase <= 27) {
        for (i = 0; i < CALLER_COUNT; i++) {
            if (wk->callers[i].timer != 0 && GFL_STD_MemCmp(wk->callers[i].mac, ((const WMBssDesc *)beacon)->bssid, 6) == 0) {
                wk->callers[i].timer = CALLER_FRAMES;
                wk->callers[i].count = childCount;
                return;
            }
        }
        for (j = 0; j < CALLER_COUNT; j++) {
            if (wk->callers[j].timer == 0) {
                MI_CpuCopy8(((const WMBssDesc *)beacon)->bssid, wk->callers[j].mac, 6);
                wk->callers[j].timer = CALLER_FRAMES;
                wk->callers[j].count = childCount;
                return;
            }
        }
    }
}

// pokebw2's CommTvtField_TickCallers
static void func_ov122_021d5310(CommTvtFieldWork *wk) {
    int i;

    for (i = 0; i < CALLER_COUNT; i++) {
        if (wk->callers[i].timer != 0) {
            wk->callers[i].timer--;
        }
    }
}

// pokebw2's CommTvtField_CountCalls
static u32 func_ov122_021d533c(CommTvtFieldWork *wk) {
    int i;
    u32 total = 0;

    for (i = 0; i < CALLER_COUNT; i++) {
        if (wk->callers[i].timer != 0) {
            total += wk->callers[i].count;
        }
    }
    return total;
}

// pokebw2's CommTvtField_VBlank
static void func_ov122_021d5368(TCB *tcb, void *data) {
    func_0204a648();
}

// pokebw2's sCommTvtFieldClActSetup
static const ClActSysSetup data_ov122_021d5740 = { 0, 0, 0, 512, 4, 124, 4, 124, 0, 100, 100, 100, 100, 16, 16 };

// pokebw2's COMM_TVT_FIELD_PROC_FUNCTIONS
const GameProcFunctions data_ov122_021d5764 = {
    func_ov122_021d5370,
    func_ov122_021d5514,
    func_ov122_021d55fc,
};

// pokebw2's CommTvtFieldApp_Init
static BOOL func_ov122_021d5370(GameProc *proc, u32 *state, void *param, void *work) {
    CommTvtFieldWork *wk;
    CommTvtFieldParam *fieldParam = param;

    GFL_HeapCreateChild(HEAPID_USER, HEAPID_COMM_TVT_FIELD, 0x18000);
    wk = GFL_ProcInitSubsystem(proc, sizeof(CommTvtFieldWork), HEAPID_COMM_TVT_FIELD);
    MI_CpuFill8(wk, 0, sizeof(CommTvtFieldWork));
    wk->heapId = HEAPID_COMM_TVT_FIELD;
    wk->gameData = fieldParam->gameData;
    wk->gsys = fieldParam->gsys;
    GFL_BGSysSetDisplayLayout(1);
    GXS_DispOn();
    GX_DispOn();
    GFL_BGSysCreate(wk->heapId);
    BmpWin_InitAllocator(wk->heapId);
    func_0201dd60();
    GFL_BGSysSetVRAMBanks(&data_ov122_021d58b0);
    {
        BGSysLCDConfig lcdConfig = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_2D };

        GFL_BGSysSetLCDConfig(&lcdConfig);
    }
    GFL_BGSysSetEnabledBGsA(0);
    GFL_BGSysSetEnabledBGsB(0);
    ClActSys_Create(&data_ov122_021d5740, &data_ov122_021d58b0, wk->heapId);
    wk->unit = func_0204b100(40, 0, wk->heapId);
    wk->vblankTask = GFL_VBlankTCBAdd(func_ov122_021d5368, wk, 0);
    func_ov122_021d424c(wk);
    func_ov122_021d48d8(wk);
    wk->wordSet = GFL_WordSetSystemCreate(11, 200, wk->heapId);
    wk->tcbManager = GFL_TCBExMgrCreate(wk->heapId, wk->heapId, 1, 0);
    wk->queue = func_0201c440(wk->heapId);
    wk->taskMenuRes = func_02027f60(5, 13, wk->font, wk->queue, wk->heapId);
    G2S_SetBlendAlpha(GX_BLEND_PLANEMASK_BG2, GX_BLEND_PLANEMASK_BG0, 15, 4);
    GFL_WipeSet(WIPE_MODE_SUB, WIPE_TYPE_FADE_IN, WIPE_TYPE_FADE_IN, 0, 6, 1, wk->heapId);
    GFL_BGSysSetEnabledBGsB(GX_PLANEMASK_ALL);
    func_ov122_021d4244(wk, func_ov122_021d4a64, 1739);
    wk->param = fieldParam;
    func_0203d798();
    wk->param->commParam.unk4 = 1;
    wk->param->commParam.gameData = wk->gameData;
    func_0203d34c(FALSE, wk->heapId);
    if (func_0203cd98()) {
        // The callback takes the command as an int; net_whpipe.h says u8
        func_ov014_02178118((void (*)(void *, const void *, u8, u8))func_ov122_021d5278, wk);
    }
    func_020355d8(TRUE);
    return TRUE;
}

// pokebw2's CommTvtFieldApp_Main
static BOOL func_ov122_021d5514(GameProc *proc, u32 *state, void *param, void *work) {
    CommTvtFieldWork *wk = work;
    BOOL result = TRUE;

    if (wk->func != NULL) {
        wk->func(wk);
        result = FALSE;
    }
    if (wk->taskMenu != NULL) {
        func_02027974(wk->taskMenu);
    }
    if (wk->mode != 1) {
        func_ov122_021d5110(wk);
    }
    func_ov122_021d5310(wk);
    func_0204a600();
    if (func_0203cd98()) {
        wk->prevWifiState = wk->wifiState;
        wk->wifiState = func_0201058c(func_0201283c(wk->gameData));
        wk->prevCallCount = wk->callCount;
        wk->callCount = func_ov122_021d533c(wk);
    }
    GFL_TCBExMgrUpdate(wk->tcbManager);
    func_0201c4e4(wk->queue);
    GFL_BGSysMoveBG(4, BG_MOVE_SET_Y, wk->scroll);
    wk->scroll--;
    if (GFL_WipeIsFinished() && !wk->leaving) {
        func_020121cc(wk->gsys);
    }
    if (GFL_WipeIsFinished() && func_02012204(wk->gsys)) {
        wk->mode = 2;
        result = TRUE;
        func_0202223c(0, 0);
        func_0202223c(1, 0);
    }
    return result;
}

// pokebw2's CommTvtFieldApp_Exit
static BOOL func_ov122_021d55fc(GameProc *proc, u32 *state, void *param, void *work) {
    CommTvtFieldWork *wk = work;
    CommTvtFieldParam *fieldParam = param;
    int i;

    if (!GFL_WipeIsFinished()) {
        return FALSE;
    }
    if (func_0203cd98()) {
        func_ov014_02178118(NULL, NULL);
    }
    func_ov122_021d4ad4(wk);
    fieldParam->mode = wk->mode;
    GFL_TCBExMgrFree(wk->tcbManager);
    GFL_BGSysReleaseBG(5);
    func_0201c6ec(wk->queue);
    func_0201c4c0(wk->queue);
    for (i = 0; i < 3; i++) {
        func_0204b3b4(wk->actors[i]);
    }
    func_0204b3b4(wk->actors[3]);
    func_0204b3b4(wk->actors[4]);
    func_0204b3b4(wk->actors[5]);
    func_0204ada4(wk->palette);
    func_0204a8d4(wk->chars);
    func_0204afd8(wk->cellAnims);
    GFL_TCBRemove(wk->vblankTask);
    func_0204b1cc(wk->unit);
    func_0204a5ac();
    if (wk->stream != NULL) {
        func_0201ce48(wk->stream);
    }
    GFL_WordSetSystemFree(wk->wordSet);
    if (wk->taskMenu != NULL) {
        func_02027858(wk->taskMenu);
        wk->taskMenu = NULL;
    }
    if (wk->unk314 != NULL) {
        BmpWin_Free(wk->unk314);
    }
    func_02027fd8(wk->taskMenuRes);
    BmpWin_FreeAllocator();
    GFL_BGSysFree();
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(HEAPID_COMM_TVT_FIELD);
    return TRUE;
}
