// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov186/badge_view.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/trainer_card.h"
#include "constants/arc.h"
#include "constants/sound.h"
#include "constants/text_banks.h"
#include "field/trcard_sys.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/g3d.h"
#include "gfl/graphics.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/net.h"
#include "gfl/overlay.h"
#include "gfl/proc.h"
#include "gfl/rtc_cache.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "gfl/touchpanel.h"
#include "gfl/ui.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_trig.h"
#include "nitro/fx/fx_mtx.h"
#include "nitro/gx.h"
#include "nitro/rtc.h"
#include "nnsys/g2d.h"
#include "nnsys/gfd.h"
#include "pml/poke_graphic.h"
#include "save/timesig_savedata.h"
#include "save/trainer_card.h"
#include "system/app_menu_common.h"
#include "system/bmp_winframe.h"
#include "system/game_data.h"
#include "system/gf_font.h"
#include "system/printsys.h"
#include "system/wipe.h"
#include "system/wordset.h"
#include "save/save_control.h"

// The Gym Badge case of the trainer card (badge_view.c, from the ROM's string)

#define BADGE_COUNT 8

// The actors
enum {
    BADGE_ACTOR_CARD,
    BADGE_ACTOR_RETURN,
    BADGE_ACTOR_EXIT,
    BADGE_ACTOR_SHORTCUT,
    BADGE_ACTOR_BADGE,
    BADGE_ACTOR_SPARKLE = BADGE_ACTOR_BADGE + BADGE_COUNT,
    BADGE_ACTOR_COUNT = BADGE_ACTOR_SPARKLE + BADGE_COUNT * 3,
};

// The actors' resources: the case's own and the touch bar's. The actor table's indices go on into the Gym Leaders'
enum {
    BADGE_RES_PALETTE,
    BADGE_RES_CHARS,
    BADGE_RES_CELLS,
    BADGE_RES_BAR_PALETTE,
    BADGE_RES_BAR_CHARS,
    BADGE_RES_BAR_CELLS,
    BADGE_RES_COUNT,
};

// What is touched: the badges, the Gym Leaders above them and the buttons
enum {
    BADGE_HIT_BADGE,
    BADGE_HIT_LEADER = BADGE_HIT_BADGE + BADGE_COUNT,
    BADGE_HIT_CARD = BADGE_HIT_LEADER + BADGE_COUNT,
    BADGE_HIT_SHORTCUT,
    BADGE_HIT_EXIT,
    BADGE_HIT_RETURN,
};

// The proc's states
enum {
    BADGE_SEQ_FADE_IN,
    BADGE_SEQ_MAIN,
    BADGE_SEQ_BUTTON,
    BADGE_SEQ_FADE_OUT_START,
    BADGE_SEQ_FADE_OUT,
};

// What the message window shows
enum {
    BADGE_MODE_BADGE,
    BADGE_MODE_LEADER,
};



// A badge's polishing, by rubbing it on the touch screen
typedef struct {
    u8 unk_0[2];
    s8 unk_2;
    s8 unk_3;
    u8 unk_4;
    u8 unk_5;
    u8 unk_6[2];
} BadgeViewPolish;

// The palettes of the touch bar's and the badges' sets: what is loaded, and its palette data
typedef struct {
    void *res[3];
    NNSG2dPaletteData *data[3];
} BadgeViewPalettes;

// Black's layout: 0x1c8 bytes, which func_ov111_021c6fc4 allocates
typedef struct {
    ArcTool *arc;
    Font *font;
    BmpWin *win;
    PrintWindow printWin;
    PrintQueue *queue;
    u8 unk_18[8];
    MsgData *msgData;
    WordSet *wordSet;
    StrBuf *expandBuf;
    StrBuf *strs[7];
    ClActUnit *unit;
    ClActor *actors[BADGE_ACTOR_COUNT];
    u32 res[BADGE_RES_COUNT];
    TCB *vblankTask;
    G3DManager *g3dMgr;
    u16 scene;
    G3DLight *lights;
    G3DCamera *camera;
    TrainerCardParam *param;
    int seq;
    // The button that ends the screen, BADGE_HIT_*
    int result;
    u16 page;
    u16 pageMax;
    int badgeCount;
    int frame;
    u16 unk_120;
    u16 unk_122;
    u32 unk_124;
    int unk_128;
    int unk_12c;
    int unk_130;
    int unk_134;
    int unk_138;
    int unk_13c;
    int unk_140;
    int unk_144;
    int unk_148;
    BadgeViewPalettes palettes[2];
    u8 hasBadge[BADGE_COUNT];
    TimeSigSave *timeSig;
    int polish[BADGE_COUNT];
    u8 level[BADGE_COUNT];
    u8 prevLevel[BADGE_COUNT];
    BadgeViewPolish rub;
    u32 unk_1c0;
    u32 unk_1c4;
} BadgeViewWork;


static int func_ov111_021c7e48(BadgeViewWork *work);
void func_ov111_021c8364(BadgeViewWork *work);
void func_ov111_021c810c(BadgeViewWork *work, u16 page, u16 pageMax);

// An actor: its position, its animation, its resources as indices into the work's
typedef struct {
    int x;
    int y;
    int sequence;
    int charRes;
    int paletteRes;
    int cellRes;
} BadgeViewActorDef;


typedef struct {
    u32 bg;
    u8 x;
    u8 y;
    u8 width;
    u8 height;
    u32 palette;
} BadgeViewWinDef;

typedef BOOL (*BadgeViewSeqFunc)(BadgeViewWork *wk);

