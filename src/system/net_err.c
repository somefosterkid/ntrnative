// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/net_err.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "constants/arc.h"
#include "constants/text_banks.h"
#include "gfl/arc_util.h"
#include "gfl/bmp.h"
#include "gfl/dwc_rap.h"
#include "gfl/graphics.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/net.h"
#include "gfl/net_state.h"
#include "gfl/net_system.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/ui.h"
#include "gfl/wih.h"
#include "gfl/wm_icon.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "nitro/memorymap.h"
#include "nitro/mi.h"
#include "nitro/os.h"
#include "nnsys/g2d.h"
#include "system/dsi.h"
#include "system/gf_font.h"
#include "system/main.h"
#include "system/printsys.h"
#include "system/wordset.h"
#include "text/system/startmenu_gfl_net_err_disp_message.h"
#include "text/system/wifi_error.h"

// The network error screen: a window on BG1 of the main screen with the error's message, until A or B is pressed, over
// whatever the game was showing, which it saves and puts back. A fatal error stops the game on the screen instead.
// The file's name is the ROM's string, from GFL_HeapAllocate's calls. The names of the functions are swan's

// The archives of the window's graphics, for a network error and for a failed assertion, and their files
#define ARCID_ERROR_WINDOW_GRA 22
#define ARCID_ASSERT_WINDOW_GRA 230
#define ERROR_WINDOW_GRA_PLTT 0
#define ERROR_WINDOW_GRA_CHAR 1
#define ERROR_WINDOW_GRA_SCRN 2

enum {
    ERROR_WINDOW_NET,
    ERROR_WINDOW_ASSERT,
    ERROR_WINDOW_COUNT,
};

// The size of the heap for the network, and of what the screen saves of BG1: its characters, screen and palette
#define NET_HEAP_SIZE 0x4920
#define SAVED_CHAR_SIZE 0x4000
#define SAVED_SCREEN_SIZE 0x800
#define SAVED_PLTT_SIZE 0x20

// The text area: the tiles from FIRST_TEXT_TILE, laid out in rows of the screen from TEXT_TOP, TEXT_LEFT
#define TILE_SIZE 0x20
#define FIRST_TEXT_TILE 0x20
#define TEXT_LEFT 1
#define TEXT_TOP 4
#define TEXT_WIDTH 30
#define TEXT_HEIGHT 16
#define TEXT_BACKGROUND 7

// The keys that close the screen
#define CLOSE_KEYS (PAD_BUTTON_A | PAD_BUTTON_B)

// The messages a Wi-Fi error can have without a table: the largest is NET_ERR_MESSAGE_TABLE_MAX - 1
#define NET_ERR_MESSAGE_TABLE_MAX 16

// The state of the error: none, found, and ready to be shown
enum {
    NET_ERR_STATE_NONE,
    NET_ERR_STATE_FOUND,
    NET_ERR_STATE_SHOW,
};

// What func_0200fb24 tells the game to do about the error
enum {
    NET_ERR_CHECK_NONE,
    NET_ERR_CHECK_RETRY,
    NET_ERR_CHECK_END,
};

// A network type that only the error screen has: an assertion failed
#define NET_ERR_TYPE_ASSERT 0x60

// The first of the types of GFLNetErrorInfo that have a message of their own
#define NET_ERR_INFO_TYPE_FIRST 0x3ee

// The keys held, up cancelling down and left cancelling right, as key.c reads them
static inline u16 ReadKeys(void) {
    u16 keys = PAD_Read();

    keys = keys & ~((keys & PAD_KEY_UP) << 1) & ~((keys & PAD_KEY_LEFT) >> 1);
    return keys;
}

