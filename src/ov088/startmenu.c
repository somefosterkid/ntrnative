// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov162/startmenu.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "constants/version.h"
#include "app/dwc_utility.h"
#include "app/dwc_raputil.h"
#include "app/gsync.h"
#include "app/mictest.h"
#include "app/startmenu.h"
#include "app/mb_parent.h"
#include "app/title.h"
#include "app/unova_link.h"
#include "app/wifibattlematch.h"
#include "constants/arc.h"
#include "constants/sound.h"
#include "constants/text_banks.h"
#include "field/zone.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/graphics.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/net.h"
#include "gfl/overlay.h"
#include "gfl/proc.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "gfl/tcbl.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "nitro/os.h"
#include "save/dream_world.h"
#include "save/event_work.h"
#include "save/key_info.h"
#include "save/player_info.h"
#include "save/pokedex.h"
#include "save/save_control.h"
#include "save/save_outside.h"
#include "system/app_keycursor.h"
#include "system/bgwinfrm.h"
#include "system/blink_palanm.h"
#include "system/bmp_menu.h"
#include "system/bmp_menulist.h"
#include "system/bmp_winframe.h"
#include "system/dsi.h"
#include "system/game_beacon.h"
#include "system/game_system.h"
#include "system/gf_font.h"
#include "system/printsys.h"
#include "system/text_speed.h"
#include "system/wipe.h"
#include "system/wordset.h"
#include "text/system/startmenu_fieldmap_ctrl_hybrid.h"
#include "text/system/wifi_error.h"
#include "field/beacon_view.h"

PlayerInfo *func_02008730(SaveControl *save);
#include "save/player_savedata.h"
#include "save/wifi_list.h"
#include "system/str_tool.h"
#include "save/playtime.h"
#include "save/trainer_data_savedata.h"

// The menu after the title screen. Its items scroll on the main engine's BGs 1 and 2, and the sub engine shows the
// saved game. Picking an item ends the menu, and its exit starts what the item leads to

enum {
    STATE_SETUP,
    STATE_FREE,
    STATE_WAIT_WIPE,
    STATE_BLINK,
    STATE_WAIT_PRINT,
    STATE_SELECT,
    STATE_SCROLL,
    STATE_CONTINUE,
    STATE_NEW_GAME,
    STATE_WFC_SETTINGS,
    STATE_BROKEN_SAVE,
    STATE_LEAVE,
    STATE_END,
};

// The items, in the order of their names in the message file. The list of items ends with ITEM_NONE, which is also
// the selection that goes back to the title screen
enum {
    ITEM_CONTINUE,
    ITEM_NEW_GAME,
    ITEM_MYSTERY_GIFT,
    ITEM_BATTLE_COMPETITION,
    ITEM_GAME_SYNC_SETTINGS,
    ITEM_WFC_SETTINGS,
    ITEM_MIC_TEST,
    ITEM_UNOVA_LINK,
    ITEM_NONE,
    ITEM_COUNT = ITEM_NONE,
};

// The windows: the name of Continue and the saved game it shows, then the name of each other item
#define WINDOW_CONTINUE 0
#define WINDOW_PLAYER_NAME 1
#define WINDOW_LOCATION 2
#define WINDOW_TIME 3
#define WINDOW_POKEDEX 4
#define WINDOW_BADGES 5
#define WINDOW_ITEMS 6
#define WINDOW_MESSAGE 13
#define WINDOW_COUNT 14

// The cell actors: the player, markers of new items, then the marker of the WFC settings
#define ACTOR_PLAYER 0
#define ACTOR_NEW_FIRST 1
#define ACTOR_BATTLE_COMPETITION 2
#define ACTOR_GAME_SYNC_SETTINGS 3
#define ACTOR_UNOVA_LINK 4
#define ACTOR_NEW_LAST 4
#define ACTOR_WFC_SETTINGS 5
#define ACTOR_COUNT 6

// The graphics of the actors
enum {
    OBJ_RES_PLAYER,
    OBJ_RES_NEW,
    OBJ_RES_WFC_SETTINGS,
    OBJ_RES_COUNT,
};

// The width of an item in tiles
#define ITEM_WIDTH 26
// The screen row that the items start at, and the row that the list scrolls to keep the cursor within
#define FIRST_ITEM_ROW 2
#define LAST_ITEM_ROW 24
// Frames to scroll the list by one item
#define SCROLL_FRAMES 3
#define SCROLL_HEIGHT 512

// Palettes of the items' screens
#define ITEM_PALETTE_NORMAL 1
#define ITEM_PALETTE_SELECTED 2
#define ITEM_PALETTE_BLINK 3

typedef struct {
    u16 screenFile;
    u16 rows;
} ItemLayout;

typedef struct {
    u8 bg;
    u8 x;
    u8 y;
    u8 width;
    u8 height;
    u8 palette;
} WindowLayout;

// The messages about save data that was erased, for the blocks that func_020071e8 reports
typedef struct {
    u32 blocks;
    u32 message;
} BrokenSaveMessage;

typedef struct {
    SaveControl *save;
    PlayerInfo *playerInfo;
    TrainerGameInfoSave *gameInfo;
    CGearSettings *unkC;
    EventWork *eventWork;
    TCB *vblankTask;
    ClActUnit *clactUnit;
    ClActor *actors[ACTOR_COUNT];
    u32 chars[OBJ_RES_COUNT];
    u32 palettes[OBJ_RES_COUNT];
    u32 cellAnims[OBJ_RES_COUNT];
    Font *font;
    MsgData *msgData;
    WordSet *wordSet;
    StrBuf *strbuf;
    PrintQueue *printQueue;
    PrintStream *printStream;
    TCBExManager *tcbManager;
    KeyCursor *keyCursor;
    BOOL continued;
    BmpMenu *dialog;
    PrintWindow windows[WINDOW_COUNT];
    PrintWindow notice;
    u16 *itemScreens[ITEM_COUNT];
    BGWinFrame *textScreens;
    void *unk11C;
    u8 items[ITEM_COUNT];
    u8 cursor;
    u8 selection;
    u8 seq;
    u8 wait;
    u8 cgearOff;
    u8 blinkSeq;
    u8 blinkWait;
    s8 cursorRow;
    s32 scrollY;
    u32 scrollFrames;
    s32 scrollSpeed;
    u32 state;
    u32 blinkNextState;
    u32 wipeNextState;
    u32 brokenBlocks;
    u32 brokenIndex;
} StartMenuWork;

void func_020139b0(HeapID heapId);
DreamWorldSave *getDreamWorldStuffAddress(SaveControl *save);
u8 func_020092ec(DreamWorldSave *dreamWorld);
void func_0200c2f0(TrainerGameInfoSave *info, u32 item, u32 bits);
void GFL_BGSysSetEnabledBGsA(u32 enabled);
void GFL_BGSysSetEnabledBGsB(u32 enabled);
void GFL_BGSysSetDisplayLayout(u32 layout);
BOOL func_0203d210(void);