BOOL func_ov111_021c6fc4(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov111_021c705c(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov111_021c7098(GameProc *proc, u32 *state, void *param, void *work);
void func_ov111_021c7830(BadgeViewWork *work);
static void func_ov111_021c7970(BadgeViewWork *wk);
BOOL func_ov111_021c79a0(BadgeViewWork *work);
BOOL func_ov111_021c79b8(BadgeViewWork *work);
static BOOL func_ov111_021c79d0(BadgeViewWork *work);
BOOL func_ov111_021c79f4(BadgeViewWork *work);
BOOL func_ov111_021c7a44(BadgeViewWork *work);
static BOOL func_ov111_021c7a70(BadgeViewWork *work);
void func_ov111_021c70ec(TCB *tcb, void *work);
void func_ov111_021c7130(BadgeViewWork *work, TrainerCardParam *param);
void func_ov111_021c7208(void);
void func_ov111_021c7228(BadgeViewWork *work);
void func_ov111_021c72c8(BadgeViewWork *work);
void func_ov111_021c72fc(BadgeViewWork *work);
void func_ov111_021c7504(BadgeViewWork *work);
void func_ov111_021c75e4(BadgeViewWork *work);
void func_ov111_021c7658(BadgeViewWork *work);
void func_ov111_021c76a4(BadgeViewWork *work);
void func_ov111_021c7744(BadgeViewWork *work);
void func_ov111_021c7908(BadgeViewWork *work);
void func_ov111_021c7ad4(BadgeViewWork *work, int line, int badge, StrBuf *buf);
void func_ov111_021c7b20(BadgeViewWork *work, int mode, int badge);
s32 func_ov111_021c7c78(s32 a, s32 b);
void func_ov111_021c7c94(BadgeViewWork *work);
void func_ov111_021c7cd0(BadgeViewWork *work, u32 hit, u32 touch, u32 x);
int func_ov111_021c7ef8(int angle);
void func_ov111_021c7f2c(BadgeViewWork *work, int hit, BOOL key);
void func_ov111_021c80b4(BadgeViewWork *work);
void func_ov111_021c81c0(BadgeViewWork *work);
void func_ov111_021c8208(BadgeViewWork *work);
void func_ov111_021c829c(BadgeViewWork *work, int badge, int level);
void func_ov111_021c82c0(BadgeViewWork *work, int badge, int level);
void func_ov111_021c832c(BadgeViewWork *work, int badge, int level);
void func_ov111_021c842c(BadgeViewWork *work, u32 badge);
void func_ov111_021c71cc(BadgeViewWork *work);
void func_ov111_021c74d0(BadgeViewWork *work);
void func_ov111_021c7730(BadgeViewWork *work);
void func_ov111_021c7808(BadgeViewWork *work);
void func_ov111_021c78c4(BadgeViewWork *work);
void func_ov111_021c78f8(BadgeViewWork *work);
void func_ov111_021c7934(BadgeViewWork *wk);
void func_ov111_021c795c(BadgeViewWork *wk);
void func_ov111_021c8078(BadgeViewWork *work);
void func_ov111_021c80e8(BadgeViewWork *wk, int badge, BOOL visible);
void func_ov111_021c81a4(BadgeViewWork *work, u16 value);
void func_ov111_021c83dc(BadgeViewPolish *rub, int polish);
void func_ov111_021c840c(BadgeViewPolish *rub);
void func_ov111_021c8414(BadgeViewWork *work, int index);

// The file's tables are declared where they are, here and between the functions below, for the order of its
// .rodata: MWCC's sort of them by size depends on where each one is declared

// pokebw2's sBadgeViewAnimation
static const G3DSceneAnimationSetup data_ov111_021c8804 = { 1, 0 };

// pokebw2's sBadgeViewResources
static const G3DSceneResourceSetup data_ov111_021c8844[] = {
#include "asset/badge_view_data_ov111_021c8844.inc"
};

// pokebw2's sBadgeViewVRAMConfig
static const BGSysVRAMConfig data_ov111_021c88fc = {
    GX_VRAM_BG_128_B, GX_VRAM_BGEXTPLTT_NONE,  GX_VRAM_SUB_BG_128_C,       GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_64_E, GX_VRAM_OBJEXTPLTT_NONE, 0x100,      0,
    GX_VRAM_TEX_0_A,  GX_VRAM_TEXPLTT_01_FG,   GX_OBJVRAMMODE_CHAR_1D_32K, GX_OBJVRAMMODE_CHAR_1D_32K,
};

// The first of each Gym Leader's three messages
// pokebw2's sBadgeViewLeaderMessages
static const u32 data_ov111_021c8894[BADGE_COUNT] = {
#include "asset/badge_view_data_ov111_021c8894.inc"
};

// pokebw2's sBadgeViewModelActor
static const G3DSceneActorSetup data_ov111_021c8824 = { 0, 0, 0, 0, &data_ov111_021c8804, 1 };

// pokebw2's sBadgeViewScene
static const G3DSceneSetup data_ov111_021c8834 = { data_ov111_021c8844, NELEMS(data_ov111_021c8844), &data_ov111_021c8824, 1 };

// The message window
static const BadgeViewWinDef sBadgeViewWinDef = { 4, 2, 10, 28, 12, 15 };

// The angle of each badge's model facing the front, in 0x100000 for a full turn
// pokebw2's sBadgeViewAngles
static const s32 data_ov111_021c88b4[BADGE_COUNT] = {
#include "asset/badge_view_data_ov111_021c88b4.inc"
};

// pokebw2's sBadgeViewMessageRect
static const TouchRect data_ov111_021c8808[] = {
#include "asset/badge_view_data_ov111_021c8808.inc"
};

// Nothing reads the second word but func_ov111_021c7830, which adds the first to it
static struct {
    int step;
    int count;
    int unk8[6];
// pokebw2's sBadgeViewCounter
} data_ov111_021c8e80;

// pokebw2's sBadgeViewActorDefs
static const BadgeViewActorDef data_ov111_021c8aa0[BADGE_ACTOR_COUNT] = {
#include "asset/badge_view_data_ov111_021c8aa0.inc"
};

// pokebw2's sBadgeViewLights
static const LightSetup data_ov111_021c88d4[] = {
    { 0, { { -0xfff, -0xfff, -0xfff }, GX_RGB(31, 31, 31) } },
    { 1, { { 0xfff, -0xfff, -0xfff }, GX_RGB(31, 31, 31) } },
    { 2, { { -0xfff, -0xfff, -0xfff }, GX_RGB(31, 31, 31) } },
    { 3, { { -0xfff, -0xfff, -0xfff }, GX_RGB(31, 31, 31) } },
};

// pokebw2's sBadgeViewLightList
static const LightSetupList data_ov111_021c8810 = { data_ov111_021c88d4, NELEMS(data_ov111_021c88d4) };

// pokebw2's sBadgeViewRects
static const TouchRect data_ov111_021c898c[] = {
#include "asset/badge_view_data_ov111_021c898c.inc"
};

// Which of the palette file's palettes each badge's model uses
// pokebw2's sBadgeViewPaletteIndices
static const u32 data_ov111_021c8874[BADGE_COUNT] = {
#include "asset/badge_view_data_ov111_021c8874.inc"
};

// pokebw2's sBadgeViewBGs
static const u32 data_ov111_021c885c[] = { 4, 5, 6, 1, 2, 3 };

// pokebw2's sBadgeViewSeqFuncs
static BadgeViewSeqFunc data_ov111_021c8e58[] = {
    func_ov111_021c79b8, func_ov111_021c79d0, func_ov111_021c79f4, func_ov111_021c7a44, func_ov111_021c7a70,
};

// pokebw2's BadgeView_InitPrint









BOOL func_ov111_021c6fc4(GameProc *proc, u32 *state, void *param, void *work) {
    BadgeViewWork *wk;

    GFL_HeapCreateChild(HEAPID_USER, HEAPID_TRAINER_CARD_SCREEN, 0x20000);
    wk = GFL_ProcInitSubsystem(proc, sizeof(BadgeViewWork), HEAPID_TRAINER_CARD_SCREEN);
    MIi_CpuClearFast(0, wk, sizeof(BadgeViewWork));
    func_ov111_021c7130(wk, param);
    func_ov111_021c7208();
    func_ov111_021c7228(wk);
    func_ov111_021c72fc(wk);
    func_ov111_021c7504(wk);
    func_ov111_021c75e4(wk);
    func_ov111_021c76a4(wk);
    func_ov111_021c7744(wk);
    func_ov111_021c78c4(wk);
    func_ov111_021c7934(wk);
    func_ov111_021c81a4(wk, wk->page);
    func_0203d34c(TRUE, HEAPID_TRAINER_CARD_SCREEN);
    GFL_WipeSet(0, 1, 1, 0, 6, 1, HEAPID_TRAINER_CARD_SCREEN);
    return TRUE;
}

BOOL func_ov111_021c705c(GameProc *proc, u32 *state, void *param, void *work) {
    BadgeViewWork *wk = work;

    if (!func_ov111_021c79a0(wk)) {
        return TRUE;
    }
    func_ov111_021c7830(wk);
    func_ov111_021c7970(wk);
    func_0204a600();
    wk->frame++;
    if (wk->frame >= 0x100) {
        wk->frame = 0;
    }
    return FALSE;
}

BOOL func_ov111_021c7098(GameProc *proc, u32 *state, void *param, void *work) {
    BadgeViewWork *wk = work;

    func_ov111_021c795c(wk);
    func_ov111_021c78f8(wk);
    func_ov111_021c7808(wk);
    func_ov111_021c7730(wk);
    func_ov111_021c7658(wk);
    func_ov111_021c74d0(wk);
    func_ov111_021c72c8(wk);
    func_ov111_021c71cc(wk);
    func_ov111_021c81c0(wk);
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapStatusValidate(HEAPID_TRAINER_CARD_SCREEN);
    GFL_HeapDelete(HEAPID_TRAINER_CARD_SCREEN);
    return TRUE;
}


void func_ov111_021c70ec(TCB *tcb, void *work) {
    BadgeViewWork *wk = work;
    int offset = -(wk->frame / 2);

    GFL_BGSysMoveBG(6, BG_MOVE_SET_X, offset);
    GFL_BGSysMoveBG(6, BG_MOVE_SET_Y, offset);
    GFL_BGSysMoveBG(3, BG_MOVE_SET_X, offset);
    GFL_BGSysMoveBG(3, BG_MOVE_SET_Y, offset);
    func_0204a648();
    GFL_BGSysUpdate();
}

void func_ov111_021c7130(BadgeViewWork *work, TrainerCardParam *param) {
    int i;

    work->param = param;
    work->result = 0;
    work->arc = GFL_ArcSysCreateFileHandle(ARCID_TRAINER_CARD_GRA, HEAPID_TRAINER_CARD_SCREEN);
    work->msgData = GFL_MsgSysLoadData(0, 2, 0xbb, HEAPID_TRAINER_CARD_SCREEN);
    for (i = 0; i < 7; i++) {
        work->strs[i] = GFL_MsgDataLoadStrbufNew(work->msgData, i);
        GFL_StrBufGetStringPtr(work->strs[i]);
    }
    work->expandBuf = GFL_StrBufCreate(0x68, HEAPID_TRAINER_CARD_SCREEN);
    work->wordSet = GFL_WordSetSystemCreate(8, 0x40, HEAPID_TRAINER_CARD_SCREEN);
    work->vblankTask = GFL_VBlankTCBAdd(func_ov111_021c70ec, work, 0);
    work->badgeCount = 8;
    work->unk_144 = -1;
    func_ov111_021c80b4(work);
    work->timeSig = getTimeSigSaveBlock(work->param->gameData);
    func_ov111_021c8208(work);
}



void func_ov111_021c71cc(BadgeViewWork *work) {
    int i;

    for (i = 0; i < 7; i++) {
        GFL_StrBufFree(work->strs[i]);
    }
    GFL_StrBufFree(work->expandBuf);
    GFL_WordSetSystemFree(work->wordSet);
    GFL_MsgDataFree(work->msgData);
    GFL_TCBRemove(work->vblankTask);
    GFL_ArcToolFree(work->arc);
}

void func_ov111_021c7208(void) {
    GFL_BGSysSetVRAMBanks(&data_ov111_021c88fc);
    GX_SetDispSelect(GX_DISP_SELECT_SUB_MAIN);
}

void func_ov111_021c7228(BadgeViewWork *work) {
    GFL_BGSysCreate(HEAPID_TRAINER_CARD_SCREEN);
    {
        BGSysLCDConfig lcd = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_3D };

        GFL_BGSysSetLCDConfig(&lcd);
    }
    {
        BGSetup setups[6] = {
            { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xf800, GX_BG_CHARBASE_0x00000, 0x8000,
              GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE },
            { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xf000, GX_BG_CHARBASE_0x08000,
              0x4000, GX_BG_EXTPLTT_01, 1, GX_BG_AREAOVER_XLU, FALSE },
            { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xe800, GX_BG_CHARBASE_0x10000,
              0x4000, GX_BG_EXTPLTT_01, 2, GX_BG_AREAOVER_XLU, FALSE },
            { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xf800, GX_BG_CHARBASE_0x00000, 0x8000,
              GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE },
            { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xf000, GX_BG_CHARBASE_0x10000,
              0x4000, GX_BG_EXTPLTT_01, 2, GX_BG_AREAOVER_XLU, FALSE },
            { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xe800, GX_BG_CHARBASE_0x08000,
              0x4000, GX_BG_EXTPLTT_01, 3, GX_BG_AREAOVER_XLU, FALSE },
        };
        u32 i;

        for (i = 0; i < NELEMS(data_ov111_021c885c); i++) {
            u32 bg = data_ov111_021c885c[i];

            GFL_BGSysCreateBG(bg, &setups[i], BGMODE_TEXT);
            GFL_BGSysClearScr(bg);
            GFL_BGSysClearCharCore(bg, 0x20, 0, HEAPID_TRAINER_CARD_SCREEN);
            GFL_BGSysSetBGEnabled(bg, TRUE);
        }
    }
    GFL_BGSysSetBGEnabled(7, FALSE);
    G2_BlendNone();
    G2S_SetBlendAlpha(GX_BLEND_PLANEMASK_BG1, GX_BLEND_PLANEMASK_BG2, 7, 16);
}


void func_ov111_021c72c8(BadgeViewWork *work) {
    int i;

    for (i = 0; i < NELEMS(data_ov111_021c885c); i++) {
        GFL_BGSysReleaseBG(data_ov111_021c885c[i]);
    }
    GFL_BGSysFree();
    G2_BlendNone();
    G2S_BlendNone();
}

void func_ov111_021c72fc(BadgeViewWork *work) {
    ArcTool *arc = work->arc;
    ArcTool *ui;
    int i;

    GFL_BGSysLoadArcNCGRStatic(arc, 25, 6, 0, 0, FALSE, HEAPID_TRAINER_CARD_SCREEN);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 34, 6, 0, 0, FALSE, HEAPID_TRAINER_CARD_SCREEN);
    GFL_G2DIOLoadArcNCLRDefault(arc, 1, 4, 0, 0, HEAPID_TRAINER_CARD_SCREEN);
    GFL_BGSysLoadArcNCGRStatic(arc, 26, 5, 0, 0, FALSE, HEAPID_TRAINER_CARD_SCREEN);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 35, 5, 0, 0, FALSE, HEAPID_TRAINER_CARD_SCREEN);
    GFL_BGSysLoadArcNCGRStatic(arc, 25, 3, 0, 0, FALSE, HEAPID_TRAINER_CARD_SCREEN);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 34, 3, 0, 0, FALSE, HEAPID_TRAINER_CARD_SCREEN);
    GFL_G2DIOLoadArcNCLRDefault(arc, 0, 0, 0, 0, HEAPID_TRAINER_CARD_SCREEN);
    GFL_BGSysLoadArcNCGRStatic(arc, 25, 1, 0, 0, FALSE, HEAPID_TRAINER_CARD_SCREEN);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 33, 1, 0, 0, FALSE, HEAPID_TRAINER_CARD_SCREEN);
    ui = GFL_ArcSysCreateFileHandle(getUINarcIdx(), HEAPID_TRAINER_CARD_SCREEN);
    GFL_BGSysLoadArcNCGRStatic(ui, func_02027628(), 2, 0, 0, FALSE, HEAPID_TRAINER_CARD_SCREEN);
    loadBGScrToVramByFileNoReserveNegAlign(ui, func_0202762c(), 2, 0, 0, FALSE, HEAPID_TRAINER_CARD_SCREEN);
    GFL_G2DIOLoadArcNCLR(ui, func_02027624(), 0, 0, 0x1c0, 0x20, HEAPID_TRAINER_CARD_SCREEN);
    GFL_BGSysSetScrPaletteNo(2, 0, 21, 32, 3, 14);
    GFL_BGSysLoadScr(2);
    GFL_ArcToolFree(ui);
    GFL_BGSysLoadNCLRDefault(23, 5, 0, 0x1e0, 0x20, HEAPID_TRAINER_CARD_SCREEN);
    GFL_BGSysLoadNCLRDefault(23, 5, 4, 0x1e0, 0x20, HEAPID_TRAINER_CARD_SCREEN);
    LoadSysMsgBoxPalette(4, 14, 0, HEAPID_TRAINER_CARD_SCREEN);
    LoadSysMsgBoxBGChar(4, 1, 0, HEAPID_TRAINER_CARD_SCREEN);
    for (i = 0; i < 3; i++) {
        work->palettes[0].res[i] = GFL_G2DIOReadNCLRArc(arc, 4 - i, &work->palettes[0].data[i], HEAPID_TRAINER_CARD_SCREEN);
    }
    for (i = 0; i < 3; i++) {
        work->palettes[1].res[i] = GFL_G2DIOReadNCLRArc(arc, 23 - i, &work->palettes[1].data[i], HEAPID_TRAINER_CARD_SCREEN);
    }
}




