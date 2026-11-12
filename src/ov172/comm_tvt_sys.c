// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov257/comm_tvt_sys.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "app/comm_tvt/comm_tvt_sys.h"
#include "types.h"
#include "constants/arc.h"
#include "app/comm_tvt.h"
#include "app/comm_tvt/camera_system.h"
#include "app/comm_tvt/ctvt_call.h"
#include "app/comm_tvt/ctvt_camera.h"
#include "app/comm_tvt/ctvt_comm.h"
#include "app/comm_tvt/ctvt_draw.h"
#include "app/comm_tvt/ctvt_game.h"
#include "app/comm_tvt/ctvt_talk.h"
#include "app/comm_tvt/draw_system.h"
#include "constants/sound.h"
#include "constants/text_banks.h"
#include "field/field_sound.h"
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
#include "gfl/net_lower_data.h"
#include "gfl/proc.h"
#include "gfl/random.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "gfl/tcbl.h"
#include "nitro/fx/fx_trig.h"
#include "nitro/gx.h"
#include "save/player_info.h"
#include "save/save_control.h"
#include "system/app_menu_common.h"
#include "system/app_taskmenu.h"
#include "system/bmp_winframe.h"
#include "system/dsi.h"
#include "system/game_data.h"
#include "system/gf_font.h"
#include "system/printsys.h"
#include "system/time_icon.h"
#include "system/wipe.h"
#include "text/system/btl_server_flow.h"
#include "constants/pokemon.h"
#include "nnsys/gfd.h"

// The Xtransceiver's system: the proc, the screens and resources the modes share, and the switch between the modes.
// The names of the members are printed over their video on the top screen, and a cursor points at the talker

#define COMM_TVT_MEMBERS 4
// The frames of the palette animation of the top screen's frame
#define COMM_TVT_PALETTE_FRAMES 10
#define COMM_TVT_OBJ_RESOURCES 11

#define ARC_COMM_TVT 170

enum {
    COMM_TVT_START_WAIT,
    COMM_TVT_START_CALL,
    COMM_TVT_START_ANSWER,
    COMM_TVT_START_EXISTING,
};

struct CommTvtWork {
    HeapID heapId;
    TCB *vblankTask;
    u32 mode;
    u32 nextMode;
    BOOL unk10;
    BOOL errorShown;
    u8 memberCount;
    u8 selfIndex;
    BOOL zoomed;
    BOOL unk20;
    BOOL unk24;
    BOOL unk28;
    BOOL unk2C;
    BOOL canExchangePhotos;
    BOOL unk34;
    u16 paletteAngle;
    u16 paletteFrame;
    u16 palettes[COMM_TVT_PALETTE_FRAMES][16];
    ArcTool *arc;
    // Palettes, then characters, then cells and animations
    u32 objResources[COMM_TVT_OBJ_RESOURCES];
    ClActUnit *clactUnit;
    ClActor *talkerCursor;
    // A bit for each member whose name is printed, and for each window that is to be copied
    u8 namesPrinted;
    u8 namesPending;
    BmpWin *nameWindows[COMM_TVT_MEMBERS];
    Font *font;
    MsgData *msgData;
    PrintQueue *printQueue;
    AppTaskMenuRes *taskMenuRes;
    TCBExManager *tcbEx;
    WaitIcon *waitIcon;
    CtvtCamera *camera;
    CtvtComm *comm;
    CtvtTalk *talk;
    CtvtDraw *draw;
    CtvtCall *call;
    DrawSystem *drawSystem;
    CommTvtParam *param;
    // Black has no game mode: no Black 2's game and inGame after param
}; // size: 0x1fc