typedef struct {
    // What BG1 of the main screen had before the error screen
    void *charBuf;
    void *screenBuf;
    void *plttBuf;
    GXDispCnt dispCnt;
    GXBg01Control bg1Control;
    s32 brightness;
    s32 subBrightness;
    s32 hBlankIrq;
    s32 vBlankIrq;
    u32 bgBanks;
    u32 subBgBanks;
    // A NET_ERR_STATE_*
    int state;
    u32 unk30;
    u16 blend;
    u16 subBlend;
    u32 subEnabledBGs;
    int subVisibleWnd;
    u16 subBackdrop;
    // Whether the screens stay dark after the error screen, for func_0200f83c
    BOOL keepDark;
    GFLNetErrorInfo info;
    u8 stateError;
    // A GFL_NET_TYPE_*, or NET_ERR_TYPE_ASSERT
    u8 netType;
    // Whether message is shown as it is, without the tables
    u8 messageGiven;
    // A NET_ERR_CHECK_*
    int check;
    // The message of a Wi-Fi error, in TEXT_BANK_WIFI_ERROR
    u32 message;
} GFLNetErrWork;

// The files of an error window's graphics
typedef struct {
    u16 arcId;
    u16 charId;
    u16 screenId;
    u16 plttId;
} ErrorWindowGraphics;

static void func_0200f824(void);
static void GFL_NetErrAbortCore(void);
BOOL GFL_NetErrShowCore(u32 fatal);
static void GFL_NetErrDispInit(u32 fatal, BOOL disconnecting);
static void GFL_NetErrRestore(BOOL keepDark);
static BOOL GFL_NetErrIsReady(void);
void loadErrorBackground(int window);
u32 func_02010158(GFLNetErrorInfo *info, u32 message);
void GFL_NetErrDispMessage(u32 fatal, BOOL disconnecting);
static void GFL_NetErrGameUpdate(void);

static const ErrorWindowGraphics FATAL_ERROR_G2D_RESIDS[ERROR_WINDOW_COUNT] = {
    { ARCID_ERROR_WINDOW_GRA, ERROR_WINDOW_GRA_CHAR, ERROR_WINDOW_GRA_SCRN, ERROR_WINDOW_GRA_PLTT },
    { 0xea, ERROR_WINDOW_GRA_CHAR, ERROR_WINDOW_GRA_SCRN, ERROR_WINDOW_GRA_PLTT },
};

// The message of each Wi-Fi error on a DS, and on a DSi, whose messages name the system's own settings
// pokebw2's sWifiErrorMessages
static const u16 data_0209deb0[NET_ERR_MESSAGE_TABLE_MAX] = {
    WifiError_Text_000,    WifiError_Text_001,
    WifiError_Text_002,     WifiError_Text_003,
    WifiError_Text_004,      WifiError_Text_005,
    WifiError_Text_006,    WifiError_Text_007,
    WifiError_Text_008,  WifiError_Text_009,
    WifiError_Text_010, WifiError_Text_011,
    WifiError_Text_012,    WifiError_Text_013,
    WifiError_Text_014,         WifiError_Text_015,
};

// pokebw2's sWifiErrorMessagesDSi
static const u16 data_0209ded0[NET_ERR_MESSAGE_TABLE_MAX] = {
    WifiError_Text_047,  WifiError_Text_001,
    WifiError_Text_048,   WifiError_Text_003,
    WifiError_Text_004,      WifiError_Text_005,
    WifiError_Text_049,  WifiError_Text_007,
    WifiError_Text_050,  WifiError_Text_009,
    WifiError_Text_010, WifiError_Text_011,
    WifiError_Text_012,    WifiError_Text_013,
    WifiError_Text_014,         WifiError_Text_051,
};

static GFLNetErrWork g_GflNetErr;


void GFL_NetHeapInit(void) {
    GFL_HeapCreateChild(HEAPID_USER, HEAPID_NET, NET_HEAP_SIZE);
    MI_CpuFill8(&g_GflNetErr, 0, sizeof(GFLNetErrWork));
}

