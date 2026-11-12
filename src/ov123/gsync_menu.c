// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov199/gsync_menu.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// The Game Sync menu: three touch buttons, Game Sync itself (whose button also shows the wireless signal), the Wi-Fi
// settings and return. The name is the ROM's string, from GFL_HeapAllocate's calls. Function names are ours.

#include "types.h"
#include "app/gsync.h"
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
#include "gfl/graphics.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/net.h"
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
#include "nitro/memorymap.h"
#include "nitro/os.h"
#include "save/save_control.h"
#include "system/app_keycursor.h"
#include "system/app_taskmenu.h"
#include "system/bmp_winframe.h"
#include "system/game_comm.h"
#include "system/game_data.h"
#include "system/game_system.h"
#include "system/gf_font.h"
#include "system/palanm.h"
#include "system/printsys.h"
#include "system/text_speed.h"
#include "system/time_icon.h"
#include "system/wipe.h"
#include "text/system/0044.h"
#include "text/system/wifi_error.h"
#include "field/beacon_view.h"

// The menu shares the IR battle menu's heap
#define HEAPID_GSYNC_MENU HEAPID_IRC_BATTLE_MENU

#define GSYNC_MENU_BUTTON_WIN_MAX 4
#define GSYNC_MENU_ACTOR_COUNT 3

// The touch buttons, and the actors drawn on them
enum {
    GSYNC_MENU_BUTTON_GSYNC,
    GSYNC_MENU_BUTTON_WIFI_SETTINGS,
    GSYNC_MENU_BUTTON_RETURN,
};

// The actors' first sequences. The Game Sync button's sequence also shows the wireless signal
#define GSYNC_MENU_SEQ_GSYNC 14
#define GSYNC_MENU_SEQ_WIFI_SETTINGS 13

typedef struct GSyncMenuWork GSyncMenuWork;
typedef void (*GSyncMenuStateFunc)(GSyncMenuWork *wk);
typedef BOOL (*GSyncMenuTouchFunc)(u32 button, GSyncMenuWork *wk);

// A window's rectangle, in tiles
typedef struct {
    u32 x;
    u32 y;
    u32 width;
    u32 height;
} GSyncMenuWinRect;

// Black 2's; twinned code confirms the offsets of state, touchFunc, result, heapId, buttonWins, buttonMan, msgData,
// dsiMsgData, font, strBuf, frameChars, msgWin, printStream, keyCursor, tcbMgr, printQueue, yesNoMenu, yesNoItems,
// menuRes, param, buttonCount, gameData, gsys, bgScroll, selected, wifiStatus, prevWifiStatus, objPalettes, bgPalettes,
// waitIcon, ending, palette, chars, cellAnims, actors, clUnit and vblankTask, and its size in Black
struct GSyncMenuWork {
    GSyncMenuStateFunc state;
    // What a touched button does
    GSyncMenuTouchFunc touchFunc;
    // The proc's result, a GSYNC_RESULT_*
    u32 result;
    u16 heapId;
    BmpWin *buttonWins[GSYNC_MENU_BUTTON_WIN_MAX];
    ButtonMan *buttonMan;
    MsgData *msgData;
    // The messages for the Wi-Fi settings on a DSi
    MsgData *dsiMsgData;
    u32 unk2C;
    Font *font;
    u32 unk34;
    StrBuf *strBuf;
    // The message window's frame, as LoadCursorImageEndOfHeap gives it
    u32 frameChars;
    // The sub screen BG's characters
    u32 bgChars;
    // The sub screen's first 7 palettes, as loaded and brightened
    u16 palettes[2][7][16];
    u8 unk204[0x20];
    BmpWin *msgWin;
    PrintStream *printStream;
    KeyCursor *keyCursor;
    TCBExManager *tcbMgr;
    PrintQueue *printQueue;
    AppTaskMenu *yesNoMenu;
    AppTaskMenuItem yesNoItems[2];
    AppTaskMenuRes *menuRes;
    EventGameSync *param;
    u32 buttonCount;
    GameData *gameData;
    GameSystem *gsys;
    // The scroll of the sub screen's background
    s32 bgScroll;
    u32 unk26C;
    // The button touched
    u32 selected;
    u32 unk274;
    // The wireless status, from func_0201058c, and the last frame's
    u32 wifiStatus;
    u32 prevWifiStatus;
    // The sub screen's palettes kept while they are dimmed
    u16 *objPalettes;
    u16 *bgPalettes;
    WaitIcon *waitIcon;
    // Set once the menu ends and no longer boots the game's communication
    BOOL ending;
    u32 palette;
    u32 chars;
    u32 cellAnims;
    u32 unk29C;
    ClActor *actors[GSYNC_MENU_ACTOR_COUNT];
    ClActUnit *clUnit;
    TCB *vblankTask;
};

static void func_ov123_021d4240(GSyncMenuWork *wk, GSyncMenuStateFunc state);
static void func_ov123_021d4244(GSyncMenuWork *wk, GSyncMenuStateFunc state, int line);
static void func_ov123_021d424c(GSyncMenuWork *wk);
static void func_ov123_021d4384(GSyncMenuWork *wk);
void func_ov123_021d43b8(u16 (*palettes)[16], const u16 *add, int count);
static void func_ov123_021d4440(GSyncMenuWork *wk);
static void func_ov123_021d449c(GSyncMenuWork *wk);
static void func_ov123_021d44f4(GSyncMenuWork *wk);
static void func_ov123_021d451c(uintptr_t param, fx32 frame);
static void func_ov123_021d45f0(GSyncMenuWork *wk);
static void func_ov123_021d45f4(int count, const u32 *msgIds, GSyncMenuWork *wk, GSyncMenuWinRect *rects,
                                    const s32 *centerX, const s32 *centerY);