static void CommTvt_Init(CommTvtWork *sys);
static void CommTvt_Resume(CommTvtWork *sys);
static void CommTvt_Free(CommTvtWork *sys);
static void CommTvt_Suspend(CommTvtWork *sys);
static void CommTvt_FreeSuspended(CommTvtWork *sys);
BOOL func_ov172_021bd43c(CommTvtWork *sys);
void func_ov172_021bd5ac(TCB *tcb, void *data);
static void func_ov172_021bd5d8(CommTvtWork *sys);
static void func_ov172_021bd7d0(CommTvtWork *sys);
static void func_ov172_021bd820(const BGSetup *setup, u8 bg, u8 mode);
void func_ov172_021bd844(CommTvtWork *sys);
static void func_ov172_021bdb5c(CommTvtWork *sys);
static void func_ov172_021bdbc4(CommTvtWork *sys);
void func_ov172_021bdc70(CommTvtWork *sys);
void func_ov172_021bdcb8(CommTvtWork *sys);
void func_ov172_021bdd6c(CommTvtWork *sys);
void func_ov172_021bdef0(CommTvtWork *sys, const u16 *name, BmpWin *window);
BOOL func_ov172_021be364(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov172_021be43c(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov172_021be410(GameProc *proc, u32 *state, void *param, void *work);

// pokebw2's sCommTvtLCDConfig
static const BGSysLCDConfig data_ov172_021c7050 = { GX_DISPMODE_GRAPHICS, GX_BGMODE_5, GX_BGMODE_0, GX_BG0_AS_2D };

// pokebw2's sCommTvtBG0Setup
static const BGSetup data_ov172_021c70a0 = {
    0,
    0,
    0x800,
    0,
    BGRES_256x256,
    GX_BG_COLORMODE_16,
    GX_BG_SCRBASE_0x7000,
    GX_BG_CHARBASE_0x00000,
    0x7000,
    GX_BG_EXTPLTT_23,
    0,
    GX_BG_AREAOVER_XLU,
    FALSE,
};
// pokebw2's sCommTvtBG4Setup
static const BGSetup data_ov172_021c70c0 = {
    0,
    0,
    0x800,
    0,
    BGRES_256x256,
    GX_BG_COLORMODE_16,
    GX_BG_SCRBASE_0x5000,
    GX_BG_CHARBASE_0x18000,
    0x8000,
    GX_BG_EXTPLTT_23,
    0,
    GX_BG_AREAOVER_XLU,
    FALSE,
};
// pokebw2's sCommTvtBG5Setup
static const BGSetup data_ov172_021c70e0 = {
    0,
    0,
    0x800,
    0,
    BGRES_256x256,
    GX_BG_COLORMODE_16,
    GX_BG_SCRBASE_0x6000,
    GX_BG_CHARBASE_0x00000,
    0x5000,
    GX_BG_EXTPLTT_23,
    1,
    GX_BG_AREAOVER_XLU,
    FALSE,
};
// pokebw2's sCommTvtBG6Setup
static const BGSetup data_ov172_021c7080 = {
    0,
    0,
    0x1000,
    0,
    BGRES_256x512,
    GX_BG_COLORMODE_16,
    GX_BG_SCRBASE_0x6800,
    GX_BG_CHARBASE_0x10000,
    0x8000,
    GX_BG_EXTPLTT_23,
    2,
    GX_BG_AREAOVER_XLU,
    FALSE,
};
// The video's bitmaps (BG 2 and BG 3) are made in func_ov172_021bd5d8
// pokebw2's sCommTvtBG7Setup
static const BGSetup data_ov172_021c7060 = {
    0,
    0,
    0x800,
    0,
    BGRES_256x256,
    GX_BG_COLORMODE_16,
    GX_BG_SCRBASE_0x7800,
    GX_BG_CHARBASE_0x08000,
    0x8000,
    GX_BG_EXTPLTT_23,
    3,
    GX_BG_AREAOVER_XLU,
    FALSE,
};

// pokebw2's sCommTvtVRAMConfig
static const BGSysVRAMConfig data_ov172_021c7120 = {
    GX_VRAM_BG_256_AB, GX_VRAM_BGEXTPLTT_NONE,  GX_VRAM_SUB_BG_128_C,        GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_16_F,  GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_128_D,       GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_NONE,  GX_VRAM_TEXPLTT_NONE,    GX_OBJVRAMMODE_CHAR_1D_128K, GX_OBJVRAMMODE_CHAR_1D_128K,
};
// pokebw2's sCommTvtBG1Setup
static const BGSetup data_ov172_021c7100 = {
    0,
    0,
    0x800,
    0,
    BGRES_256x256,
    GX_BG_COLORMODE_16,
    GX_BG_SCRBASE_0x7800,
    GX_BG_CHARBASE_0x08000,
    0x8000,
    GX_BG_EXTPLTT_23,
    3,
    GX_BG_AREAOVER_XLU,
    FALSE,
};

BOOL func_ov172_021b7b30(CommTvtWork *sys, CtvtCamera *camera, u8 index);
u8 func_ov172_021b8b68(CommTvtWork *sys, CtvtComm *comm);

static inline void CommTvt_CreateNameWindows(CommTvtWork *sys) {
    u8 i;

    sys->nameWindows[0] = BmpWin_CreateDynamic(0, 2, 10, 12, 2, 10, TRUE);
    sys->nameWindows[1] = BmpWin_CreateDynamic(0, 18, 10, 12, 2, 10, TRUE);
    sys->nameWindows[2] = BmpWin_CreateDynamic(0, 2, 22, 12, 2, 10, TRUE);
    sys->nameWindows[3] = BmpWin_CreateDynamic(0, 18, 22, 12, 2, 10, TRUE);
    for (i = 0; i < COMM_TVT_MEMBERS; i++) {
        GFL_BitmapFill(BmpWin_GetBitmap(sys->nameWindows[i]), 0);
    }
    sys->namesPrinted = 0;
    sys->namesPending = 0;
}

// pokebw2's CommTvt_InitGraphics


void func_ov172_021b71b8(CommTvtWork *sys, CtvtCamera *camera);

void func_ov172_021b7dcc(CommTvtWork *sys, CtvtComm *comm);

void func_ov172_021bd1ac(CommTvtWork *sys) {
    DrawSystemParam drawParam;

    sys->memberCount = 1;
    sys->selfIndex = 0;
    sys->zoomed = FALSE;
    func_ov172_021bd5d8(sys);
    func_ov172_021bd844(sys);
    func_ov172_021bdbc4(sys);
    sys->taskMenuRes = func_02027f60(4, 7, sys->font, sys->printQueue, sys->heapId);
    sys->camera = func_ov172_021b6ee0(sys, sys->heapId);
    sys->comm = func_ov172_021b7ba8(sys, sys->heapId);
    sys->talk = func_ov172_021b8c3c(sys, sys->heapId);
    sys->draw = func_ov172_021ba280(sys, sys->heapId);
    sys->call = func_ov172_021bba6c(sys, sys->heapId);
    drawParam.heapId = sys->heapId;
    drawParam.unk2 = 2;
    drawParam.numCommands = 100;
    drawParam.clipLeft = 0;
    drawParam.clipRight = 256;
    drawParam.clipTop = 0;
    drawParam.clipBottom = 192;
    sys->drawSystem = func_ov172_021b6be0(&drawParam);
    sys->vblankTask = GFL_VBlankTCBAdd(func_ov172_021bd5ac, sys, 8);
    sys->mode = COMM_TVT_MODE_NONE;
    sys->paletteAngle = 0;
    sys->paletteFrame = 0;
    sys->unk10 = TRUE;
    sys->unk20 = FALSE;
    sys->unk24 = FALSE;
    sys->unk28 = FALSE;
    sys->unk2C = FALSE;
    sys->canExchangePhotos = FALSE;
    sys->unk34 = FALSE;
    if (canPlayerExchangePhotos() == TRUE) {
        sys->canExchangePhotos = TRUE;
    }
    func_0203d34c(FALSE, sys->heapId);

    switch (sys->param->unk4) {
    case COMM_TVT_START_WAIT:
        func_ov172_021b80dc(sys, sys->comm, CTVT_CONNECT_SCAN);
        sys->nextMode = COMM_TVT_MODE_TALK;
        break;
    case COMM_TVT_START_CALL:
        func_ov172_021b80dc(sys, sys->comm, CTVT_CONNECT_PARENT);
        sys->nextMode = COMM_TVT_MODE_CALL;
        break;
    case COMM_TVT_START_ANSWER:
        func_ov172_021b80dc(sys, sys->comm, CTVT_CONNECT_MAC);
        func_ov172_021b80ec(sys, sys->comm, sys->param->parentMac);
        sys->nextMode = COMM_TVT_MODE_CALL;
        func_02028a70(GameData_GetFieldSoundSystem(sys->param->gameData));
        break;
    case COMM_TVT_START_EXISTING:
        func_ov172_021b80dc(sys, sys->comm, CTVT_CONNECT_EXISTING);
        sys->nextMode = COMM_TVT_MODE_CALL;
        break;
    }

    CommTvt_CreateNameWindows(sys);
    GFL_SndBGMPlay(SEQ_BGM_SILENCE_FIELD, 0xffff);
}

void func_ov172_021bd3b8(CommTvtWork *sys) {
    u8 i;

    for (i = 0; i < 4; i++) {
        BmpWin_Free(sys->nameWindows[i]);
    }
    GFL_TCBRemove(sys->vblankTask);
    func_ov172_021b6c34(sys->drawSystem);
    func_ov172_021bba88(sys, sys->call);
    func_ov172_021ba2b8(sys, sys->draw);
    func_ov172_021b8cb0(sys, sys->talk);
    func_ov172_021b7dcc(sys, sys->comm);
    func_ov172_021b70a8(sys, sys->camera);
    func_02027fd8(sys->taskMenuRes);
    func_ov172_021bdc70(sys);
    func_ov172_021bdb5c(sys);
    func_ov172_021bd7d0(sys);
}

void func_ov172_021b7e34(CommTvtWork *sys, CtvtComm *comm);
BOOL func_ov172_021b80cc(CommTvtWork *sys, CtvtComm *comm);
int func_ov172_021bbf2c(CommTvtWork *sys, CtvtCall *call);

BOOL func_ov172_021bd43c(CommTvtWork *sys) {
    u8 frame;
    s16 sinValue;
    s32 half;

    func_ov172_021b7e34(sys, sys->comm);
    switch (sys->mode) {
    case 1:
        sys->nextMode = func_ov172_021b9044(sys, sys->talk);
        break;
    case 2:
        sys->nextMode = func_ov172_021bbf2c(sys, sys->call);
        break;
    case 3:
        sys->nextMode = func_ov172_021ba9ec(sys, sys->draw);
        break;
    case 4:
        if (func_ov172_021b80cc(sys, sys->comm) == TRUE && func_ov172_021b7590(sys, sys->camera) == TRUE) {
            return TRUE;
        }
        return FALSE;
    case 5:
        if (func_ov172_021b80cc(sys, sys->comm) == TRUE && func_ov172_021b7590(sys, sys->camera) == TRUE
            && GFL_WipeIsFinished() == TRUE) {
            return TRUE;
        }
        break;
    }

    if (sys->errorShown == TRUE && GFL_WipeIsFinished() == TRUE) {
        sys->nextMode = COMM_TVT_MODE_EXIT;
    }
    if (sys->mode != sys->nextMode) {
        func_ov172_021bdcb8(sys);
    }

    func_ov172_021b712c(sys, sys->camera);
    func_ov172_021b6c48(sys->drawSystem);
    func_0204a600();
    func_0201c4e4(sys->printQueue);
    func_ov172_021bdd6c(sys);

    sys->paletteAngle += 0x200;
    sinValue = FX_SinCosTable_[(sys->paletteAngle >> 4) * 2];
    half = (sinValue + 0x1000) / 2;
    frame = (u8)((f32)(10 * half) / 4096.0f);
    if (frame >= 10) {
        frame = 9;
    }
    if (frame != sys->paletteFrame) {
        sys->paletteFrame = frame;
        NNS_GfdRegisterNewVramTransferTask(0x1f, 0x40, sys->palettes[frame], 0x20);
    }

    GFL_TCBExMgrUpdate(sys->tcbEx);
    GFL_BGSysMoveBG(1, 0, GFL_RandomLCAlt(0x100));
    GFL_BGSysMoveBG(1, 3, GFL_RandomLCAlt(0x100));
    return FALSE;
}


void func_ov172_021bd5ac(TCB *tcb, void *data) {
    CommTvtWork *sys = data;
    func_ov172_021b71b8(sys, sys->camera);
    func_0204a648();
    func_ov172_021b6c4c(sys->drawSystem);
    func_ov172_021b9574(sys, sys->talk);
}

static void func_ov172_021bd5d8(CommTvtWork *sys) {
    ClActSysSetup clactSetup;

    GFL_BGSysEnableEngines();
    GFL_BGSysDisableAllA();
    GFL_BGSysDisableAllB();
    GX_SetVisiblePlane(0);
    GXS_SetVisiblePlane(0);
    func_0202223c(0, 0);
    func_0202223c(1, 0);
    func_02022224(0);
    func_02022224(1);
    G2_BlendNone();
    G2S_BlendNone();
    GX_SetDispSelect(GX_DISP_SELECT_MAIN_SUB);
    GFL_BGSysSetVRAMBanks(&data_ov172_021c7120);
    GFL_BGSysCreate(sys->heapId);
    BmpWin_InitAllocator(sys->heapId);
    GFL_BGSysSetLCDConfig(&data_ov172_021c7050);
    func_ov172_021bd820(&data_ov172_021c70a0, 0, BGMODE_TEXT);
    func_ov172_021bd820(&data_ov172_021c7100, 1, BGMODE_TEXT);
    func_ov172_021bd820(&data_ov172_021c70c0, 4, BGMODE_TEXT);
    func_ov172_021bd820(&data_ov172_021c70e0, 5, BGMODE_TEXT);
    func_ov172_021bd820(&data_ov172_021c7080, 6, BGMODE_TEXT);
    func_ov172_021bd820(&data_ov172_021c7060, 7, BGMODE_TEXT);
    G2_SetBG2ControlDCBmp(GX_BG_SCRSIZE_DCBMP_256x256, GX_BG_AREAOVER_XLU, GX_BG_BMPSCRBASE_0x10000);
    G2_SetBG3ControlDCBmp(GX_BG_SCRSIZE_DCBMP_256x256, GX_BG_AREAOVER_XLU, GX_BG_BMPSCRBASE_0x28000);
    MI_CpuFill8(G2_GetBG2ScrPtr(), 0, 256 * 192 * 2);
    MI_CpuFill8(G2_GetBG3ScrPtr(), 0, 256 * 192 * 2);
    G2_SetBG2Priority(1);
    G2_SetBG3Priority(2);
    GFL_BGSysSetBGEnabled(2, TRUE);
    GFL_BGSysSetBGEnabled(3, TRUE);
    clactSetup = data_020a1448;
    ClActSys_Create(&clactSetup, &data_ov172_021c7120, sys->heapId);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_OBJ, TRUE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
    sys->clactUnit = func_0204b100(64, 0, sys->heapId);
    func_0204b270(sys->clactUnit);
    GX_SetVisibleWnd(GX_WNDMASK_W0 | GX_WNDMASK_W1);
    G2_SetWnd0InsidePlane(0x1f, TRUE);
    G2_SetWnd1InsidePlane(0x1f, TRUE);
    G2_SetWndOutsidePlane(0x13, TRUE);
    G2_SetWnd0Position(0, 0, 255, 192);
    G2_SetWnd1Position(128, 0, 0, 192);
}

// pokebw2's CommTvt_FreeGraphics
static void func_ov172_021bd7d0(CommTvtWork *sys) {
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    func_0204b1cc(sys->clactUnit);
    func_0204a5ac();
    GFL_BGSysReleaseBG(7);
    GFL_BGSysReleaseBG(6);
    GFL_BGSysReleaseBG(5);
    GFL_BGSysReleaseBG(4);
    GFL_BGSysReleaseBG(1);
    GFL_BGSysReleaseBG(0);
    BmpWin_FreeAllocator();
    GFL_BGSysFree();
}

// pokebw2's CommTvt_CreateBG
static void func_ov172_021bd820(const BGSetup *setup, u8 bg, u8 mode) {
    GFL_BGSysCreateBG(bg, setup, mode);
    GFL_BGSysSetBGEnabled(bg, TRUE);
    GFL_BGSysClearBG(bg);
    GFL_BGSysLoadScr(bg);
}

void func_ov172_021bd844(CommTvtWork *sys) {
    PlayerInfo *player = GetGameDataPlayerInfo(sys->param->gameData);
    ArcTool *arc;
    ClActorSetup setup;
    NNSG2dPaletteData *palette;
    void *file;

    sys->arc = GFL_ArcSysCreateFileHandle(172, sys->heapId);
    if (getTrainerGender(player) == GENDER_MALE) {
        GFL_G2DIOLoadArcNCLRDefault(sys->arc, 7, 4, 0, 0xc0, sys->heapId);
    } else {
        GFL_G2DIOLoadArcNCLRDefault(sys->arc, 6, 4, 0, 0xc0, sys->heapId);
    }
    GFL_BGSysLoadArcNCGRStatic(sys->arc, 11, 7, 0, 0, FALSE, sys->heapId);
    loadBGScrToVramByFileNoReserveNegAlign(sys->arc, 14, 7, 0, 0, FALSE, sys->heapId);
    GFL_BGSysLoadScr(7);
    GFL_G2DIOLoadArcNCLRDefault(sys->arc, 0, 0, 0x20, 0x20, sys->heapId);
    GFL_BGSysLoadArcNCGRStatic(sys->arc, 8, 1, 0, 0, FALSE, sys->heapId);
    loadBGScrToVramByFileNoReserveNegAlign(sys->arc, 13, 1, 0, 0, FALSE, sys->heapId);
    GFL_BGSysLoadScr(1);
    sys->objResources[0] = func_0204acec(sys->arc, 3, 0, 0, sys->heapId);
    sys->objResources[4] = func_0204a6c8(sys->arc, 24, FALSE, 0, sys->heapId);
    sys->objResources[8] = func_0204af28(sys->arc, 20, 22, sys->heapId);
    sys->objResources[1] = func_0204acec(sys->arc, 3, 1, 0, sys->heapId);
    sys->objResources[5] = func_0204a6c8(sys->arc, 23, FALSE, 1, sys->heapId);
    sys->objResources[9] = func_0204af28(sys->arc, 19, 21, sys->heapId);

    arc = GFL_ArcSysCreateFileHandle(getUINarcIdx(), sys->heapId);
    GFL_G2DIOLoadArcNCLRDefault(arc, func_02027624(), 0, 0, 0x20, sys->heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, func_02027628(), 0, 0, 0, FALSE, sys->heapId);
    GFL_G2DIOLoadArcNCLRDefault(arc, func_02027624(), 4, 0xc0, 0x20, sys->heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, func_02027628(), 5, 0, 0, FALSE, sys->heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, func_0202762c(), 5, 0, 0, FALSE, sys->heapId);
    GFL_BGSysSetScrPaletteNo(5, 0, 21, 32, 3, 6);
    GFL_BGSysLoadScr(5);
    sys->objResources[3] = func_0204ac18(arc, func_02027614(), 0, 0xe0, 0, 3, sys->heapId);
    sys->objResources[7] = func_0204a6c8(arc, func_02027618(), FALSE, 0, sys->heapId);
    sys->objResources[2] = func_0204ac18(arc, func_02027614(), 1, 0xe0, 0, 3, sys->heapId);
    sys->objResources[6] = func_0204a6c8(arc, func_02027618(), FALSE, 1, sys->heapId);
    sys->objResources[10] = func_0204af28(arc, func_0202761c(2), func_02027620(2), sys->heapId);
    GFL_ArcToolFree(arc);

    setup.x = 0;
    setup.y = 0;
    setup.sequence = 0;
    setup.priority = 0;
    setup.bgPriority = 0;
    sys->talkerCursor =
        func_0204b294(func_ov172_021be024(sys), func_ov172_021be018(sys, 4), func_ov172_021be018(sys, 0),
                      func_ov172_021be018(sys, 8), &setup, 0, sys->heapId);
    func_0204b3dc(sys->talkerCursor, FALSE);
    func_0204b92c(sys->talkerCursor, TRUE);
    if (getTrainerGender(player) == GENDER_MALE) {
        file = GFL_G2DIOReadNCLRArc(sys->arc, 7, &palette, sys->heapId);
    } else {
        file = GFL_G2DIOReadNCLRArc(sys->arc, 6, &palette, sys->heapId);
    }
    MIi_CpuCopy32((u16 *)palette->pRawData + 6 * 16, sys->palettes, sizeof(sys->palettes));
    GFL_HeapFree(file);
}


// pokebw2's CommTvt_LoadResources

// pokebw2's CommTvt_FreeResources
void func_ov172_021bdb5c(CommTvtWork *sys) {
    u8 i;

    func_0204b3b4(sys->talkerCursor);
    for (i = 0; i < 4; i++) {
        func_0204ada4(sys->objResources[i]);
    }
    for (i = 4; i < 8; i++) {
        func_0204a8d4(sys->objResources[i]);
    }
    for (i = 8; i < COMM_TVT_OBJ_RESOURCES; i++) {
        func_0204afd8(sys->objResources[i]);
    }
    GFL_ArcToolFree(sys->arc);
}

// pokebw2's CommTvt_InitMessages
static void func_ov172_021bdbc4(CommTvtWork *sys) {
    sys->font = GFL_FontCreate(0x17, 0, 0, 0, sys->heapId);
    sys->msgData = GFL_MsgSysLoadData(0, 2, 0x1b, sys->heapId);
    sys->printQueue = func_0201c450(0x800, sys->heapId);
    GFL_BGSysLoadNCLRDefault(0x17, 5, 0, 0x140, 0x20, sys->heapId);
    GFL_BGSysLoadNCLRDefault(0x17, 5, 4, 0x140, 0x20, sys->heapId);
    func_0201dd68();
    LoadSysMsgBox(4, 0x140, 9, 0, sys->heapId);
    LoadSysMsgBox(0, 0x200, 9, 0, sys->heapId);
    sys->tcbEx = GFL_TCBExMgrCreate(sys->heapId, sys->heapId, 1, 0);
    sys->waitIcon = NULL;
}

void func_ov172_021bdc70(CommTvtWork *sys) {
    if (sys->waitIcon != NULL) {
        func_0202f038(sys->waitIcon);
        sys->waitIcon = NULL;
    }
    GFL_TCBExMgrFree(sys->tcbEx);
    func_0201c6ec(sys->printQueue);
    func_0201c4c0(sys->printQueue);
    GFL_MsgDataFree(sys->msgData);
    GFL_FontFree(sys->font);
}

void func_ov172_021bdcb8(CommTvtWork *sys) {
    switch (sys->mode) {
    case COMM_TVT_MODE_TALK:
        func_ov172_021b8fb8(sys, sys->talk);
        break;
    case COMM_TVT_MODE_CALL:
        func_ov172_021bbe5c(sys, sys->call);
        break;
    case COMM_TVT_MODE_DRAW:
        func_ov172_021ba8c4(sys, sys->draw);
        break;
    }

    sys->mode = sys->nextMode;
    switch (sys->mode) {
    case COMM_TVT_MODE_TALK:
        func_ov172_021b8cc8(sys, sys->talk);
        break;
    case COMM_TVT_MODE_CALL:
        func_ov172_021bba94(sys, sys->call);
        break;
    case COMM_TVT_MODE_DRAW:
        func_ov172_021ba2c4(sys, sys->draw);
        break;
    case COMM_TVT_MODE_EXIT:
        func_0202223c(0, 0);
        func_0202223c(1, 0);
        // fallthrough
    case COMM_TVT_MODE_GAME:
        func_ov172_021b8084(sys, sys->comm);
        func_ov172_021b7578(sys, sys->camera);
        func_ov172_021b7b80(sys, sys->camera);
        break;
    }
}

void func_ov172_021bdd6c(CommTvtWork *sys) {
    BOOL copied = FALSE;
    u8 i;

    if (sys->mode == COMM_TVT_MODE_DRAW) {
        return;
    }
    {
        int displayMode = func_ov172_021be05c(sys);

        for (i = 0; i < COMM_TVT_MEMBERS; i++) {
            if (func_ov172_021b8ba4(sys, sys->comm, i) == TRUE && func_ov172_021b8b8c(sys, sys->comm, i) == TRUE &&
                func_ov172_021b7b30(sys, sys->camera, i) == FALSE) {
                int bit = 1 << i;

                if (!(sys->namesPrinted & bit)) {
                    const CtvtCommMemberInfo *info = func_ov172_021b8b94(sys, sys->comm, i);
                    u8 window;

                    if (displayMode == CTDM_DOUBLE) {
                        if (i == 0) {
                            window = 2;
                        } else {
                            window = 3;
                        }
                    } else {
                        window = i;
                    }
                    func_ov172_021bdef0(sys, (const u16 *)info, sys->nameWindows[window]);
                    sys->namesPrinted += (u8)bit;
                    sys->namesPending += (u8)(1 << window);
                }
            }
        }
    }
    for (i = 0; i < COMM_TVT_MEMBERS; i++) {
        int bit = 1 << i;

        if ((sys->namesPending & bit) && !func_0201c6c4(sys->printQueue, BmpWin_GetBitmap(sys->nameWindows[i]))) {
            BmpWin_Transfer(sys->nameWindows[i]);
            sys->namesPending -= (u8)bit;
            copied = TRUE;
        }
    }
    if (copied == TRUE) {
        u8 talker = func_ov172_021b8b68(sys, sys->comm);

        if (talker != 0xff) {
            func_ov172_021be210(sys, talker);
        }
    }
}






// Prints the name centered, with a border of the shadow's color
// pokebw2's CommTvt_PrintName
void func_ov172_021bdef0(CommTvtWork *sys, const u16 *name, BmpWin *window) {
    StrBuf *strbuf;
    u8 width;
    s32 x;

    strbuf = copyTrainerNameToNewStrbuf(name, sys->heapId);
    width = GFL_FontGetBlockWidth(strbuf, sys->font, 0);
    x = (0x60 - width) / 2;
    func_0201c724(sys->printQueue, BmpWin_GetBitmap(window), x + 1, 1, strbuf, sys->font, 1 << 10);
    func_0201c724(sys->printQueue, BmpWin_GetBitmap(window), x - 1, 1, strbuf, sys->font, 1 << 10);
    func_0201c724(sys->printQueue, BmpWin_GetBitmap(window), x, 2, strbuf, sys->font, 1 << 10);
    func_0201c724(sys->printQueue, BmpWin_GetBitmap(window), x, 0, strbuf, sys->font, 1 << 10);
    func_0201c724(sys->printQueue, BmpWin_GetBitmap(window), x, 1, strbuf, sys->font, 0xf << 10);
    GFL_StrBufFree(strbuf);
}

// pokebw2's CommTvt_GetCamera
CtvtCamera *func_ov172_021bdfdc(CommTvtWork *sys) {
    return sys->camera;
}

// pokebw2's CommTvt_GetComm
CtvtComm *func_ov172_021bdfe4(CommTvtWork *sys) {
    return sys->comm;
}

// pokebw2's CommTvt_GetMic
CtvtMic *func_ov172_021bdfec(CommTvtWork *sys) {
    return func_ov172_021b9f98(sys, sys->talk);
}

DrawSystem *func_ov172_021bdffc(CommTvtWork *work) {
    return work->drawSystem;
}

CommTvtParam *func_ov172_021be004(CommTvtWork *work) {
    return work->param;
}

HeapID func_ov172_021be00c(CommTvtWork *work) {
    return work->heapId;
}




// pokebw2's CommTvt_GetArc
ArcTool *func_ov172_021be010(CommTvtWork *sys) {
    return sys->arc;
}

// pokebw2's CommTvt_GetObjResource
u32 func_ov172_021be018(CommTvtWork *sys, int index) {
    return sys->objResources[index];
}

// pokebw2's CommTvt_GetClActUnit
ClActUnit *func_ov172_021be024(CommTvtWork *sys) {
    return sys->clactUnit;
}

BOOL func_ov172_021be02c(CommTvtWork *sys) {
    return sys->unk10;
}

void func_ov172_021be030(CommTvtWork *sys, BOOL value) {
    sys->unk10 = value;
}

// pokebw2's CommTvt_GetFont
Font *func_ov172_021be034(CommTvtWork *sys) {
    return sys->font;
}

// pokebw2's CommTvt_GetMsgData
MsgData *func_ov172_021be03c(CommTvtWork *sys) {
    return sys->msgData;
}

// pokebw2's CommTvt_GetPrintQueue
PrintQueue *func_ov172_021be044(CommTvtWork *sys) {
    return sys->printQueue;
}

// pokebw2's CommTvt_GetTaskMenuRes
AppTaskMenuRes *func_ov172_021be04c(CommTvtWork *sys) {
    return sys->taskMenuRes;
}

u8 func_ov172_021be054(CommTvtWork *work) {
    return work->memberCount;
}

void func_ov172_021be058(CommTvtWork *work, u8 count) {
    work->memberCount = count;
}



// pokebw2's CommTvt_GetDisplayMode
int func_ov172_021be05c(CommTvtWork *sys) {
    u8 count = func_ov172_021be054(sys);

    if (count == 1) {
        return CTDM_DOUBLE;
    }
    if (count == 2) {
        return CTDM_DOUBLE;
    }
    return CTDM_QUAD;
}

// pokebw2's CommTvt_IsZoomed
BOOL func_ov172_021be078(CommTvtWork *sys) {
    return sys->zoomed;
}

// pokebw2's CommTvt_SendZoom
void func_ov172_021be07c(CommTvtWork *sys, BOOL zoomed) {
    func_ov172_021b8720(sys, sys->comm, zoomed);
}

// pokebw2's CommTvt_ToggleZoom
void func_ov172_021be08c(CommTvtWork *sys) {
    func_ov172_021be07c(sys, sys->zoomed == FALSE ? TRUE : FALSE);
}

// pokebw2's CommTvt_SetZoomed
void func_ov172_021be0a0(CommTvtWork *sys, BOOL zoomed) {
    sys->zoomed = zoomed;
}

// pokebw2's CommTvt_GetSelfIndex
u8 func_ov172_021be0a4(CommTvtWork *sys) {
    return sys->selfIndex;
}

// pokebw2's CommTvt_SetSelfIndex
void func_ov172_021be0a8(CommTvtWork *sys, u8 index) {
    sys->selfIndex = index;
}

BOOL func_ov172_021be0ac(CommTvtWork *sys) {
    return sys->unk20;
}

void func_ov172_021be0b0(CommTvtWork *sys, BOOL value) {
    sys->unk20 = value;
}

BOOL func_ov172_021be0b4(CommTvtWork *sys) {
    return sys->unk24;
}

void func_ov172_021be0b8(CommTvtWork *sys) {
    sys->unk24 = sys->unk24 == FALSE ? TRUE : FALSE;
}

BOOL func_ov172_021be0c8(CommTvtWork *sys) {
    return sys->unk28;
}

void func_ov172_021be0cc(CommTvtWork *sys, BOOL value) {
    sys->unk28 = value;
}

BOOL func_ov172_021be0d0(CommTvtWork *sys) {
    return sys->unk2C;
}

void func_ov172_021be0d4(CommTvtWork *sys, BOOL value) {
    sys->unk2C = value;
}

BOOL func_ov172_021be0d8(CommTvtWork *sys) {
    if (sys->param->unk4 == COMM_TVT_START_EXISTING) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's CommTvt_CanExchangePhotos
BOOL func_ov172_021be0ec(CommTvtWork *sys) {
    return sys->canExchangePhotos;
}

// pokebw2's CommTvt_ClearCanExchangePhotos
void func_ov172_021be0f0(CommTvtWork *sys) {
    sys->canExchangePhotos = FALSE;
}

BOOL func_ov172_021be0f8(CommTvtWork *sys) {
    return sys->unk34;
}

void func_ov172_021be0fc(CommTvtWork *sys, BOOL value) {
    sys->unk34 = value;
}

// pokebw2's CommTvt_IsCameraEnabled
BOOL func_ov172_021be100(void) {
    if (func_0202eb0c() == TRUE && canPlayerExchangePhotos() == FALSE) {
        return TRUE;
    }
    return FALSE;
}

AppTaskMenu *func_ov172_021be11c(CommTvtWork *sys) {
    AppTaskMenuInit init = { 0 };
    AppTaskMenuItem items[2] = { 0 };
    AppTaskMenu *menu;

    items[0].str = GFL_MsgDataLoadStrbufNew(sys->msgData, BtlServerFlow_Text_034);
    items[1].str = GFL_MsgDataLoadStrbufNew(sys->msgData, BtlServerFlow_Text_035);
    items[0].color = 0x39e3;
    items[1].color = 0x39e3;
    items[0].type = 0;
    items[1].type = 0;
    init.heapId = sys->heapId;
    init.itemCount = 2;
    init.items = items;
    init.x = 24;
    init.y = 6;
    init.width = 8;
    init.height = 3;
    init.posType = APP_TASKMENU_POS_TOP_LEFT;
    menu = func_02027778(&init, sys->taskMenuRes);
    GFL_StrBufFree(items[0].str);
    GFL_StrBufFree(items[1].str);
    return menu;
}

void func_ov172_021be1a4(CommTvtWork *sys) {
    u8 i;

    sys->namesPrinted = 0;
    sys->namesPending = 0;
    for (i = 0; i < COMM_TVT_MEMBERS; i++) {
        GFL_BitmapFill(BmpWin_GetBitmap(sys->nameWindows[i]), 0);
        if (sys->mode != COMM_TVT_MODE_DRAW) {
            func_020456f8(sys->nameWindows[i]);
        }
    }
}

void func_ov172_021be1e4(CommTvtWork *sys) {
    u8 i;

    for (i = 0; i < COMM_TVT_MEMBERS; i++) {
        GFL_BitmapFill(BmpWin_GetBitmap(sys->nameWindows[i]), 0);
        BmpWin_FlushChar(sys->nameWindows[i]);
    }
}

// Points the cursor at the talker's name
void func_ov172_021be210(CommTvtWork *sys, u8 talker) {
    int displayMode = func_ov172_021be05c(sys);
    u8 window = talker;

    if (sys->mode == COMM_TVT_MODE_DRAW) {
        return;
    }
    if (displayMode == CTDM_DOUBLE) {
        if (talker == 0) {
            window = 2;
        } else {
            window = 3;
        }
    }
    if (func_ov172_021b8ba4(sys, sys->comm, talker) == TRUE &&
        func_ov172_021b8b8c(sys, sys->comm, talker) == TRUE) {
        u8 cursorX[COMM_TVT_MEMBERS] = { 72, 200, 72, 200 };
        u8 cursorY[COMM_TVT_MEMBERS] = { 88, 88, 184, 184 };
        ClActorPos pos;
        StrBuf *name = copyTrainerNameToNewStrbuf(
            (const u16 *)func_ov172_021b8b94(sys, sys->comm, talker)->playerInfo, sys->heapId);
        u8 width = GFL_FontGetBlockWidth(name, sys->font, 0);

        GFL_StrBufFree(name);
        pos.x = cursorX[window] + width / 2;
        pos.y = cursorY[window];
        func_0204b404(sys->talkerCursor, &pos, 0);
        func_0204b3dc(sys->talkerCursor, TRUE);
        func_0204b9a0(sys->talkerCursor);
    }
}

void func_ov172_021be2e0(CommTvtWork *sys) {
    func_0204b3dc(sys->talkerCursor, FALSE);
}

void func_ov172_021be2f0(CommTvtWork *sys, const CtvtCommMemberInfo *info) {
    func_0200ed54(getHollow_RivalData(GameData_GetSaveControl(sys->param->gameData)),
                  getIDAsUInt((PlayerInfo *)info->playerInfo));
}

void func_ov172_021be318(CommTvtWork *sys, BmpWin *window) {
    if (sys->waitIcon != NULL) {
        func_ov172_021be34c(sys);
    }
    sys->waitIcon = func_0202ee8c(sys->tcbEx, window, 15, 16, sys->heapId);
}

void func_ov172_021be34c(CommTvtWork *sys) {
    if (sys->waitIcon != NULL) {
        func_0202f038(sys->waitIcon);
        sys->waitIcon = NULL;
    }
}

void func_ov172_021bd1ac(CommTvtWork *sys);



BOOL func_ov172_021be364(GameProc *proc, u32 *state, void *param, void *work) {
    CommTvtWork *sys;
    CommTvtParam *tvtParam = param;

    if (tvtParam != NULL && tvtParam->unk4 == 3) {
        GFL_HeapCreateChild(1, 0x6d, 0xd << 16);
    } else {
        GFL_HeapCreateChild(1, 0x6d, 0x3a << 14);
    }
    sys = GFL_ProcInitSubsystem(proc, sizeof(CommTvtWork), 0x6d);
    sys->heapId = 0x6d;
    if (tvtParam == NULL) {
        sys->param = GFL_HeapAllocate(1, sizeof(CommTvtParam), TRUE, "comm_tvt_sys.c", 0x54d);
        sys->param->gameData = GameData_Create(1);
        sys->param->unk4 = 0;
        if (GCTX_HIDGetHeldKeys() & PAD_BUTTON_R) {
            sys->param->unk4 = 1;
        }
        if (GCTX_HIDGetHeldKeys() & PAD_BUTTON_X) {
            sys->param->unk4 = 2;
        }
    } else {
        sys->param = tvtParam;
    }
    sys->errorShown = FALSE;
    func_ov172_021bd1ac(sys);
    return TRUE;
}


void func_ov172_021bd3b8(CommTvtWork *sys);

static BOOL func_ov172_021be410(GameProc *proc, u32 *state, void *param, void *work) {
    CommTvtWork *sys = work;

    func_ov172_021bd3b8(sys);
    if (param == NULL) {
        GFL_HeapFree(sys->param);
    }
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(0x6d);
    return TRUE;
}


// pokebw2's CommTvt_ProcMain
static BOOL func_ov172_021be43c(GameProc *proc, u32 *state, void *param, void *work) {
    CommTvtWork *sys = work;

    if (func_0200fb24() && sys->errorShown == FALSE) {
        func_0200fb38();
        sys->errorShown = TRUE;
    }
    if (func_ov172_021bd43c(sys) == TRUE) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's COMM_TVT_PROC_FUNCTIONS
GameProcFunctions data_ov172_021c7240 = {
    func_ov172_021be364,
    func_ov172_021be43c,
    func_ov172_021be410,
};