void GFL_NetErrInit(u32 heapId) {
    GFLNetErrWork *work = &g_GflNetErr;

    MI_CpuFill8(work, 0, sizeof(GFLNetErrWork));
    work->charBuf = GFL_HeapAllocate(heapId, SAVED_CHAR_SIZE, FALSE, "net_err.c", 187);
    work->screenBuf = GFL_HeapAllocate(heapId, SAVED_SCREEN_SIZE, FALSE, "net_err.c", 188);
    work->plttBuf = GFL_HeapAllocate(heapId, SAVED_PLTT_SIZE, FALSE, "net_err.c", 189);
}

// Finds the network's error, each frame
void func_0200f734(void) {
    u8 netType;

    if (g_GflNetErr.state == NET_ERR_STATE_NONE && func_0203cd98() == TRUE) {
        if (func_0203d880() || func_020393d0() || func_0203c990()) {
            func_0203d864();
            func_0200faac();
            netType = func_0203d798()->bNetType;
            if (netType == GFL_NET_TYPE_WIFI || netType == GFL_NET_TYPE_WIFI_LOBBY ||
                netType == GFL_NET_TYPE_WIFI_GTS) {
                GFLNetErrorInfo *info = func_0203ca98();

                if (info->type == NET_ERR_INFO_TYPE_FIRST + 2 || info->type == NET_ERR_INFO_TYPE_FIRST + 3) {
                    g_GflNetErr.message = WifiError_Text_040;
                } else if (info->type == NET_ERR_INFO_TYPE_FIRST + 4 || info->type == NET_ERR_INFO_TYPE_FIRST + 5 ||
                           info->type == NET_ERR_INFO_TYPE_FIRST + 6 || info->type == NET_ERR_INFO_TYPE_FIRST + 7 ||
                           info->type == NET_ERR_INFO_TYPE_FIRST + 8) {
                    g_GflNetErr.message = WifiError_Text_014;
                } else {
                    g_GflNetErr.message = func_ov009_02156460(info->code, info->unk4);
                }
            }
        }
    }
}

BOOL GFL_NetErrShow(u32 a0) {
    return GFL_NetErrShowCore(a0);
}


void func_0200f7c0(u32 message) {
    g_GflNetErr.message = message;
    g_GflNetErr.messageGiven = TRUE;
    g_GflNetErr.netType = GFL_NET_TYPE_WIFI;
    GFL_NetErrShow(TRUE);
}

void func_0200f7dc(void) {
    GFL_NetErrShow(1);
}


void func_0200f7e8(BOOL keepDark) {
    g_GflNetErr.keepDark = keepDark;
}

void func_0200f7f4(void) {
    func_0200f7e8(FALSE);
}

// Sets the brightness the screens had before the error screen
void func_0200f800(void) {
    GFLNetErrWork *work = &g_GflNetErr;

    GX_SetMasterBrightness(work->brightness);
    GXS_SetMasterBrightness(work->subBrightness);
}

static void func_0200f824(void) {
    if (func_0203cd98() == TRUE) {
        func_0203cf5c();
        OS_WaitIrq(TRUE, OS_IE_V_BLANK);
    }
}