static void func_ov123_021d4730(GSyncMenuWork *wk);
static void func_ov123_021d4778(u32 button, u32 event, void *work);
void func_ov123_021d478c(GSyncMenuWork *wk);
void func_ov123_021d4884(GSyncMenuWork *wk);
static void func_ov123_021d4a04(GSyncMenuWork *wk);
static void func_ov123_021d4a64(GSyncMenuWork *wk);
static void func_ov123_021d4ab4(GSyncMenuWork *wk);
static void func_ov123_021d4afc(GSyncMenuWork *wk);
static void func_ov123_021d4b44(GSyncMenuWork *wk);
static BOOL func_ov123_021d4ba8(u32 button, GSyncMenuWork *wk);
static void func_ov123_021d4c24(GSyncMenuWork *wk, int sequence);
static void func_ov123_021d4c4c(GSyncMenuWork *wk);
static void func_ov123_021d4ca0(GSyncMenuWork *wk);
static void func_ov123_021d4cbc(GSyncMenuWork *wk, u16 buffer, int count);
static void func_ov123_021d4d40(GSyncMenuWork *wk, BOOL dim);
static void func_ov123_021d4da4(GSyncMenuWork *wk);
static void func_ov123_021d4e34(GSyncMenuWork *wk);
static void func_ov123_021d4e64(GSyncMenuWork *wk);
static BOOL func_ov123_021d4e7c(GSyncMenuWork *wk);
static void func_ov123_021d4ef0(GSyncMenuWork *wk, int height, BOOL stream);
static void func_ov123_021d4fac(GSyncMenuWork *wk);
static void func_ov123_021d4fb8(GSyncMenuWork *wk);
static void func_ov123_021d4fec(GSyncMenuWork *wk);
static void func_ov123_021d5020(GSyncMenuWork *wk);
static void func_ov123_021d504c(GSyncMenuWork *wk);
static void func_ov123_021d50f8(GSyncMenuWork *wk);
static void func_ov123_021d5120(TCB *task, void *work);
static BOOL func_ov123_021d5128(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov123_021d5320(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov123_021d53d0(GameProc *proc, u32 *state, void *param, void *work);

// What the Game Sync button's palettes are brightened by in each color
// pokebw2's sBrightenAmounts
static u16 data_ov123_021da9d0[16] = { 0, 5, 15, 15, 15, 15, 15, 5, 5, 0, 0, 0, 0, 15, 15, 15 };

// pokebw2's sGSyncMenuVRAMConfig
static BGSysVRAMConfig data_ov123_021da9f0 = {
    GX_VRAM_BG_128_A,  GX_VRAM_BGEXTPLTT_NONE,  GX_VRAM_SUB_BG_128_C,        GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_128_B, GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_128_D,       GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_NONE,  GX_VRAM_TEXPLTT_NONE,    GX_OBJVRAMMODE_CHAR_1D_128K, GX_OBJVRAMMODE_CHAR_1D_128K,
};

// pokebw2's sGSyncMenuClActSetup
static const ClActSysSetup data_ov123_021da658 = { 0, 0, 0, 512, 4, 124, 4, 124, 0, 100, 100, 100, 100, 16, 16 };

// pokebw2's GSyncMenu_SetState
static void func_ov123_021d4240(GSyncMenuWork *wk, GSyncMenuStateFunc state) {
    wk->state = state;
}

// Changes the state, from a line that the debug build reported
// pokebw2's GSyncMenu_ChangeState
static void func_ov123_021d4244(GSyncMenuWork *wk, GSyncMenuStateFunc state, int line) {
    func_ov123_021d4240(wk, state);
}

// pokebw2's GSyncMenu_CreateBGs
static void func_ov123_021d424c(GSyncMenuWork *wk) {
    {
        BGSetup setup = {
            0,
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
            FALSE,
        };
        GFL_BGSysCreateBG(0, &setup, BGMODE_TEXT);
        GFL_BGSysClearBG(0);
        GFL_BGSysLoadScr(0);
        GFL_BGSysSetBGPriority(0, 0);
        GFL_BGSysSetBGEnabled(0, TRUE);
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
            0x8000,
            GX_BG_EXTPLTT_01,
            3,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };
        GFL_BGSysCreateBG(4, &setup, BGMODE_TEXT);
        GFL_BGSysSetBGEnabled(4, TRUE);
        GFL_BGSysLoadScr(4);
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
            0x8000,
            GX_BG_EXTPLTT_01,
            0,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };
        GFL_BGSysCreateBG(5, &setup, BGMODE_TEXT);
        GFL_BGSysFillChar(5, 0, 1, 0);
        GFL_BGSysFillScrArea(5, 0, 0, 0, 32, 32, BGSYS_FILL_TILE_PALETTE);
        GFL_BGSysLoadScr(5);
        GFL_BGSysSetBGEnabled(5, TRUE);
    }
    {
        BGSetup setup = {
            0,
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
            FALSE,
        };
        GFL_BGSysCreateBG(6, &setup, BGMODE_TEXT);
        GFL_BGSysSetBGEnabled(6, TRUE);
        GFL_BGSysLoadScr(6);
    }
    {
        BGSetup setup = {
            0,
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
            FALSE,
        };
        GFL_BGSysCreateBG(7, &setup, BGMODE_TEXT);
        GFL_BGSysSetBGEnabled(7, TRUE);
        GFL_BGSysLoadScr(7);
    }
}