BOOL func_ov088_021b9f94(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov088_021b9fe4(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov088_021ba014(GameProc *proc, u32 *state, void *param, void *work);
static u32 func_ov088_021ba114(StartMenuWork *wk);
static u32 func_ov088_021ba1a0(StartMenuWork *wk);
static u32 func_ov088_021ba218(StartMenuWork *wk);
u32 func_ov088_021ba230(StartMenuWork *wk);
u32 func_ov088_021ba2c8(StartMenuWork *wk);
static u32 func_ov088_021ba338(StartMenuWork *wk);
u32 func_ov088_021ba4a0(StartMenuWork *wk);
static u32 func_ov088_021ba540(StartMenuWork *wk);
u32 func_ov088_021ba6b0(StartMenuWork *wk);
u32 func_ov088_021ba7ac(StartMenuWork *wk);
u32 func_ov088_021ba880(StartMenuWork *wk);
static u32 func_ov088_021ba984(StartMenuWork *wk);
static void func_ov088_021ba990(TCB *tcb, void *data);
static void func_ov088_021ba9b0(StartMenuWork *wk);
static void func_ov088_021ba9c8(StartMenuWork *wk);
static void func_ov088_021ba9d4(void);
static void func_ov088_021ba9e8(void);
static void func_ov088_021bab20(void);
static void func_ov088_021bab64(void);
static void func_ov088_021bac24(StartMenuWork *wk);
static void func_ov088_021bac64(StartMenuWork *wk);
void func_ov088_021bac74(StartMenuWork *wk);
void func_ov088_021bacd8(StartMenuWork *wk);
static void func_ov088_021bad08(StartMenuWork *wk);
static void func_ov088_021bb164(StartMenuWork *wk);
void func_ov088_021bb184(StartMenuWork *wk);
static void func_ov088_021bb328(StartMenuWork *wk);
void func_ov088_021bb380(StartMenuWork *wk);
static void func_ov088_021bb474(StartMenuWork *wk);
static void func_ov088_021bb484(void);
void func_ov088_021bb4b4(StartMenuWork *wk);
void func_ov088_021bb544(StartMenuWork *wk);
static void func_ov088_021bb5c4(StartMenuWork *wk);
static void func_ov088_021bb5e0(StartMenuWork *wk, u32 actor, s16 y);
void func_ov088_021bb608(StartMenuWork *wk);
static void func_ov088_021bb798(StartMenuWork *wk, u32 item, u32 row);
static void func_ov088_021bb810(StartMenuWork *wk, u32 item, s8 row, u8 palette);
s8 func_ov088_021bb840(StartMenuWork *wk, s8 row);
void func_ov088_021bb858(StartMenuWork *wk, s32 dy);
BOOL func_ov088_021bb890(StartMenuWork *wk, s32 dir);
void func_ov088_021bb9dc(StartMenuWork *wk, BOOL visible);
static void func_ov088_021bba64(u8 x, u8 y, u8 width, u8 height, u8 bg);
static void func_ov088_021bbb84(u8 x, u8 y, u8 width, u8 height, u8 bg);
static void func_ov088_021bbbac(StartMenuWork *wk);
static void func_ov088_021bbd10(StartMenuWork *wk, BOOL leaving);
static void func_ov088_021bbd80(StartMenuWork *wk);
static void func_ov088_021bbe3c(StartMenuWork *wk);
void func_ov088_021bbe48(StartMenuWork *wk);
static void func_ov088_021bbf1c(StartMenuWork *wk, u32 messageId);
static void func_ov088_021bbf98(StartMenuWork *wk);
BOOL func_ov088_021bbfa8(StartMenuWork *wk);
static void func_ov088_021bc024(StartMenuWork *wk);
u32 func_ov088_021bc054(StartMenuWork *wk, u32 next);
u32 func_ov088_021bc080(StartMenuWork *wk, u32 next);
static u32 func_ov088_021bc0ac(StartMenuWork *wk, u32 next);
static BOOL func_ov088_021bc0c8(StartMenuWork *wk);

// Declared in an order that gives the original layout of the data (tools/decomp/rodata_order.py)

// pokebw2's sWFCSettingsActorSetup
static const ClActorSetup data_ov088_021bd5d4 = { 208, 0, 1, 0, 1 };

// pokebw2's sPlayerActorSetup
static const ClActorSetup data_ov088_021bd5dc = { 48, 64, 1, 0, 1 };

// pokebw2's sNewActorSetup
static const ClActorSetup data_ov088_021bd5e4 = { 208, 0, 0, 0, 1 };

// pokebw2's sWindowLayouts
static const WindowLayout data_ov088_021bd7c4[WINDOW_COUNT] = {
#include "asset/startmenu_data_ov088_021bd7c4.inc"
};

// pokebw2's sVRAMConfig
static const BGSysVRAMConfig data_ov088_021bd60c = {
    GX_VRAM_BG_128_A,  GX_VRAM_BGEXTPLTT_NONE,  GX_VRAM_SUB_BG_128_C,        GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_128_B, GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_128_D,       GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_NONE,  GX_VRAM_TEXPLTT_NONE,    GX_OBJVRAMMODE_CHAR_1D_128K, GX_OBJVRAMMODE_CHAR_1D_128K,
};

// pokebw2's sStateFuncs
static u32 (*const data_ov088_021bd63c[])(StartMenuWork *wk) = {
    func_ov088_021ba114, func_ov088_021ba1a0, func_ov088_021ba218, func_ov088_021ba230,
    func_ov088_021ba2c8, func_ov088_021ba338, func_ov088_021ba4a0, func_ov088_021ba540,
    func_ov088_021ba6b0, func_ov088_021ba7ac, func_ov088_021ba880, func_ov088_021ba984,
};

const GameProcFunctions data_ov088_021bd66c = { func_ov088_021b9f94, func_ov088_021b9fe4, func_ov088_021ba014 };

// pokebw2's sItemLayouts
static const ItemLayout data_ov088_021bd5ec[ITEM_COUNT] = {
#include "asset/startmenu_data_ov088_021bd5ec.inc"
};

// pokebw2's sBrokenSaveMessages
static const BrokenSaveMessage data_ov088_021bd784[] = {
    { 0x2, StartmenuFieldmapCtrlHybrid_Text_028 },
    { 0x3c, StartmenuFieldmapCtrlHybrid_Text_029 },
    { 0x40, StartmenuFieldmapCtrlHybrid_Text_030 },
    { 0x80, StartmenuFieldmapCtrlHybrid_Text_031 },
    { 0x100, StartmenuFieldmapCtrlHybrid_Text_032 },
    { 0x200, StartmenuFieldmapCtrlHybrid_Text_033 },
    { 0x400, StartmenuFieldmapCtrlHybrid_Text_034 },
    { 0x800, StartmenuFieldmapCtrlHybrid_Text_035 },
};

// pokebw2's StartMenu_VBlank


BOOL func_ov088_021b9f94(GameProc *proc, u32 *state, void *param, void *work) {
    StartMenuWork *wk;

    GFL_HeapCreateChild(1, 0xb, 0x80000);
    wk = GFL_ProcInitSubsystem(proc, sizeof(StartMenuWork), 0xb);
    MI_CpuFill8(wk, 0, sizeof(StartMenuWork));
    wk->save = SaveControl_GetInstance();
    wk->playerInfo = func_02008730(wk->save);
    wk->gameInfo = getTrainerGameInfoAddress(wk->save);
    wk->unkC = func_02009210(wk->save);
    wk->eventWork = getConstDataBlock(wk->save);
    return TRUE;
}

BOOL func_ov088_021b9fe4(GameProc *proc, u32 *state, void *param, void *work) {
    StartMenuWork *wk = work;

    wk->state = data_ov088_021bd63c[wk->state](wk);
    if (wk->state == STATE_END) {
        return TRUE;
    }
    GFL_TCBExMgrUpdate(wk->tcbManager);
    func_0204a600();
    return FALSE;
}

BOOL func_ov088_021ba014(GameProc *proc, u32 *state, void *param, void *work) {
    StartMenuWork *wk = work;
    u8 selection = wk->selection;
    u8 cgearOff = wk->cgearOff;
    WifiBattleMatchParam *wifiParam;
    UnovaLinkParam *unovaLinkParam;

    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(0xb);
    switch (selection) {
    case ITEM_CONTINUE:
        func_020274c8();
        if (cgearOff == 0) {
            func_ov088_021bc114();
        } else {
            func_ov088_021bc0fc();
        }
        break;
    case ITEM_NEW_GAME:
        func_020274c8();
        func_ov088_021bc0e4();
        break;
    case ITEM_MYSTERY_GIFT:
        GCTX_ProcMgrReplaceProc(OVERLAY_ID(121), &data_ov121_021df538, NULL);
        break;
    case ITEM_BATTLE_COMPETITION:
        wifiParam = GFL_HeapAllocate(1, sizeof(WifiBattleMatchParam), FALSE, "startmenu.c", 0x200);
        MI_CpuFill8(wifiParam, 0, sizeof(WifiBattleMatchParam));
        wifiParam->unk8 = 0;
        wifiParam->unk10 = 1;
        GCTX_ProcMgrReplaceProc(OVERLAY_ID(199), &data_ov199_02213c4c, wifiParam);
        break;
    case ITEM_GAME_SYNC_SETTINGS:
        GCTX_ProcMgrReplaceProc(OVERLAY_NONE, &data_0209def0, NULL);
        break;
    case ITEM_WFC_SETTINGS:
        GCTX_ProcMgrReplaceProc(OVERLAY_ID(16), &data_ov016_021d77ac, NULL);
        break;
    case ITEM_MIC_TEST:
        GCTX_ProcMgrReplaceProc(OVERLAY_ID(168), &data_ov168_02184904, NULL);
        break;
    case ITEM_UNOVA_LINK:
        unovaLinkParam = GFL_HeapAllocate(1, sizeof(UnovaLinkParam), TRUE, "startmenu.c", 0x216);
        unovaLinkParam->unk0 = 1;
        GCTX_ProcMgrReplaceProc(OVERLAY_ID(107), &MB_PARENT_PROC_FUNCTIONS, unovaLinkParam);
        break;
    default:
        GCTX_ProcMgrReplaceProc(OVERLAY_ID(88), &data_ov088_021bd574, NULL);
        break;
    }
    return TRUE;
}



static u32 func_ov088_021ba114(StartMenuWork *wk) {
    DreamWorldSave *dreamWorld;

    dreamWorld = getDreamWorldStuffAddress(wk->save);
    if (func_020092ec(dreamWorld) == 1) {
        func_0200c2f0(wk->gameInfo, 2, 2);
    }
    GFL_BGSysSetEnabledBGsA(0);
    GFL_BGSysSetEnabledBGsB(0);
    reg_G2_BLDCNT = 0;
    reg_G2S_DB_BLDCNT = 0;
    GFL_BGSysSetDisplayLayout(1);
    func_ov088_021ba9d4();
    func_ov088_021ba9e8();
    func_ov088_021bab64();
    func_ov088_021bac74(wk);
    func_ov088_021bb4b4(wk);
    func_ov088_021bb544(wk);
    func_ov088_021bad08(wk);
    func_ov088_021bb184(wk);
    func_ov088_021bb380(wk);
    func_ov088_021bb608(wk);
    func_ov088_021bac24(wk);
    func_ov088_021bb484();
    func_ov088_021ba9b0(wk);
    return 4;
}

static u32 func_ov088_021ba1a0(StartMenuWork *wk) {
    switch (wk->seq) {
    case 0:
        wk->seq++;
    case 1:
        if (func_0203d210()) {
            func_ov088_021ba9c8(wk);
            func_ov088_021bac64(wk);
            func_ov088_021bb474(wk);
            func_ov088_021bb328(wk);
            func_ov088_021bb164(wk);
            func_ov088_021bb5c4(wk);
            func_ov088_021bacd8(wk);
            func_ov088_021bab20();
            G2_BlendNone();
            G2S_BlendNone();
            GFL_BGSysSetEnabledBGsA(0);
            GFL_BGSysSetEnabledBGsB(0);
            return STATE_END;
        }
        break;
    }
    return STATE_FREE;
}



static u32 func_ov088_021ba218(StartMenuWork *wk) {
    if (GFL_WipeIsFinished() == TRUE) {
        return wk->wipeNextState;
    }
    return 2;
}

u32 func_ov088_021ba230(StartMenuWork *wk) {
    switch (wk->blinkSeq) {
    case 0:
    case 2:
        if (wk->blinkWait == 0) {
            func_ov088_021bb810(wk, wk->items[wk->cursor], func_ov088_021bb840(wk, wk->cursorRow), ITEM_PALETTE_BLINK);
            wk->blinkWait = 4;
            wk->blinkSeq++;
        } else {
            wk->blinkWait--;
        }
        break;
    case 1:
    case 3:
        if (wk->blinkWait == 0) {
            func_ov088_021bb810(wk, wk->items[wk->cursor], func_ov088_021bb840(wk, wk->cursorRow), ITEM_PALETTE_NORMAL);
            wk->blinkWait = 4;
            wk->blinkSeq++;
        } else {
            wk->blinkWait--;
        }
        break;
    case 4:
        return wk->blinkNextState;
    }
    return STATE_BLINK;
}

u32 func_ov088_021ba2c8(StartMenuWork *wk) {
    u32 i;

    func_0201c4e4(wk->printQueue);
    for (i = 0; i < WINDOW_COUNT; i++) {
        PrintWindow_Flush(&wk->windows[i], wk->printQueue);
    }
    if (func_0201c6b4(wk->printQueue) == TRUE) {
        if (func_ov088_021bc0c8(wk) == TRUE) {
            return STATE_BROKEN_SAVE;
        }
        return func_ov088_021bc054(wk, STATE_SELECT);
    }
    return STATE_WAIT_PRINT;
}

static u32 func_ov088_021ba338(StartMenuWork *wk) {
    func_0202ea40(wk->unk11C);
    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_A) {
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        switch (wk->items[wk->cursor]) {
        case ITEM_CONTINUE:
            wk->selection = wk->items[wk->cursor];
            return func_ov088_021bc0ac(wk, STATE_CONTINUE);
        case ITEM_NEW_GAME:
            return func_ov088_021bc0ac(wk, STATE_NEW_GAME);
        case ITEM_MYSTERY_GIFT:
            wk->selection = wk->items[wk->cursor];
            return func_ov088_021bc0ac(wk, STATE_LEAVE);
        case ITEM_BATTLE_COMPETITION:
            func_0200c2f0(wk->gameInfo, 1, 2);
            wk->selection = wk->items[wk->cursor];
            return func_ov088_021bc0ac(wk, STATE_LEAVE);
        case ITEM_GAME_SYNC_SETTINGS:
            func_0200c2f0(wk->gameInfo, 2, 2);
            wk->selection = wk->items[wk->cursor];
            return func_ov088_021bc0ac(wk, STATE_LEAVE);
        case ITEM_UNOVA_LINK:
            func_0200c2f0(wk->gameInfo, 3, 2);
            wk->selection = wk->items[wk->cursor];
            return func_ov088_021bc0ac(wk, STATE_LEAVE);
        case ITEM_WFC_SETTINGS:
            return func_ov088_021bc0ac(wk, STATE_WFC_SETTINGS);
        case ITEM_MIC_TEST:
            wk->selection = wk->items[wk->cursor];
            return func_ov088_021bc0ac(wk, STATE_LEAVE);
        }
    }
    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_B) {
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        wk->selection = ITEM_NONE;
        return func_ov088_021bc080(wk, STATE_FREE);
    }
    if (GCTX_HIDGetTypedKeys() & PAD_KEY_UP) {
        if (func_ov088_021bb890(wk, -1) == TRUE) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            return STATE_SCROLL;
        }
    }
    if (GCTX_HIDGetTypedKeys() & PAD_KEY_DOWN) {
        if (func_ov088_021bb890(wk, 1) == TRUE) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            return STATE_SCROLL;
        }
    }
    return STATE_SELECT;
}