// Shows the error screen while the network goes on, on the screen that has the wireless icon, with a window that
// leaves the icon's OBJ visible
void func_0200f83c(void) {
    GXSDispCnt subDispCnt = GXS_GetDispCnt();
    GXWndPlane outside = G2_GetWndOutsidePlane();
    GXWndPlane subOutside = G2S_GetWndOutsidePlane();
    GXWndPlane objInside = G2_GetWndOBJInsidePlane();
    GXWndPlane subObjInside = G2S_GetWndOBJInsidePlane();

    if (!GFL_NetErrIsReady()) {
        return;
    }
    GFL_SndStop();
    GFL_NetErrDispInit(FALSE, FALSE);
    GX_VBlankIntr(TRUE);
    func_02037174();
    if (func_02037200() == 1) {
        G2_SetWndOutsidePlane(GX_WND_PLANEMASK_BG1, FALSE);
        G2_SetWndOBJInsidePlane(GX_WND_PLANEMASK_OBJ, FALSE);
        GX_SetVisibleWnd(GX_WNDMASK_OW);
        GX_SetVisiblePlane(GX_PLANEMASK_BG1 | GX_PLANEMASK_OBJ);
    } else {
        G2S_SetWndOutsidePlane(GX_WND_PLANEMASK_BG1, FALSE);
        G2S_SetWndOBJInsidePlane(GX_WND_PLANEMASK_OBJ, FALSE);
        GXS_SetVisibleWnd(GX_WNDMASK_OW);
        GXS_SetVisiblePlane(GX_PLANEMASK_BG1 | GX_PLANEMASK_OBJ);
    }
    // Wait for the keys to be released, then pressed, with the lid open
    while (PAD_DetectFold() == TRUE || (ReadKeys() & CLOSE_KEYS)) {
        GFL_NetErrGameUpdate();
        func_0200f824();
    }
    while (PAD_DetectFold() == TRUE || !(ReadKeys() & CLOSE_KEYS)) {
        GFL_NetErrGameUpdate();
        func_0200f824();
    }
    GFL_NetErrRestore(g_GflNetErr.keepDark == TRUE ? TRUE : FALSE);
    func_02037190();
    if (func_02037200() == 2) {
        GXS_SetGraphicsMode(subDispCnt.bgMode);
        GXS_SetVisiblePlane(subDispCnt.visiblePlane);
        GXS_SetVisibleWnd(subDispCnt.visibleWnd);
        G2S_SetWndOutsidePlane(subOutside.planeMask, subOutside.effect);
        G2S_SetWndOBJInsidePlane(subObjInside.planeMask, subObjInside.effect);
    } else {
        G2_SetWndOutsidePlane(outside.planeMask, outside.effect);
        G2_SetWndOBJInsidePlane(objInside.planeMask, objInside.effect);
    }
}

void GFL_NetErrAbort(void) {
    if (GFL_NetErrIsReady()) {
        GFL_NetErrAbortCore();
    }
}

static void GFL_NetErrAbortCore(void) {
    GFL_NetErrDispInit(TRUE, FALSE);
    func_020351a0(2);
    while (TRUE) {
        func_020053ec();
    }
}


// Records the network's error, and what the game is to do about it
void func_0200faac(void) {
    GFLNetErrWork *work = &g_GflNetErr;

    if (GFL_NetErrIsReady() && work->state == NET_ERR_STATE_NONE) {
        work->state = NET_ERR_STATE_FOUND;
        if (!func_0203cdbc()) {
            work->netType = func_0203d798()->bNetType;
            work->info = *func_0203ca98();
            work->stateError = func_0203c990();
            if (work->netType == GFL_NET_TYPE_WIFI || work->netType == GFL_NET_TYPE_WIFI_LOBBY ||
                work->netType == GFL_NET_TYPE_WIFI_GTS) {
                if (work->info.unk4 == 1 || work->info.unk4 == 2) {
                    work->check = NET_ERR_CHECK_END;
                } else {
                    work->check = NET_ERR_CHECK_RETRY;
                }
            } else {
                work->check = NET_ERR_CHECK_END;
            }
        }
    }
}

// pokebw2's GFL_NetErrCheck
BOOL func_0200fb24(void) {
    GFLNetErrWork *work = &g_GflNetErr;

    if (work->state != NET_ERR_STATE_NONE) {
        return work->check;
    }
    return NET_ERR_CHECK_NONE;
}

// pokebw2's GFL_NetErrMarkShown
void func_0200fb38(void) {
    if (g_GflNetErr.state == NET_ERR_STATE_FOUND) {
        g_GflNetErr.state = NET_ERR_STATE_SHOW;
    }
}