void func_ov111_021c74d0(BadgeViewWork *work) {
    int i;

    for (i = 0; i < 3; i++) {
        GFL_HeapFree(work->palettes[1].res[i]);
    }
    for (i = 0; i < 3; i++) {
        GFL_HeapFree(work->palettes[0].res[i]);
    }
}

void func_ov111_021c7504(BadgeViewWork *work) {
    ArcTool *ui = GFL_ArcSysCreateFileHandle(getUINarcIdx(), HEAPID_TRAINER_CARD_SCREEN);

    ClActSys_Create(&data_020a1448, &data_ov111_021c88fc, HEAPID_TRAINER_CARD_SCREEN);
    work->unit = func_0204b100(36, 1, HEAPID_TRAINER_CARD_SCREEN);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_OBJ, TRUE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
    work->res[BADGE_RES_CHARS] = func_0204a6c8(work->arc, 27, FALSE, 0, HEAPID_TRAINER_CARD_SCREEN);
    work->res[BADGE_RES_PALETTE] = func_0204abf0(work->arc, 2, 0, 0, HEAPID_TRAINER_CARD_SCREEN);
    work->res[BADGE_RES_CELLS] = func_0204af28(work->arc, 45, 47, HEAPID_TRAINER_CARD_SCREEN);
    work->res[BADGE_RES_BAR_CHARS] = func_0204a6c8(ui, func_02027618(), FALSE, 0, HEAPID_TRAINER_CARD_SCREEN);
    work->res[BADGE_RES_BAR_PALETTE] = func_0204ac18(ui, func_02027614(), 0, 0x160, 0, 3, HEAPID_TRAINER_CARD_SCREEN);
    work->res[BADGE_RES_BAR_CELLS] =
        func_0204af28(ui, func_0202761c(0), func_02027620(0), HEAPID_TRAINER_CARD_SCREEN);
    GFL_ArcToolFree(ui);
}


