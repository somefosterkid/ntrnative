// Black's code at ov135 0x021f4580-0x021f5154. No name is known: the module and address.

#include "types.h"
#include "app/worldtrade.h"
#include "dpw/dpw_tr.h"
#include "field/calender.h"
#include "field/city_state.h"
#include "field/encounter.h"
#include "field/event_data.h"
#include "field/field_actor_internal.h"
#include "field/field_map.h"
#include "field/field_status.h"
#include "field/player_state.h"
#include "field/pleasure_boat.h"
#include "field/trial_house.h"
#include "field/unity_tower.h"
#include "field/zone.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "gfl/proc.h"
#include "gfl/std.h"
#include "gfl/tcb.h"
#include "gfl/tcbl.h"
#include "nitro/fs.h"
#include "nitro/fx/fx.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "nnsys/g2d.h"
#include "pml/poke_party.h"
#include "save/bag.h"
#include "save/config.h"
#include "save/event_work.h"
#include "save/gimmick_savedata.h"
#include "save/high_link.h"
#include "save/player_info.h"
#include "save/player_savedata.h"
#include "save/playtime.h"
#include "save/save_control.h"
#include "save/wifi_list.h"
#include "struct_decls.h"
#include "system/app_taskmenu.h"
#include "system/game_data.h"
#include "system/game_system.h"
#include "system/printsys.h"
#include "system/shortcut_util.h"
#include "system/time_icon.h"
#include "field/beacon_view.h"
#include "constants/text_banks.h"
#include "system/bmp_menuwork.h"
#include "worldtrade_local.h"
#include "gfl/sound.h"
#include "gfl/msg.h"
#include "system/wordset.h"
#include "gfl/str.h"
#include "gfl/wm_icon.h"
#include "system/wipe.h"
#include "constants/arc.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"

#define BUTTON_COLOR 0x39e3