void func_0200fb4c(u32 message) {
    if (g_GflNetErr.state != NET_ERR_STATE_SHOW) {
        g_GflNetErr.message = message;
        g_GflNetErr.messageGiven = TRUE;
        g_GflNetErr.netType = GFL_NET_TYPE_WIFI;
        g_GflNetErr.state = NET_ERR_STATE_SHOW;
    }
}

void AssertFailErrorDisp(void) {
    GFL_HeapFree(g_GflNetErr.charBuf);
    GFL_HeapFree(g_GflNetErr.screenBuf);
    GFL_HeapFree(g_GflNetErr.plttBuf);
    GFL_HeapDelete(HEAPID_NET);
    g_GflNetErr.netType = NET_ERR_TYPE_ASSERT;
    GFL_NetErrShow(TRUE);
}


void func_0200fba0(void) {
    g_GflNetErr.state = NET_ERR_STATE_NONE;
    g_GflNetErr.check = NET_ERR_CHECK_NONE;
}

BOOL func_0200fbb0(void) {
    if (func_0203cd98()) {
        GFLNetInitData *init = func_0203d798();

        func_0203cf04();
        func_0203ceac(NULL);
        func_0203d0d0();
        do {
            if (init != NULL && (init->bNetType == 0 || init->bNetType == 5)) {
                if (func_ov014_02178e4c() == 10) {
                    return FALSE;
                }
            }
            func_0203cfa0();
            GFL_NetErrGameUpdate();
            OS_WaitIrq(TRUE, OS_IE_V_BLANK);
        } while (!func_0203cdbc());
    }
    return TRUE;
}

// Shows the error that was marked, shuts the connection down, and waits for A or B. A Wi-Fi error shows a message
// while the connection shuts down
BOOL GFL_NetErrShowCore(u32 fatal) {
    GFLNetErrWork *work = &g_GflNetErr;
    BOOL wifi;

    if (!GFL_NetErrIsReady()) {
        return FALSE;
    }
    if (fatal == TRUE) {
        GFL_NetErrAbortCore();
    }
    if (work->state == NET_ERR_STATE_SHOW) {
        wifi = FALSE;
        if (work->netType == GFL_NET_TYPE_WIFI || work->netType == GFL_NET_TYPE_WIFI_LOBBY ||
            work->netType == GFL_NET_TYPE_WIFI_GTS) {
            wifi = TRUE;
        }
        if (wifi == TRUE) {
            GFL_NetErrDispInit(fatal, TRUE);
        }
        if (!func_0200fbb0()) {
            GFL_NetErrAbortCore();
        }
        if (wifi == TRUE) {
            work->hBlankIrq = GX_HBlankIntr(FALSE);
            work->vBlankIrq = GX_VBlankIntr(FALSE);
            GFL_NetErrDispMessage(fatal, FALSE);
        } else {
            GFL_NetErrDispInit(fatal, FALSE);
        }
        // Wait for the keys to be released, then pressed, with the lid open
        while (PAD_DetectFold() == TRUE || (ReadKeys() & CLOSE_KEYS)) {
            GFL_NetErrGameUpdate();
        }
        while (PAD_DetectFold() == TRUE || !(ReadKeys() & CLOSE_KEYS)) {
            GFL_NetErrGameUpdate();
        }
        GFL_NetErrRestore(FALSE);
        func_0200fba0();
        work->unk30 = 0;
        return TRUE;
    }
    return FALSE;
}