void func_ov111_021c75e4(BadgeViewWork *work) {
    ClActorSetup setup;
    int i;

    for (i = 0; i < BADGE_ACTOR_COUNT; i++) {
        const BadgeViewActorDef *def = &data_ov111_021c8aa0[i];

        setup.x = def->x;
        setup.y = def->y;
        setup.sequence = def->sequence;
        setup.bgPriority = 0;
        setup.priority = BADGE_ACTOR_COUNT - i;
        work->actors[i] = func_0204b294(work->unit, work->res[def->charRes], work->res[def->paletteRes],
                                        work->res[def->cellRes], &setup, CLACT_SURFACE_MAIN,
                                        HEAPID_TRAINER_CARD_SCREEN);
        func_0204b92c(work->actors[i], TRUE);
    }
}


void func_ov111_021c7658(BadgeViewWork *work) {
    func_0204a8d4(work->res[BADGE_RES_BAR_CHARS]);
    func_0204ada4(work->res[BADGE_RES_BAR_PALETTE]);
    func_0204afd8(work->res[BADGE_RES_BAR_CELLS]);
    func_0204a8d4(work->res[BADGE_RES_CHARS]);
    func_0204ada4(work->res[BADGE_RES_PALETTE]);
    func_0204afd8(work->res[BADGE_RES_CELLS]);
    func_0204b1cc(work->unit);
    func_0204a5ac();
}