struct AppTaskMenuRes {
    u16 bg;
    u16 palette;
    NNSG2dCharacterData *character;
    void *characterFile;
    u32 unkC;
    Font *font;
    PrintQueue *queue;
};
struct AppTaskMenu {
    u8 cursorPos;
    u8 flashCount;
    u16 pulseAngle;
    GXRgb pulseColor;
    BOOL isUpdatingText;
    BOOL isDecided;
    BOOL isLocked;
    u32 flashSpeed;
    BmpWin **windows;
    AppTaskMenuItem *items;
    AppTaskMenuInit init;
    AppTaskMenuRes *res;
};
struct BmpWin {
    u16 magic : 15;
    u16 colorMode : 1;
    u8 bg;
    u8 x;
    u8 y;
    u8 sizeX;
    u8 sizeY;
    u8 width;
    u8 height;
    u8 palette;
    u16 charPos;
    GFLBitmap *bitmap;
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
struct StrBuf {
    u16 size;
    u16 length;
    u32 magic;
    u16 chars[1];
};
struct ArcTool {
    FSFile file;
    u32 fimgOffset;
    u16 fatbOffset;
    u16 fileCount;
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
struct WaitIcon {
    TCB *tcb;
    TCBEx *tcbEx;
    BmpWin *window;
    GFLBitmap *bitmap;
    HeapID heapId;
    u8 clearColor;
    u8 wait;
    u8 interval;
    u8 frame;
    BOOL active;
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
struct BoxSaveAccessor {
    SaveControl *save;
};
struct CommPlayerSupport {
    u32 type;
    u32 usedType;
    u32 lastType;
    PlayerInfo supporter;
    PlayerInfo usedSupporter;
};
struct GameData {
    SaveControl *saveControl;
    PlayerState playerStates[5];
    EventData *eventData;
    PlayerSave *playerSave;
    ZoneSpawnInfo *nowSpawn;
    ZoneSpawnInfo *outboundWarpSpawn;
    ZoneSpawnInfo *nextSpawn;
    ZoneSpawnInfo *escapeRopeSpawn;
    ZoneSpawnInfo entralinkParentSpawn;
    void *bagCursor;
    BagSave *bag;
    PokeParty *party;
    Config *config;
    BoxSaveAccessor *boxes;
    WifiList *wifiList;
    BGMInfo *bgmInfo;
    MMSys *mmSys;
    EventWork *eventWork;
    MapMatrix *mapMatrix;
    FieldSound *fieldSound;
    FieldStatus *fieldStatus;
    u32 lastBtlResult;
    u32 unk1C0;
    u8 season;
    u8 weather;
    u8 unk1C6;
    u8 unk1C7;
    u8 counter;
    u8 counterEnabled;
    u8 lastSubscreen;
    u8 netId;
    u8 forceSeasonSync;
    u8 savedCGearPowerRequest;
    u8 saving;
    u8 is30FPSMode;
    u8 seasons[4];
    PassPowerLevel levels[4];
    CityState cityStates[2];
    EncountState *encountState;
    ShortcutMenuPos shortcutMenuPos;
    GimmickState gimmickState;
    PleasureBoat *pleasureBoat;
    CommPlayerSupport *commPlayerSupport;
    u32 unk688;
    void *unk68C;
    Calendar *calendar;
    UnityTowerFloor unityTowerFloor;
    TrialHouseWork *trialHouseWork;
    u32 unk6A8;
    PlayTime *playTime;
};
struct BGMInfo {
    u8 count;
    u16 *bgms;
    u8 *subsystems;
};

extern u8 data_ov135_02203b08;
void func_ov135_021f5060(void);
void func_ov135_021f4894(const BGSysVRAMConfig *config);
static const BGSysVRAMConfig data_ov135_02202bfc = {
    GX_VRAM_BG_128_D,  GX_VRAM_BGEXTPLTT_NONE,  GX_VRAM_SUB_BG_128_C,        GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_256_AB, GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_16_I,        GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_NONE,  GX_VRAM_TEXPLTT_NONE,    GX_OBJVRAMMODE_CHAR_1D_128K, GX_OBJVRAMMODE_CHAR_1D_128K,
};
void func_ov135_021f49f8(WorldTradeWork *wk, const BGSysVRAMConfig *vramConfig);
void func_ov135_021ffcd4(WorldTradeWork *wk);
void func_ov135_021f49a8(void);
int func_ov135_021f4eb4(void);
void func_ov135_021f4f84(WorldTradeWork *wk);

void func_ov135_021f49a4(WorldTradeWork *wk);



static WorldTradeSubProcFunc sSubProcessTable[][3] = {
    { func_ov135_021f89b4, func_ov135_021f89f0, func_ov135_021f8a18 },
    { func_ov135_021ffda8, func_ov135_021ffeb4, func_ov135_021ffec8 },
    { func_ov135_021fc06c, func_ov135_021fc174, func_ov135_021fc188 },
    { func_ov135_021fcc40, func_ov135_021fce64, func_ov135_021fced4 },
    { func_ov135_021fd924, func_ov135_021fdae4, func_ov135_021fdb74 },
    { func_ov135_021f5154, func_ov135_021f51f0, func_ov135_021f5204 },
    { func_ov135_021f73dc, func_ov135_021f74e4, func_ov135_021f74f8 },
    { func_ov135_02200c0c, func_ov135_02200c80, func_ov135_02200c94 },
    { func_ov135_021ff58c, func_ov135_021ff65c, func_ov135_021ff67c },
    { func_ov135_021f6e58, func_ov135_021f6fd4, func_ov135_021f7268 },
};

void func_ov135_021f5008(s32 param_1);


BOOL func_ov135_021f4580(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov135_021f46a8(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov135_021f47c0(GameProc *proc, u32 *state, void *param, void *work);

const GameProcFunctions data_ov135_02202bac = {
    func_ov135_021f4580,
    func_ov135_021f46a8,
    func_ov135_021f47c0,
};

static WorldTradeWork *sWorldTradeWork;

PartyPkm *func_ov135_02202608(HeapID heapId);
void func_ov135_021f489c(WorldTradeWork *wk, WorldTradeParam *param);

BOOL func_ov135_021f4580(GameProc *proc, u32 *state, void *param, void *work) {
    WorldTradeParam *wtParam = param;
    WorldTradeWork *wk;

    GFL_OvlLoad(OVERLAY_ID(114));
    GFL_OvlLoad(OVERLAY_ID(67));
    GFL_HeapCreateChild(HEAPID_USER, HEAPID_WORLDTRADE, 0x30100);
    wk = GFL_ProcInitSubsystem(proc, sizeof(WorldTradeWork), HEAPID_WORLDTRADE);
    MI_CpuFill8(wk, 0, sizeof(WorldTradeWork));
    sWorldTradeWork = wk;

    wk->demoPokemon = func_ov135_02202608(HEAPID_WORLDTRADE);
    wk->procManager = CreateGameProcManager(HEAPID_WORLDTRADE);
    wk->tcbBuffer = GFL_HeapAllocate(HEAPID_WORLDTRADE, GFL_TCBMgrCalcAllocSize(8), FALSE, "worldtrade.c", 161);
    wk->tcbManager = GFL_TCBMgrCreate(8, wk->tcbBuffer);
    func_ov135_022026d8(&wk->print, wtParam->config);

    wk->wordSet = GFL_WordSetSystemCreate(11, 64, HEAPID_WORLDTRADE);
    wk->msgManager = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_0415, HEAPID_WORLDTRADE);
    wk->lobbyMsgManager = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_0416, HEAPID_WORLDTRADE);
    wk->systemMsgManager = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_WIFI_ERROR, HEAPID_WORLDTRADE);
    wk->monsNameManager = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_SPECIES_NAMES, HEAPID_WORLDTRADE);
    wk->countryNameManager = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE,
                                                TEXT_BANK_EVENT_MAPCHANGE_LOAD_COUNTRY_TO_STRBUF, HEAPID_WORLDTRADE);