static void GFL_NetErrDispInit(u32 fatal, BOOL disconnecting) {
    GFLNetErrWork *work = &g_GflNetErr;

    if (!GFL_NetErrIsReady()) {
        return;
    }
    work->brightness = GXx_GetMasterBrightness_(REG_MASTER_BRIGHT_ADDR);
    work->subBrightness = GXx_GetMasterBrightness_(REG_DB_MASTER_BRIGHT_ADDR);
    GX_SetMasterBrightness(-16);
    GXS_SetMasterBrightness(-16);
    if (!disconnecting) {
        work->hBlankIrq = GX_HBlankIntr(FALSE);
        work->vBlankIrq = GX_VBlankIntr(FALSE);
    }
    work->bgBanks = GX_ResetBankForBG();
    work->subBgBanks = GX_ResetBankForSubBG();
    GX_SetBankForBG(GX_VRAM_BG_128_C);
    work->dispCnt = GX_GetDispCnt();
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BG0_AS_2D);
    GX_SetVisiblePlane(GX_PLANEMASK_BG1);
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    work->subVisibleWnd = GXS_GetVisibleWnd();
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    work->bg1Control = G2_GetBG1Control();
    G2_SetBG1Control(GX_BG_SCRSIZE_TEXT_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x04000,
                     work->bg1Control.bgExtPltt);
    G2_BG1Mosaic(FALSE);
    G2_SetBG1Offset(0, 0);
    if (work->netType != NET_ERR_TYPE_ASSERT) {
        MI_CpuCopy16(G2_GetBG1CharPtr(), work->charBuf, SAVED_CHAR_SIZE);
        MI_CpuCopy16(G2_GetBG1ScrPtr(), work->screenBuf, SAVED_SCREEN_SIZE);
        MI_CpuCopy16((void *)HW_BG_PLTT, work->plttBuf, SAVED_PLTT_SIZE);
    }
    work->blend = reg_G2_BLDCNT;
    // Game Freak's bug: the sub screen's blend control is saved into blend too, so subBlend is never set
    work->blend = reg_G2S_DB_BLDCNT;
    G2_BlendNone();
    G2S_BlendNone();
    work->subEnabledBGs = GFL_BGSysGetEnabledBGsB();
    GFL_BGSysSetEnabledBGsB(0);
    work->subBackdrop = *(u16 *)HW_DB_BG_PLTT;
    *(u16 *)HW_DB_BG_PLTT = GX_RGB(10, 23, 31);
    if (g_GflNetErr.netType == NET_ERR_TYPE_ASSERT) {
        loadErrorBackground(ERROR_WINDOW_ASSERT);
    } else {
        loadErrorBackground(ERROR_WINDOW_NET);
        GFL_NetErrDispMessage(fatal, disconnecting);
    }
    GX_SetMasterBrightness(0);
    GXS_SetMasterBrightness(0);
}


// Puts back what the screens showed
static void GFL_NetErrRestore(BOOL keepDark) {
    GFLNetErrWork *work = &g_GflNetErr;
    int x;
    int y;

    if (!GFL_NetErrIsReady()) {
        return;
    }
    GX_SetMasterBrightness(-16);
    GXS_SetMasterBrightness(-16);
    *(u16 *)HW_DB_BG_PLTT = work->subBackdrop;
    GFL_BGSysSetEnabledBGsB(work->subEnabledBGs);
    reg_G2_BLDCNT = work->blend;
    reg_G2S_DB_BLDCNT = work->subBlend;
    MI_CpuCopy16(work->charBuf, G2_GetBG1CharPtr(), SAVED_CHAR_SIZE);
    MI_CpuCopy16(work->screenBuf, G2_GetBG1ScrPtr(), SAVED_SCREEN_SIZE);
    MI_CpuCopy16(work->plttBuf, (void *)HW_BG_PLTT, SAVED_PLTT_SIZE);
    G2_SetBG1Control(work->bg1Control.screenSize, work->bg1Control.colorMode, work->bg1Control.screenBase,
                     work->bg1Control.charBase, work->bg1Control.bgExtPltt);
    G2_BG1Mosaic(work->bg1Control.mosaic);
    func_0203d828(&x, &y);
    G2_SetBG1Offset(x, y);
    GX_SetGraphicsMode(work->dispCnt.dispMode, work->dispCnt.bgMode, work->dispCnt.bg0_2d3d);
    GX_SetVisiblePlane(work->dispCnt.visiblePlane);
    GX_SetVisibleWnd(work->dispCnt.visibleWnd);
    GXS_SetVisibleWnd(work->subVisibleWnd);
    GX_ResetBankForSubBG();
    GX_ResetBankForBG();
    GX_SetBankForBG(work->bgBanks);
    GX_SetBankForSubBG(work->subBgBanks);
    GX_HBlankIntr(work->hBlankIrq);
    GX_VBlankIntr(work->vBlankIrq);
    OS_WaitIrq(TRUE, OS_IE_V_BLANK);
    if (!keepDark) {
        GX_SetMasterBrightness(work->brightness);
        GXS_SetMasterBrightness(work->subBrightness);
    }
}