void func_ov111_021c76a4(BadgeViewWork *work) {
    GFL_G3DSysCreate(FALSE, 1, FALSE, 1, 0x1000, HEAPID_TRAINER_CARD_SCREEN, NULL);
    work->lights = GFL_G3DLightCreate(&data_ov111_021c8810, HEAPID_TRAINER_CARD_SCREEN);
    GFL_G3DLightFlush(work->lights);
    {
        G3DCameraProjection projection = { G3DCAM_PROJECTION_PERSPECTIVE,
                                           FX_SinIdx(DEG_TO_IDX(16)),
                                           FX_CosIdx(DEG_TO_IDX(16)),
                                           FX32_CONST(4.0 / 3.0),
                                           0,
                                           FX32_ONE,
                                           FX32_CONST(1024),
                                           0 };

        GFL_G3DSysMtxSetProjection(&projection);
    }
    GFL_BGSysSetBGEnabled(0, TRUE);
    G3X_AlphaBlend(TRUE);
}





void func_ov111_021c7730(BadgeViewWork *work) {
    GFL_G3DLightFree(work->lights);
    GFL_G3DSysFree();
}

void func_ov111_021c7744(BadgeViewWork *work) {
    G3DActor *actor;
    int i;
    int count;

    work->g3dMgr = GFL_G3DMgrCreate(10, 10, HEAPID_TRAINER_CARD_SCREEN);
    work->scene = GFL_G3DMgrNewScene(work->g3dMgr, &data_ov111_021c8834);
    actor = GFL_G3DMgrGetActor(work->g3dMgr, work->scene);
    count = GFL_G3DActorGetAnmCount(actor);
    for (i = 0; i < count; i++) {
        GFL_G3DActorBindAnm(actor, i);
    }
    {
        VecFx32 position = { 0, 0x1666, 0x57d7 };
        VecFx32 target = { 0, -0x17d7, 0 };
        VecFx32 up = { 0, FX32_ONE, 0 };

        work->camera = GFL_G3DCameraCreate(G3DCAM_PROJECTION_PERSPECTIVE, FX_SinIdx(DEG_TO_IDX(20)),
                                           FX_CosIdx(DEG_TO_IDX(20)), FX32_CONST(4.0 / 3.0), 0, FX32_ONE,
                                           FX32_CONST(1024), 0, &position, &up, &target, HEAPID_TRAINER_CARD_SCREEN);
    }
}


void func_ov111_021c7808(BadgeViewWork *work) {
    GFL_G3DCameraFree(work->camera);
    GFL_G3DMgrDeleteScene(work->g3dMgr, work->scene);
    GFL_G3DMgrFree(work->g3dMgr);
}

void func_ov111_021c7830(BadgeViewWork *work) {
    SRTMatrix mtx;
    G3DActor *actor;
    fx32 frame;
    int count;
    int i;

    mtx.scale.x = FX32_ONE;
    mtx.scale.y = FX32_ONE;
    mtx.scale.z = FX32_ONE;
    mtx.translation.x = 0;
    mtx.translation.y = 0;
    mtx.translation.z = 0;
    MTX_Identity33_(&mtx.rotation);
    GFL_G3DCameraFlush(work->camera);
    actor = GFL_G3DMgrGetActor(work->g3dMgr, work->scene);
    count = GFL_G3DActorGetAnmCount(actor);
    GFL_G3DActorGetAnmFrame(actor, 0, &frame);
    for (i = 0; i < count; i++) {
        GFL_G3DActorSetAnmFrame(actor, i, &work->unk_13c);
    }
    GFL_G3DSysReset();
    GFL_G3DSysMtxViewFlush();
    GFL_G3DSysDrawObj(GFL_G3DMgrGetActor(work->g3dMgr, work->scene), &mtx);
    GFL_G3DSysReqSwapBuffers();
    data_ov111_021c8e80.step += data_ov111_021c8e80.count;
}




void func_ov111_021c78c4(BadgeViewWork *work) {
    BmpWin_InitAllocator(0x27);
    work->win = BmpWin_CreateDynamic(sBadgeViewWinDef.bg, sBadgeViewWinDef.x, sBadgeViewWinDef.y, sBadgeViewWinDef.width, sBadgeViewWinDef.height, sBadgeViewWinDef.palette, 1);
    GFL_BitmapFill(BmpWin_GetBitmap(work->win), 15);
}



void func_ov111_021c78f8(BadgeViewWork *work) {
    BmpWin_Free(work->win);
    BmpWin_FreeAllocator();
}

void func_ov111_021c7908(BadgeViewWork *work) {
    BmpWin *win;

    BmpWin_DrawFrame(work->win, 0, 1, 14);
    win = work->win;
    BmpWin_FlushChar(win);
    BmpWin_FlushMap(win);
    GFL_BGSysLoadScr(BmpWin_GetBGIndex(win));
}




void func_ov111_021c7934(BadgeViewWork *wk) {
    wk->font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, HEAPID_TRAINER_CARD_SCREEN);
    wk->queue = func_0201c440(HEAPID_TRAINER_CARD_SCREEN);
    PrintWindow_Init(&wk->printWin, wk->win);
}

// pokebw2's BadgeView_ExitPrint
void func_ov111_021c795c(BadgeViewWork *wk) {
    func_0201c4c0(wk->queue);
    GFL_FontFree(wk->font);
}

// pokebw2's BadgeView_UpdatePrint
static void func_ov111_021c7970(BadgeViewWork *wk) {
    func_0201c4e4(wk->queue);
    PrintWindow_Flush(&wk->printWin, wk->queue);
}

BOOL func_ov111_021c79a0(BadgeViewWork *work) {
    return data_ov111_021c8e58[work->seq](work);
}


BOOL func_ov111_021c79b8(BadgeViewWork *work) {
    if (GFL_WipeIsFinished()) {
        work->seq = BADGE_SEQ_MAIN;
    }
    return TRUE;
}