    func_ov135_021f489c(wk, wtParam);
    GFL_BGSysInitVRAM(0);
    func_ov135_021f5074(wk);
    GFL_SndBGMSetPaused(FALSE);
    func_02005d28();
    return TRUE;
}

BOOL func_ov135_021f46a8(GameProc *proc, u32 *state, void *param, void *work) {
    WorldTradeWork *wk = work;

    if (func_0203cd98()) {
        func_0205d074();
        func_020370f4(func_ov135_021f4eb4());
    }
    func_ov114_021c7c1c();
    wk->procResult = GFL_ProcMgrUpdate(wk->procManager);
    GFL_TCBMgrUpdate(wk->tcbManager);
    func_ov135_02202770(&wk->print);

    switch (*state) {
    case WT_SEQ_INIT:
        *state = sSubProcessTable[wk->subProcess][0](wk, *state);
        func_ov135_021f49a8();
        break;
    case WT_SEQ_FADEIN:
        if (GFL_WipeIsFinished()) {
            *state = WT_SEQ_MAIN;
        }
        break;
    case WT_SEQ_MAIN:
        *state = sSubProcessTable[wk->subProcess][1](wk, *state);
        break;
    case WT_SEQ_FADEOUT:
        if (GFL_WipeIsFinished()) {
            *state = sSubProcessTable[wk->subProcess][2](wk, *state);
            if (wk->subprocFlag) {
                func_ov135_021ff6a4(wk);
                func_ov135_021ffb20(wk, wk->searchResult, 0);
                wk->subprocFlag = 0;
            }
        }
        break;
    case WT_SEQ_OUT:
        *state = WT_SEQ_END;
        break;
    case WT_SEQ_END:
        return TRUE;
    }

    func_ov135_021f4f84(wk);
    if (wk->clactUnit != NULL) {
        func_0204a600();
    }
    if (wk->unk12E8 > 0) {
        wk->unk12E8--;
    }
    return FALSE;
}

BOOL func_ov135_021f47c0(GameProc *proc, u32 *state, void *param, void *work) {
    WorldTradeWork *wk = work;

    func_02005d28();
    func_ov135_021f50ec(wk);
    func_ov135_021f49a4(wk);

    GFL_MsgDataFree(wk->monsNameManager);
    GFL_MsgDataFree(wk->systemMsgManager);
    GFL_MsgDataFree(wk->lobbyMsgManager);
    GFL_MsgDataFree(wk->msgManager);
    GFL_MsgDataFree(wk->countryNameManager);
    GFL_WordSetSystemFree(wk->wordSet);

    func_02030da4(wk->tcbManager);
    GFL_HeapFree(wk->tcbBuffer);
    FreeGameProcManager(wk->procManager);
    GFL_HeapFree(wk->demoPokemon);
    func_ov135_02202744(&wk->print);
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(HEAPID_WORLDTRADE);
    GFL_OvlUnload(OVERLAY_ID(67));
    GFL_OvlUnload(OVERLAY_ID(114));
    return TRUE;
}