// Whether GFL_NetErrInit has run
static BOOL GFL_NetErrIsReady(void) {
    if (g_GflNetErr.charBuf != NULL) {
        return TRUE;
    }
    return FALSE;
}

void loadErrorBackground(int window) {
    void *file;
    NNSG2dCharacterData *chars;
    NNSG2dScreenData *screen;
    NNSG2dPaletteData *palette;
    NNSG2dPaletteCompressInfo *compressInfo;
    u32 arcId;

    MIi_CpuClear32(0, G2_GetBG1CharPtr(), SAVED_CHAR_SIZE);

    arcId = FATAL_ERROR_G2D_RESIDS[window].arcId;
    file = GFL_ArcSysReadHeapNewLZ(arcId, FATAL_ERROR_G2D_RESIDS[window].charId, FALSE, HEAPID_USER);
    if (NNS_G2dGetUnpackedBGCharacterData(file, &chars)) {
        DC_FlushRange(chars->pRawData, chars->szByte);
        MI_CpuCopy16(chars->pRawData, G2_GetBG1CharPtr(), chars->szByte);
    }
    GFL_HeapFree(file);

    file = GFL_ArcSysReadHeapNewLZ(arcId, FATAL_ERROR_G2D_RESIDS[window].screenId, FALSE, HEAPID_USER);
    if (NNS_G2dGetUnpackedScreenData(file, &screen)) {
        DC_FlushRange(screen->rawData, screen->szByte);
        MI_CpuCopy16(screen->rawData, G2_GetBG1ScrPtr(), screen->szByte);
    }
    GFL_HeapFree(file);

    file = GFL_ArcSysReadHeapNewLZ(arcId, FATAL_ERROR_G2D_RESIDS[window].plttId, FALSE, HEAPID_USER);
    NNS_G2dGetUnpackedPaletteCompressInfo(file, &compressInfo);
    if (NNS_G2dGetUnpackedPaletteData(file, &palette)) {
        DC_FlushRange(palette->pRawData, SAVED_PLTT_SIZE);
        MI_CpuCopy16(palette->pRawData, (void *)HW_BG_PLTT, SAVED_PLTT_SIZE);
    }
    GFL_HeapFree(file);
}

// The message of a Wi-Fi error. The second argument is unused
u32 func_02010158(GFLNetErrorInfo *info, u32 message) {
    switch (info->type) {
    case NET_ERR_INFO_TYPE_FIRST:
    case NET_ERR_INFO_TYPE_FIRST + 9:
        return WifiError_Text_013;
    case NET_ERR_INFO_TYPE_FIRST + 3:
        return WifiError_Text_040;
    case NET_ERR_INFO_TYPE_FIRST + 4:
    case NET_ERR_INFO_TYPE_FIRST + 5:
    case NET_ERR_INFO_TYPE_FIRST + 6:
    case NET_ERR_INFO_TYPE_FIRST + 7:
    case NET_ERR_INFO_TYPE_FIRST + 8:
        return WifiError_Text_014;
    case NET_ERR_INFO_TYPE_FIRST + 2:
        return WifiError_Text_040;
    }
    if (func_0202eb0c()) {
        if (g_GflNetErr.message >= NET_ERR_MESSAGE_TABLE_MAX) {
            return WifiError_Text_011;
        }
        return data_0209ded0[g_GflNetErr.message];
    }
    if (g_GflNetErr.message >= NET_ERR_MESSAGE_TABLE_MAX) {
        return WifiError_Text_011;
    }
    return data_0209deb0[g_GflNetErr.message];
}