static BOOL func_ov111_021c79d0(BadgeViewWork *work) {
    if (func_0201c6b4(work->queue) == 0) {
        return TRUE;
    }
    func_ov111_021c7e48(work);
    func_ov111_021c8364(work);
    return TRUE;
}

BOOL func_ov111_021c79f4(BadgeViewWork *work) {
    switch (work->result) {
    case BADGE_HIT_EXIT:
        if (!func_0204b98c(work->actors[BADGE_ACTOR_EXIT])) {
            work->seq = BADGE_SEQ_FADE_OUT_START;
        }
    case BADGE_HIT_RETURN:
        if (!func_0204b98c(work->actors[BADGE_ACTOR_RETURN])) {
            work->seq = BADGE_SEQ_FADE_OUT_START;
        }
    case BADGE_HIT_CARD:
        if (!func_0204b98c(work->actors[BADGE_ACTOR_CARD])) {
            work->seq = BADGE_SEQ_FADE_OUT_START;
        }
    }
    return TRUE;
}


BOOL func_ov111_021c7a44(BadgeViewWork *work) {
    GFL_WipeSet(0, 0, 0, 0, 6, 1, 0x27);
    work->seq = BADGE_SEQ_FADE_OUT;
    return TRUE;
}

static BOOL func_ov111_021c7a70(BadgeViewWork *work) {
    if (GFL_WipeIsFinished()) {
        if (work->result == BADGE_HIT_EXIT) {
            work->param->next = 0;
            work->param->result = 1;
            work->seq = 0;
        } else if (work->result == BADGE_HIT_RETURN) {
            work->param->next = 0;
            work->param->result = 0;
            work->seq = 0;
        } else if (work->result == BADGE_HIT_CARD) {
            work->param->next = 3;
            work->seq = 0;
        }
        return FALSE;
    }
    return TRUE;
}


void func_ov111_021c7ad4(BadgeViewWork *work, int line, int badge, StrBuf *buf) {
    GFL_MsgDataLoadStrbuf(work->msgData, data_ov111_021c8894[badge] + line, buf);
    PrintWindow_PrintColor(&work->printWin, work->queue, 0, line * 32, buf, work->font, PRINT_COLOR(1, 2, 15));
}

void func_ov111_021c7b20(BadgeViewWork *work, int mode, int badge) {
    StrBuf *buf = GFL_StrBufCreate(0x68, HEAPID_TRAINER_CARD_SCREEN);
    TimeSigBadgeDate date;
    int i;

    GFL_BitmapFill(BmpWin_GetBitmap(work->win), 15);
    switch (mode) {
    case BADGE_MODE_BADGE:
        date = func_02008a54(work->timeSig, badge);
        WordSetNumber(work->wordSet, 0, 2000 + date.year, 4, 0, TRUE);
        WordSetNumber(work->wordSet, 1, date.month, 2, 0, TRUE);
        WordSetNumber(work->wordSet, 2, date.day, 2, 0, TRUE);
        GFL_MsgDataLoadStrbuf(work->msgData, 88 + badge * 3, buf);
        GFL_WordSetFormatStrbuf(work->wordSet, work->expandBuf, buf);
        PrintWindow_PrintColor(&work->printWin, work->queue, 0, 0, work->expandBuf, work->font, PRINT_COLOR(1, 2, 15));
        for (i = 1; i < 3; i++) {
            GFL_MsgDataLoadStrbuf(work->msgData, 88 + badge * 3 + i, buf);
            PrintWindow_PrintColor(&work->printWin, work->queue, 0, i * 32, buf, work->font, PRINT_COLOR(1, 2, 15));
        }
        break;
    case BADGE_MODE_LEADER:
        for (i = 0; i < 2; i++) {
            func_ov111_021c7ad4(work, i, badge, buf);
        }
        if (work->hasBadge[badge]) {
            func_ov111_021c7ad4(work, 2, badge, buf);
        }
        break;
    }
    GFL_StrBufFree(buf);
}






s32 func_ov111_021c7c78(s32 a, s32 b) {
    if (a >= b) {
        return a % b;
    }
    if (a < 0) {
        a += 0x100000;
    }
    return a;
}

void func_ov111_021c7c94(BadgeViewWork *work) {
    if (work->unk_140 > 0x1000) {
        work->unk_140 -= 400;
    } else if (work->unk_140 < -0x1000) {
        work->unk_140 += 400;
    } else if (work->unk_140 == 0) {
        work->unk_140 = 0x1000;
    }
}

void func_ov111_021c7cd0(BadgeViewWork *work, u32 hit, u32 touch, u32 x) {
    int i;

    if (work->unk_122 == 0 && touch != 0) {
        work->unk_138 = work->unk_13c;
        work->unk_130 = x;
    } else if (work->unk_122 == 1 && touch == 1) {
        work->unk_13c = work->unk_138 + (x - work->unk_130) * 0x400;
        work->unk_13c = func_ov111_021c7c78(work->unk_13c, 0x100000);
    } else if (work->unk_122 == 1 && touch == 0) {
        if (work->unk_12c != 0) {
            work->unk_140 = (work->unk_128 - work->unk_12c) * 0x800;
        }
        if (work->unk_140 > 40000) {
            work->unk_140 = 40000;
        } else if (work->unk_140 < -40000) {
            work->unk_140 = -40000;
        }
    }
    if (work->unk_122 == 0 && touch == 0) {
        work->unk_13c += work->unk_140;
        work->unk_13c = func_ov111_021c7c78(work->unk_13c, 0x100000);
        if (work->unk_144 >= 0) {
            if (work->unk_148 > work->unk_13c - 50000 && work->unk_148 < work->unk_13c + 50000) {
                work->unk_140 = 0;
                work->unk_13c = work->unk_148;
                work->unk_144 = -1;
            }
        } else {
            func_ov111_021c7c94(work);
            for (i = 0; i < BADGE_COUNT; i++) {
                if (data_ov111_021c88b4[i] > work->unk_13c - 10000 && data_ov111_021c88b4[i] < work->unk_13c + 10000) {
                    int speed = work->unk_140;

                    if (speed < 0) {
                        speed = -speed;
                    }
                    if (speed < 10000) {
                        work->unk_13c = data_ov111_021c88b4[i];
                        work->unk_140 = 0;
                    }
                }
            }
        }
    }
    work->unk_12c = work->unk_128;
    work->unk_128 = x;
    work->unk_122 = touch;
}