// pokebw2's GSyncMenu_StartWaitIcon
static void func_ov123_021d4384(GSyncMenuWork *wk) {
    if (wk->waitIcon != NULL) {
        func_0202f038(wk->waitIcon);
        wk->waitIcon = NULL;
    }
    wk->waitIcon = func_0202ee8c(wk->tcbMgr, wk->msgWin, 15, 16, wk->heapId);
}

// Adds add's amount to each color's components, up to 31
// pokebw2's GSyncMenu_BrightenPalettes
void func_ov123_021d43b8(u16 (*palettes)[16], const u16 *add, int count) {
    int i, j;

    for (i = 0; i < count; i++) {
        u16 *palette = palettes[i];

        for (j = 0; j < 16; j++) {
            u16 r = palette[j] & 0x1f;
            u16 g = (palette[j] & 0x3e0) >> 5;
            u16 b = (palette[j] & 0x7c00) >> 10;

            r += add[j];
            g += add[j];
            b += add[j];
            if (r > 31) {
                r = 31;
            }
            if (g > 31) {
                g = 31;
            }
            if (b > 31) {
                b = 31;
            }
            palette[j] = r + (g << 5) + (b << 10);
        }
    }
}

// pokebw2's GSyncMenu_StateFadeOut
static void func_ov123_021d4440(GSyncMenuWork *wk) {
    wk->ending = TRUE;
    GFL_WipeSet(WIPE_MODE_BOTH, WIPE_TYPE_FADE_OUT, WIPE_TYPE_FADE_OUT, WIPE_COLOR_BLACK, 6, 1, wk->heapId);
    switch (wk->result) {
    case GSYNC_RESULT_CONNECT:
    case GSYNC_RESULT_WIFI_SETTINGS:
        if (func_0203cd98()) {
            func_02026400(GSYS_GetGameCommSystem(wk->gsys));
        }
        break;
    }
    func_ov123_021d4244(wk, func_ov123_021d4ab4, 426);
}

// pokebw2's GSyncMenu_StateWifiSettingsAnswer
static void func_ov123_021d449c(GSyncMenuWork *wk) {
    if (func_020279e8(wk->yesNoMenu)) {
        if (func_02027a04(wk->yesNoMenu) == 0) {
            func_ov123_021d4244(wk, func_ov123_021d4fb8, 438);
        } else {
            GFL_BGSysClearScr(5);
            wk->result = GSYNC_RESULT_NONE;
            func_ov123_021d4244(wk, func_ov123_021d4a04, 443);
        }
        func_02027858(wk->yesNoMenu);
        wk->yesNoMenu = NULL;
        func_ov123_021d4d40(wk, FALSE);
    }
}

// pokebw2's GSyncMenu_StateWifiSettingsYesNo
static void func_ov123_021d44f4(GSyncMenuWork *wk) {
    if (func_ov123_021d4e7c(wk)) {
        func_ov123_021d4da4(wk);
        func_ov123_021d4244(wk, func_ov123_021d449c, 457);
    }
}

// Called by the touched button's actor once its animation ends
// pokebw2's GSyncMenu_OnButtonAnimEnd
static void func_ov123_021d451c(uintptr_t param, fx32 frame) {
    GSyncMenuWork *wk = (GSyncMenuWork *)param;

    func_0204ba10(wk->actors[wk->selected]);
    func_0204b92c(wk->actors[wk->selected], FALSE);
    if (wk->result == GSYNC_RESULT_WIFI_SETTINGS) {
        if (wk->msgWin != NULL) {
            BmpWin_Free(wk->msgWin);
            wk->msgWin = NULL;
        }
        if (OS_IsRunOnTwl()) {
            // A DSi's own settings can't be opened from here
            GFL_MsgDataLoadStrbuf(wk->dsiMsgData, WifiError_Text_032, wk->strBuf);
            func_ov123_021d4ef0(wk, 10, FALSE);
            func_ov123_021d4d40(wk, TRUE);
            func_ov123_021d4244(wk, func_ov123_021d4afc, 479);
        } else {
            GFL_MsgDataLoadStrbuf(wk->msgData, Bank0044_Text_007, wk->strBuf);
            func_ov123_021d4fac(wk);
            func_ov123_021d4d40(wk, TRUE);
            func_ov123_021d4244(wk, func_ov123_021d44f4, 486);
        }
    } else if (wk->result == GSYNC_RESULT_CONNECT || wk->result == GSYNC_RESULT_WIFI_SETTINGS) {
        func_ov123_021d4244(wk, func_ov123_021d4fb8, 498);
    } else {
        func_ov123_021d4244(wk, func_ov123_021d4440, 501);
    }
}

// pokebw2's GSyncMenu_StateWaitButtonAnim
static void func_ov123_021d45f0(GSyncMenuWork *wk) {
}