u32 func_ov088_021ba4a0(StartMenuWork *wk) {
    if (wk->scrollFrames == SCROLL_FRAMES) {
        wk->scrollFrames = 0;
        wk->scrollSpeed = 0;
        func_0202eb04(wk->unk11C);
        func_ov088_021bb810(wk, wk->items[wk->cursor], func_ov088_021bb840(wk, wk->cursorRow), ITEM_PALETTE_SELECTED);
        return STATE_SELECT;
    }
    wk->scrollY += wk->scrollSpeed;
    if (wk->scrollY < 0) {
        wk->scrollY += SCROLL_HEIGHT;
    } else if (wk->scrollY >= SCROLL_HEIGHT) {
        wk->scrollY -= SCROLL_HEIGHT;
    }
    GFL_BGSysMoveBGReq(1, BG_MOVE_SET_Y, wk->scrollY);
    GFL_BGSysMoveBGReq(2, BG_MOVE_SET_Y, wk->scrollY);
    func_ov088_021bb858(wk, -wk->scrollSpeed);
    wk->scrollFrames++;
    return STATE_SCROLL;
}

static u32 func_ov088_021ba540(StartMenuWork *wk) {
    switch (wk->seq) {
    case 0:
        GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG1 | GX_PLANEMASK_BG2, FALSE);
        func_ov088_021bb9dc(wk, FALSE);
        if (func_020090e0(wk->unkC) == 0) {
            wk->cgearOff = TRUE;
            wk->seq = 0;
            return func_ov088_021bc080(wk, STATE_FREE);
        }
        wk->wait = 8;
        wk->seq++;
        break;
    case 1:
        if (wk->wait != 0) {
            wk->wait--;
        } else {
            func_ov088_021bbf1c(wk, StartmenuFieldmapCtrlHybrid_Text_026);
            func_ov088_021bbd80(wk);
            wk->seq++;
        }
        break;
    case 2:
        if (func_ov088_021bbfa8(wk) == FALSE) {
            func_ov088_021bc024(wk);
            wk->seq++;
        }
        break;
    case 3:
        switch (func_0201febc(wk->dialog)) {
        case 0:
            if (func_0202eb44() == FALSE) {
                func_ov088_021bbf1c(wk, StartmenuFieldmapCtrlHybrid_Text_017);
                wk->seq = 5;
            } else {
                wk->cgearOff = FALSE;
                wk->seq = 4;
            }
            break;
        case -2:
            func_ov088_021bbf1c(wk, StartmenuFieldmapCtrlHybrid_Text_027);
            wk->seq = 7;
            break;
        }
        break;
    case 4:
        func_ov088_021bbe3c(wk);
        func_ov088_021bbf98(wk);
        wk->seq = 0;
        return func_ov088_021bc080(wk, STATE_FREE);
    case 5:
        if (func_ov088_021bbfa8(wk) == FALSE) {
            wk->seq++;
        }
        break;
    case 6:
        if (GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)) {
            wk->cgearOff = TRUE;
            wk->seq = 4;
        }
        break;
    case 7:
        if (func_ov088_021bbfa8(wk) == FALSE) {
            func_ov088_021bc024(wk);
            wk->seq++;
        }
        break;
    case 8:
        switch (func_0201febc(wk->dialog)) {
        case 0:
            wk->cgearOff = TRUE;
            wk->seq = 4;
            break;
        case -2:
            func_ov088_021bbf1c(wk, StartmenuFieldmapCtrlHybrid_Text_026);
            wk->seq = 2;
            break;
        }
        break;
    }
    func_0201c4e4(wk->printQueue);
    PrintWindow_Flush(&wk->notice, wk->printQueue);
    return STATE_CONTINUE;
}

u32 func_ov088_021ba6b0(StartMenuWork *wk) {
    switch (wk->seq) {
    case 0:
        if (wk->items[0] == ITEM_NEW_GAME) {
            wk->selection = ITEM_NEW_GAME;
            wk->seq = 0;
            return func_ov088_021bc080(wk, STATE_FREE);
        }
        func_ov088_021bbbac(wk);
        wk->seq++;
        break;
    case 1:
        if (func_0201c6b4(wk->printQueue) == TRUE) {
            wk->seq++;
        }
        break;
    case 2:
        if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_A) {
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            func_ov088_021bbd10(wk, TRUE);
            wk->selection = ITEM_NEW_GAME;
            wk->seq = 0;
            return func_ov088_021bc080(wk, STATE_FREE);
        }
        if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_B) {
            GFL_SndSEPlay(SEQ_SE_CANCEL1);
            func_ov088_021bbd10(wk, FALSE);
            func_ov088_021bb810(wk, wk->items[wk->cursor], func_ov088_021bb840(wk, wk->cursorRow), ITEM_PALETTE_SELECTED);
            wk->seq = 0;
            return STATE_SELECT;
        }
        break;
    }
    func_0201c4e4(wk->printQueue);
    PrintWindow_Flush(&wk->notice, wk->printQueue);
    return STATE_NEW_GAME;
}