static int func_ov111_021c7e48(BadgeViewWork *work) {
    u32 touchX = 0;
    u32 touchY = 0;
    BOOL key = FALSE;
    int hit;
    int messageHit;
    int held;
    int messageHeld;

    hit = func_02035c08(data_ov111_021c898c);
    held = func_02035ba4(data_ov111_021c898c);
    messageHit = func_02035c08(data_ov111_021c8808);
    messageHeld = func_02035ba4(data_ov111_021c8808);
    func_02035cb8(&touchX, &touchY);
    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_B) {
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        key = TRUE;
        hit = BADGE_HIT_RETURN;
    } else if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_X) {
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        hit = BADGE_HIT_EXIT;
    } else if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_Y) {
        hit = BADGE_HIT_SHORTCUT;
    } else if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_A) {
        hit = BADGE_HIT_CARD;
    }
    func_ov111_021c7f2c(work, hit, key);
    func_ov111_021c7cd0(work, messageHit, messageHeld + 1, touchX);
    func_ov111_021c842c(work, held);
    return hit;
}



int func_ov111_021c7ef8(int angle) {
    int i;

    for (i = 0; i < 8; i++) {
        if (data_ov111_021c88b4[i] > angle - 10000 && data_ov111_021c88b4[i] < angle + 10000) {
            break;
        }
    }
    if (i == 8) {
        i = 0;
    }
    return i;
}

void func_ov111_021c7f2c(BadgeViewWork *work, int hit, BOOL key) {
    int angle;

    if (hit == TOUCH_RECT_NONE) {
        return;
    }
    switch (hit) {
    case BADGE_HIT_BADGE + 0:
    case BADGE_HIT_BADGE + 1:
    case BADGE_HIT_BADGE + 2:
    case BADGE_HIT_BADGE + 3:
    case BADGE_HIT_BADGE + 4:
    case BADGE_HIT_BADGE + 5:
    case BADGE_HIT_BADGE + 6:
    case BADGE_HIT_BADGE + 7:
        if (work->hasBadge[hit]) {
            GFL_SndSEPlay(SEQ_SE_SYS_32);
            func_ov111_021c7b20(work, BADGE_MODE_BADGE, hit);
            func_ov111_021c7908(work);
            work->unk_144 = 0;
            work->unk_148 = data_ov111_021c88b4[hit];
            work->unk_140 = 40000;
        }
        break;
    case BADGE_HIT_LEADER + 0:
    case BADGE_HIT_LEADER + 1:
    case BADGE_HIT_LEADER + 2:
    case BADGE_HIT_LEADER + 3:
    case BADGE_HIT_LEADER + 4:
    case BADGE_HIT_LEADER + 5:
    case BADGE_HIT_LEADER + 6:
    case BADGE_HIT_LEADER + 7:
        if (work->unk_140 == 0) {
            angle = func_ov111_021c7ef8(work->unk_13c);
            GFL_SndSEPlay(SEQ_SE_SYS_31);
            func_ov111_021c7b20(work, BADGE_MODE_LEADER, (hit - BADGE_HIT_LEADER + angle) % BADGE_COUNT);
            func_ov111_021c7908(work);
        }
        break;
    case BADGE_HIT_CARD:
        work->seq = BADGE_SEQ_BUTTON;
        work->result = hit;
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        func_0204b878(work->actors[BADGE_ACTOR_CARD], 12);
        break;
    case BADGE_HIT_SHORTCUT:
        func_ov111_021c8078(work);
        break;
    case BADGE_HIT_EXIT:
        GFL_SndSEPlay(SEQ_SE_CLOSE1);
        work->seq = BADGE_SEQ_BUTTON;
        work->result = hit;
        func_0204b878(work->actors[BADGE_ACTOR_EXIT], 8);
        break;
    case BADGE_HIT_RETURN:
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        work->seq = BADGE_SEQ_BUTTON;
        work->result = hit;
        func_0204b878(work->actors[BADGE_ACTOR_RETURN], 9);
        if (key) {
            func_020355d8(FALSE);
        } else {
            func_020355d8(TRUE);
        }
        break;
    }
}



void func_ov111_021c8078(BadgeViewWork *work) {
    BOOL registered = GameData_IsShortcutRegistered(work->param->gameData, SHORTCUT_TRAINER_CARD_BADGES) ^ 1;

    func_0204b878(work->actors[BADGE_ACTOR_SHORTCUT], registered + 6);
    GameData_SetKeyItemRegistration(work->param->gameData, SHORTCUT_TRAINER_CARD_BADGES, registered);
    GFL_SndSEPlay(SEQ_SE_SYS_07);
}

void func_ov111_021c80b4(BadgeViewWork *work) {
    work->page = 0;
    work->pageMax = work->badgeCount / 10;
    if (work->badgeCount % 10 != 0) {
        work->pageMax++;
    }
}








// pokebw2's BadgeView_SetSparklesVisible
void func_ov111_021c80e8(BadgeViewWork *wk, int badge, BOOL visible) {
    int i;

    for (i = 0; i < 3; i++) {
        func_0204b3dc(wk->actors[BADGE_ACTOR_SPARKLE + badge * 3 + i], visible);
    }
}

void func_ov111_021c810c(BadgeViewWork *work, u16 page, u16 pageMax) {
    TrainerCardSave *card = getTrainerCardDataBlkAddress(work->param->gameData);
    int i;

    for (i = 0; i < BADGE_COUNT; i++) {
        if (isBadgeObtained(card, i)) {
            func_0204b3dc(work->actors[BADGE_ACTOR_BADGE + i], TRUE);
            func_ov111_021c80e8(work, i, TRUE);
            work->hasBadge[i] = TRUE;
        } else {
            func_0204b3dc(work->actors[BADGE_ACTOR_BADGE + i], FALSE);
            func_ov111_021c80e8(work, i, FALSE);
            work->hasBadge[i] = FALSE;
        }
    }
    func_0204b878(work->actors[BADGE_ACTOR_SHORTCUT], GameData_IsShortcutRegistered(work->param->gameData, 0x17) + 6);
}


void func_ov111_021c81a4(BadgeViewWork *work, u16 value) {
    work->page = value;
    func_ov111_021c810c(work, value, work->pageMax);
    func_ov111_021c8364(work);
}

void func_ov111_021c81c0(BadgeViewWork *work) {
    RTCDate date;
    RTCTime time;
    s64 seconds;
    int i;

    RTC_GetCachedDateTime(&date, &time);
    seconds = RTC_ConvertDateTimeToSecond(&date, &time);
    saveSecondsTime(work->timeSig, seconds, seconds >> 32);
    for (i = 0; i < BADGE_COUNT; i++) {
        func_02008adc(work->timeSig, i, work->polish[i]);
    }
}