void func_ov135_021f4864(TCB *tcb, void *data) {
    WorldTradeWork *wk = data;

    GFL_BGSysUpdate();
    func_0204a648();
    if (wk->vfunc != NULL) {
        wk->vfunc(wk);
        wk->vfunc = NULL;
    }
    if (wk->vfunc2 != NULL) {
        wk->vfunc2(wk);
    }
}

void func_ov135_021f4894(const BGSysVRAMConfig *config) {
    GFL_BGSysSetVRAMBanks(config);
}


void func_ov135_021f489c(WorldTradeWork *wk, WorldTradeParam *param) {
    GFL_ASSERT(param->savedata != NULL);
    GFL_ASSERT(param->worldtrade_data != NULL);
    GFL_ASSERT(param->systemdata != NULL);
    GFL_ASSERT(param->myparty != NULL);
    GFL_ASSERT(param->mybox != NULL);
    GFL_ASSERT(param->wifilist != NULL);
    GFL_ASSERT(param->wifihistory != NULL);
    GFL_ASSERT(param->mystatus != NULL);
    GFL_ASSERT(param->config != NULL);
    GFL_ASSERT(param->record != NULL);

    wk->param = param;
    wk->subProcess = WORLDTRADE_ENTER;
    func_ov135_021f4ec0(wk, WORLDTRADE_ENTER, 0x13);
    wk->titleCursorPos = 0;
    wk->search.characterNo = 0;
    wk->search.gender = 3;
    wk->search.level_min = 0;
    wk->search.level_max = 0;
    wk->searchBackup.characterNo = 0;
    wk->demoEnd = 0;
    wk->boxTrayNo = 0xff;
    wk->boxPokeNum = 0;
    wk->boxSearchFlag = 0;
    wk->subLcdTouchOK = 0;
    wk->timeWaitWork = NULL;
    wk->countryCode = 0;
}

void func_ov135_021f49a4(WorldTradeWork *wk) {
}

void func_ov135_021f49a8(void) {
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    GX_SetVisibleWnd(GX_WNDMASK_W0);
    G2_SetWnd0InsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3 | GX_WND_PLANEMASK_OBJ, FALSE);
    G2_SetWndOutsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3 | GX_WND_PLANEMASK_OBJ, TRUE);
    G2_SetWnd0Position(256 - 16, 0, 255, 16);
}