u32 func_ov088_021ba7ac(StartMenuWork *wk) {
    if (func_0202eb0c() == TRUE) {
        switch (wk->seq) {
        case 0:
            func_ov088_021bbe48(wk);
            wk->seq++;
            break;
        case 1:
            if (func_0201c6b4(wk->printQueue) == TRUE) {
                wk->seq++;
            }
            break;
        case 2:
            if (GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)) {
                GFL_SndSEPlay(SEQ_SE_DECIDE1);
                func_ov088_021bbd10(wk, FALSE);
                func_ov088_021bb810(wk, wk->items[wk->cursor], func_ov088_021bb840(wk, wk->cursorRow), ITEM_PALETTE_SELECTED);
                wk->seq = 0;
                return STATE_SELECT;
            }
            break;
        }
        func_0201c4e4(wk->printQueue);
        PrintWindow_Flush(&wk->notice, wk->printQueue);
        return STATE_WFC_SETTINGS;
    }
    wk->selection = wk->items[wk->cursor];
    return STATE_LEAVE;
}

u32 func_ov088_021ba880(StartMenuWork *wk) {
    switch (wk->seq) {
    case 0:
        GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG1 | GX_PLANEMASK_BG2 | GX_PLANEMASK_BG3 | GX_PLANEMASK_OBJ, FALSE);
        GFL_BGSysSetBGEnabledB(GX_PLANEMASK_BG0 | GX_PLANEMASK_BG1 | GX_PLANEMASK_BG2 | GX_PLANEMASK_OBJ, FALSE);
        GFL_BGSysResetStdPalette(0, GX_RGB(12, 12, 31));
        GFL_BGSysResetStdPalette(4, GX_RGB(12, 12, 31));
        wk->seq++;
        return func_ov088_021bc054(wk, STATE_BROKEN_SAVE);
    case 1:
        while (TRUE) {
            if (wk->brokenIndex >= NELEMS(data_ov088_021bd784)) {
                func_ov088_021bbf98(wk);
                wk->seq = 3;
                return func_ov088_021bc080(wk, STATE_BROKEN_SAVE);
            }
            if (data_ov088_021bd784[wk->brokenIndex].blocks & wk->brokenBlocks) {
                break;
            }
            wk->brokenIndex++;
        }
        func_ov088_021bbf1c(wk, data_ov088_021bd784[wk->brokenIndex].message);
        wk->brokenIndex++;
        wk->seq++;
        break;
    case 2:
        if (func_ov088_021bbfa8(wk) == FALSE && (GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B))) {
            GFL_SndSEPlay(SEQ_SE_MESSAGE);
            wk->seq = 1;
        }
        break;
    case 3:
        GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG1 | GX_PLANEMASK_BG2 | GX_PLANEMASK_BG3 | GX_PLANEMASK_OBJ, TRUE);
        GFL_BGSysSetBGEnabledB(GX_PLANEMASK_BG0 | GX_PLANEMASK_BG1 | GX_PLANEMASK_BG2 | GX_PLANEMASK_OBJ, TRUE);
        wk->seq = 0;
        return func_ov088_021bc054(wk, STATE_SELECT);
    }
    return STATE_BROKEN_SAVE;
}










static u32 func_ov088_021ba984(StartMenuWork *wk) {
    return func_ov088_021bc080(wk, STATE_FREE);
}

static void func_ov088_021ba990(TCB *tcb, void *data) {
    GFL_BGSysUpdate();
    func_0204a648();
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

// pokebw2's StartMenu_AddVBlankTask
static void func_ov088_021ba9b0(StartMenuWork *wk) {
    wk->vblankTask = GFL_VBlankTCBAdd(func_ov088_021ba990, wk, 0);
}

// pokebw2's StartMenu_RemoveVBlankTask
static void func_ov088_021ba9c8(StartMenuWork *wk) {
    GFL_TCBRemove(wk->vblankTask);
}

// pokebw2's StartMenu_InitVRAM
static void func_ov088_021ba9d4(void) {
    GFL_BGSysInitVRAM(GX_VRAM_LCDC_NONE);
    GFL_BGSysSetVRAMBanks(&data_ov088_021bd60c);
}

static void func_ov088_021ba9e8(void) {
    GFL_BGSysCreate(0xb);
    {
        BGSysLCDConfig lcd = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_2D };
        GFL_BGSysSetLCDConfig(&lcd);
    }
    {
        BGSetup bg0 = { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xf800, GX_BG_CHARBASE_0x00000, 0x8000, GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE };
        GFL_BGSysCreateBG(0, &bg0, BGMODE_TEXT);
    }
    {
        BGSetup bg1 = { 0, 0, 0x2000, 0, BGRES_512x512, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xd000, GX_BG_CHARBASE_0x10000, 0x8000, GX_BG_EXTPLTT_01, 1, GX_BG_AREAOVER_XLU, FALSE };
        GFL_BGSysCreateBG(1, &bg1, BGMODE_TEXT);
    }
    {
        BGSetup bg2 = { 0, 0, 0x2000, 0, BGRES_512x512, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xb000, GX_BG_CHARBASE_0x08000, 0x8000, GX_BG_EXTPLTT_01, 2, GX_BG_AREAOVER_XLU, FALSE };
        GFL_BGSysCreateBG(2, &bg2, BGMODE_TEXT);
    }
    {
        BGSetup bg3 = { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xf000, GX_BG_CHARBASE_0x08000, 0x8000, GX_BG_EXTPLTT_01, 3, GX_BG_AREAOVER_XLU, FALSE };
        GFL_BGSysCreateBG(3, &bg3, BGMODE_TEXT);
    }
    {
        BGSetup bg4 = { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xf800, GX_BG_CHARBASE_0x10000, 0x8000, GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE };
        GFL_BGSysCreateBG(4, &bg4, BGMODE_TEXT);
    }
    {
        BGSetup bg5 = { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xf000, GX_BG_CHARBASE_0x00000, 0x8000, GX_BG_EXTPLTT_01, 1, GX_BG_AREAOVER_XLU, FALSE };
        GFL_BGSysCreateBG(5, &bg5, BGMODE_TEXT);
    }
    {
        BGSetup bg6 = { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xe800, GX_BG_CHARBASE_0x00000, 0x8000, GX_BG_EXTPLTT_01, 2, GX_BG_AREAOVER_XLU, FALSE };
        GFL_BGSysCreateBG(6, &bg6, BGMODE_TEXT);
    }
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG0 | GX_PLANEMASK_BG1 | GX_PLANEMASK_BG2 | GX_PLANEMASK_BG3, TRUE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_BG0 | GX_PLANEMASK_BG1 | GX_PLANEMASK_BG2, TRUE);
}


// pokebw2's StartMenu_FreeBG
static void func_ov088_021bab20(void) {
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG0 | GX_PLANEMASK_BG1 | GX_PLANEMASK_BG2 | GX_PLANEMASK_BG3, FALSE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_BG0 | GX_PLANEMASK_BG1 | GX_PLANEMASK_BG2, FALSE);
    GFL_BGSysReleaseBG(6);
    GFL_BGSysReleaseBG(5);
    GFL_BGSysReleaseBG(4);
    GFL_BGSysReleaseBG(3);
    GFL_BGSysReleaseBG(2);
    GFL_BGSysReleaseBG(1);
    GFL_BGSysReleaseBG(0);
    GFL_BGSysFree();
}

static void func_ov088_021bab64(void) {
    ArcTool *arc;

    arc = GFL_ArcSysCreateFileHandle(0x23, HEAPID_TAIL(0xb));
    GFL_G2DIOLoadArcNCLRDefault(arc, 4, 0, 0, 0xc0, 0xb);
    GFL_BGSysLoadArcNCGRStatic(arc, 3, 2, 0, 0, 1, 0xb);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 2, 3, 0, 0, 1, 0xb);
    GFL_G2DIOLoadArcNCLRDefault(arc, 5, 4, 0, 0xc0, 0xb);
    GFL_BGSysLoadArcNCGRStatic(arc, 1, 5, 0, 0, 1, 0xb);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 0, 6, 0, 0, 1, 0xb);
    GFL_ArcToolFree(arc);
    LoadSysMsgBox(0, 1, 0xe, 0, 0xb);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, 0, 0x1e0, 0x20, 0xb);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, 4, 0x1e0, 0x20, 0xb);
#if GAME_VERSION == VERSION_WHITE
    GFL_BGSysSetScrPaletteNo(3, 0, 0, 0x20, 0x18, 4);
    GFL_BGSysSetScrPaletteNo(6, 0, 0, 0x20, 0x18, 4);
    GFL_BGSysQueueScrLoad(3);
    GFL_BGSysQueueScrLoad(6);
#endif
}

static void func_ov088_021bac24(StartMenuWork *wk) {
    ArcTool *arc;

    arc = GFL_ArcSysCreateFileHandle(0x23, HEAPID_TAIL(0xb));
    wk->unk11C = func_0202e940(0x20, 0x10, 2, 0xb);
    func_0202e9ac(wk->unk11C, arc, 4, 0x20, 0x30);
    GFL_ArcToolFree(arc);
}