void func_ov111_021c8208(BadgeViewWork *work) {
    RTCDate nowDate;
    RTCDate savedDate;
    RTCTime nowTime;
    RTCTime savedTime;
    s64 saved;
    s64 elapsed;
    int i;

    saved = loadSecondsTime(work->timeSig);
    RTC_ConvertSecondToDateTime(&savedDate, &savedTime, saved);
    RTC_GetCachedDateTime(&nowDate, &nowTime);
    elapsed = (RTC_ConvertDateTimeToSecond(&nowDate, &nowTime) - saved) / 3600;
    if (elapsed < 0) {
        elapsed = 0;
    }
    for (i = 0; i < BADGE_COUNT; i++) {
        work->polish[i] = func_02008acc(work->timeSig, i);
        work->polish[i] -= elapsed * 2;
        if (work->polish[i] < 0) {
            work->polish[i] = 0;
        }
    }
    for (i = 0; i < BADGE_COUNT; i++) {
        work->prevLevel[i] = 0xff;
    }
}



void func_ov111_021c829c(BadgeViewWork *work, int badge, int level) {
    if (level > 2) {
        level = 2;
    }
    GX_LoadOBJPltt((u16 *)work->palettes[0].data[level]->pRawData + badge * 16, badge * 0x20, 0x20);
}

static inline const NNSG3dResDict *NNS_G3dGetPlttDict(const NNSG3dResTex *tex) {
    if (tex->plttInfo.ofsDict != 0) {
        return (const NNSG3dResDict *)((u8 *)tex + tex->plttInfo.ofsDict);
    }
    return NULL;
}

void func_ov111_021c82c0(BadgeViewWork *work, int badge, int level) {
    void *palette;
    NNSG3dResTex *tex;
    const NNSG3dResDict *dict;
    const u16 *entry;

    if (level > 2) {
        level = 2;
    }
    palette = work->palettes[1].data[level]->pRawData;
    tex = GFL_G3DResGetTexData(GFL_G3DMgrGetResource(work->g3dMgr, 0));
    if (tex != NULL && tex->plttInfo.ofsDict != 0) {
        dict = (const NNSG3dResDict *)((u8 *)tex + tex->plttInfo.ofsDict);
        entry = NNS_G3dGetResDataByIdx(dict, badge);
    } else {
        entry = NULL;
    }
    NNS_GfdRegisterNewVramTransferTask(NNS_GFD_DST_3D_TEX_PLTT, tex->plttInfo.vramKey + (*entry << 3),
                                       (u8 *)palette + (data_ov111_021c8874[badge] << 5), 0x20);
}


void func_ov111_021c832c(BadgeViewWork *work, int badge, int level) {
    int i;
    int n = level - 3;

    for (i = 0; i < 3; i++) {
        if (level < 3) {
            func_0204b3dc(work->actors[BADGE_ACTOR_SPARKLE + badge * 3 + i], FALSE);
        } else if (i <= n) {
            func_0204b3dc(work->actors[BADGE_ACTOR_SPARKLE + badge * 3 + i], TRUE);
        } else {
            func_0204b3dc(work->actors[BADGE_ACTOR_SPARKLE + badge * 3 + i], FALSE);
        }
    }
}

void func_ov111_021c8364(BadgeViewWork *work) {
    int i;

    for (i = 0; i < BADGE_COUNT; i++) {
        work->level[i] = work->polish[i] / 96;
        if (work->level[i] > 5) {
            work->level[i] = 5;
        }
        if (work->level[i] != work->prevLevel[i]) {
            func_ov111_021c829c(work, i, work->level[i]);
            func_ov111_021c82c0(work, i, work->level[i]);
            func_ov111_021c832c(work, i, work->level[i]);
        }
        work->prevLevel[i] = work->level[i];
    }
}




void func_ov111_021c83dc(BadgeViewPolish *rub, int polish) {
    if (rub->unk_5 == 0) {
        if (polish > 0x210) {
            GFL_SndSEPlay(SEQ_SE_SYS_34);
        } else {
            GFL_SndSEPlay(SEQ_SE_SYS_33);
        }
        rub->unk_5 = 5;
    } else {
        rub->unk_5--;
    }
}



void func_ov111_021c840c(BadgeViewPolish *rub) {
    rub->unk_2 = 0;
    rub->unk_3 = 0;
}







void func_ov111_021c8414(BadgeViewWork *work, int index) {
    if (work->polish[index] < 0x23f) {
        work->polish[index]++;
    }
}

void func_ov111_021c842c(BadgeViewWork *work, u32 badge) {
    u32 x = 0xffff;
    BOOL rubbing = FALSE;
    u32 y = 0xffff;
    int dx;
    int dy;

    func_02035cb8(&x, &y);
    if (x != 0xffff && y != 0xffff && work->unk_1c0 != 0xffff && work->unk_1c4 != 0xffff &&
        badge != TOUCH_RECT_NONE && badge < BADGE_COUNT && work->hasBadge[badge]) {
        if (work->unk_1c0 > x) {
            dx = work->unk_1c0 - x;
            work->rub.unk_2 = -1;
        } else {
            dx = x - work->unk_1c0;
            work->rub.unk_2 = 1;
        }
        if (dx >= 3 && dx <= 40) {
            if (work->unk_1c4 > y) {
                dy = work->unk_1c4 - y;
                work->rub.unk_3 = -1;
            } else {
                dy = y - work->unk_1c4;
                work->rub.unk_3 = 1;
            }
            if (dy <= 40) {
                rubbing = TRUE;
                func_ov111_021c83dc(&work->rub, work->polish[badge]);
            } else {
                func_ov111_021c840c(&work->rub);
            }
        } else if (dx <= 40) {
            if (work->unk_1c4 > y) {
                dy = work->unk_1c4 - y;
                work->rub.unk_3 = -1;
            } else {
                dy = y - work->unk_1c4;
                work->rub.unk_3 = 1;
            }
            if (dy >= 3 && dy <= 40) {
                rubbing = TRUE;
                func_ov111_021c83dc(&work->rub, work->polish[badge]);
            } else {
                func_ov111_021c840c(&work->rub);
            }
        }
        if (rubbing) {
            func_ov111_021c8414(work, badge);
        }
    }
    work->unk_1c0 = x;
    work->unk_1c4 = y;
}






















// pokebw2's BADGE_VIEW_PROC_FUNCTIONS
const GameProcFunctions data_ov111_021c892c = {
    func_ov111_021c6fc4,
    func_ov111_021c705c,
    func_ov111_021c7098,
};