// Creates the buttons' windows, with their messages centered on centerX and centerY where they are given
static void func_ov123_021d45f4(int count, const u32 *msgIds, GSyncMenuWork *wk, GSyncMenuWinRect *rects,
                                    const s32 *centerX, const s32 *centerY) {
    int i;

    wk->buttonCount = count;
    func_0201dd68();
    for (i = 0; i < GSYNC_MENU_BUTTON_WIN_MAX; i++) {
        if (wk->buttonWins[i] != NULL) {
            func_020456f8(wk->buttonWins[i]);
            func_0201f7a8(wk->buttonWins[i], 2);
            BmpWin_Free(wk->buttonWins[i]);
        }
        wk->buttonWins[i] = NULL;
    }
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, PALTYPE_SUB_BG, 8 * 0x20, 0x20, wk->heapId);
    for (i = 0; i < count; i++) {
        s32 x;
        s32 y;

        wk->buttonWins[i] = BmpWin_CreateDynamic(7, rects[i].x, rects[i].y, rects[i].width, rects[i].height, 8, FALSE);
        GFL_BitmapFill(BmpWin_GetBitmap(wk->buttonWins[i]), 0);
        GFL_MsgDataLoadStrbuf(wk->msgData, msgIds[i], wk->strBuf);
        GFL_TextRndUpdateColorIndexLUT(15, 2, 0);
        if (centerX != NULL) {
            x = centerX[i] - GFL_FontGetBlockWidth(wk->strBuf, wk->font, 0) / 2;
        } else {
            x = 0;
        }
        if (centerY != NULL) {
            y = centerY[i] - GFL_FontGetBlockHeight(wk->strBuf, wk->font) / 2;
        } else {
            y = 0;
        }
        GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(wk->buttonWins[i]), x, y, wk->strBuf, wk->font);
        BmpWin_FlushChar(wk->buttonWins[i]);
        BmpWin_FlushMap(wk->buttonWins[i]);
    }
    if (wk->buttonMan != NULL) {
        func_02051c6c(wk->buttonMan);
    }
    wk->buttonMan = NULL;
    GFL_BGSysLoadScr(7);
}

// pokebw2's GSyncMenu_FreeButtons
static void func_ov123_021d4730(GSyncMenuWork *wk) {
    int i;

    if (wk->buttonMan != NULL) {
        func_02051c6c(wk->buttonMan);
    }
    wk->buttonMan = NULL;
    for (i = 0; i < GSYNC_MENU_BUTTON_WIN_MAX; i++) {
        if (wk->buttonWins[i] != NULL) {
            func_020456f8(wk->buttonWins[i]);
            GFL_BGSysQueueScrLoad(5);
            func_0201f7a8(wk->buttonWins[i], 2);
            BmpWin_Free(wk->buttonWins[i]);
        }
        wk->buttonWins[i] = NULL;
    }
    wk->buttonCount = 0;
}

// pokebw2's GSyncMenu_ButtonCallback
static void func_ov123_021d4778(u32 button, u32 event, void *work) {
    GSyncMenuWork *wk = work;

    switch (event) {
    case BMN_EVENT_TOUCH:
        if (wk->touchFunc != NULL) {
            if (wk->touchFunc(button, wk)) {
                return;
            }
        }
        break;
    }
}

// pokebw2's GSyncMenu_CreateActors
void func_ov123_021d478c(GSyncMenuWork *wk) {
    u8 x[] = { 128, 128, 224 };
    u8 y[] = { 96, 96, 177 };
    u8 sequences[] = { GSYNC_MENU_SEQ_GSYNC, GSYNC_MENU_SEQ_WIFI_SETTINGS, 0 };
    ClActorSetup setup;
    int i;

    for (i = 0; i < GSYNC_MENU_ACTOR_COUNT; i++) {
        setup.x = x[i];
        setup.y = y[i];
        setup.sequence = sequences[i];
        setup.priority = 0;
        setup.bgPriority = 2;
        wk->actors[i] =
            func_0204b294(wk->clUnit, wk->chars, wk->palette, wk->cellAnims, &setup, CLACT_SURFACE_SUB, wk->heapId);
        func_0204b92c(wk->actors[i], FALSE);
        func_0204b3dc(wk->actors[i], TRUE);
        func_0204b688(wk->actors[i], 1);
    }
}

void func_ov123_021d4884(GSyncMenuWork *wk) {
    ArcTool *arc;
    NNSG2dPaletteData *palette;
    void *file;

    wk->strBuf = GFL_StrBufCreate(300, wk->heapId);
    wk->font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, wk->heapId);
    wk->msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_0044, wk->heapId);
    wk->dsiMsgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_WIFI_ERROR, wk->heapId);

    arc = GFL_ArcSysCreateFileHandle(ARCID_GSYNC_MENU, wk->heapId);
    GFL_G2DIOLoadArcNCLRDefault(arc, 1, PALTYPE_SUB_BG, 0, 0, wk->heapId);
    GFL_G2DIOLoadArcNCLRDefault(arc, 0, PALTYPE_MAIN_BG, 0, 0, wk->heapId);
    file = GFL_G2DIOReadNCLRArc(arc, 1, &palette, wk->heapId);
    MI_CpuCopy8(palette->pRawData, wk->palettes[0], 0xe0);
    MI_CpuCopy8(palette->pRawData, wk->palettes[1], 0xe0);
    func_ov123_021d43b8(wk->palettes[1], data_ov123_021da9d0, 7);
    GFL_HeapFree(file);
    wk->bgChars = GFL_BGSysLoadArcNCGRDynamic(arc, 4, 4, 0, FALSE, wk->heapId);
    GFL_G2DIOLoadNSCRSync(arc, 9, 4, 0, CHAR_POS(wk->bgChars), 0, FALSE, wk->heapId);
    GFL_G2DIOLoadNSCRSync(arc, 14, 6, 0, CHAR_POS(wk->bgChars), 0, FALSE, wk->heapId);
    GFL_ArcToolFree(arc);

    wk->frameChars = LoadCursorImageEndOfHeap(5, 12, 0, wk->heapId);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, PALTYPE_SUB_BG, 13 * 0x20, 0x20, wk->heapId);

    arc = GFL_ArcSysCreateFileHandle(ARCID_GSYNC_MENU, wk->heapId);
    wk->chars = func_0204a6c8(arc, 5, FALSE, CLACT_VRAM_SUB, wk->heapId);
    wk->palette = func_0204ac18(arc, 3, CLACT_VRAM_SUB, 0, 0, 10, wk->heapId);
    wk->cellAnims = func_0204af28(arc, 7, 16, wk->heapId);
    GFL_ArcToolFree(arc);
    func_ov123_021d478c(wk);
}