static const ClActSysSetup sClActSysSetup = { 0, 0, 0, 512, 4, 124, 4, 124, 0, 32, 32, 32, 32, 16, 16 };
void func_ov135_021f49f8(WorldTradeWork *wk, const BGSysVRAMConfig *vramConfig) {
    void *plttBuf;
    int animFile;
    int cellFile;
    int plttFile;
    int charFile;
    ArcTool *arc;
    NNSG2dPaletteData *pltt;
    u16 *colors;
    int i;

    arc = GFL_ArcSysCreateFileHandle(ARCID_WORLDTRADE, HEAPID_WORLDTRADE);
    ClActSys_Create(&sClActSysSetup, vramConfig, HEAPID_WORLDTRADE);
    wk->clactUnit = func_0204b100(96, 0, HEAPID_WORLDTRADE);

    wk->clactRes[WT_CLACT_RES_MAIN][WT_CLACT_RES_CHAR] = func_0204a6c8(arc, 0x11, TRUE, 0, HEAPID_WORLDTRADE);
    wk->clactRes[WT_CLACT_RES_MAIN][WT_CLACT_RES_PLTT] = func_0204abf0(arc, 8, 0, 0, HEAPID_WORLDTRADE);
    wk->clactRes[WT_CLACT_RES_MAIN][WT_CLACT_RES_CELL] = func_0204af28(arc, 0x12, 0x13, HEAPID_WORLDTRADE);
    wk->clactRes[WT_CLACT_RES_MAIN2][WT_CLACT_RES_CHAR] = func_0204a6c8(arc, 0x27, TRUE, 0, HEAPID_WORLDTRADE);
    wk->clactRes[WT_CLACT_RES_MAIN2][WT_CLACT_RES_CELL] = func_0204af28(arc, 0x28, 0x29, HEAPID_WORLDTRADE);
    wk->clactRes[WT_CLACT_RES_SUB][WT_CLACT_RES_CHAR] = func_0204a6c8(arc, 0x24, TRUE, 1, HEAPID_WORLDTRADE);
    wk->clactRes[WT_CLACT_RES_SUB][WT_CLACT_RES_PLTT] = func_0204abf0(arc, 7, 1, 0, HEAPID_WORLDTRADE);
    wk->clactRes[WT_CLACT_RES_SUB][WT_CLACT_RES_CELL] = func_0204af28(arc, 0x25, 0x26, HEAPID_WORLDTRADE);
    GFL_ArcToolFree(arc);

    // The Pokemon icons' palettes, and a copy at half brightness for icons that can't be chosen
    plttBuf = GFL_G2DIOReadNCLR(7, 0, &pltt, HEAPID_WORLDTRADE);
    DC_FlushRange(pltt->pRawData, 0x60);
    GX_LoadOBJPltt(pltt->pRawData, 0xa0, 0x60);
    colors = pltt->pRawData;
    for (i = 0; i < 48; i++) {
        colors[i] =
            (((colors[i] >> 10) & 0x1f) / 2 << 10) | (((colors[i] >> 5) & 0x1f) / 2 << 5) | (colors[i] & 0x1f) / 2;
    }
    DC_FlushRange(pltt->pRawData, 0x60);
    GX_LoadOBJPltt(pltt->pRawData, 0x100, 0x60);
    GFL_HeapFree(plttBuf);

    if ((u8)getTrainerGender(wk->param->mystatus) == 1) {
        charFile = 0x11;
        animFile = 0x10;
        cellFile = 0xf;
        plttFile = 6;
    } else {
        charFile = 0xe;
        animFile = 0xd;
        cellFile = 0xc;
        plttFile = 4;
    }
    arc = GFL_ArcSysCreateFileHandle(31, HEAPID_WORLDTRADE);
    wk->clactRes[WT_CLACT_RES_HERO][WT_CLACT_RES_CHAR] = func_0204a6c8(arc, charFile, FALSE, 1, HEAPID_WORLDTRADE);
    wk->clactRes[WT_CLACT_RES_HERO][WT_CLACT_RES_PLTT] = func_0204ac18(arc, plttFile, 1, 0, 0, 1, HEAPID_WORLDTRADE);
    wk->clactRes[WT_CLACT_RES_HERO][WT_CLACT_RES_CELL] = func_0204af28(arc, cellFile, animFile, HEAPID_WORLDTRADE);
    GFL_ArcToolFree(arc);
}



void func_ov135_021f4bcc(WorldTradeWork *wk) {
    GFL_BGSysSetBGEnabledA(0x10, 1);
    GFL_BGSysSetBGEnabledB(0x10, 1);
}

void func_ov135_021f4be0(WorldTradeWork *wk, int y, int cgx, int palette, u8 passive) {
    func_ov135_021f4bf8(wk, y, cgx, palette, 0, passive);
}

void func_ov135_021f4bf8(WorldTradeWork *wk, int y, int cgx, int palette, int frame, u8 passive) {
    GFL_ASSERT(wk->task_res == NULL);
    GFL_ASSERT(wk->task_work == NULL);

    wk->task_res = func_02027f60((u8)frame, (u8)palette, wk->print.font, wk->print.printQueue, HEAPID_WORLDTRADE);
    {
        AppTaskMenuItem items[2] = {
            { NULL, BUTTON_COLOR, 0 },
            { NULL, BUTTON_COLOR, 0 },
        };
        AppTaskMenuInit init;

        MI_CpuFill8(&init, 0, sizeof(AppTaskMenuInit));
        items[0].str = GFL_MsgDataLoadStrbufNew(wk->msgManager, 0x3f);
        items[1].str = GFL_MsgDataLoadStrbufNew(wk->msgManager, 0x40);
        init.heapId = HEAPID_WORLDTRADE;
        init.itemCount = 2;
        init.items = items;
        init.posType = APP_TASKMENU_POS_BOTTOM_RIGHT;
        init.x = 32;
        init.y = y;
        init.width = 8;
        init.height = 3;
        wk->task_work = func_02027778(&init, wk->task_res);
        GFL_StrBufFree(items[0].str);
        GFL_StrBufFree(items[1].str);
    }

    if (passive) {
        BOOL isMain = FALSE;

        if (frame <= 3) {
            isMain = TRUE;
        }
        func_ov135_021f5008(isMain);
    }
}