// pokebw2's StartMenu_FreeUnk16C
static void func_ov088_021bac64(StartMenuWork *wk) {
    func_0202ea20(wk->unk11C);
}

void func_ov088_021bac74(StartMenuWork *wk) {
    wk->msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_STARTMENU_FIELDMAP_CTRL_HYBRID, 0xb);
    wk->font = GFL_FontCreate(0x17, 0, 0, FALSE, 0xb);
    wk->wordSet = GFL_WordSetSystemCreateDefault(0xb);
    wk->printQueue = func_0201c440(0xb);
    wk->strbuf = GFL_StrBufCreate(0x400, 0xb);
    wk->tcbManager = GFL_TCBExMgrCreate(0xb, 0xb, 1, 4);
    wk->keyCursor = func_02028590(0xf, TRUE, FALSE, 0xb);
}

void func_ov088_021bacd8(StartMenuWork *wk) {
    func_02028604(wk->keyCursor);
    GFL_TCBExMgrFree(wk->tcbManager);
    GFL_StrBufFree(wk->strbuf);
    func_0201c4c0(wk->printQueue);
    GFL_WordSetSystemFree(wk->wordSet);
    GFL_FontFree(wk->font);
    GFL_MsgDataFree(wk->msgData);
}



static void func_ov088_021bad08(StartMenuWork *wk) {
    u32 i;
    StrBuf *str;
    PlayTime *playTime;
    u16 zoneId;
    SaveLocation location;
    u32 placeName;

    BmpWin_InitAllocator(0xb);
    for (i = 0; i < WINDOW_COUNT; i++) {
        const WindowLayout *layout = &data_ov088_021bd7c4[i];

        wk->windows[i].window = BmpWin_CreateDynamic(layout->bg, layout->x, layout->y, layout->width, layout->height, layout->palette, 1);
    }

    str = GFL_MsgDataLoadStrbufNew(wk->msgData, StartmenuFieldmapCtrlHybrid_Text_000);
    PrintWindow_Print(&wk->windows[WINDOW_CONTINUE], wk->printQueue, 0, 4, str, wk->font, PRINT_COLOR(1, 2, 0));
    GFL_StrBufFree(str);

    str = GFL_MsgDataLoadStrbufNew(wk->msgData, StartmenuFieldmapCtrlHybrid_Text_012);
    copyVarForText(wk->wordSet, 0, wk->playerInfo);
    GFL_WordSetFormatStrbuf(wk->wordSet, wk->strbuf, str);
    if (getTrainerGender(wk->playerInfo) == 0) {
        PrintWindow_Print(&wk->windows[WINDOW_PLAYER_NAME], wk->printQueue, 0, 0, wk->strbuf, wk->font, PRINT_COLOR(3, 4, 0));
    } else {
        PrintWindow_Print(&wk->windows[WINDOW_PLAYER_NAME], wk->printQueue, 0, 0, wk->strbuf, wk->font, PRINT_COLOR(5, 6, 0));
    }
    GFL_StrBufFree(str);

    str = GFL_MsgDataLoadStrbufNew(wk->msgData, StartmenuFieldmapCtrlHybrid_Text_016);
    func_020139b0(0x800b);
    if (EventWork_FlagGet(wk->eventWork, 0x966) == TRUE) {
        zoneId = PlayerSave_GetNextSpawnZone(SaveControl_GetPlayerSave(wk->save))->zoneId;
    } else {
        func_0200890c(wk->save, &location);
        zoneId = location.zoneId;
    }
    placeName = ZoneData_GetPlaceNameID(zoneId);
    FreeZoneDataSystem();
    loadLocationNameToStrbuf(wk->wordSet, 0, placeName);
    GFL_WordSetFormatStrbuf(wk->wordSet, wk->strbuf, str);
    PrintWindow_Print(&wk->windows[WINDOW_LOCATION], wk->printQueue, 0, 0, wk->strbuf, wk->font, PRINT_COLOR(1, 2, 0));
    GFL_StrBufFree(str);

    str = GFL_MsgDataLoadStrbufNew(wk->msgData, StartmenuFieldmapCtrlHybrid_Text_009);
    playTime = func_02008748(wk->save);
    WordSetNumber(wk->wordSet, 0, func_0200864c(playTime), 3, 0, TRUE);
    WordSetNumber(wk->wordSet, 1, func_02008650(playTime), 2, 2, TRUE);
    GFL_WordSetFormatStrbuf(wk->wordSet, wk->strbuf, str);
    PrintWindow_Print(&wk->windows[WINDOW_TIME], wk->printQueue, 0, 0, wk->strbuf, wk->font, PRINT_COLOR(1, 2, 0));
    GFL_StrBufFree(str);

    if (EventWork_FlagGet(wk->eventWork, 0x962) == TRUE) {
        str = GFL_MsgDataLoadStrbufNew(wk->msgData, StartmenuFieldmapCtrlHybrid_Text_010);
        WordSetNumber(wk->wordSet, 0, func_0200d0ac(func_0200ca10(wk->save), 0xb), 3, 0, TRUE);
        GFL_WordSetFormatStrbuf(wk->wordSet, wk->strbuf, str);
        PrintWindow_Print(&wk->windows[WINDOW_POKEDEX], wk->printQueue, 0, 0, wk->strbuf, wk->font, PRINT_COLOR(1, 2, 0));
        GFL_StrBufFree(str);
    }

    str = GFL_MsgDataLoadStrbufNew(wk->msgData, StartmenuFieldmapCtrlHybrid_Text_011);
    WordSetNumber(wk->wordSet, 0, getBadgeCount(getTrainerGameInfoAddress(wk->save)), 2, 0, TRUE);
    GFL_WordSetFormatStrbuf(wk->wordSet, wk->strbuf, str);
    PrintWindow_Print(&wk->windows[WINDOW_BADGES], wk->printQueue, 0, 0, wk->strbuf, wk->font, PRINT_COLOR(1, 2, 0));
    GFL_StrBufFree(str);

    str = GFL_MsgDataLoadStrbufNew(wk->msgData, StartmenuFieldmapCtrlHybrid_Text_001);
    PrintWindow_Print(&wk->windows[WINDOW_ITEMS], wk->printQueue, 0, 4, str, wk->font, PRINT_COLOR(1, 2, 0));
    GFL_StrBufFree(str);
    str = GFL_MsgDataLoadStrbufNew(wk->msgData, StartmenuFieldmapCtrlHybrid_Text_002);
    PrintWindow_Print(&wk->windows[WINDOW_ITEMS + 1], wk->printQueue, 0, 4, str, wk->font, PRINT_COLOR(1, 2, 0));
    GFL_StrBufFree(str);
    str = GFL_MsgDataLoadStrbufNew(wk->msgData, StartmenuFieldmapCtrlHybrid_Text_004);
    PrintWindow_Print(&wk->windows[WINDOW_ITEMS + 2], wk->printQueue, 0, 4, str, wk->font, PRINT_COLOR(1, 2, 0));
    GFL_StrBufFree(str);
    str = GFL_MsgDataLoadStrbufNew(wk->msgData, StartmenuFieldmapCtrlHybrid_Text_005);
    PrintWindow_Print(&wk->windows[WINDOW_ITEMS + 3], wk->printQueue, 0, 4, str, wk->font, PRINT_COLOR(1, 2, 0));
    GFL_StrBufFree(str);
    str = GFL_MsgDataLoadStrbufNew(wk->msgData, StartmenuFieldmapCtrlHybrid_Text_003);
    PrintWindow_Print(&wk->windows[WINDOW_ITEMS + 4], wk->printQueue, 0, 4, str, wk->font, PRINT_COLOR(1, 2, 0));
    GFL_StrBufFree(str);
    str = GFL_MsgDataLoadStrbufNew(wk->msgData, StartmenuFieldmapCtrlHybrid_Text_007);
    PrintWindow_Print(&wk->windows[WINDOW_ITEMS + 5], wk->printQueue, 0, 4, str, wk->font, PRINT_COLOR(1, 2, 0));
    GFL_StrBufFree(str);
    str = GFL_MsgDataLoadStrbufNew(wk->msgData, StartmenuFieldmapCtrlHybrid_Text_006);
    PrintWindow_Print(&wk->windows[WINDOW_ITEMS + 6], wk->printQueue, 0, 4, str, wk->font, PRINT_COLOR(1, 2, 0));
    GFL_StrBufFree(str);
}


// pokebw2's StartMenu_FreeWindows
static void func_ov088_021bb164(StartMenuWork *wk) {
    u32 i;

    for (i = 0; i < WINDOW_COUNT; i++) {
        BmpWin_Free(wk->windows[i].window);
    }
    BmpWin_FreeAllocator();
}