// pokebw2's GSyncMenu_StateShowButtons
static void func_ov123_021d4a04(GSyncMenuWork *wk) {
    static const s32 data_ov123_021da640[] = { 72 };
    static const s32 data_ov123_021da644[] = { 8 };
    static const TouchRect data_ov123_021da648[] = {
        { 72, 136, 40, 216 },
        { 160, 192, 0, 56 },
        { 160, 192, 192, 0 },
        { TOUCH_RECT_END, 0, 0, 0 },
    };
    static GSyncMenuWinRect data_ov123_021da9c0[] = {
        { 7, 12, 18, 5 },
    };
    u32 msgIds[] = { 0 };

    func_ov123_021d45f4(1, msgIds, wk, data_ov123_021da9c0, data_ov123_021da640, data_ov123_021da644);
    wk->buttonMan = func_02051b84(data_ov123_021da648, func_ov123_021d4778, wk, wk->heapId);
    wk->touchFunc = func_ov123_021d4ba8;
    func_ov123_021d4244(wk, func_ov123_021d4ca0, 801);
}

// pokebw2's GSyncMenu_FreeGraphics
static void func_ov123_021d4a64(GSyncMenuWork *wk) {
    func_0201dd68();
    func_ov123_021d4730(wk);
    GFL_BGSysFreeFilledChar(5, 1, 0);
    GFL_BGSysFreeCharMemory(5, CHAR_POS(wk->frameChars), CHAR_SIZE(wk->frameChars));
    GFL_BGSysReleaseBG(5);
    GFL_BGSysReleaseBG(4);
    GFL_MsgDataFree(wk->msgData);
    GFL_MsgDataFree(wk->dsiMsgData);
    GFL_FontFree(wk->font);
    GFL_StrBufFree(wk->strBuf);
}

// pokebw2's GSyncMenu_StateWaitFadeOut
static void func_ov123_021d4ab4(GSyncMenuWork *wk) {
    if (GFL_WipeIsFinished()) {
        switch (wk->result) {
        case GSYNC_RESULT_CONNECT:
        case GSYNC_RESULT_WIFI_SETTINGS:
            if (!GameCommSys_BootCheck(GSYS_GetGameCommSystem(wk->gsys))) {
                func_ov123_021d4244(wk, NULL, 837);
            }
            break;
        default:
            func_ov123_021d4244(wk, NULL, 841);
            break;
        }
    }
}

// pokebw2's GSyncMenu_StateWaitMessageTouch
static void func_ov123_021d4afc(GSyncMenuWork *wk) {
    if (func_ov123_021d4e7c(wk) && func_02035c60()) {
        func_ov123_021d4e64(wk);
        wk->prevWifiStatus = 0;
        GFL_SndSEPlay(SEQ_SE_SYS_69);
        func_ov123_021d4d40(wk, FALSE);
        func_ov123_021d4244(wk, func_ov123_021d4a04, 864);
    }
}

// pokebw2's GSyncMenu_StatePressButton
static void func_ov123_021d4b44(GSyncMenuWork *wk) {
    ClActorCallback callback;

    callback.type = CLACT_CALLBACK_LAST_FRAME;
    callback.param = (uintptr_t)wk;
    callback.func = func_ov123_021d451c;
    func_0204b92c(wk->actors[wk->selected], TRUE);
    func_0204ba00(wk->actors[wk->selected], &callback);
    func_0204b9a0(wk->actors[wk->selected]);
    func_ov123_021d4244(wk, func_ov123_021d45f0, 886);
}

// pokebw2's GSyncMenu_OnTouch
static BOOL func_ov123_021d4ba8(u32 button, GSyncMenuWork *wk) {
    wk->selected = button;
    switch (button) {
    case GSYNC_MENU_BUTTON_GSYNC:
        GFL_SndSEPlay(SEQ_SE_SYS_69);
        func_0204b878(wk->actors[GSYNC_MENU_BUTTON_GSYNC], GSYNC_MENU_SEQ_GSYNC);
        func_ov123_021d4244(wk, func_ov123_021d4b44, 905);
        wk->result = GSYNC_RESULT_CONNECT;
        break;
    case GSYNC_MENU_BUTTON_WIFI_SETTINGS:
        GFL_SndSEPlay(SEQ_SE_SYS_69);
        wk->result = GSYNC_RESULT_WIFI_SETTINGS;
        func_ov123_021d4244(wk, func_ov123_021d4b44, 915);
        break;
    case GSYNC_MENU_BUTTON_RETURN:
        GFL_SndSEPlay(SEQ_SE_SYS_70);
        wk->result = GSYNC_RESULT_NONE;
        func_ov123_021d4244(wk, func_ov123_021d4b44, 921);
        break;
    }
    return TRUE;
}

// pokebw2's GSyncMenu_SetSignalSequence
static void func_ov123_021d4c24(GSyncMenuWork *wk, int sequence) {
    if (sequence != func_0204b898(wk->actors[GSYNC_MENU_BUTTON_GSYNC])) {
        func_0204b92c(wk->actors[GSYNC_MENU_BUTTON_GSYNC], TRUE);
        func_0204b878(wk->actors[GSYNC_MENU_BUTTON_GSYNC], sequence);
    }
}