void func_ov135_021f4ce4(WorldTradeWork *wk) {
    if (wk->task_work != NULL) {
        func_02027858(wk->task_work);
        wk->task_work = NULL;
    }
    if (wk->task_res != NULL) {
        func_02027fd8(wk->task_res);
        wk->task_res = NULL;
    }
}

u8 func_ov135_021f4d14(WorldTradeWork *wk) {
    u8 result;
    BOOL BVar2;

    result = 0;
    if (wk->task_work != NULL) {
        func_02027974(wk->task_work);
        BVar2 = func_020279e8(wk->task_work);
        if (BVar2 != 0) {
            result = func_02027a04(wk->task_work);
            if (result == 0) {
                result = 1;
            }
            else if (result == 1) {
                result = 2;
            }
            func_ov135_021f5060();
        }
    }
    return result;
}

void func_ov135_021f4d50(WorldTradeWork *wk, u8 frame, int count, int y) {
    int i;

    GFL_ASSERT(wk->task_res == NULL);
    GFL_ASSERT(wk->task_work == NULL);

    wk->task_res = func_02027f60(frame, 10, wk->print.font, wk->print.printQueue, HEAPID_WORLDTRADE);
    {
        AppTaskMenuItem itemWork[3] = {
            { NULL, BUTTON_COLOR, 0 },
            { NULL, BUTTON_COLOR, 0 },
            { NULL, BUTTON_COLOR, 0 },
        };
        AppTaskMenuInit init;

        MI_CpuFill8(&init, 0, sizeof(AppTaskMenuInit));
        GFL_ASSERT(count <= NELEMS(itemWork));
        for (i = 0; i < count; i++) {
            itemWork[i].str = wk->menuList[i].text;
        }
        init.heapId = HEAPID_WORLDTRADE;
        init.itemCount = count;
        init.items = itemWork;
        init.posType = APP_TASKMENU_POS_BOTTOM_RIGHT;
        init.x = 32;
        init.y = y;
        init.width = 13;
        init.height = 3;
        wk->task_work = func_02027778(&init, wk->task_res);
    }
    func_ov135_021f5008(TRUE);
}


s32 func_ov135_021f4e30(WorldTradeWork *wk) {
    BOOL BVar2;
    s32 result;

    result = -1;
    if (wk->task_work != NULL) {
        func_02027974(wk->task_work);
        BVar2 = func_020279e8(wk->task_work);
        if (BVar2 != 0) {
            result = func_02027a04(wk->task_work) + 1;
            func_ov135_021f5060();
        }
    }
    return result;
}

void func_ov135_021f4e64(WorldTradeWork *wk) {
    if (wk->task_work != NULL) {
        func_02027858(wk->task_work);
        wk->task_work = NULL;
    }
    if (wk->task_res != NULL) {
        func_02027fd8(wk->task_res);
        wk->task_res = NULL;
    }
    func_ov135_021f5060();
}

void func_ov135_021f4e98(WorldTradeWork *wk, int toSeq, int nextSeq) {
    wk->subprocessSeq = toSeq;
    wk->subprocessNextSeq = nextSeq;
}

void func_ov135_021f4ea0(ClActor *act, int x, int y) {
    ClActorPos pos;

    pos.x = x;
    pos.y = y;
    func_0204b404(act, &pos, 1);
}


int func_ov135_021f4eb4(void) {
    return 3 - func_0205ccd8();
}

void func_ov135_021f4ec0(WorldTradeWork *wk, int subProcess, int mode) {
    wk->subNextProcess = subProcess;
    wk->subProcessMode = mode;
}

void func_ov135_021f4ec8(WorldTradeWork *wk) {
    wk->oldSubProcess = wk->subProcess;
    wk->subProcess = wk->subNextProcess;
}

int func_ov135_021f4ed4(WorldTradeWork *wk) {
    return func_02012ffc();
}


void func_ov135_021f4edc(WorldTradeWork *wk) {
    func_ov135_021f4894(&data_ov135_02202bfc);
    func_ov135_021f49f8(wk, &data_ov135_02202bfc);
    func_ov135_021f4bcc(wk);
}