void func_ov088_021bb184(StartMenuWork *wk) {
    ClActSysSetup setup = { 0, 0, 0, 512, 4, 124, 4, 124, 0, 3, 3, 3, 0, 16, 16 };
    ArcTool *arc;
    u32 i;

    ClActSys_Create(&setup, &data_ov088_021bd60c, 0xb);
    arc = GFL_ArcSysCreateFileHandle(0x1f, HEAPID_TAIL(0xb));
    if (getTrainerGender(wk->playerInfo) == 0) {
        wk->chars[OBJ_RES_PLAYER] = func_0204a6c8(arc, 5, FALSE, 0, 0xb);
        wk->palettes[OBJ_RES_PLAYER] = func_0204abf0(arc, 4, 0, 0, 0xb);
        wk->cellAnims[OBJ_RES_PLAYER] = func_0204af28(arc, 0, 1, 0xb);
    } else {
        wk->chars[OBJ_RES_PLAYER] = func_0204a6c8(arc, 7, FALSE, 0, 0xb);
        wk->palettes[OBJ_RES_PLAYER] = func_0204abf0(arc, 6, 0, 0, 0xb);
        wk->cellAnims[OBJ_RES_PLAYER] = func_0204af28(arc, 2, 3, 0xb);
    }
    GFL_ArcToolFree(arc);

    arc = GFL_ArcSysCreateFileHandle(ARCID_STARTMENU, HEAPID_TAIL(0xb));
    wk->chars[OBJ_RES_NEW] = func_0204a6c8(arc, 8, TRUE, 0, 0xb);
    wk->palettes[OBJ_RES_NEW] = func_0204abf0(arc, 9, 0, 0x20, 0xb);
    wk->cellAnims[OBJ_RES_NEW] = func_0204af28(arc, 10, 11, 0xb);
    wk->chars[OBJ_RES_WFC_SETTINGS] = func_0204a6c8(arc, 12, TRUE, 0, 0xb);
    wk->palettes[OBJ_RES_WFC_SETTINGS] = func_0204abf0(arc, 13, 0, 0x40, 0xb);
    wk->cellAnims[OBJ_RES_WFC_SETTINGS] = func_0204af28(arc, 14, 15, 0xb);
    GFL_ArcToolFree(arc);

    wk->clactUnit = func_0204b100(6, 0, 0xb);
    wk->actors[ACTOR_PLAYER] = func_0204b294(wk->clactUnit, wk->chars[OBJ_RES_PLAYER], wk->palettes[OBJ_RES_PLAYER],
                                             wk->cellAnims[OBJ_RES_PLAYER], &data_ov088_021bd5dc, 0, 0xb);
    func_0204b3dc(wk->actors[ACTOR_PLAYER], FALSE);
    for (i = ACTOR_NEW_FIRST; i <= ACTOR_NEW_LAST; i++) {
        wk->actors[i] = func_0204b294(wk->clactUnit, wk->chars[OBJ_RES_NEW], wk->palettes[OBJ_RES_NEW],
                                      wk->cellAnims[OBJ_RES_NEW], &data_ov088_021bd5e4, 0, 0xb);
        func_0204b92c(wk->actors[i], TRUE);
        func_0204b3dc(wk->actors[i], FALSE);
    }
    wk->actors[ACTOR_WFC_SETTINGS] = func_0204b294(wk->clactUnit, wk->chars[OBJ_RES_WFC_SETTINGS], wk->palettes[OBJ_RES_WFC_SETTINGS],
                                                   wk->cellAnims[OBJ_RES_WFC_SETTINGS], &data_ov088_021bd5d4, 0, 0xb);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_OBJ, TRUE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
}


// pokebw2's StartMenu_FreeObj
static void func_ov088_021bb328(StartMenuWork *wk) {
    u32 i;

    for (i = 0; i < ACTOR_COUNT; i++) {
        func_0204b3b4(wk->actors[i]);
    }
    func_0204b1cc(wk->clactUnit);
    for (i = 0; i < OBJ_RES_COUNT; i++) {
        func_0204a8d4(wk->chars[i]);
    }
    for (i = 0; i < OBJ_RES_COUNT; i++) {
        func_0204ada4(wk->palettes[i]);
    }
    for (i = 0; i < OBJ_RES_COUNT; i++) {
        func_0204afd8(wk->cellAnims[i]);
    }
    func_0204a5ac();
}

void func_ov088_021bb380(StartMenuWork *wk) {
    u32 i;

    wk->textScreens = func_0202c9b4(0, ITEM_COUNT, 0xb);
    for (i = 0; i < ITEM_COUNT; i++) {
        func_0202ca3c(wk->textScreens, i, 1, ITEM_WIDTH, data_ov088_021bd5ec[i].rows);
    }
    func_0202cebc(wk->textScreens, ITEM_CONTINUE, wk->windows[WINDOW_CONTINUE].window);
    func_0202cebc(wk->textScreens, ITEM_CONTINUE, wk->windows[WINDOW_PLAYER_NAME].window);
    func_0202cebc(wk->textScreens, ITEM_CONTINUE, wk->windows[WINDOW_LOCATION].window);
    func_0202cebc(wk->textScreens, ITEM_CONTINUE, wk->windows[WINDOW_TIME].window);
    func_0202cebc(wk->textScreens, ITEM_CONTINUE, wk->windows[WINDOW_POKEDEX].window);
    func_0202cebc(wk->textScreens, ITEM_CONTINUE, wk->windows[WINDOW_BADGES].window);
    func_0202cebc(wk->textScreens, ITEM_NEW_GAME, wk->windows[WINDOW_ITEMS].window);
    func_0202cebc(wk->textScreens, ITEM_MYSTERY_GIFT, wk->windows[WINDOW_ITEMS + 1].window);
    func_0202cebc(wk->textScreens, ITEM_BATTLE_COMPETITION, wk->windows[WINDOW_ITEMS + 2].window);
    func_0202cebc(wk->textScreens, ITEM_GAME_SYNC_SETTINGS, wk->windows[WINDOW_ITEMS + 3].window);
    func_0202cebc(wk->textScreens, ITEM_WFC_SETTINGS, wk->windows[WINDOW_ITEMS + 4].window);
    func_0202cebc(wk->textScreens, ITEM_MIC_TEST, wk->windows[WINDOW_ITEMS + 5].window);
    func_0202cebc(wk->textScreens, ITEM_UNOVA_LINK, wk->windows[WINDOW_ITEMS + 6].window);
}


// pokebw2's StartMenu_FreeTextScreens
static void func_ov088_021bb474(StartMenuWork *wk) {
    func_0202ca0c(wk->textScreens);
}

// pokebw2's StartMenu_SetBlend
static void func_ov088_021bb484(void) {
    G2_SetBlendAlpha(GX_PLANEMASK_BG2, GX_PLANEMASK_BG3, 16, 3);
    G2S_SetBlendAlpha(GX_PLANEMASK_BG1, GX_PLANEMASK_BG2, 16, 3);
}

void func_ov088_021bb4b4(StartMenuWork *wk) {
    u32 count = 0;

    if (SaveControl_IsDataAlreadyPresent(wk->save) == TRUE) {
        wk->items[count] = ITEM_CONTINUE;
        count++;
    }
    wk->items[count] = ITEM_NEW_GAME;
    (wk->items + count)[1] = ITEM_MYSTERY_GIFT;
    count += 2;
    if (func_0200c308(wk->gameInfo, 1)) {
        wk->items[count] = ITEM_BATTLE_COMPETITION;
        count++;
    }
    if (func_0200c308(wk->gameInfo, 2)) {
        wk->items[count] = ITEM_GAME_SYNC_SETTINGS;
        count++;
    }
    wk->items[count] = ITEM_WFC_SETTINGS;
    (wk->items + count)[1] = ITEM_MIC_TEST;
    count += 2;
    if (func_0200c308(wk->gameInfo, 3)) {
        wk->items[count] = ITEM_UNOVA_LINK;
        count++;
    }
    while (count < ITEM_COUNT) {
        wk->items[count] = ITEM_NONE;
        count++;
    }
}

void func_ov088_021bb544(StartMenuWork *wk) {
    ArcTool *arc;
    NNSG2dScreenData *screen;
    void *file;
    u32 size;
    u32 i;

    arc = GFL_ArcSysCreateFileHandle(ARCID_STARTMENU, HEAPID_TAIL(0xb));
    for (i = 0; i < ITEM_COUNT; i++) {
        size = data_ov088_021bd5ec[i].rows * ITEM_WIDTH * 2;
        wk->itemScreens[i] = GFL_HeapAllocate(0xb, size, FALSE, "startmenu.c", 0x7e9);
        file = GFL_G2DIOReadNSCRArc(arc, data_ov088_021bd5ec[i].screenFile, TRUE, &screen, 0xb);
        MIi_CpuCopy16(screen->rawData, wk->itemScreens[i], size);
        GFL_HeapFree(file);
    }
    GFL_ArcToolFree(arc);
}



// pokebw2's StartMenu_FreeItemScreens
static void func_ov088_021bb5c4(StartMenuWork *wk) {
    u32 i;

    for (i = 0; i < 8; i++) {
        GFL_HeapFree(wk->itemScreens[i]);
    }
}

// pokebw2's StartMenu_SetActorY
static void func_ov088_021bb5e0(StartMenuWork *wk, u32 actor, s16 y) {
    ClActorPos pos;

    func_0204b454(wk->actors[actor], &pos, 0);
    pos.y = y;
    func_0204b404(wk->actors[actor], &pos, 0);
}