// Shows the wireless signal's strength on the Game Sync button, from the bits of func_0201058c's status that
// are set, strongest first
// pokebw2's GSyncMenu_UpdateSignal
static void func_ov123_021d4c4c(GSyncMenuWork *wk) {
    if (wk->wifiStatus != wk->prevWifiStatus) {
        if (wk->wifiStatus & 0x40) {
            func_ov123_021d4c24(wk, 24);
        } else if (wk->wifiStatus & 0x80) {
            func_ov123_021d4c24(wk, 22);
        } else if (wk->wifiStatus & 0x100) {
            func_ov123_021d4c24(wk, 21);
        } else if (wk->wifiStatus & 0x200) {
            func_ov123_021d4c24(wk, 21);
        } else {
            func_ov123_021d4c24(wk, GSYNC_MENU_SEQ_GSYNC);
        }
    }
}

// pokebw2's GSyncMenu_StateSelect
static void func_ov123_021d4ca0(GSyncMenuWork *wk) {
    if (GFL_WipeIsFinished()) {
        func_02051c88(wk->buttonMan);
    }
    func_ov123_021d4c4c(wk);
}

// Dims the first count palettes of the sub screen's BG or OBJ palettes. buffer is both the PaletteFade buffer and the
// VRAM it loads from, whose IDs are the same for the sub screen
// pokebw2's GSyncMenu_DimPalettes
static void func_ov123_021d4cbc(GSyncMenuWork *wk, u16 buffer, int count) {
    PaletteFade *fade = func_02021518(wk->heapId);
    u16 *faded;

    func_0202155c(fade, buffer, 0x200, wk->heapId);
    func_020216d4(fade, buffer, 0, count * 32);
    func_02021fd8(fade, buffer, 0, count * 16, 6, 0);
    faded = func_02021730(fade, buffer);
    DC_FlushRange(faded, count * 32);
    switch (buffer) {
    case PALFADE_BUFFER_SUB_OBJ:
        GXS_LoadOBJPltt(faded, 0, count * 32);
        break;
    case PALFADE_BUFFER_SUB_BG:
        GXS_LoadBGPltt(faded, 0, count * 32);
        break;
    }
    func_020215a0(fade, buffer);
    func_02021540(fade);
}

// Dims the sub screen behind a message, or restores it
// pokebw2's GSyncMenu_DimSubScreen
static void func_ov123_021d4d40(GSyncMenuWork *wk, BOOL dim) {
    if (dim) {
        MI_CpuCopy8((void *)HW_DB_OBJ_PLTT, wk->objPalettes, 0x200);
        MI_CpuCopy8((void *)HW_DB_BG_PLTT, wk->bgPalettes, 0x200);
        func_ov123_021d4cbc(wk, PALFADE_BUFFER_SUB_OBJ, 14);
        func_ov123_021d4cbc(wk, PALFADE_BUFFER_SUB_BG, 9);
    } else {
        GXS_LoadOBJPltt(wk->objPalettes, 0, 14 * 32);
        GXS_LoadBGPltt(wk->bgPalettes, 0, 9 * 32);
    }
}

// pokebw2's GSyncMenu_CreateYesNo
static void func_ov123_021d4da4(GSyncMenuWork *wk) {
    AppTaskMenuInit init;

    init.heapId = wk->heapId;
    init.itemCount = 2;
    init.items = wk->yesNoItems;
    init.posType = APP_TASKMENU_POS_BOTTOM_RIGHT;
    init.x = 32;
    init.y = 14;
    init.width = 13;
    init.height = 3;
    wk->yesNoItems[0].str = GFL_StrBufCreate(100, wk->heapId);
    GFL_MsgDataLoadStrbuf(wk->msgData, Bank0044_Text_004, wk->yesNoItems[0].str);
    wk->yesNoItems[0].color = PRINT_COLOR(14, 15, 0);
    wk->yesNoItems[1].str = GFL_StrBufCreate(100, wk->heapId);
    GFL_MsgDataLoadStrbuf(wk->msgData, Bank0044_Text_005, wk->yesNoItems[1].str);
    wk->yesNoItems[1].color = PRINT_COLOR(14, 15, 0);
    wk->yesNoMenu = func_02027778(&init, wk->menuRes);
    func_02027cd8(wk->yesNoMenu, TRUE);
    GFL_StrBufFree(wk->yesNoItems[0].str);
    GFL_StrBufFree(wk->yesNoItems[1].str);
}

// pokebw2's GSyncMenu_ClearMessage
static void func_ov123_021d4e34(GSyncMenuWork *wk) {
    if (wk->waitIcon != NULL) {
        func_0202f038(wk->waitIcon);
        wk->waitIcon = NULL;
    }
    func_0201f7a8(wk->msgWin, 2);
    func_020456f8(wk->msgWin);
    GFL_BGSysQueueScrLoad(5);
}

// pokebw2's GSyncMenu_FreeMessage
static void func_ov123_021d4e64(GSyncMenuWork *wk) {
    func_ov123_021d4e34(wk);
    BmpWin_Free(wk->msgWin);
    wk->msgWin = NULL;
}

// pokebw2's GSyncMenu_IsMessageFinished
static BOOL func_ov123_021d4e7c(GSyncMenuWork *wk) {
    if (wk->printStream != NULL) {
        if (wk->msgWin != NULL) {
            func_020286c4(wk->keyCursor, wk->printStream, wk->msgWin);
        }
        switch (func_0201ce30(wk->printStream)) {
        case PRINT_STREAM_DONE:
            func_0201ce48(wk->printStream);
            wk->printStream = NULL;
            break;
        case PRINT_STREAM_PAUSED:
            if (func_02035c60()) {
                GFL_SndSEPlay(SEQ_SE_MESSAGE);
                func_0201ce38(wk->printStream);
            }
            break;
        case PRINT_STREAM_RUNNING:
            if (func_02035c38()) {
                func_0201ce5c(wk->printStream, 0);
            }
            break;
        }
        return FALSE;
    }
    return TRUE;
}