// Lays the text area's tiles out on the screen, and prints the error's message in them
void GFL_NetErrDispMessage(u32 fatal, BOOL disconnecting) {
    GFLNetErrWork *work = &g_GflNetErr;
    int x;
    int y;
    u16 *screen;
    u16 tile;
    GFLBitmap *bitmap;
    WordSet *wordSet;
    Font *font;
    MsgData *msgData;
    u32 message;
    StrBuf *src;
    StrBuf *str;

    screen = G2_GetBG1ScrPtr();
    tile = FIRST_TEXT_TILE;
    for (y = TEXT_TOP; y < TEXT_TOP + TEXT_HEIGHT; y++) {
        for (x = TEXT_LEFT; x < TEXT_LEFT + TEXT_WIDTH; x++) {
            screen[x + y * 32] = tile;
            tile++;
        }
    }
    MIi_CpuClear16(TEXT_BACKGROUND * 0x1111, (u8 *)G2_GetBG1CharPtr() + FIRST_TEXT_TILE * TILE_SIZE,
                 TEXT_WIDTH * TEXT_HEIGHT * TILE_SIZE);
    bitmap = GFL_BitmapWrapVRAM((u8 *)G2_GetBG1CharPtr() + FIRST_TEXT_TILE * TILE_SIZE, TEXT_WIDTH, TEXT_HEIGHT,
                                TILE_SIZE, HEAPID_USER);
    wordSet = GFL_WordSetSystemCreateDefault(HEAPID_USER);
    font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, HEAPID_USER);
    if (disconnecting == TRUE) {
        msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_WIFI_ERROR, HEAPID_USER);
        message = WifiError_Text_064;
    } else if (work->netType == GFL_NET_TYPE_WIFI || work->netType == GFL_NET_TYPE_WIFI_LOBBY ||
               work->netType == GFL_NET_TYPE_WIFI_GTS) {
        WordSetNumber(wordSet, 0, work->info.code, 5, 2, TRUE);
        msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_WIFI_ERROR, HEAPID_USER);
        if (work->messageGiven) {
            message = work->message;
            work->messageGiven = FALSE;
        } else {
            message = func_02010158(&work->info, work->message);
        }
    } else {
        WordSetNumber(wordSet, 0, work->stateError, 5, 2, TRUE);
        msgData =
            GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_STARTMENU_GFL_NET_ERR_DISP_MESSAGE, HEAPID_USER);
        if (fatal == TRUE) {
            message = StartmenuGflNetErrDispMessage_Text_001;
        } else {
            message = StartmenuGflNetErrDispMessage_Text_000;
        }
    }
    src = GFL_MsgDataLoadStrbufNew(msgData, message);
    str = GFL_StrBufCreate(0x189, HEAPID_USER);
    GFL_WordSetFormatStrbuf(wordSet, str, src);
    GFL_TextRendererDrawToBitmapEx(bitmap, 0, 0, str, font, PRINT_COLOR(4, 11, TEXT_BACKGROUND));
    GFL_StrBufFree(str);
    GFL_StrBufFree(src);
    GFL_MsgDataFree(msgData);
    GFL_FontFree(font);
    GFL_WordSetSystemFree(wordSet);
    GFL_BitmapFree(bitmap);
}

static void GFL_NetErrGameUpdate(void) {
    func_020053ec();
    GCTX_HIDUpdate();
    GFL_SndUpdate();
    PokeVoice_Update();
}