void func_ov088_021bb608(StartMenuWork *wk) {
    u32 i;
    s8 row;

    wk->cursor = 0;
    wk->scrollY = 0;
    wk->cursorRow = FIRST_ITEM_ROW;
    row = FIRST_ITEM_ROW;
    for (i = 0; i < ITEM_COUNT; i++) {
        if (wk->items[i] == ITEM_NONE) {
            break;
        }
        func_ov088_021bb798(wk, wk->items[i], row);
        if (wk->items[i] == ITEM_CONTINUE) {
            func_0204b3dc(wk->actors[ACTOR_PLAYER], TRUE);
        } else if (wk->items[i] == ITEM_WFC_SETTINGS) {
            func_ov088_021bb5e0(wk, ACTOR_WFC_SETTINGS, row * 8 + data_ov088_021bd5ec[wk->items[i]].rows * 8 / 2);
            func_0204b3dc(wk->actors[ACTOR_WFC_SETTINGS], TRUE);
        } else if (wk->items[i] == ITEM_BATTLE_COMPETITION) {
            if (func_0200c308(wk->gameInfo, 1) == TRUE) {
                func_ov088_021bb5e0(wk, ACTOR_BATTLE_COMPETITION, row * 8 + data_ov088_021bd5ec[wk->items[i]].rows * 8 / 2);
                func_0204b3dc(wk->actors[ACTOR_BATTLE_COMPETITION], TRUE);
            }
        } else if (wk->items[i] == ITEM_GAME_SYNC_SETTINGS) {
            if (func_0200c308(wk->gameInfo, 2) == TRUE) {
                func_ov088_021bb5e0(wk, ACTOR_GAME_SYNC_SETTINGS, row * 8 + data_ov088_021bd5ec[wk->items[i]].rows * 8 / 2);
                func_0204b3dc(wk->actors[ACTOR_GAME_SYNC_SETTINGS], TRUE);
            }
        } else if (wk->items[i] == ITEM_UNOVA_LINK) {
            if (func_0200c308(wk->gameInfo, 3) == TRUE) {
                func_ov088_021bb5e0(wk, ACTOR_UNOVA_LINK, row * 8 + data_ov088_021bd5ec[wk->items[i]].rows * 8 / 2);
                func_0204b3dc(wk->actors[ACTOR_UNOVA_LINK], TRUE);
            }
        }
        row += data_ov088_021bd5ec[wk->items[i]].rows;
    }
    func_ov088_021bb5e0(wk, ACTOR_PLAYER, 0x40);
    func_ov088_021bb810(wk, wk->items[0], wk->cursorRow, ITEM_PALETTE_SELECTED);
}


// Draws an item's screen on BG 2 and its name on BG 1
// pokebw2's StartMenu_DrawItem
static void func_ov088_021bb798(StartMenuWork *wk, u32 item, u32 row) {
    u16 rows = data_ov088_021bd5ec[item].rows;
    u8 y = row;

    GFL_BGSysLoadScrArea(2, 3, y, ITEM_WIDTH, rows, wk->itemScreens[item], 0, 0, ITEM_WIDTH, rows);
    GFL_BGSysLoadScrArea(1, 3, y, ITEM_WIDTH, rows, func_0202cf84(wk->textScreens, item), 0, 0, ITEM_WIDTH, rows);
    GFL_BGSysQueueScrLoad(1);
    GFL_BGSysQueueScrLoad(2);
}

// pokebw2's StartMenu_SetItemPalette
static void func_ov088_021bb810(StartMenuWork *wk, u32 item, s8 row, u8 palette) {
    GFL_BGSysSetScrPaletteNo(2, 3, row, ITEM_WIDTH, data_ov088_021bd5ec[item].rows, palette);
    GFL_BGSysQueueScrLoad(2);
}

s8 func_ov088_021bb840(StartMenuWork *wk, s8 row) {
    return row + wk->scrollY / 8;
}

void func_ov088_021bb858(StartMenuWork *wk, s32 dy) {
    u32 i;
    ClActorPos pos;

    for (i = 0; i < ACTOR_COUNT; i++) {
        func_0204b454(wk->actors[i], &pos, 0);
        pos.y += (s16)dy;
        func_0204b404(wk->actors[i], &pos, 0);
    }
}

BOOL func_ov088_021bb890(StartMenuWork *wk, s32 dir) {
    u8 item = wk->items[wk->cursor];
    s8 row = 0;
    u16 rows;
    u8 nextItem;
    s8 next;

    wk->scrollFrames = 0;
    if (dir < 0) {
        if (wk->cursor == 0) {
            return FALSE;
        }
        next = wk->cursor + dir;
        rows = data_ov088_021bd5ec[wk->items[next]].rows;
        row = wk->cursorRow - rows;
        func_ov088_021bb810(wk, item, func_ov088_021bb840(wk, wk->cursorRow), ITEM_PALETTE_NORMAL);
        if (wk->items[0] == ITEM_CONTINUE) {
            if (row < FIRST_ITEM_ROW) {
                if (next == 0) {
                    wk->scrollSpeed = -((rows - (wk->cursorRow - FIRST_ITEM_ROW)) / SCROLL_FRAMES * 8);
                    wk->cursorRow = FIRST_ITEM_ROW;
                } else {
                    wk->scrollSpeed = -(rows / SCROLL_FRAMES * 8);
                }
            } else {
                wk->cursorRow = row;
            }
        } else {
            wk->cursorRow = row;
        }
    } else {
        if (wk->cursor + dir >= ITEM_COUNT) {
            return FALSE;
        }
        if (wk->items[wk->cursor + dir] == ITEM_NONE) {
            return FALSE;
        }
        nextItem = wk->items[(s8)(wk->cursor + dir)];
        row = wk->cursorRow + data_ov088_021bd5ec[item].rows;
        func_ov088_021bb810(wk, item, func_ov088_021bb840(wk, wk->cursorRow), ITEM_PALETTE_NORMAL);
        if (wk->items[0] == ITEM_CONTINUE) {
            if (row == LAST_ITEM_ROW) {
                wk->scrollSpeed = data_ov088_021bd5ec[nextItem].rows / SCROLL_FRAMES * 8;
            } else if (row + data_ov088_021bd5ec[nextItem].rows > LAST_ITEM_ROW) {
                wk->scrollSpeed = data_ov088_021bd5ec[nextItem].rows / SCROLL_FRAMES * 8;
            } else {
                wk->cursorRow = row;
            }
        } else {
            wk->cursorRow = row;
        }
    }
    wk->cursor += dir;
    return TRUE;
}


void func_ov088_021bb9dc(StartMenuWork *wk, BOOL visible) {
    u32 i;

    if (visible == FALSE) {
        for (i = 0; i <= ACTOR_WFC_SETTINGS; i++) {
            func_0204b3dc(wk->actors[i], visible);
        }
        return;
    }
    func_0204b3dc(wk->actors[ACTOR_PLAYER], TRUE);
    for (i = 0; i < ITEM_COUNT; i++) {
        if (wk->items[i] == ITEM_NONE) {
            break;
        }
        if (wk->items[i] == ITEM_BATTLE_COMPETITION) {
            if (func_0200c308(wk->gameInfo, 1) == TRUE) {
                func_0204b3dc(wk->actors[ACTOR_BATTLE_COMPETITION], TRUE);
            }
        } else if (wk->items[i] == ITEM_GAME_SYNC_SETTINGS) {
            if (func_0200c308(wk->gameInfo, 2) == TRUE) {
                func_0204b3dc(wk->actors[ACTOR_GAME_SYNC_SETTINGS], TRUE);
            }
        } else if (wk->items[i] == ITEM_UNOVA_LINK) {
            if (func_0200c308(wk->gameInfo, 3) == TRUE) {
                func_0204b3dc(wk->actors[ACTOR_UNOVA_LINK], TRUE);
            }
        } else if (wk->items[i] == ITEM_WFC_SETTINGS) {
            func_0204b3dc(wk->actors[ACTOR_WFC_SETTINGS], TRUE);
        }
    }
}








// Draws the frame of a notice with the characters of the frame loaded to the BG
// pokebw2's StartMenu_DrawFrame
static void func_ov088_021bba64(u8 x, u8 y, u8 width, u8 height, u8 bg) {
    GFL_BGSysFillScrArea(bg, 1, x, y, 1, 1, 1);
    GFL_BGSysFillScrArea(bg, 3, x + width - 1, y, 1, 1, 1);
    GFL_BGSysFillScrArea(bg, 7, x, y + height - 1, 1, 1, 1);
    GFL_BGSysFillScrArea(bg, 9, x + width - 1, y + height - 1, 1, 1, 1);
    GFL_BGSysFillScrArea(bg, 2, x + 1, y, width - 2, 1, 1);
    GFL_BGSysFillScrArea(bg, 8, x + 1, y + height - 1, width - 2, 1, 1);
    GFL_BGSysFillScrArea(bg, 4, x, y + 1, 1, height - 2, 1);
    GFL_BGSysFillScrArea(bg, 6, x + width - 1, y + 1, 1, height - 2, 1);
    GFL_BGSysFillScrArea(bg, 5, x + 1, y + 1, width - 2, height - 2, 1);
    GFL_BGSysQueueScrLoad(bg);
}

// pokebw2's StartMenu_ClearFrame
static void func_ov088_021bbb84(u8 x, u8 y, u8 width, u8 height, u8 bg) {
    GFL_BGSysFillScrArea(bg, 0, x, y, width, height, 0);
    GFL_BGSysQueueScrLoad(bg);
}