// Prints strBuf in the message window, at once or a character at a time
// pokebw2's GSyncMenu_PrintMessage
static void func_ov123_021d4ef0(GSyncMenuWork *wk, int height, BOOL stream) {
    BmpWin *win;

    if (wk->msgWin == NULL) {
        wk->msgWin = BmpWin_CreateDynamic(5, 1, 3, 30, height, 13, TRUE);
    }
    win = wk->msgWin;
    GFL_BitmapFill(BmpWin_GetBitmap(win), 15);
    GFL_TextRndUpdateColorIndexLUT(1, 2, 15);
    if (stream) {
        wk->printStream =
            func_0201cce4(win, 0, 0, wk->strBuf, wk->font, func_02012ffc(), wk->tcbMgr, 2, wk->heapId, 15);
    } else {
        GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(win), 0, 0, wk->strBuf, wk->font);
    }
    BmpWin_DrawFrame(win, TRUE, wk->frameChars, 12);
    BmpWin_FlushChar(win);
    BmpWin_FlushMap(win);
    GFL_BGSysQueueScrLoad(5);
}

// pokebw2's GSyncMenu_PrintMessageStream
static void func_ov123_021d4fac(GSyncMenuWork *wk) {
    func_ov123_021d4ef0(wk, 4, TRUE);
}

// pokebw2's GSyncMenu_StateSaveAsk
static void func_ov123_021d4fb8(GSyncMenuWork *wk) {
    func_ov123_021d4d40(wk, TRUE);
    GFL_BGSysFillScrAsync(5, 0);
    GFL_MsgDataLoadStrbuf(wk->msgData, Bank0044_Text_003, wk->strBuf);
    func_ov123_021d4fac(wk);
    func_ov123_021d4244(wk, func_ov123_021d50f8, 1232);
}

// pokebw2's GSyncMenu_StateWaitSave
static void func_ov123_021d4fec(GSyncMenuWork *wk) {
    if (func_02012dd0(wk->gameData) == 2) {
        func_ov123_021d4e64(wk);
        func_ov123_021d4d40(wk, FALSE);
        func_ov123_021d4244(wk, func_ov123_021d4440, 1258);
    }
}

// pokebw2's GSyncMenu_StateStartSave
static void func_ov123_021d5020(GSyncMenuWork *wk) {
    if (func_ov123_021d4e7c(wk)) {
        func_02012dac(wk->gameData);
        func_ov123_021d4244(wk, func_ov123_021d4fec, 1274);
    }
}

// pokebw2's GSyncMenu_StateSaveAnswer
static void func_ov123_021d504c(GSyncMenuWork *wk) {
    if (func_020279e8(wk->yesNoMenu)) {
        if (func_02027a04(wk->yesNoMenu) == 0) {
            if (func_020071f0(GameData_GetSaveControl(wk->gameData))) {
                GFL_MsgDataLoadStrbuf(wk->msgData, Bank0044_Text_046, wk->strBuf);
                func_ov123_021d4fac(wk);
                func_ov123_021d4244(wk, func_ov123_021d4afc, 1293);
            } else {
                GFL_MsgDataLoadStrbuf(wk->msgData, Bank0044_Text_006, wk->strBuf);
                func_ov123_021d4ef0(wk, 4, FALSE);
                func_ov123_021d4384(wk);
                func_ov123_021d4244(wk, func_ov123_021d5020, 1305);
            }
        } else {
            GFL_BGSysClearScr(5);
            wk->result = GSYNC_RESULT_NONE;
            func_ov123_021d4244(wk, func_ov123_021d4a04, 1311);
            func_ov123_021d4d40(wk, FALSE);
        }
        func_02027858(wk->yesNoMenu);
        wk->yesNoMenu = NULL;
    }
}

// pokebw2's GSyncMenu_StateSaveYesNo
static void func_ov123_021d50f8(GSyncMenuWork *wk) {
    if (func_ov123_021d4e7c(wk)) {
        func_ov123_021d4da4(wk);
        func_ov123_021d4244(wk, func_ov123_021d504c, 1334);
    }
}

// pokebw2's GSyncMenu_VBlank
static void func_ov123_021d5120(TCB *task, void *work) {
    func_0204a648();
}