void func_ov135_021f4efc(WorldTradeWork *param_1) {
    func_ov135_021ffcd4(param_1);
    func_0204ada4(param_1->clactRes[2][1]);
    func_0204a8d4(param_1->clactRes[2][0]);
    func_0204afd8(param_1->clactRes[2][2]);
    func_0204ada4(param_1->clactRes[1][1]);
    func_0204a8d4(param_1->clactRes[1][0]);
    func_0204afd8(param_1->clactRes[1][2]);
    func_0204ada4(param_1->clactRes[0][1]);
    func_0204a8d4(param_1->clactRes[0][0]);
    func_0204afd8(param_1->clactRes[0][2]);
    func_0204a8d4(param_1->clactRes[3][0]);
    func_0204afd8(param_1->clactRes[3][2]);
    func_0204b1cc(param_1->clactUnit);
    param_1->clactUnit = (ClActUnit *)0x0;
    func_0204a5ac();
}

void func_ov135_021f4f84(WorldTradeWork *wk) {
    if (wk->serverWaitTime != 0) {
        wk->serverWaitTime--;
    }
}


void func_ov135_021f4f90(WorldTradeWork *wk) {
    wk->boxSearchFlag = 1;
    wk->boxPokeNum = 0;
}

void func_ov135_021f4fa4(WorldTradeWork *wk) {
    TCBManager *tcbManager;
    WaitIcon *icon;

    tcbManager = GFL_VBlankGetTCBMgr();
    icon = func_0202ee30(tcbManager, wk->msgWin, 0xf, 0x10, 0x48);
    wk->timeWaitWork = icon;
}

void func_ov135_021f4fc4(WorldTradeWork *wk) {
    u8 bg;

    if (wk->timeWaitWork != NULL) {
        func_0202f038(wk->timeWaitWork);
        wk->timeWaitWork = NULL;
        BmpWin_FlushMap(wk->msgWin);
        bg = BmpWin_GetBGIndex(wk->msgWin);
        GFL_BGSysLoadScr(bg);
    }
}

void func_ov135_021f4ff4(ClActor *actor, int x, int y) {
    ClActorPos pos;

    pos.x = x;
    pos.y = y;
    func_0204b404(actor, &pos, 0);
}


void func_ov135_021f5008(s32 param_1) {
    if (param_1 != 0) {
        G2_SetBlendBrightness(0x1e, -7);
        return;
    }
    G2S_SetBlendBrightness(0x1e, -7);
}

void func_ov135_021f5034(s32 param_1) {
    if (param_1 != 0) {
        G2_SetBlendBrightness(0x1b, -7);
        return;
    }
    G2S_SetBlendBrightness(0x1b, -7);
}

void func_ov135_021f5060(void) {
    reg_G2_BLDCNT = 0;
    reg_G2S_DB_BLDCNT = 0;
}

void func_ov135_021f5074(WorldTradeWork *wk) {
    GFL_BGSysDisableAllA();
    GFL_BGSysDisableAllB();
    GFL_BGSysSetEnabledBGsA(0);
    GFL_BGSysSetEnabledBGsB(0);
    func_ov135_021f4edc(wk);
    GFL_BGSysCreate(HEAPID_WORLDTRADE);
    BmpWin_InitAllocator(HEAPID_WORLDTRADE);
    {
        BGSysLCDConfig config = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_2D };
        GFL_BGSysSetLCDConfig(&config);
    }
    func_ov135_021f49a8();
    G2_BlendNone();
    G2S_BlendNone();
    if (wk->vblankTask == NULL) {
        wk->vblankTask = GFL_VBlankTCBAdd(func_ov135_021f4864, wk, 0);
    }
}


void func_ov135_021f50ec(WorldTradeWork *wk) {
    if (wk->vblankTask != NULL) {
        GFL_TCBRemove(wk->vblankTask);
        wk->vblankTask = NULL;
    }
    reg_G2_BLDCNT = 0;
    reg_G2S_DB_BLDCNT = 0;
    func_ov135_021f4efc(wk);
    BmpWin_FreeAllocator();
    GFL_BGSysFree();
    reg_GX_DISPCNT = reg_GX_DISPCNT & 0xffff1fff;
    reg_GXS_DB_DISPCNT = reg_GXS_DB_DISPCNT & 0xffff1fff;
}

void func_ov135_021f513c(WorldTradeWork *wk) {
    GameData *gameData;

    gameData = GSYS_GetGameData(wk->param->gsys);
    func_02012e08(gameData);
    func_0200f7c0(0x29);
}