// The notice goes on the main engine's BG 0 over a frame on the part of BGs 1 and 2 past the items, which they are
// scrolled to
// pokebw2's StartMenu_OpenNewGameWarning
static void func_ov088_021bbbac(StartMenuWork *wk) {
    StrBuf *str;

    wk->notice.window = BmpWin_CreateDynamic(0, 2, 2, 30, 20, 5, 1);
    str = GFL_MsgDataLoadStrbufNew(wk->msgData, StartmenuFieldmapCtrlHybrid_Text_022);
    PrintWindow_Print(&wk->notice, wk->printQueue, 0, 0, str, wk->font, PRINT_COLOR(5, 6, 0));
    GFL_StrBufFree(str);
    str = GFL_MsgDataLoadStrbufNew(wk->msgData, StartmenuFieldmapCtrlHybrid_Text_023);
    PrintWindow_Print(&wk->notice, wk->printQueue, 0, 24, str, wk->font, PRINT_COLOR(1, 2, 0));
    GFL_StrBufFree(str);
    str = GFL_MsgDataLoadStrbufNew(wk->msgData, StartmenuFieldmapCtrlHybrid_Text_024);
    PrintWindow_Print(&wk->notice, wk->printQueue, 0, 128, str, wk->font, PRINT_COLOR(1, 2, 0));
    GFL_StrBufFree(str);
    str = GFL_MsgDataLoadStrbufNew(wk->msgData, StartmenuFieldmapCtrlHybrid_Text_025);
    PrintWindow_Print(&wk->notice, wk->printQueue, 0, 144, str, wk->font, PRINT_COLOR(1, 2, 0));
    GFL_StrBufFree(str);
    BmpWin_FlushMap(wk->notice.window);
    GFL_BGSysQueueScrLoad(0);
    func_ov088_021bba64(32, 1, 32, 22, 2);
    GFL_BGSysMoveBGReq(1, BG_MOVE_SET_X, 256);
    GFL_BGSysMoveBGReq(1, BG_MOVE_SET_Y, 0);
    GFL_BGSysMoveBGReq(2, BG_MOVE_SET_X, 256);
    GFL_BGSysMoveBGReq(2, BG_MOVE_SET_Y, 0);
    func_ov088_021bb9dc(wk, FALSE);
}

// Closes the notice, and unless the menu is being left, brings the items back
// pokebw2's StartMenu_CloseNotice
static void func_ov088_021bbd10(StartMenuWork *wk, BOOL leaving) {
    BmpWin_Free(wk->notice.window);
    if (leaving == TRUE) {
        return;
    }
    func_ov088_021bbb84(32, 1, 32, 22, 2);
    GFL_BGSysFillScrAsync(0, 0);
    GFL_BGSysMoveBGReq(1, BG_MOVE_SET_X, 0);
    GFL_BGSysMoveBGReq(1, BG_MOVE_SET_Y, wk->scrollY);
    GFL_BGSysMoveBGReq(2, BG_MOVE_SET_X, 0);
    GFL_BGSysMoveBGReq(2, BG_MOVE_SET_Y, wk->scrollY);
    if (wk->items[0] == ITEM_CONTINUE) {
        func_ov088_021bb9dc(wk, TRUE);
    }
}

// The warning about wireless communications on the sub screen, over the saved game
// pokebw2's StartMenu_OpenCGearWarning
static void func_ov088_021bbd80(StartMenuWork *wk) {
    StrBuf *str;

    wk->notice.window = BmpWin_CreateDynamic(4, 2, 3, 30, 19, 5, 1);
    str = GFL_MsgDataLoadStrbufNew(wk->msgData, StartmenuFieldmapCtrlHybrid_Text_020);
    PrintWindow_Print(&wk->notice, wk->printQueue, 0, 0, str, wk->font, PRINT_COLOR(5, 6, 0));
    GFL_StrBufFree(str);
    str = GFL_MsgDataLoadStrbufNew(wk->msgData, StartmenuFieldmapCtrlHybrid_Text_021);
    PrintWindow_Print(&wk->notice, wk->printQueue, 0, 24, str, wk->font, PRINT_COLOR(1, 2, 0));
    GFL_StrBufFree(str);
    BmpWin_FlushMap(wk->notice.window);
    GFL_BGSysQueueScrLoad(4);
    func_ov088_021bba64(0, 1, 32, 22, 5);
}

// pokebw2's StartMenu_CloseCGearWarning
static void func_ov088_021bbe3c(StartMenuWork *wk) {
    BmpWin_Free(wk->notice.window);
}

void func_ov088_021bbe48(StartMenuWork *wk) {
    MsgData *msgData;
    StrBuf *str;

    msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_WIFI_ERROR, HEAPID_TAIL(0xb));
    wk->notice.window = BmpWin_CreateDynamic(0, 2, 2, 30, 20, 5, 1);
    str = GFL_MsgDataLoadStrbufNew(msgData, WifiError_Text_032);
    PrintWindow_Print(&wk->notice, wk->printQueue, 0, 0, str, wk->font, PRINT_COLOR(1, 2, 0));
    GFL_StrBufFree(str);
    GFL_MsgDataFree(msgData);
    BmpWin_FlushMap(wk->notice.window);
    GFL_BGSysQueueScrLoad(0);
    func_ov088_021bba64(32, 1, 32, 22, 2);
    GFL_BGSysMoveBGReq(1, BG_MOVE_SET_X, 256);
    GFL_BGSysMoveBGReq(1, BG_MOVE_SET_Y, 0);
    GFL_BGSysMoveBGReq(2, BG_MOVE_SET_X, 256);
    GFL_BGSysMoveBGReq(2, BG_MOVE_SET_Y, 0);
    func_ov088_021bb9dc(wk, FALSE);
}

static void func_ov088_021bbf1c(StartMenuWork *wk, u32 messageId) {
    BmpWin *window;

    GFL_MsgDataLoadStrbuf(wk->msgData, messageId, wk->strbuf);
    GFL_BitmapFill(BmpWin_GetBitmap(wk->windows[WINDOW_MESSAGE].window), 15);
    BmpWin_DrawFrame(wk->windows[WINDOW_MESSAGE].window, 2, 1, 14);
    wk->printStream = func_0201cce4(wk->windows[WINDOW_MESSAGE].window, 0, 0, wk->strbuf, wk->font, func_02012ffc(), wk->tcbManager, 10, 0xb, 15);
    wk->continued = FALSE;
    window = wk->windows[WINDOW_MESSAGE].window;
    BmpWin_FlushChar(window);
    BmpWin_FlushMap(window);
    GFL_BGSysQueueScrLoad(BmpWin_GetBGIndex(window));
}



// pokebw2's StartMenu_ClearMessage
static void func_ov088_021bbf98(StartMenuWork *wk) {
    func_0201f7a8(wk->windows[WINDOW_MESSAGE].window, WINFRAME_TRANSFER_VBLANK);
}

BOOL func_ov088_021bbfa8(StartMenuWork *wk) {
    if (wk->printStream == NULL) {
        return FALSE;
    }
    func_020286c4(wk->keyCursor, wk->printStream, wk->windows[WINDOW_MESSAGE].window);
    switch (func_0201ce30(wk->printStream)) {
    case PRINT_STREAM_RUNNING:
        if (GCTX_HIDGetHeldKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)) {
            func_0201ce5c(wk->printStream, 0);
        }
        wk->continued = FALSE;
        break;
    case PRINT_STREAM_PAUSED:
        if (wk->continued == FALSE && (GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B))) {
            GFL_SndSEPlay(SEQ_SE_MESSAGE);
            func_0201ce38(wk->printStream);
            wk->continued = TRUE;
        }
        break;
    case PRINT_STREAM_DONE:
        func_0201ce48(wk->printStream);
        wk->printStream = NULL;
        wk->continued = FALSE;
        return FALSE;
    }
    return TRUE;
}

static void func_ov088_021bc024(StartMenuWork *wk) {
    ConfirmDialogSetup setup;

    setup.bg = 0;
    setup.x = 24;
    setup.y = 13;
    setup.palette = 15;
    setup.unk4 = 0;
    wk->dialog = ShopUI_CreateConfirmDialog(&setup, 1, 14, 0, 0xb);
}

u32 func_ov088_021bc054(StartMenuWork *wk, u32 next) {
    GFL_WipeSet(WIPE_MODE_BOTH, WIPE_TYPE_FADE_IN, 1, WIPE_COLOR_BLACK, 6, 1, 0xb);
    wk->wipeNextState = next;
    return STATE_WAIT_WIPE;
}

u32 func_ov088_021bc080(StartMenuWork *wk, u32 next) {
    GFL_WipeSet(WIPE_MODE_BOTH, WIPE_TYPE_FADE_OUT, 0, WIPE_COLOR_BLACK, 6, 1, 0xb);
    wk->wipeNextState = next;
    return STATE_WAIT_WIPE;
}





// pokebw2's StartMenu_StartBlink
static u32 func_ov088_021bc0ac(StartMenuWork *wk, u32 next) {
    wk->blinkWait = 0;
    wk->blinkSeq = 0;
    wk->blinkNextState = next;
    return 3;
}

// pokebw2's StartMenu_CheckBrokenSave
static BOOL func_ov088_021bc0c8(StartMenuWork *wk) {
    wk->brokenBlocks = func_020071e8(wk->save);
    if (wk->brokenBlocks != 0) {
        return TRUE;
    }
    return FALSE;
}