// pokebw2's GSyncMenu_ProcInit
static BOOL func_ov123_021d5128(GameProc *proc, u32 *state, void *param, void *work) {
    EventGameSync *event = param;
    GSyncMenuWork *wk;
    s32 brightness;
    u32 color;

    GFL_HeapCreateChild(HEAPID_USER, HEAPID_GSYNC_MENU, 0x18000);
    wk = GFL_ProcInitSubsystem(proc, sizeof(GSyncMenuWork), HEAPID_GSYNC_MENU);
    MI_CpuFill8(wk, 0, sizeof(GSyncMenuWork));
    wk->heapId = HEAPID_GSYNC_MENU;
    wk->gameData = event->gameData;
    wk->gsys = event->gsys;
    GFL_BGSysSetDisplayLayout(1);
    if (event->fromWifiSettings) {
        brightness = 16;
    } else {
        brightness = -16;
    }
    GX_SetMasterBrightness(brightness);
    GXS_SetMasterBrightness(brightness);
    GXS_DispOn();
    GX_DispOn();
    GFL_BGSysCreate(wk->heapId);
    BmpWin_InitAllocator(wk->heapId);
    func_0201dd60();
    GFL_BGSysSetVRAMBanks(&data_ov123_021da9f0);
    {
        BGSysLCDConfig lcd = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_2D };
        GFL_BGSysSetLCDConfig(&lcd);
    }
    GFL_BGSysSetEnabledBGsA(0);
    GFL_BGSysSetEnabledBGsB(0);
    ClActSys_Create(&data_ov123_021da658, &data_ov123_021da9f0, wk->heapId);
    wk->clUnit = func_0204b100(40, 0, wk->heapId);
    wk->vblankTask = GFL_VBlankTCBAdd(func_ov123_021d5120, wk, 0);
    func_ov123_021d424c(wk);
    func_ov123_021d4884(wk);
    wk->objPalettes = GFL_HeapAllocate(wk->heapId, 0x200, FALSE, "gsync_menu.c", 1446);
    wk->bgPalettes = GFL_HeapAllocate(wk->heapId, 0x200, FALSE, "gsync_menu.c", 1447);
    wk->tcbMgr = GFL_TCBExMgrCreate(wk->heapId, wk->heapId, 3, 0);
    wk->keyCursor = func_02028590(15, FALSE, TRUE, wk->heapId);
    wk->printQueue = func_0201c440(wk->heapId);
    wk->menuRes = func_02027f60(5, 9, wk->font, wk->printQueue, wk->heapId);
    G2S_SetBlendAlpha(GX_BLEND_PLANEMASK_BG2, GX_BLEND_PLANEMASK_BG0, 15, 4);
    color = WIPE_COLOR_BLACK;
    if (event->fromWifiSettings) {
        color = WIPE_COLOR_WHITE;
    }
    GFL_WipeSet(WIPE_MODE_BOTH, WIPE_TYPE_FADE_IN, WIPE_TYPE_FADE_IN, color, 6, 1, wk->heapId);
    event->fromWifiSettings = FALSE;
    GFL_BGSysSetEnabledBGsB(GX_PLANEMASK_ALL);
    func_ov123_021d4244(wk, func_ov123_021d4a04, 1471);
    wk->param = event;
    func_0203d34c(FALSE, wk->heapId);
    func_020355d8(TRUE);
    return TRUE;
}

// pokebw2's GSyncMenu_ProcMain
static BOOL func_ov123_021d5320(GameProc *proc, u32 *state, void *param, void *work) {
    GSyncMenuWork *wk = work;
    GSyncMenuStateFunc func = wk->state;
    BOOL done = TRUE;

    if (func != NULL) {
        func(wk);
        done = FALSE;
    }
    if (wk->yesNoMenu != NULL) {
        func_02027974(wk->yesNoMenu);
    }
    if (func_0203cd98()) {
        wk->prevWifiStatus = wk->wifiStatus;
        wk->wifiStatus = func_0201058c(NULL);
    }
    GFL_TCBExMgrUpdate(wk->tcbMgr);
    func_0201c4e4(wk->printQueue);
    GFL_BGSysMoveBG(4, BG_MOVE_SET_Y, wk->bgScroll);
    wk->bgScroll--;
    func_0204a600();
    if (!wk->ending) {
        func_020121cc(wk->gsys);
    }
    if (GFL_WipeIsFinished() && func_02012204(wk->gsys)) {
        wk->result = GSYNC_RESULT_NONE;
        done = TRUE;
        func_0202223c(0, WIPE_COLOR_BLACK);
        func_0202223c(1, WIPE_COLOR_BLACK);
    }
    return done;
}

// pokebw2's GSyncMenu_ProcExit
static BOOL func_ov123_021d53d0(GameProc *proc, u32 *state, void *param, void *work) {
    EventGameSync *event = param;
    GSyncMenuWork *wk = work;
    int i;

    if (!GFL_WipeIsFinished()) {
        return FALSE;
    }
    for (i = 0; i < GSYNC_MENU_ACTOR_COUNT; i++) {
        func_0204b3b4(wk->actors[i]);
    }
    func_0204ada4(wk->palette);
    func_0204a8d4(wk->chars);
    func_0204afd8(wk->cellAnims);
    GFL_TCBRemove(wk->vblankTask);
    func_0204b1cc(wk->clUnit);
    func_0204a5ac();
    if (wk->waitIcon != NULL) {
        func_0202f038(wk->waitIcon);
        wk->waitIcon = NULL;
    }
    if (wk->printStream != NULL) {
        func_0201ce48(wk->printStream);
    }
    if (wk->yesNoMenu != NULL) {
        func_02027858(wk->yesNoMenu);
        wk->yesNoMenu = NULL;
    }
    func_ov123_021d4a64(wk);
    event->gsyncResult = wk->result;
    GFL_HeapFree(wk->objPalettes);
    GFL_HeapFree(wk->bgPalettes);
    func_02028604(wk->keyCursor);
    GFL_BGSysReleaseBG(5);
    func_0201c6ec(wk->printQueue);
    func_0201c4c0(wk->printQueue);
    if (wk->msgWin != NULL) {
        BmpWin_Free(wk->msgWin);
    }
    GFL_TCBExMgrFree(wk->tcbMgr);
    func_02027fd8(wk->menuRes);
    BmpWin_FreeAllocator();
    GFL_BGSysFree();
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(HEAPID_GSYNC_MENU);
    return TRUE;
}

// pokebw2's GSYNC_MENU_PROC_FUNCTIONS
const GameProcFunctions data_ov123_021da674 = {
    func_ov123_021d5128,
    func_ov123_021d5320,
    func_ov123_021d53d0,
};
