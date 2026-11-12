// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov208/dup_fitting.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "constants/arc.h"
#include "constants/sound.h"
#include "constants/text_banks.h"
#include "field/dup_fitting.h"
#include "field/dup_fitting_item.h"
#include "field/musical_dressup_sys.h"
#include "app/musical/mus_item_data.h"
#include "app/musical/mus_item_draw.h"
#include "app/musical/mus_poke_draw.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/blact.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/g3d.h"
#include "gfl/graphics.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/math_util.h"
#include "gfl/msg.h"
#include "gfl/net.h"
#include "gfl/random.h"
#include "gfl/sound.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "gfl/touchpanel.h"
#include "gfl/ui.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_trig.h"
#include "nitro/fx/fx_cp.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "nitro/os.h"
#include "nnsys/g3d.h"
#include "nnsys/gfd.h"
#include "save/musical_savedata.h"
#include "save/save_control.h"
#include "system/bmp_winframe.h"
#include "system/gf_font.h"
#include "system/printsys.h"
#include "system/wipe.h"
#include "system/wordset.h"
#include "text/system/title_13.h"
#include "text/system/title_3.h"
#include "text/system/title_s005d_word_set_musical_info.h"
#include "app/musical/musical_system.h"
#include "field/mus_comm_func.h"
#include "nitro/math.h"

// Overlay 208's dup_fitting.c, named after its string: the musical's dressing room. A ring of the props the player
// owns turns on the touch screen, and the player pulls them off it to put them on the Pokémon on the top screen.
// The names are guessed from what the code does

#define DUP_FITTING_ENTRY_MAX 250
// How far apart the props of the ring are, in angle (0x10000 for a full turn, over 14 props)
#define DUP_FITTING_RING_STEP 0x1249
#define DUP_FITTING_RING_PROPS 14
// The position of a prop that is not worn on the Pokémon
#define DUP_FITTING_EQUIP_NONE 10
// The category that shows the props in the order they are kept in
#define DUP_FITTING_CATEGORY_ORIGINAL 8
#define DUP_FITTING_MSG_ID_NONE 0xff

enum {
    DUP_FITTING_STATE_DEMO,
    DUP_FITTING_STATE_DRESS,
    DUP_FITTING_STATE_CONFIRM_IN,
    DUP_FITTING_STATE_CONFIRM,
    DUP_FITTING_STATE_WAIT_TIMING,
    DUP_FITTING_STATE_WAIT_SOUND,
    DUP_FITTING_STATE_CONFIRM_OUT,
    DUP_FITTING_STATE_START,
    DUP_FITTING_STATE_FADE_IN,
    DUP_FITTING_STATE_FADE_OUT,
    DUP_FITTING_STATE_DONE_WAIT,
};

struct DupFittingWork {
    HeapID heapId;
    MusicalDressUpParam *param;
    TCB *vblankTcb;
    s32 state;
    // How far the confirmation screen has slid in
    s32 transition;
    // Whether the touch screen was touched this frame, and whether it is held
    BOOL touchStart;
    BOOL touching;
    u32 touchX;
    u32 touchY;
    u32 prevTouchX;
    u32 prevTouchY;
    u16 prevAngle;
    BOOL dragging;
    // The position of the Pokémon that the held prop is over, or DUP_FITTING_EQUIP_NONE
    s32 hoverPos;
    DupFittingItemNode *heldNode;
    // How the held prop is held: 1 on the ring, 2 pulled off it
    s32 holdState;
    // Where it was taken from: 1 the ring, 2 the stage, 3 the Pokémon
    s32 holdKind;
    DupFittingPoint holdOrigin;
    // The prop that shows what the held prop looks like when it is worn, as a shadow
    MusItemDraw *shadowDraw;
    u16 shadowItemId;
    u32 shadowMaterial;
    u32 badgeMaterial;
    u32 badgeActors[DUP_FITTING_RING_PROPS];
    u16 badgeFrame;
    BOOL autoScroll;
    u32 scrolled;
    u32 scrollTarget;
    s32 category;
    u16 entryCount;
    DupFittingItemEntry entries[DUP_FITTING_ENTRY_MAX];
    DupFittingItemEntry *sorted[DUP_FITTING_ENTRY_MAX];
    // The props on the ring, on the stage, worn, and going back to the ring
    DupFittingItemList *carouselList;
    DupFittingItemList *stageList;
    DupFittingItemList *wornList;
    DupFittingItemList *returnList;
    u16 angle;
    s16 angleSpeed;
    u16 dragAccum;
    s32 carouselPos;
    s16 tilt;
    s16 tiltSpeed;
    // The demonstration of the first visit: whether it plays, and the stage it is at
    BOOL demo;
    BOOL showDemo;
    u16 demoFrame;
    u8 demoStage;
    u8 demoItem;
    ClActUnit *clactUnit;
    ClActor *buttons[2];
    ClActor *hand;
    // The palettes, characters and cells of the actors
    u32 clactRes[9];
    MusPokeDrawSys *pokeSys;
    MusPokeDraw *poke;
    MusItemDrawSys *itemSys;
    G3DCamera *camera;
    BlActScene *blact;
    ClActor *sparkles[8];
    ClActor *okMark;
    ClActor *touchEffect;
    u8 sparkleTimer[8];
    u8 confirmTimer;
    u8 bgScrollFrame;
    u16 sndCooldown;
    s16 rotateAccum;
    u8 msgSlide;
    BOOL msgClosed;
    BOOL printing;
    BOOL msgPhase;
    u16 msgItemId;
    MsgData *dressMsg;
    MsgData *itemMsg;
    MsgData *categoryMsg;
    Font *font;
    PrintQueue *printQueue;
    BmpWin *bmpWin;
};

void func_ov133_021b6884(TCB *tcb, void *data);
void func_ov133_021b69ac(DupFittingWork *work);
void func_ov133_021b6ba4(DupFittingWork *work);
void func_ov133_021b6e4c(const BGSetup *setup, u8 bg);
void func_ov133_021b6e74(DupFittingWork *work);
void func_ov133_021b6f00(DupFittingWork *work);
void func_ov133_021b7368(DupFittingWork *work, u16 itemId);
void func_ov133_021b7458(DupFittingWork *work, DupFittingPoint *pos);
void func_ov133_021b7498(DupFittingWork *work);
void func_ov133_021b75e0(DupFittingWork *work);
DupFittingItemNode *func_ov133_021b765c(DupFittingWork *work, u16 angle, s16 delta, u16 radiusX, u16 radiusY);
void func_ov133_021b7a18(DupFittingWork *work);
void func_ov133_021b7a70(DupFittingWork *work);
void func_ov133_021b7cfc(DupFittingWork *work, s16 x, s16 y);
BOOL func_ov133_021b7f00(DupFittingWork *work, DupFittingItemList *list, s32 kind);
void func_ov133_021b7f54(DupFittingWork *work, BOOL playSound);
void func_ov133_021b8140(DupFittingWork *work);
void func_ov133_021b8254(DupFittingWork *work);
void func_ov133_021b82e8(DupFittingWork *work, BOOL unused);
void func_ov133_021b8444(DupFittingWork *work);
void func_ov133_021b8680(DupFittingWork *work);
BOOL func_ov133_021b8930(DupFittingWork *work);
void func_ov133_021b8a30(DupFittingWork *work);
void func_ov133_021b8ac8(DupFittingWork *work);
BOOL func_ov133_021b8b78(DupFittingWork *work, s32 pos);
s32 func_ov133_021b8bb4(DupFittingWork *work, MusItemDraw *item, s32 *target, u16 *dist);
void func_ov112_021f5350(MusPokeDraw *poke, VecFx32 *pos);
void func_ov133_021b8c6c(DupFittingWork *work);
void func_ov133_021b8fa8(DupFittingWork *work);
BOOL func_ov133_021b8ff4(s16 x, s16 y, int radius, f32 xScale);
void func_ov133_021b9028(DupFittingWork *work, s32 category, u16 start);
u16 func_ov133_021b9118(DupFittingWork *work, DupFittingItemEntry *entry, s32 pos);
void func_ov133_021b918c(DupFittingWork *work);
u32 func_ov133_021b940c(DupFittingWork *work);
void func_ov133_021b94c0(DupFittingWork *work);
void func_ov133_021b952c(DupFittingWork *work);
void func_ov133_021b95ac(DupFittingWork *work);
void func_ov133_021b96d8(DupFittingWork *work);
void func_ov133_021b9784(DupFittingWork *work);
void func_ov133_021b9818(DupFittingWork *work);
void func_ov133_021b987c(DupFittingWork *work);
void func_ov133_021b98a0(DupFittingWork *work);
void func_ov133_021b9990(DupFittingWork *work, u16 target, s32 total, u16 frames);
void func_ov133_021b9a88(DupFittingWork *work, u16 frames);
void func_ov133_021b9ac0(DupFittingWork *work, const DupFittingPoint *from, const DupFittingPoint *to,
                                    u16 frames);
void func_ov133_021b9b48(DupFittingWork *work);
void func_ov133_021b9bd0(DupFittingWork *work);
void func_ov133_021b9c08(DupFittingWork *work, u32 messageId, WordSet *wordSet);
void func_ov133_021b9c90(DupFittingWork *work);
void func_ov133_021b9cbc(DupFittingWork *work, DupFittingItemNode *node);
void func_ov133_021b9d54(DupFittingWork *work, s32 category);
void func_ov133_021b9de8(DupFittingWork *work);
void func_ov133_021b9f24(DupFittingWork *work);
void func_ov133_021b9f5c(DupFittingWork *work);
void func_ov133_021ba098(DupFittingWork *work);
void func_ov133_021ba0c8(DupFittingWork *work, u8 id);

// The index of the prop on the ring that is "offset" turns past the front of the carousel
static inline u16 DupFitting_GetRingIndex(DupFittingWork *work, u32 offset) {
    u32 pos = work->carouselPos + offset;
    u32 total = work->entryCount * DUP_FITTING_RING_STEP;

    if (pos >= total) {
        return (pos - total) / DUP_FITTING_RING_STEP;
    }
    return pos / DUP_FITTING_RING_STEP;
}

static inline void DupFitting_SlideMessageBGs(DupFittingWork *work) {
    GFL_BGSysMoveBGReq(6, BG_MOVE_SET_X, 128 - work->msgSlide);
    GFL_BGSysMoveBGReq(5, BG_MOVE_SET_X, work->msgSlide - 128);
}

// The data are defined in pokebw2's order, which gives Black's .rodata layout under MWCC's size sort

// The ring angles up to which a prop is full size, shrinks, and shrinks to nothing
static const u16 DUP_FITTING_SCALE_ANGLES[3] = { 0x4000, 0x6e00, 0x78e0 };

// pokebw2's DUP_FITTING_BG7_SETUP
static const BGSetup data_ov133_021ba4f8 = {
    0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x7800, GX_BG_CHARBASE_0x10000, 0x8000,
    GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE,
};

// pokebw2's DUP_FITTING_VRAM_CONFIG
static const BGSysVRAMConfig data_ov133_021ba518 = {
    GX_VRAM_BG_64_E,
    GX_VRAM_BGEXTPLTT_NONE,
    GX_VRAM_SUB_BG_128_C,
    GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_16_F,
    GX_VRAM_OBJEXTPLTT_NONE,
    GX_VRAM_SUB_OBJ_16_I,
    GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_A | GX_VRAM_B | GX_VRAM_D,
    GX_VRAM_TEXPLTT_0_G,
    GX_OBJVRAMMODE_CHAR_1D_64K,
    GX_OBJVRAMMODE_CHAR_1D_32K,
};

// pokebw2's DUP_FITTING_CAMERA_POSITION
static const VecFx32 data_ov133_021ba3f4 = { 0, 0, 301 * FX32_ONE };
// pokebw2's DUP_FITTING_CAMERA_TARGET
static const VecFx32 data_ov133_021ba400 = { 0, 0, 0 };
// pokebw2's DUP_FITTING_EDGE_COLORS
static const GXRgb data_ov133_021ba418[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
// pokebw2's DUP_FITTING_CAMERA_UP
static const VecFx32 data_ov133_021ba40c = { 0, FX32_ONE, 0 };
// pokebw2's DUP_FITTING_LCD_CONFIG
static const BGSysLCDConfig data_ov133_021ba428 = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_3D };

// pokebw2's DUP_FITTING_BG1_SETUP
static const BGSetup data_ov133_021ba438 = {
    0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x7800, GX_BG_CHARBASE_0x00000, 0x1000,
    GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE,
};
// pokebw2's DUP_FITTING_BG2_SETUP
static const BGSetup data_ov133_021ba458 = {
    0, 0, 0x1000, 0, BGRES_256x512, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x6800, GX_BG_CHARBASE_0x08000, 0,
    GX_BG_EXTPLTT_01, 2, GX_BG_AREAOVER_XLU, FALSE,
};
// pokebw2's DUP_FITTING_BG3_SETUP
static const BGSetup data_ov133_021ba478 = {
    0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x6000, GX_BG_CHARBASE_0x08000, 0x8000,
    GX_BG_EXTPLTT_01, 3, GX_BG_AREAOVER_XLU, FALSE,
};
// pokebw2's DUP_FITTING_BG4_SETUP
static const BGSetup data_ov133_021ba498 = {
    0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x7000, GX_BG_CHARBASE_0x08000, 0,
    GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE,
};
// pokebw2's DUP_FITTING_BG5_SETUP
static const BGSetup data_ov133_021ba4b8 = {
    0, 0, 0x1000, 0, BGRES_512x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x6000, GX_BG_CHARBASE_0x08000, 0,
    GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE,
};
// pokebw2's DUP_FITTING_BG6_SETUP
static const BGSetup data_ov133_021ba4d8 = {
    0, 0, 0x1000, 0, BGRES_512x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x5000, GX_BG_CHARBASE_0x08000, 0,
    GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE,
};

// pokebw2's DupFitting_InitGraphics


void *func_ov133_021b6100(MusicalDressUpParam *param, HeapID heapId) {
    u32 pos;
    u32 total;
    u16 idx;
    DupFittingWork *work = GFL_HeapAllocate(heapId, sizeof(DupFittingWork), TRUE, "dup_fitting.c", 478);

    work->heapId = heapId;
    work->param = param;
    work->state = 7;
    work->transition = 0;
    work->touchStart = FALSE;
    work->touching = FALSE;
    work->dragging = FALSE;
    work->heldNode = NULL;
    work->holdState = 0;
    work->holdKind = 0;
    work->autoScroll = FALSE;
    work->category = 8;
    work->angle = 0;
    work->tilt = 0;
    work->tiltSpeed = 0;
    work->demo = FALSE;
    work->msgSlide = 0;
    work->msgClosed = FALSE;
    work->msgPhase = FALSE;
    work->printing = FALSE;
    work->msgItemId = 0xff;
    work->bgScrollFrame = 0;
    work->carouselPos = 0;
    work->bmpWin = NULL;
    work->angleSpeed = 0;
    work->hoverPos = 10;
    func_ov133_021b69ac(work);
    func_ov133_021b6ba4(work);
    func_ov133_021b6e74(work);
    func_ov133_021b6f00(work);
    func_ov133_021b9de8(work);
    func_ov133_021b9b48(work);
    work->vblankTcb = GFL_VBlankTCBAdd(func_ov133_021b6884, work, 8);
    GFL_WipeSet(2, 1, 1, 0x7fff, 0x12, 1, work->heapId);
    pos = work->carouselPos + 0x18000;
    work->sndCooldown = 10;
    total = work->entryCount * DUP_FITTING_RING_STEP;
    if (pos >= total) {
        idx = (pos - total) / DUP_FITTING_RING_STEP;
    } else {
        idx = pos / DUP_FITTING_RING_STEP;
    }
    func_ov133_021b9028(work, 8, idx);
    work->category = 8;
    work->autoScroll = TRUE;
    work->scrolled = 0;
    work->scrollTarget = work->angle % DUP_FITTING_RING_STEP + 0x10000;
    if (func_0200a828(work->param->save) == FALSE) {
        func_0200a834(work->param->save, TRUE);
        work->showDemo = TRUE;
    } else {
        work->showDemo = FALSE;
    }
    GFL_SndBGMPlay(0x454, 0xffff);
    func_0203d34c(FALSE, work->heapId);
    return work;
}

void func_ov133_021b6294(DupFittingWork *work) {
    u8 i;

    func_020355d8(TRUE);
    func_0200a6f8(work->param->save);
    func_02005d28();
    func_ov133_021b9bd0(work);
    func_ov133_021b9f24(work);
    func_0204b3b4(work->buttons[0]);
    func_0204b3b4(work->buttons[1]);
    func_0204b3b4(work->hand);
    for (i = 0; i <= 2; i++) {
        func_0204ada4(work->clactRes[i]);
    }
    for (i = 3; i <= 5; i++) {
        func_0204a8d4(work->clactRes[i]);
    }
    for (i = 6; i <= 8; i++) {
        func_0204afd8(work->clactRes[i]);
    }
    if (work->bmpWin != NULL) {
        BmpWin_Free(work->bmpWin);
    }
    func_0204b1cc(work->clactUnit);
    func_0204a5ac();
    func_ov133_021b7498(work);
    func_ov112_021f5330(work->pokeSys, work->poke);
    func_ov112_021f51b0(work->pokeSys);
    GFL_TCBRemove(work->vblankTcb);
    func_0204ed24(work->blact);
    GFL_G3DCameraFree(work->camera);
    GFL_G3DSysFree();
    GFL_BGSysReleaseBG(0);
    GFL_BGSysReleaseBG(1);
    GFL_BGSysReleaseBG(2);
    GFL_BGSysReleaseBG(3);
    GFL_BGSysReleaseBG(4);
    GFL_BGSysReleaseBG(6);
    GFL_BGSysReleaseBG(5);
    GFL_BGSysReleaseBG(7);
    BmpWin_FreeAllocator();
    GFL_BGSysFree();
    GFL_HeapFree(work);
}

u32 func_ov133_021b63bc(DupFittingWork *work) {
    switch (work->state) {
    case DUP_FITTING_STATE_START:
        GFL_SndSEPlay(SEQ_SE_DECIDE3);
        work->state = DUP_FITTING_STATE_FADE_IN;
        // fallthrough
    case DUP_FITTING_STATE_FADE_IN:
        if (GFL_WipeIsFinished() == TRUE) {
            if (work->showDemo == TRUE) {
                func_ov133_021b9818(work);
            } else {
                work->state = DUP_FITTING_STATE_DRESS;
            }
        }
        if (work->autoScroll == TRUE) {
            func_ov133_021b8a30(work);
        }
        break;
    case DUP_FITTING_STATE_FADE_OUT:
        if (GFL_WipeIsFinished() == TRUE && GFL_SndBGMIsFading() == FALSE) {
            return 2;
        }
        break;
    case DUP_FITTING_STATE_DEMO:
        func_ov133_021b98a0(work);
        break;
    case DUP_FITTING_STATE_DRESS:
        func_ov133_021b75e0(work);
        break;
    case DUP_FITTING_STATE_CONFIRM_IN:
        work->transition += 6;
        if (work->transition >= 192) {
            func_ov133_021b9c08(work, Title3_Text_001, NULL);
            work->transition = 192;
            work->state = DUP_FITTING_STATE_CONFIRM;
            work->tilt = 0;
            work->tiltSpeed = 0;
            work->rotateAccum = 0;
        }
        func_ov133_021b918c(work);
        break;
    case DUP_FITTING_STATE_CONFIRM_OUT:
        work->transition -= 6;
        if (work->transition < 0) {
            work->transition = 0;
            work->state = DUP_FITTING_STATE_DRESS;
        }
        func_ov133_021b918c(work);
        break;
    case DUP_FITTING_STATE_CONFIRM:
        if (func_ov133_021b940c(work) == 2) {
            work->state = DUP_FITTING_STATE_DONE_WAIT;
            work->confirmTimer = 0;
            func_0204b3dc(work->okMark, TRUE);
        }
        break;
    case DUP_FITTING_STATE_DONE_WAIT:
        work->confirmTimer++;
        func_ov133_021b9784(work);
        if (work->confirmTimer > 60) {
            if (work->param->comm == NULL) {
                GFL_WipeSet(WIPE_MODE_BOTH, WIPE_TYPE_FADE_OUT, WIPE_TYPE_FADE_OUT, WIPE_COLOR_WHITE, 18, 1,
                            work->heapId);
                GFL_SndBGMFadeOut(60);
                work->state = DUP_FITTING_STATE_FADE_OUT;
                work->msgClosed = TRUE;
            } else {
                func_ov133_021b9c08(work, Title3_Text_002, NULL);
                func_ov132_021fcdec(work->param->comm, 0x45);
                work->state = DUP_FITTING_STATE_WAIT_TIMING;
            }
        }
        break;
    case DUP_FITTING_STATE_WAIT_TIMING:
        if (func_ov132_021fce00(work->param->comm, 0x45) == TRUE) {
            work->state = DUP_FITTING_STATE_WAIT_SOUND;
        }
        break;
    case DUP_FITTING_STATE_WAIT_SOUND:
        if (func_ov132_021fd6a4(work->param->comm) == TRUE) {
            GFL_WipeSet(WIPE_MODE_BOTH, WIPE_TYPE_FADE_OUT, WIPE_TYPE_FADE_OUT, WIPE_COLOR_WHITE, 18, 1, work->heapId);
            GFL_SndBGMFadeOut(60);
            work->state = DUP_FITTING_STATE_FADE_OUT;
            work->msgClosed = TRUE;
        }
        break;
    }
    func_ov133_021b9f5c(work);
    func_ov133_021b8c6c(work);
    func_ov112_021f51e8(work->pokeSys);
    func_ov112_021f4990(work->itemSys);
    func_0204a600();
    GFL_G3DSysReset();
    GFL_G3DSysMtxViewFlush();
    func_ov112_021f51f4(work->pokeSys);
    BlActScene_Draw(work->blact, work->camera, NULL);
    GFL_G3DSysReqSwapBuffers();
    work->bgScrollFrame++;
    if (work->bgScrollFrame > 1) {
        work->bgScrollFrame = 0;
        GFL_BGSysMoveBGReq(3, BG_MOVE_RIGHT, 1);
        GFL_BGSysMoveBGReq(3, BG_MOVE_UP, 1);
    }
    if (work->msgClosed == FALSE) {
        if (work->msgSlide < 128) {
            work->msgSlide += 8;
            DupFitting_SlideMessageBGs(work);
        } else if (work->msgPhase == FALSE) {
            work->msgPhase = TRUE;
            GFL_BGSysFillScr(7, 0);
            work->bmpWin = BmpWin_CreateDynamic(7, 1, 19, 30, 4, 14, TRUE);
            if (work->showDemo == FALSE) {
                func_ov133_021b9c90(work);
            } else {
                func_ov133_021b9c08(work, Title3_Text_006, NULL);
            }
            BmpWin_DrawFrame(work->bmpWin, WINFRAME_TRANSFER_VBLANK, 0x80, 12);
        }
    } else if (work->msgPhase == TRUE) {
        func_ov133_021ba0c8(work, 2);
        work->msgPhase = FALSE;
        GFL_BGSysSetBGPriority(7, 3);
        GFL_BGSysSetBGPriority(4, 0);
        GFL_BGSysSetBGPriority(5, 1);
        GFL_BGSysSetBGPriority(6, 2);
        func_ov133_021ba0c8(work, 4);
        loadBGScrToVramByNarcNoReserveNegAlign(ARCID_MUSICAL_SHOT, 13, 7, 0, 0, FALSE, work->heapId);
        GFL_BGSysQueueScrLoad(7);
        work->printing = FALSE;
        func_0201c6ec(work->printQueue);
        BmpWin_Free(work->bmpWin);
        work->bmpWin = NULL;
        func_ov133_021ba0c8(work, 6);
    } else if (work->msgSlide != 0) {
        func_ov133_021ba0c8(work, work->msgSlide + 0x60);
        work->msgSlide -= 8;
        DupFitting_SlideMessageBGs(work);
    }
    func_ov133_021ba0c8(work, 8);
    if (work->printing == FALSE && work->state == DUP_FITTING_STATE_DRESS) {
        if (work->heldNode != NULL && work->msgItemId != func_ov133_021ba20c(work->heldNode)->itemId) {
            func_ov133_021b9cbc(work, work->heldNode);
            work->msgItemId = func_ov133_021ba20c(work->heldNode)->itemId;
        } else if (work->msgItemId != DUP_FITTING_MSG_ID_NONE && work->heldNode == NULL) {
            func_ov133_021b9c90(work);
            work->msgItemId = DUP_FITTING_MSG_ID_NONE;
        }
    }
    func_ov133_021ba0c8(work, 10);
    if (work->printing == TRUE && func_0201c6b4(work->printQueue) == TRUE) {
        work->printing = FALSE;
        BmpWin_Transfer(work->bmpWin);
        GFL_BGSysSetBGPriority(4, 1);
        GFL_BGSysSetBGPriority(5, 2);
        GFL_BGSysSetBGPriority(6, 3);
        GFL_BGSysSetBGPriority(7, 0);
    }
    func_ov133_021ba0c8(work, 12);
    if (work->printQueue != NULL) {
        func_0201c4e4(work->printQueue);
    }
    func_ov133_021ba0c8(work, 14);
    if (work->sndCooldown != 0) {
        work->sndCooldown--;
    }
    if (work->param->comm != NULL && work->state != DUP_FITTING_STATE_START &&
        work->state != DUP_FITTING_STATE_FADE_IN && work->state != DUP_FITTING_STATE_FADE_OUT) {
        func_ov133_021ba0c8(work, 0x20);
        if (func_0200fb24()) {
            func_ov133_021ba0c8(work, 0x22);
            GFL_WipeSet(WIPE_MODE_BOTH, WIPE_TYPE_FADE_OUT, WIPE_TYPE_FADE_OUT, WIPE_COLOR_WHITE, 18, 1, work->heapId);
            GFL_SndBGMFadeOut(60);
            func_ov133_021ba0c8(work, 0x24);
            work->state = DUP_FITTING_STATE_FADE_OUT;
            func_ov133_021ba0c8(work, 0x26);
        } else {
            func_ov133_021ba0c8(work, 0x40);
        }
    }
    func_ov133_021ba0c8(work, 16);
    return 0;
}


void func_ov133_021b6884(TCB *tcb, void *data) {
    DupFittingWork *work = data;
    DupFittingItemNode *node = func_ov133_021ba184(work->carouselList);
    BOOL changed = FALSE;

    for (; node != NULL; node = func_ov133_021ba208(node)) {
        DupFittingItemEntry *entry;
        DupFittingItemEntry *nextEntry;

        func_ov133_021ba218(node);
        entry = func_ov133_021ba20c(node);
        nextEntry = func_ov133_021ba214(node);
        if (entry->itemId != nextEntry->itemId) {
            func_ov133_021ba1ec(node, work->itemSys, nextEntry);
            changed = TRUE;
        }
    }
    if (changed == TRUE) {
        func_ov133_021b7a18(work);
    }
    if (work->heldNode != NULL) {
        DupFittingItemEntry *entry = func_ov133_021ba20c(work->heldNode);

        if (work->shadowItemId != entry->itemId) {
            MusItemData *items = func_ov112_021f4f10(work->itemSys);
            NNSGfdPlttKey plttKey;
            NNSGfdTexKey texKey;
            u16 width;
            u16 height;
            u32 size;
            u16 material;

            work->shadowItemId = entry->itemId;
            func_0204f32c(work->blact, work->shadowMaterial, &plttKey);
            func_0204f2e4(work->blact, entry->material, &texKey);
            func_0204f2fc(work->blact, work->shadowMaterial, texKey);
            size = func_ov112_021f4f70(func_ov112_021f4f54(items, work->shadowItemId));
            func_0204edd0(size, &width, &height);
            material = BlActScene_AddMaterialExistTex(work->blact, texKey, plttKey, 0, size, width, height);
            func_ov112_021f4d74(work->itemSys, work->shadowDraw, work->shadowItemId, material);
            func_0204f1a0(work->blact, work->shadowMaterial);
            work->shadowMaterial = material;
        }
    }
    func_0204a648();
    func_ov112_021f4aa4(work->itemSys);
}


void func_ov133_021b69ac(DupFittingWork *work) {
    ClActSysSetup clactSetup;

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
    GX_SetDispSelect(GX_DISP_SELECT_SUB_MAIN);
    GFL_BGSysSetVRAMBanks(&data_ov133_021ba518);
    GFL_G3DSysCreate(FALSE, 2, FALSE, 1, 0, work->heapId, NULL);
    GFL_G3DSysSetSwapBufferParams(0, 1);
    work->camera = GFL_G3DCameraCreate(G3DCAM_PROJECTION_ORTHO, FX32_CONST(12), 0, 0, FX32_CONST(16), FX32_ONE,
                                       FX32_CONST(1000), 0, &data_ov133_021ba3f4, &data_ov133_021ba40c,
                                       &data_ov133_021ba400, work->heapId);
    GFL_G3DCameraFlush(work->camera);
    G3X_SetEdgeColorTable(data_ov133_021ba418);
    G3X_EdgeMarking(FALSE);
    GFL_G3DSysSetSwapBufferParams(0, 0);
    GFL_BGSysCreate(work->heapId);
    BmpWin_InitAllocator(work->heapId);
    GFL_BGSysSetLCDConfig(&data_ov133_021ba428);
    GFL_BGSysSet3DBGPriority(1);
    func_ov133_021b6e4c(&data_ov133_021ba438, 1);
    func_ov133_021b6e4c(&data_ov133_021ba458, 2);
    func_ov133_021b6e4c(&data_ov133_021ba478, 3);
    func_ov133_021b6e4c(&data_ov133_021ba4f8, 7);
    func_ov133_021b6e4c(&data_ov133_021ba498, 4);
    func_ov133_021b6e4c(&data_ov133_021ba4b8, 5);
    func_ov133_021b6e4c(&data_ov133_021ba4d8, 6);
    GFL_BGSysSetBGEnabled(0, TRUE);
    G2S_SetBlendAlpha(0, 0xf, 4, 16);
    clactSetup = data_020a1448;
    clactSetup.oamStartMain = 16;
    clactSetup.oamCountMain = 112;
    ClActSys_Create(&clactSetup, &data_ov133_021ba518, work->heapId);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_OBJ, TRUE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
    work->clactUnit = func_0204b100(24, 0, work->heapId);
    func_0204b270(work->clactUnit);
    {
        BlActSceneSetup blactSetup = { 0x200, 0xa0, { FX32_ONE, FX32_ONE, 0 }, 0, 0, 0, 0, 0x3f, BLACT_ORIGIN_CENTER };

        work->blact = BlActScene_Create(&blactSetup, work->heapId);
    }
}

// pokebw2's DupFitting_LoadResources
void func_ov133_021b6ba4(DupFittingWork *work) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_MUSICAL_SHOT, work->heapId);
    ClActorSetup setup;

    GFL_G2DIOLoadArcNCLRDefault(arc, 4, PALTYPE_MAIN_BG, 0, 0, work->heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 7, 3, 0, 0, FALSE, work->heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 12, 3, 0, 0, FALSE, work->heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 11, 2, 0, 0, FALSE, work->heapId);
    GFL_G2DIOLoadArcNCLRDefault(arc, 3, PALTYPE_SUB_BG, 0, 0, work->heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 10, 4, 0, 0, FALSE, work->heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 9, 7, 0, 0, FALSE, work->heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 16, 4, 0, 0, FALSE, work->heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 15, 6, 0, 0, FALSE, work->heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 14, 5, 0, 0, FALSE, work->heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 13, 7, 0, 0, FALSE, work->heapId);
    GFL_BGSysMoveBGReq(6, BG_MOVE_SET_X, 128);
    GFL_BGSysMoveBGReq(5, BG_MOVE_SET_X, -128);
    work->clactRes[0] = func_0204abf0(arc, 2, CLACT_VRAM_MAIN, 0, work->heapId);
    work->clactRes[3] = func_0204a6c8(arc, 8, FALSE, CLACT_VRAM_MAIN, work->heapId);
    work->clactRes[6] = func_0204af28(arc, 20, 24, work->heapId);
    work->clactRes[2] = func_0204abf0(arc, 1, CLACT_VRAM_SUB, 0, work->heapId);
    work->clactRes[5] = func_0204a6c8(arc, 6, FALSE, CLACT_VRAM_SUB, work->heapId);
    work->clactRes[8] = func_0204af28(arc, 19, 23, work->heapId);
    work->clactRes[1] = func_0204abf0(arc, 0, CLACT_VRAM_MAIN, 0x40, work->heapId);
    work->clactRes[4] = func_0204a6c8(arc, 5, FALSE, CLACT_VRAM_MAIN, work->heapId);
    work->clactRes[7] = func_0204af28(arc, 17, 21, work->heapId);
    setup.x = 32;
    setup.y = 224;
    setup.sequence = 0;
    setup.priority = 0;
    setup.bgPriority = 2;
    work->buttons[0] = func_0204b294(work->clactUnit, work->clactRes[3], work->clactRes[0], work->clactRes[6], &setup,
                                     CLACT_SURFACE_MAIN, work->heapId);
    setup.x = 224;
    setup.y = 224;
    setup.sequence = 1;
    work->buttons[1] = func_0204b294(work->clactUnit, work->clactRes[3], work->clactRes[0], work->clactRes[6], &setup,
                                     CLACT_SURFACE_MAIN, work->heapId);
    setup.x = 128;
    setup.y = 96;
    setup.sequence = 2;
    setup.priority = 0;
    setup.bgPriority = 0;
    work->hand = func_0204b294(work->clactUnit, work->clactRes[3], work->clactRes[0], work->clactRes[6], &setup,
                               CLACT_SURFACE_MAIN, work->heapId);
    func_0204b3dc(work->buttons[0], TRUE);
    func_0204b3dc(work->buttons[1], TRUE);
    func_0204b3dc(work->hand, FALSE);
    GFL_ArcToolFree(arc);
    GFL_BGSysLoadScr(3);
    GFL_BGSysLoadScr(2);
    GFL_BGSysLoadScr(7);
}

// pokebw2's DupFitting_InitBG
void func_ov133_021b6e4c(const BGSetup *setup, u8 bg) {
    GFL_BGSysCreateBG(bg, setup, BGMODE_TEXT);
    GFL_BGSysSetBGEnabled(bg, TRUE);
    GFL_BGSysClearBG(bg);
    GFL_BGSysLoadScr(bg);
}

void func_ov133_021b6e74(DupFittingWork *work) {
    VecFx32 *mark;
    VecFx32 pos;

    work->pokeSys = func_ov112_021f5164(work->heapId);
    func_ov112_021f5438(work->pokeSys, 0x50000);
    func_ov112_021f5444(work->pokeSys, 0x3fe0);
    work->poke = func_ov112_021f5204(work->pokeSys, work->param->poke, FALSE);
    func_ov112_021f51e8(work->pokeSys);
    func_ov112_021f51f4(work->pokeSys);
    mark = func_ov112_021f545c(work->poke);
    pos.x = (0x80000 - mark->x) / 16;
    pos.y = (0xc0000 - (0x70000 - mark->y)) / 16;
    pos.z = 0x50000 / 2;
    func_ov112_021f5350(work->poke, &pos);
}

void func_ov133_021b6f00(DupFittingWork *work) {
    static const BOOL hidden = FALSE;
    MusItemData *items;
    u8 i;
    int k;
    int n;

    work->entryCount = 0;
    for (n = 0; n < DUP_FITTING_ENTRY_MAX; n++) {
        if (func_0200a644(work->param->save, n) == TRUE) {
            work->entries[work->entryCount].itemId = n;
            work->entries[work->entryCount].used = FALSE;
            if (func_0200a680(work->param->save, n) == TRUE) {
                work->entries[work->entryCount].isNew = TRUE;
            } else {
                work->entries[work->entryCount].isNew = FALSE;
            }
            work->entryCount++;
        }
    }
    for (k = 0; work->entryCount + k < DUP_FITTING_ENTRY_MAX; k++) {
        work->entries[work->entryCount + k].itemId = 0xff;
        work->entries[work->entryCount + k].used = FALSE;
        work->entries[work->entryCount].isNew = TRUE;
    }
    work->itemSys = func_ov112_021f48c0(work->blact, 0x98, work->heapId);
    items = func_ov112_021f4f10(work->itemSys);
    for (n = 0; n < work->entryCount; n++) {
        void *res;
        NNSGfdTexKey texKey;
        NNSGfdPlttKey plttKey;
        NNSGfdPlttKey dimPlttKey;
        u32 plttSize;
        int size;
        u16 *palette;
        NNSG3dResFileHeader *file;
        NNSG3dResTex *tex;
        u16 width;
        u16 height;
        u8 j;

        res = func_ov112_021f4b3c(work->entries[n].itemId);
        GFL_G3DResUploadTexData(res);
        texKey = GFL_G3DResGetTexVRAMHandle(res);
        plttKey = GFL_G3DResGetPltVRAMHandle(res);
        plttSize = NNS_GfdGetPlttKeySize(plttKey);
        dimPlttKey = NNS_GfdAllocPlttVram(plttSize, FALSE, 0);
        size = func_ov112_021f4f70(func_ov112_021f4f54(items, work->entries[n].itemId));
        func_0204edd0(size, &width, &height);
        work->entries[n].material = BlActScene_AddMaterialExistTex(work->blact, texKey, plttKey, 0, size, width, height);
        palette = GFL_G3DResGetTexPaletteData(res);
        for (j = 0; j < plttSize / 2; j++) {
            u16 color = palette[j];
            u8 b = (color & GX_RGB_B_MASK) >> GX_RGB_B_SHIFT;
            u8 r = color & GX_RGB_R_MASK;
            u8 g = (color & GX_RGB_G_MASK) >> GX_RGB_G_SHIFT;

            b >>= 3;
            r >>= 3;
            g >>= 3;
            palette[j] = GX_RGB(r, g, b);
        }
        file = GFL_G3DResGetResData(res);
        tex = GFL_G3DResGetTexData(res);
        NNS_G3dPlttSetPlttKey(tex, dimPlttKey);
        DC_FlushRange(file, file->fileSize);
        NNS_G3dPlttLoad(tex, TRUE);
        work->entries[n].dimMaterial =
            BlActScene_AddMaterialExistTex(work->blact, texKey, dimPlttKey, 0, size, width, height);
        func_ov112_021f4b4c(res);
    }
    func_ov133_021b9028(work, DUP_FITTING_CATEGORY_ORIGINAL, 0);
    work->carouselList = func_ov133_021ba0cc(work->heapId, DUP_FITTING_RING_PROPS);
    work->stageList = func_ov133_021ba0cc(work->heapId, 128);
    work->wornList = func_ov133_021ba0cc(work->heapId, 9);
    work->returnList = func_ov133_021ba0cc(work->heapId, 4);
    for (k = 0; k < DUP_FITTING_RING_PROPS; k++) {
        VecFx32 pos = { 0, 0, 0 };
        DupFittingItemNode *node = func_ov133_021ba19c(work->heapId, work->itemSys, &work->entries[k], &pos);

        func_ov133_021ba100(work->carouselList, node);
        func_ov112_021f4ee8(work->itemSys, func_ov133_021ba218(node), FALSE);
    }
    {
        VecFx32 pos = { 0, 0, 0 };
        u16 width;
        u16 height;
        u32 size;
        void *res;

        work->shadowItemId = 0;
        size = func_ov112_021f4f70(func_ov112_021f4f54(items, work->shadowItemId));
        func_0204edd0(size, &width, &height);
        res = func_ov112_021f4b3c(work->shadowItemId);
        GFL_G3DResUploadTexData(res);
        work->shadowMaterial = BlActScene_AddMaterialExistTex(work->blact, GFL_G3DResGetTexVRAMHandle(res),
                                                              GFL_G3DResGetPltVRAMHandle(res), 0, size, width, height);
        work->shadowDraw = func_ov112_021f4c1c(work->itemSys, work->shadowItemId, work->shadowMaterial, &pos);
        func_ov112_021f4e6c(work->itemSys, work->shadowDraw, FALSE);
        func_ov112_021f4eb4(work->itemSys, work->shadowDraw, FX32_ONE, FX32_ONE);
        func_ov112_021f4de0(work->itemSys, work->shadowDraw, res);
        func_ov112_021f4b4c(res);
    }
    work->badgeMaterial = func_0204efc8(work->blact, ARCID_MUSICAL_SHOT, 25, 2, 0x22, 32, 32);
    for (i = 0; i < DUP_FITTING_RING_PROPS; i++) {
        VecFx32 pos = { 0, 0, 0 };
        u32 actor = BlActScene_AddNewActor(work->blact, work->badgeMaterial, FX32_ONE, FX32_ONE, &pos, 31, 0, 0);

        work->badgeActors[i] = actor;
        BlActScene_SetActorHidden(work->blact, actor, &hidden);
    }
    work->badgeFrame = 0;
    {
        MusicalSaveUnk1B0 *saved = func_0200a628(work->param->save);

        for (i = 0; i < 8; i++) {
            if (saved[i].unk0 != DUP_FITTING_EQUIP_NONE) {
                func_ov133_021b7368(work, saved[i].itemId);
            }
        }
    }
    func_ov133_021b7a18(work);
    work->carouselPos = work->entryCount * DUP_FITTING_RING_STEP - 0x8000;
    func_ov133_021b765c(work, 0, 0, 72, 48);
}


void func_ov133_021b7368(DupFittingWork *work, u16 itemId) {
    DupFittingItemEntry *entry = NULL;
    DupFittingItemNode *node;
    VecFx32 vec;
    DupFittingPoint pos;
    u8 i;

    for (i = 0; i < work->entryCount; i++) {
        if (itemId == work->entries[i].itemId) {
            entry = &work->entries[i];
            break;
        }
    }
    if (entry != NULL) {
        func_ov133_021b7458(work, &pos);
        vec.x = FX32_CONST(pos.x);
        vec.y = FX32_CONST(pos.y);
        vec.z = 10 * FX32_ONE - func_ov133_021ba188(work->stageList) * 0x400;
        node = func_ov133_021ba19c(work->heapId, work->itemSys, entry, &vec);
        func_ov133_021ba100(work->stageList, node);
        func_ov133_021ba21c(node, &pos);
        func_ov112_021f4eb4(work->itemSys, func_ov133_021ba218(node), FX32_ONE, FX32_ONE);
        entry->used = TRUE;
    }
}



// pokebw2's DupFitting_GetRandomStagePos
void func_ov133_021b7458(DupFittingWork *work, DupFittingPoint *pos) {
    do {
        pos->x = GFL_RandomLCAlt(240) + 8;
        pos->y = GFL_RandomLCAlt(128) + 32;
    } while (func_ov133_021b8ff4(pos->x - 128, pos->y - 80, 84, 17.0f / 23.0f) == TRUE);
}

// pokebw2's DupFitting_FreeItems
void func_ov133_021b7498(DupFittingWork *work) {
    DupFittingItemNode *node;
    DupFittingItemNode *next;
    int i;

    for (node = func_ov133_021ba184(work->carouselList); node != NULL; node = next) {
        next = func_ov133_021ba208(node);
        func_ov133_021ba1d8(node, work->itemSys);
    }
    for (node = func_ov133_021ba184(work->stageList); node != NULL; node = next) {
        next = func_ov133_021ba208(node);
        func_ov133_021ba1d8(node, work->itemSys);
    }
    for (node = func_ov133_021ba184(work->wornList); node != NULL; node = next) {
        next = func_ov133_021ba208(node);
        func_ov133_021ba1d8(node, work->itemSys);
    }
    for (node = func_ov133_021ba184(work->returnList); node != NULL; node = next) {
        next = func_ov133_021ba208(node);
        func_ov133_021ba1d8(node, work->itemSys);
    }
    for (i = 0; i < DUP_FITTING_RING_PROPS; i++) {
        BlActScene_ClearActorMaterial(work->blact, work->badgeActors[i]);
    }
    BlActScene_FreeMaterial(work->blact, work->badgeMaterial);
    func_ov112_021f4d40(work->itemSys, work->shadowDraw);
    BlActScene_FreeMaterial(work->blact, work->shadowMaterial);
    for (i = 0; i < work->entryCount; i++) {
        BlActScene_FreeMaterial(work->blact, work->entries[i].material);
        BlActScene_FreeMaterial(work->blact, work->entries[i].dimMaterial);
    }
    func_ov133_021ba0f8(work->carouselList);
    func_ov133_021ba0f8(work->stageList);
    func_ov133_021ba0f8(work->wornList);
    func_ov133_021ba0f8(work->returnList);
    func_ov112_021f4948(work->itemSys);
}

// pokebw2's DupFitting_UpdateDress
void func_ov133_021b75e0(DupFittingWork *work) {
    if (work->autoScroll == TRUE) {
        func_ov133_021b8a30(work);
    } else {
        func_ov133_021b7a70(work);
        func_ov133_021b8ac8(work);
    }
    func_ov133_021b8fa8(work);
    work->badgeFrame++;
    if (work->badgeFrame >= 90) {
        work->badgeFrame = 0;
        func_ov133_021b765c(work, work->angle, 0, 72, 48);
    }
    if (work->badgeFrame == 45) {
        func_ov133_021b765c(work, work->angle, 0, 72, 48);
    }
}

DupFittingItemNode *func_ov133_021b765c(DupFittingWork *work, u16 angle, s16 delta, u16 radiusX,
                                                     u16 radiusY) {
    static const BOOL hidden = FALSE;
    static const BOOL shown = TRUE;
    s32 i = 0;
    DupFittingItemNode *node = func_ov133_021ba184(work->carouselList);
    s32 total;
    u16 tilt;
    s32 offsetX;
    s32 offsetY;
    s16 scale;

    work->carouselPos += delta;
    if (work->carouselPos < 0) {
        work->carouselPos += work->entryCount * DUP_FITTING_RING_STEP;
    }
    total = work->entryCount * DUP_FITTING_RING_STEP;
    if (work->carouselPos >= total) {
        work->carouselPos -= total;
    }
    tilt = work->tilt;
    offsetX = (FX_SinIdx(0) - FX_SinIdx(tilt)) * 8;
    offsetY = FX_CosIdx(0) - 0x8008 - FX_CosIdx(tilt) * -8;
    if (delta > 2500 || delta < -2500) {
        if (work->sndCooldown == 0) {
            work->sndCooldown = 30;
            GFL_SndSEPlay(SEQ_SE_MSCL_03);
        }
    }
    for (; node != NULL; node = func_ov133_021ba208(node), i++) {
        MusItemDraw *draw;
        u16 a;
        s32 x;
        s32 y;
        fx16 sin;
        fx16 cos;
        DupFittingItemEntry *entry;
        DupFittingItemEntry *nextEntry;
        VecFx32 vec;
        DupFittingPoint pt;
        s32 pos;
        u16 idx;

        a = (angle + (i << 16) / DUP_FITTING_RING_PROPS) % 0x10000;
        scale = 0;
        draw = func_ov133_021ba218(node);
        sin = FX_SinIdx(a);
        x = sin * radiusX + 128 * FX32_ONE;
        cos = FX_CosIdx(a);
        y = cos * radiusY + 80 * FX32_ONE;
        entry = func_ov133_021ba20c(node);
        nextEntry = entry;
        pos = work->carouselPos + (a + 0x8000) % 0x10000;
        while (pos < 0) {
            pos += work->entryCount * DUP_FITTING_RING_STEP;
        }
        while (pos >= work->entryCount * DUP_FITTING_RING_STEP) {
            pos -= work->entryCount * DUP_FITTING_RING_STEP;
        }
        if (work->autoScroll == FALSE || work->scrolled > (0x18000 - a) % 0x10000) {
            idx = (u16)((pos + 0x924) / DUP_FITTING_RING_STEP);
            if (idx >= work->entryCount) {
                idx -= work->entryCount;
            }
            nextEntry = work->sorted[idx];
            func_ov133_021ba210(node, nextEntry);
            if (entry->itemId != nextEntry->itemId && node == work->heldNode) {
                work->heldNode = NULL;
            }
        }
        if (a >= 0x8000) {
            a = 0x10000 - a;
        }
        pt.x = FX_FX32_TO_F32(x);
        pt.y = FX_FX32_TO_F32(y);
        vec.x = x + offsetX;
        vec.y = y + offsetY;
        vec.z = (fx32)(0x8000 - a) + 0x14000;
        func_ov112_021f4e8c(work->itemSys, draw, &vec);
        func_ov112_021f4ed4(work->itemSys, draw, tilt);
        func_ov133_021ba21c(node, &pt);
        if (a < DUP_FITTING_SCALE_ANGLES[0]) {
            scale = FX32_ONE;
            func_ov112_021f4eb4(work->itemSys, draw, FX32_ONE, FX32_ONE);
            func_ov112_021f4e6c(work->itemSys, draw, TRUE);
            func_ov133_021ba244(node, FX32_ONE);
        } else if (a < DUP_FITTING_SCALE_ANGLES[1]) {
            scale = ((u16)(DUP_FITTING_SCALE_ANGLES[1] - a) << 11) / 0x2e00 + 0x800;
            func_ov112_021f4eb4(work->itemSys, draw, scale, scale);
            func_ov112_021f4e6c(work->itemSys, draw, TRUE);
            func_ov133_021ba244(node, scale);
        } else if (a < DUP_FITTING_SCALE_ANGLES[2]) {
            scale = ((u16)(DUP_FITTING_SCALE_ANGLES[2] - a) << 11) / 0xae0;
            func_ov112_021f4eb4(work->itemSys, draw, scale, scale);
            func_ov112_021f4e6c(work->itemSys, draw, TRUE);
            func_ov133_021ba244(node, scale);
        } else {
            func_ov112_021f4e6c(work->itemSys, draw, FALSE);
            func_ov133_021ba244(node, 0);
            scale = 0;
        }
        if (nextEntry->isNew == TRUE && (s32)work->badgeFrame < 45) {
            VecFx32 badgePos;

            badgePos.x = vec.x / 16;
            badgePos.y = (192 * FX32_ONE - (vec.y + 0x8000)) / 16;
            badgePos.z = vec.z + 192 * FX32_ONE / 512;
            BlActScene_SetActorPos(work->blact, work->badgeActors[i], &badgePos);
            func_0204f684(work->blact, work->badgeActors[i], &scale, &scale);
            BlActScene_SetActorHidden(work->blact, work->badgeActors[i], &shown);
        } else {
            BlActScene_SetActorHidden(work->blact, work->badgeActors[i], &hidden);
        }
    }
    return func_ov133_021ba184(work->carouselList);
}


// pokebw2's DupFitting_RefreshCarouselItems
void func_ov133_021b7a18(DupFittingWork *work) {
    DupFittingItemNode *node;

    for (node = func_ov133_021ba184(work->carouselList); node != NULL; node = func_ov133_021ba208(node)) {
        DupFittingItemEntry *entry = func_ov133_021ba20c(node);
        MusItemDraw *draw = func_ov133_021ba218(node);

        if (entry->used == TRUE) {
            func_ov112_021f4d74(work->itemSys, draw, entry->itemId, entry->dimMaterial);
        } else {
            func_ov112_021f4d74(work->itemSys, draw, entry->itemId, entry->material);
        }
    }
}

// pokebw2's DupFitting_UpdateInput
void func_ov133_021b7a70(DupFittingWork *work) {
    // The speed under which a spin stops
    static const u16 spinStopSpeed = 128;
    BOOL ringDragged = FALSE;
    BOOL selected = FALSE;
    BOOL picked = FALSE;

    if (work->demo == FALSE) {
        work->touchStart = func_02035c60();
        work->touching = func_02035cb8(&work->touchX, &work->touchY);
    }
    if (work->touchStart != FALSE || work->touching != FALSE) {
        if (work->touchY > 176) {
            work->touchY = 176;
        }
        if (work->touchY < 16) {
            work->touchY = 16;
        }
        if (work->holdState == 2 && work->heldNode != NULL) {
            func_ov133_021b7f54(work, TRUE);
        } else if (work->touchStart == TRUE) {
            picked = func_ov133_021b7f00(work, work->wornList, 3);
            if (picked == TRUE) {
                func_ov133_021ba148(work->wornList, work->heldNode);
                func_ov133_021ba100(work->stageList, work->heldNode);
                func_ov133_021b7f54(work, FALSE);
            } else {
                selected = func_ov133_021b8930(work);
            }
        }
        if (picked == FALSE && selected == FALSE) {
            if ((work->heldNode == NULL && work->touchStart == TRUE) || work->holdState == 1) {
                s16 dy = work->touchY - 80;
                s16 dx = work->touchX - 128;
                BOOL inner = func_ov133_021b8ff4(dx, dy, 28, 7.0f / 13.0f);
                BOOL outer = func_ov133_021b8ff4(dx, dy, 68, 17.0f / 23.0f);

                if (inner == FALSE && outer == TRUE) {
                    ringDragged = TRUE;
                    func_ov133_021b7cfc(work, dx, dy);
                } else if ((inner == FALSE && outer == FALSE) || (inner == TRUE && outer == TRUE)) {
                    if (func_ov133_021ba18c(work->stageList) == FALSE && work->holdState == 1 &&
                        work->heldNode != NULL && MATH_ABS(work->angleSpeed) < 364) {
                        func_ov133_021b8140(work);
                    }
                }
                if (ringDragged == FALSE && work->touchStart == TRUE) {
                    func_ov133_021b7f00(work, work->stageList, 2);
                }
            }
        }
        if (work->touchStart != FALSE && work->heldNode == NULL && work->touchY > 160 && work->touchX > 96 &&
            work->touchX < 160) {
            work->state = DUP_FITTING_STATE_CONFIRM_IN;
            work->transition = 0;
            GFL_SndSEPlay(SEQ_SE_MSCL_01);
            func_ov133_021ba098(work);
        }
    } else {
        s16 dy = work->prevTouchY - 80;
        s16 dx = work->prevTouchX - 128;
        BOOL inner = func_ov133_021b8ff4(dx, dy, 28, 7.0f / 13.0f);
        BOOL outer = func_ov133_021b8ff4(dx, dy, 68, 17.0f / 23.0f);

        if (work->holdState == 2 && work->heldNode != NULL) {
            if (work->hoverPos != DUP_FITTING_EQUIP_NONE) {
                func_ov133_021b8444(work);
            } else if (inner == FALSE && outer == TRUE) {
                func_ov133_021b82e8(work, FALSE);
            } else if (inner == TRUE) {
                func_ov133_021b8680(work);
            } else {
                func_ov133_021b8254(work);
            }
        }
        work->dragging = FALSE;
        work->heldNode = NULL;
        work->holdState = 0;
        work->holdKind = 0;
        work->hoverPos = DUP_FITTING_EQUIP_NONE;
    }
    if (work->angleSpeed != 0 && ringDragged == FALSE) {
        s32 angle = work->angle + 0x10000 - work->angleSpeed;

        if (angle < 0) {
            angle += 0x10000;
        }
        if (angle >= 0x10000) {
            angle -= 0x10000;
        }
        work->angle = angle;
        func_ov133_021b765c(work, work->angle, work->angleSpeed, 72, 48);
        if (work->angleSpeed > spinStopSpeed || work->angleSpeed < -spinStopSpeed) {
            work->angleSpeed *= 0.75f;
        } else {
            work->angleSpeed = 0;
        }
    }
    work->prevTouchX = work->touchX;
    work->prevTouchY = work->touchY;
}

// pokebw2's DupFitting_UpdateCarouselDrag
void func_ov133_021b7cfc(DupFittingWork *work, s16 x, s16 y) {
    // The tilt change per frame while dragging; the original's table has a second, unused entry
    static const u16 tiltSteps[2] = { 0x100, 0 };
    // The smallest turn of a drag that moves the ring
    static const u16 dragMinTurn = 227;
    s32 delta = func_0203f768(work->prevTouchX, work->prevTouchY * 72 / 48, work->touchX, work->touchY * 72 / 48, 128,
                              80, 72);

    work->dragAccum += MATH_ABS(delta);
    if (work->touchStart == TRUE || work->dragAccum >= 0x71c) {
        DupFittingItemNode *node = func_ov133_021ba184(work->carouselList);
        BOOL hit = FALSE;

        if (work->touchStart == TRUE) {
            work->angleSpeed = 0;
        }
        work->dragAccum = 0;
        work->heldNode = NULL;
        while (node != NULL) {
            DupFittingItemEntry *entry = func_ov133_021ba20c(node);

            if (func_ov133_021ba25c(node, work->touchX, work->touchY) == TRUE) {
                hit = TRUE;
                if (entry->used == FALSE) {
                    work->heldNode = node;
                    work->holdState = 1;
                    node = NULL;
                }
            }
            if (work->heldNode == NULL) {
                node = func_ov133_021ba208(node);
            }
        }
        if (work->touchStart == TRUE && hit == TRUE && work->heldNode == NULL) {
            work->holdState = 1;
            GFL_SndSEPlay(SEQ_SE_BEEP);
        }
    }
    if (work->dragging == TRUE) {
        if (MATH_ABS(delta) > dragMinTurn && MATH_ABS(delta) < 0x4000) {
            s32 angle = work->angle + 0x10000 - delta;

            if (angle < 0) {
                angle += 0x10000;
            }
            if (angle >= 0x10000) {
                angle -= 0x10000;
            }
            work->angle = angle;
            func_ov133_021b765c(work, work->angle, delta, 72, 48);
            if (delta >= 0 && work->angleSpeed > 0 && delta < work->angleSpeed) {
                work->angleSpeed = (work->angleSpeed + delta) / 2;
            } else if (delta <= 0 && work->angleSpeed < 0 && delta > work->angleSpeed) {
                work->angleSpeed = (work->angleSpeed + delta) / 2;
            } else {
                work->angleSpeed = delta;
            }
            if ((work->angleSpeed > 0 && work->tilt > 0) || (work->angleSpeed < 0 && work->tilt < 0)) {
                work->tilt = 0;
            } else if (work->angleSpeed > 0 && work->tilt > -0x3000) {
                if (work->angleSpeed * 15 > work->tilt * -10) {
                    work->tilt -= tiltSteps[0];
                }
            } else if (work->angleSpeed < 0 && work->tilt < 0x3000) {
                if (work->angleSpeed * -15 > work->tilt * 10) {
                    work->tilt += tiltSteps[0];
                }
            }
        } else {
            work->angleSpeed = 0;
        }
    }
    work->dragging = TRUE;
}

// pokebw2's DupFitting_PickFromList
BOOL func_ov133_021b7f00(DupFittingWork *work, DupFittingItemList *list, s32 kind) {
    DupFittingItemNode *node;

    for (node = func_ov133_021ba184(list); node != NULL; node = func_ov133_021ba208(node)) {
        if (func_ov133_021ba25c(node, work->touchX, work->touchY) == TRUE) {
            DupFittingPoint *pos = func_ov133_021ba228(node);

            work->holdState = 2;
            work->heldNode = node;
            GFL_SndSEPlay(SEQ_SE_MSCL_02);
            work->holdKind = kind;
            work->holdOrigin.x = pos->x;
            work->holdOrigin.y = pos->y;
            return TRUE;
        }
    }
    return FALSE;
}

void func_ov133_021b7f54(DupFittingWork *work, BOOL playSound) {
    u16 distance = 12;
    VecFx32 vec = { 0, 0, FX32_CONST(70) };
    MusItemDraw *draw;
    s32 oldHover = work->hoverPos;
    DupFittingPoint target;
    s32 touch[2];
    u16 rotation = 0;
    s16 scaleX;
    s16 scaleY;
    MusPokeDrawEquipPos *equip;

    draw = func_ov133_021ba218(work->heldNode);
    touch[0] = work->touchX - 128;
    touch[1] = work->touchY - 112;
    work->hoverPos = func_ov133_021b8bb4(work, draw, touch, &distance);
    if (work->hoverPos != DUP_FITTING_EQUIP_NONE) {
        equip = func_ov112_021f5450(work->poke, work->hoverPos);
        target.x = (s32)FX_FX32_TO_F32(equip->info.pos.x + equip->info.offset.x) + 128;
        target.y = (s32)FX_FX32_TO_F32(equip->info.pos.y + equip->info.offset.y) + 96;
        rotation = equip->info.cellRotation;
        func_ov112_021f4ee8(work->itemSys, draw, TRUE);
        if (func_ov112_021f4b24(draw) == TRUE) {
            vec.z = 26 * FX32_ONE;
        }
        if (func_ov133_021b8b78(work, work->hoverPos) == FALSE) {
            work->holdKind = 3;
            work->holdOrigin.x = target.x;
            work->holdOrigin.y = target.y;
        }
        func_ov133_021ba254(work->heldNode, work->hoverPos);
        if (oldHover != work->hoverPos && playSound == TRUE) {
            GFL_SndSEPlay(SEQ_SE_MSCL_06);
        }
    } else {
        work->hoverPos = DUP_FITTING_EQUIP_NONE;
        target.x = work->touchX;
        target.y = work->touchY;
        func_ov112_021f4ee8(work->itemSys, draw, FALSE);
    }
    vec.x = FX32_CONST(target.x);
    vec.y = FX32_CONST(target.y);
    func_ov112_021f4e8c(work->itemSys, draw, &vec);
    func_ov133_021ba21c(work->heldNode, &target);
    func_ov112_021f4ed4(work->itemSys, func_ov133_021ba218(work->heldNode), rotation);
    func_ov112_021f4ec4(work->itemSys, draw, &scaleX, &scaleY);
    if (scaleX < FX32_ONE || scaleY < FX32_ONE) {
        scaleX += 0x19a;
        scaleY += 0x19a;
        if (scaleX > FX32_ONE) {
            scaleX = FX32_ONE;
        }
        if (scaleY > FX32_ONE) {
            scaleY = FX32_ONE;
        }
        func_ov112_021f4eb4(work->itemSys, draw, scaleX, scaleY);
    }
}


void func_ov133_021b8140(DupFittingWork *work) {
    VecFx32 vec = { 0, 0, FX32_CONST(70) };
    DupFittingItemEntry *entry = func_ov133_021ba20c(work->heldNode);
    DupFittingPoint touch;
    DupFittingItemNode *node;
    MusItemDraw *srcDraw;
    MusItemDraw *newDraw;
    s16 size[2];
    DupFittingPoint *origin;

    touch.x = work->touchX;
    touch.y = work->touchY;
    vec.x = FX32_CONST(work->touchX);
    vec.y = FX32_CONST(work->touchY);
    node = func_ov133_021ba19c(work->heapId, work->itemSys, entry, &vec);
    func_ov133_021ba100(work->stageList, node);
    func_ov133_021ba21c(node, &touch);
    srcDraw = func_ov133_021ba218(work->heldNode);
    newDraw = func_ov133_021ba218(node);
    func_ov112_021f4ec4(work->itemSys, srcDraw, &size[1], &size[0]);
    func_ov112_021f4eb4(work->itemSys, newDraw, size[1], size[0]);
    func_ov112_021f4ee8(work->itemSys, newDraw, FALSE);
    origin = func_ov133_021ba228(work->heldNode);
    work->holdKind = 1;
    work->holdOrigin.x = origin->x;
    work->holdOrigin.y = origin->y;
    work->holdState = 2;
    work->heldNode = node;
    work->holdKind = 1;
    entry->used = TRUE;
    func_ov133_021b7a18(work);
    GFL_SndSEPlay(SEQ_SE_MSCL_04);
}


void func_ov133_021b8254(DupFittingWork *work) {
    fx32 z = 10 * FX32_ONE;
    MusItemDraw *draw = func_ov133_021ba218(work->heldNode);
    DupFittingItemNode *node;
    VecFx32 pos;

    func_ov133_021ba148(work->stageList, work->heldNode);
    func_ov133_021ba124(work->stageList, work->heldNode);
    func_ov112_021f4eb4(work->itemSys, draw, FX32_ONE, FX32_ONE);
    node = func_ov133_021ba184(work->stageList);
    while (node != NULL) {
        MusItemDraw *nodeDraw = func_ov133_021ba218(node);

        func_ov112_021f4ea4(work->itemSys, nodeDraw, &pos);
        pos.z = z;
        func_ov112_021f4e8c(work->itemSys, nodeDraw, &pos);
        z -= FX32_ONE / 4;
        node = func_ov133_021ba208(node);
    }
}

void func_ov133_021b82e8(DupFittingWork *work, BOOL unused) {
    DupFittingPoint touch;
    DupFittingItemEntry *entry;
    DupFittingItemNode *found;
    DupFittingItemNode *node;
    MusItemDraw *draw;

    func_ov133_021ba218(work->heldNode);
    draw = func_ov133_021ba218(work->heldNode);
    entry = func_ov133_021ba20c(work->heldNode);
    func_ov133_021ba148(work->stageList, work->heldNode);
    func_ov133_021ba124(work->returnList, work->heldNode);
    {
        VecFx32 vec = { 0, 0, FX32_CONST(70) };

        touch.x = work->touchX;
        touch.y = work->touchY;
        vec.x = FX32_CONST(work->holdOrigin.x);
        vec.y = FX32_CONST(work->holdOrigin.y);
        func_ov133_021ba22c(work->heldNode, &touch);
        func_ov112_021f4e8c(work->itemSys, draw, &vec);
    }
    func_ov112_021f4eb4(work->itemSys, draw, FX32_ONE, FX32_ONE);
    func_ov133_021ba244(work->heldNode, FX32_ONE);
    found = NULL;
    node = func_ov133_021ba184(work->carouselList);
    while (node != NULL && found == NULL) {
        DupFittingItemEntry *nodeEntry = func_ov133_021ba20c(node);

        if (nodeEntry->itemId == entry->itemId) {
            found = node;
        }
        node = func_ov133_021ba208(node);
    }
    if (found != NULL) {
        func_ov133_021ba21c(work->heldNode, func_ov133_021ba228(found));
        func_ov133_021ba23c(work->heldNode, func_ov133_021ba248(found));
    } else {
        DupFittingPoint shelf = { 128, 32 };

        func_ov133_021ba21c(work->heldNode, &shelf);
        func_ov133_021ba23c(work->heldNode, 0);
    }
    func_ov133_021ba24c(work->heldNode, 10);
    entry->used = FALSE;
    func_ov133_021b7a18(work);
    GFL_SndSEPlay(SEQ_SE_MSCL_05);
}

void func_ov133_021b8444(DupFittingWork *work) {
    MusItemDraw *heldDraw;
    DupFittingItemNode *node;
    s32 z;

    func_ov133_021ba218(work->heldNode);
    heldDraw = func_ov133_021ba218(work->heldNode);
    if (func_ov133_021b8b78(work, work->hoverPos) == TRUE) {
        s32 category = func_ov112_021f50f4(work->hoverPos);
        DupFittingItemNode *other;

        for (other = func_ov133_021ba184(work->wornList); other != NULL; other = func_ov133_021ba208(other)) {
            if (category == func_ov112_021f50f4(func_ov133_021ba258(other))) {
                break;
            }
        }
        if (other != NULL) {
            VecFx32 vec = { 0, 0, FX32_CONST(70) };
            MusItemDraw *otherDraw = func_ov133_021ba218(other);
            DupFittingPoint pt;
            s32 offset[2];
            DupFittingPoint *prevPos;

            func_ov112_021f4f04(otherDraw, offset);
            func_ov133_021b7458(work, &pt);
            pt.x += offset[0];
            pt.y += offset[1];
            func_ov133_021ba148(work->wornList, other);
            func_ov133_021ba124(work->stageList, other);
            prevPos = func_ov133_021ba228(other);
            vec.x = FX32_CONST(pt.x);
            vec.y = FX32_CONST(pt.y);
            func_ov133_021ba22c(other, prevPos);
            func_ov112_021f4e8c(work->itemSys, otherDraw, &vec);
            func_ov112_021f4ed4(work->itemSys, otherDraw, 0);
            func_ov133_021ba21c(other, &pt);
            func_ov133_021ba24c(other, 10);
            z = 0;
            for (node = func_ov133_021ba184(work->stageList); node != NULL; node = func_ov133_021ba208(node)) {
                MusItemDraw *nodeDraw = func_ov133_021ba218(node);
                VecFx32 nodePos;

                func_ov112_021f4ea4(work->itemSys, nodeDraw, &nodePos);
                nodePos.z = z;
                func_ov112_021f4e8c(work->itemSys, nodeDraw, &nodePos);
                z -= FX32_ONE / 4;
            }
        }
    }
    func_ov133_021ba148(work->stageList, work->heldNode);
    func_ov133_021ba124(work->wornList, work->heldNode);
    func_ov112_021f4eb4(work->itemSys, heldDraw, FX32_ONE, FX32_ONE);
    func_ov133_021ba254(work->heldNode, work->hoverPos);
    for (node = func_ov133_021ba184(work->wornList), z = 0; node != NULL; node = func_ov133_021ba208(node)) {
        MusItemDraw *nodeDraw = func_ov133_021ba218(node);
        VecFx32 nodePos;
        s32 depth;

        func_ov112_021f4ea4(work->itemSys, nodeDraw, &nodePos);
        if (func_ov112_021f4b24(nodeDraw) == TRUE) {
            depth = 0x66;
            nodePos.z = z + (depth << 10);
        } else if (func_ov133_021ba258(node) == 7 || func_ov133_021ba258(node) == 8) {
            depth = 0xde;
            nodePos.z = z + (depth << 10);
        } else {
            depth = 0xca;
            nodePos.z = z + (depth << 10);
        }
        func_ov112_021f4e8c(work->itemSys, nodeDraw, &nodePos);
        z -= FX32_ONE / 4;
    }
}

void func_ov133_021b8680(DupFittingWork *work) {
    if (work->holdKind == 2) {
        VecFx32 vec = { 0, 0, FX32_CONST(70) };
        MusItemDraw *draw;
        DupFittingPoint touch;

        draw = func_ov133_021ba218(work->heldNode);
        touch.x = work->touchX;
        touch.y = work->touchY;
        vec.x = FX32_CONST(work->holdOrigin.x);
        vec.y = FX32_CONST(work->holdOrigin.y);
        func_ov133_021ba22c(work->heldNode, &touch);
        func_ov112_021f4e8c(work->itemSys, draw, &vec);
        func_ov133_021ba21c(work->heldNode, &work->holdOrigin);
        func_ov133_021ba24c(work->heldNode, 10);
        func_ov133_021b8254(work);
    } else if (work->holdKind == 3) {
        MusItemDraw *draw = func_ov133_021ba218(work->heldNode);
        s32 pos = func_ov133_021ba258(work->heldNode);
        MusPokeDrawEquipPos *equip = func_ov112_021f5450(work->poke, pos);
        DupFittingPoint touch;
        DupFittingPoint target;
        VecFx32 vec;
        s32 depth;
        u16 rotation;

        target.x = (s32)FX_FX32_TO_F32(equip->info.pos.x + equip->info.offset.x) + 128;
        target.y = (s32)FX_FX32_TO_F32(equip->info.pos.y + equip->info.offset.y) + 96;
        vec.x = FX32_CONST(target.x);
        vec.y = FX32_CONST(target.y);
        if (func_ov112_021f4b24(draw) == TRUE) {
            depth = 0x66;
        } else if (func_ov133_021ba258(work->heldNode) == 7 || func_ov133_021ba258(work->heldNode) == 8) {
            depth = 0xde;
        } else {
            depth = 0xca;
        }
        vec.z = depth << 10;
        rotation = equip->info.cellRotation;
        func_ov112_021f4ee8(work->itemSys, draw, TRUE);
        func_ov112_021f4e8c(work->itemSys, draw, &vec);
        func_ov112_021f4ed4(work->itemSys, func_ov133_021ba218(work->heldNode), rotation);
        func_ov133_021ba21c(work->heldNode, &target);
        touch.x = work->touchX;
        touch.y = work->touchY;
        func_ov133_021ba22c(work->heldNode, &touch);
        func_ov133_021ba24c(work->heldNode, 10);
        work->hoverPos = pos;
        func_ov133_021b8444(work);
        work->hoverPos = DUP_FITTING_EQUIP_NONE;
    } else if (work->holdKind == 1) {
        MusItemDraw *draw;
        DupFittingPoint touch;

        func_ov133_021ba238(work->heldNode);
        {
            VecFx32 vec = { 0, 0, FX32_CONST(70) };

            draw = func_ov133_021ba218(work->heldNode);
            func_ov133_021ba21c(work->heldNode, &work->holdOrigin);
            func_ov133_021ba24c(work->heldNode, 10);
            touch.x = work->touchX;
            touch.y = work->touchY;
            vec.x = FX32_CONST(work->holdOrigin.x);
            vec.y = FX32_CONST(work->holdOrigin.y);
            func_ov133_021ba22c(work->heldNode, &touch);
            func_ov112_021f4e8c(work->itemSys, draw, &vec);
        }
        func_ov133_021b82e8(work, TRUE);
    } else {
        func_ov133_021b82e8(work, FALSE);
    }
}





// pokebw2's DupFitting_SelectCategory
BOOL func_ov133_021b8930(DupFittingWork *work) {
    u16 distance = 16;
    s32 touch[2];
    u16 index;
    u8 category = 0;
    s32 pos;

    index = DupFitting_GetRingIndex(work, 0x19249);
    touch[0] = work->touchX - 128;
    touch[1] = work->touchY - 112;
    pos = func_ov133_021b8bb4(work, NULL, touch, &distance);
    if (pos != DUP_FITTING_EQUIP_NONE) {
        category = func_ov112_021f50f4(pos);
        if (category == 7) {
            category = 6;
        }
        if (category == work->category) {
            func_ov133_021b9028(work, DUP_FITTING_CATEGORY_ORIGINAL, index);
            work->category = DUP_FITTING_CATEGORY_ORIGINAL;
        } else {
            func_ov133_021b9028(work, category, index);
            work->category = category;
        }
        func_ov133_021b9d54(work, work->category);
        work->autoScroll = TRUE;
        work->scrolled = 0;
        work->scrollTarget = work->angle % DUP_FITTING_RING_STEP + 0x10000;
        if (work->scrollTarget >= 0x10000 && work->scrollTarget <= 0x10002) {
            work->scrollTarget += DUP_FITTING_RING_STEP;
        }
        func_ov133_021b765c(work, work->angle, 0, 72, 48);
        GFL_SndSEPlay(SEQ_SE_MSCL_07);
        return TRUE;
    }
    return category;
}

// pokebw2's DupFitting_UpdateAutoScroll
void func_ov133_021b8a30(DupFittingWork *work) {
    u32 diff = work->scrollTarget - work->scrolled;
    u32 step = diff / 10;

    if (step < 0x100) {
        if (diff < 0x100) {
            step = diff;
            work->autoScroll = FALSE;
        } else {
            step = 0x100;
        }
    }
    if (diff > FX32_ONE) {
        work->tilt -= FX32_ONE / 8;
        if (work->tilt < -0x3000) {
            work->tilt = -0x3000;
        }
    } else {
        func_ov133_021b8ac8(work);
    }
    work->scrolled += step;
    work->angle -= step;
    func_ov133_021b765c(work, work->angle, step, 72, 48);
}

// pokebw2's DupFitting_UpdateTilt
void func_ov133_021b8ac8(DupFittingWork *work) {
    if (MATH_ABS(work->angleSpeed) < 0x30) {
        if (work->tilt != 0 || work->tiltSpeed != 0) {
            work->tilt += work->tiltSpeed;
            if (work->tilt > 0) {
                if (work->tiltSpeed > 0) {
                    work->tiltSpeed -= 0x80;
                } else {
                    work->tiltSpeed -= 0x2a;
                }
            } else if (work->tilt < 0) {
                if (work->tiltSpeed < 0) {
                    work->tiltSpeed += 0x80;
                } else {
                    work->tiltSpeed += 0x2a;
                }
            }
            if (MATH_ABS(work->tilt) < 0x200 && MATH_ABS(work->tiltSpeed) < 0x200) {
                work->tiltSpeed = 0;
                work->tilt = 0;
            }
            func_ov133_021b765c(work, work->angle, 0, 72, 48);
        }
    }
}

// pokebw2's DupFitting_HasWornInCategory
BOOL func_ov133_021b8b78(DupFittingWork *work, s32 pos) {
    s32 category = func_ov112_021f50f4(pos);
    DupFittingItemNode *node;

    for (node = func_ov133_021ba184(work->wornList); node != NULL; node = func_ov133_021ba208(node)) {
        if (category == func_ov112_021f50f4(func_ov133_021ba258(node))) {
            return TRUE;
        }
    }
    return FALSE;
}

s32 func_ov133_021b8bb4(DupFittingWork *work, MusItemDraw *item, s32 *target, u16 *dist) {
    s32 best = (u16)(*dist * *dist);
    s32 index = 10;

    enum { EQUIP_POS_FIRST = 0 } i;

    for (i = EQUIP_POS_FIRST; i < 9; i++) {
        BOOL ok;
        MusPokeDrawEquipPos *equip;

        if (item == NULL) {
            ok = TRUE;
        } else {
            ok = func_ov112_021f4af0(item, i);
        }
        equip = func_ov112_021f5450(work->poke, i);
        if (equip->valid == TRUE && ok == TRUE) {
            s32 x = (s32)FX_FX32_TO_F32(equip->info.pos.x + equip->info.offset.x) + 128 - 128;
            s32 y = (s32)((f32)(equip->info.pos.y + equip->info.offset.y) / (f32)FX32_ONE);
            s32 dx = target[0] - x;
            s32 dy = target[1] - (y - 0x10);
            s32 dist2 = dx * dx + dy * dy;

            if (dist2 < best) {
                best = (u16)dist2;
                index = i;
            }
        }
    }
    *dist = (u16)best;
    return index;
}

// Where a prop is on its way from origin to pos, with frames of the 10 frames still to go
static inline void DupFitting_GetMovePos(VecFx32 *vec, DupFittingPoint *origin, DupFittingPoint *pos, u16 frames) {
    s32 oy;
    s32 t;
    s32 ox;
    s32 d;

    t = 10 - frames;
    ox = origin->x;
    d = pos->x - ox;
    vec->x = FX32_CONST(ox + d * t / 10);
    oy = origin->y;
    d = pos->y - oy;
    vec->y = FX32_CONST(oy + d * t / 10);
}

void func_ov133_021b8c6c(DupFittingWork *work) {
    DupFittingItemNode *node;
    u16 frames;
    DupFittingItemNode *cur;
    MusItemDraw *draw;
    DupFittingPoint *origin;
    DupFittingPoint *pos;
    s16 baseScale;
    BOOL onWorn;
    u16 next;
    DupFittingPoint *from;

    node = func_ov133_021ba184(work->returnList);
    while (node != NULL) {
        cur = node;
        draw = func_ov133_021ba218(node);
        frames = func_ov133_021ba250(node);
        origin = func_ov133_021ba238(node);
        pos = func_ov133_021ba228(node);
        baseScale = func_ov133_021ba240(node);
        node = func_ov133_021ba208(cur);
        frames--;
        if (frames != 0) {
            s16 scale = baseScale + (FX32_ONE - baseScale) * frames / 10;
            VecFx32 vec;

            func_ov112_021f4eb4(work->itemSys, draw, scale, scale);
            func_ov112_021f4ea4(work->itemSys, draw, &vec);
            DupFitting_GetMovePos(&vec, origin, pos, (s32)frames);
            vec.z = FX32_CONST(70);
            func_ov112_021f4e8c(work->itemSys, draw, &vec);
            func_ov133_021ba24c(cur, frames);
        } else {
            func_ov133_021ba148(work->returnList, cur);
            func_ov133_021ba1d8(cur, work->itemSys);
        }
    }
    onWorn = FALSE;
    node = func_ov133_021ba184(work->stageList);
    if (node == NULL) {
        onWorn = TRUE;
        node = func_ov133_021ba184(work->wornList);
    }
    while (node != NULL) {
        u16 frames = func_ov133_021ba250(node);

        if (frames != 0) {
            draw = func_ov133_021ba218(node);
            from = func_ov133_021ba238(node);
            pos = func_ov133_021ba228(node);
            next = frames - 1;
            if (next != 0) {
                VecFx32 vec;

                func_ov112_021f4ea4(work->itemSys, draw, &vec);
                DupFitting_GetMovePos(&vec, from, pos, next);
                func_ov112_021f4e8c(work->itemSys, draw, &vec);
            } else {
                VecFx32 vec;

                func_ov112_021f4ea4(work->itemSys, draw, &vec);
                vec.x = FX32_CONST(pos->x);
                vec.y = FX32_CONST(pos->y);
                func_ov112_021f4e8c(work->itemSys, draw, &vec);
                func_ov133_021ba22c(node, pos);
            }
            func_ov133_021ba24c(node, next);
        }
        node = func_ov133_021ba208(node);
        if (node == NULL && !onWorn) {
            onWorn = TRUE;
            node = func_ov133_021ba184(work->wornList);
        }
    }
}



// pokebw2's DupFitting_UpdateShadow
void func_ov133_021b8fa8(DupFittingWork *work) {
    if (work->heldNode == NULL) {
        func_ov112_021f4e6c(work->itemSys, work->shadowDraw, FALSE);
        return;
    }
    if (work->shadowItemId == func_ov133_021ba20c(work->heldNode)->itemId) {
        func_ov112_021f4e6c(work->itemSys, work->shadowDraw, TRUE);
        func_ov112_021f4e2c(work->itemSys, func_ov133_021ba218(work->heldNode), work->shadowDraw);
    }
}

// pokebw2's DupFitting_IsInEllipse
BOOL func_ov133_021b8ff4(s16 x, s16 y, int radius, f32 xScale) {
    s16 scaledX = x * xScale;

    if (scaledX * scaledX + y * y < radius * radius) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's DupFitting_SortEntries
void func_ov133_021b9028(DupFittingWork *work, s32 category, u16 start) {
    u16 i;
    u16 j;
    u16 first;
    u16 best;
    u16 key;

    for (i = 0; i < work->entryCount; i++) {
        work->sorted[(u16)((start + i) % work->entryCount)] = &work->entries[i];
    }
    for (i = 0; i < work->entryCount; i++) {
        first = (start + i) % work->entryCount;
        best = first;
        key = func_ov133_021b9118(work, work->sorted[first], category);

        for (j = i + 1; j < work->entryCount; j++) {
            u16 index = (start + j) % work->entryCount;
            u16 other = func_ov133_021b9118(work, work->sorted[index], category);

            if (key < other) {
                key = other;
                best = index;
            }
        }
        if (best != first) {
            DupFittingItemEntry *tmp = work->sorted[first];

            work->sorted[first] = work->sorted[best];
            work->sorted[best] = tmp;
        }
    }
}

// pokebw2's DupFitting_GetSortKey
u16 func_ov133_021b9118(DupFittingWork *work, DupFittingItemEntry *entry, s32 pos) {
    BOOL ofPos = func_ov112_021f4b10(work->itemSys, entry->itemId, pos);
    BOOL canEquip = func_ov112_021f4afc(work->itemSys, entry->itemId, pos);
    s32 key;

    if (ofPos == TRUE) {
        if (entry->isNew == TRUE) {
            key = 6000;
        } else {
            key = 5000;
        }
    } else if (canEquip == TRUE) {
        if (entry->isNew == TRUE) {
            key = 4000;
        } else {
            key = 3000;
        }
    } else if (entry->isNew == TRUE) {
        key = 2000;
    } else {
        key = 1000;
    }
    return key + (999 - entry->itemId);
}

// pokebw2's DupFitting_UpdateTransition
void func_ov133_021b918c(DupFittingWork *work) {
    DupFittingItemNode *node;
    ClActorPos pos;
    s32 y;
    s32 t = work->transition;

    func_ov133_021b765c(work, work->angle, 0, t + 72, t * (2.0f / 3.0f) + 48.0f);
    for (node = func_ov133_021ba184(work->stageList); node != NULL; node = func_ov133_021ba208(node)) {
        DupFittingPoint *nodePos = func_ov133_021ba228(node);
        MusItemDraw *draw = func_ov133_021ba218(node);
        s16 dx = nodePos->x - 128;
        s16 dy = nodePos->y - 80;
        fx32 dist = FX_Sqrt(FX32_CONST(dx * dx + dy * dy));
        u16 angle = FX_Atan2Idx(FX32_CONST(dy), FX32_CONST(dx));
        VecFx32 vec;
        fx32 radius;

        func_ov112_021f4ea4(work->itemSys, draw, &vec);
        radius = dist + FX32_CONST(work->transition);
        vec.x = FX_Mul(FX_CosIdx(angle), radius) + 128 * FX32_ONE;
        radius = dist + FX32_CONST(work->transition * (2.0f / 3.0f));
        vec.y = FX_Mul(FX_SinIdx(angle), radius) + 80 * FX32_ONE;
        func_ov112_021f4e8c(work->itemSys, draw, &vec);
    }
    GFL_BGSysMoveBG(2, BG_MOVE_SET_Y, -work->transition);
    pos.x = 32;
    y = 192 - work->transition;
    if (y >= 64) {
        y = 64;
    }
    pos.y = y + 160;
    func_0204b404(work->buttons[0], &pos, CLACT_SURFACE_MAIN);
    pos.x = 224;
    y = 192 - work->transition;
    if (y >= 64) {
        y = 64;
    }
    pos.y = y + 160;
    func_0204b404(work->buttons[1], &pos, CLACT_SURFACE_MAIN);
}

// pokebw2's DupFitting_UpdateConfirm
u32 func_ov133_021b940c(DupFittingWork *work) {
    TouchRect rects[3] = {
        { 136, 192, 0, 48 },
        { 136, 192, 216, 255 },
        { TOUCH_RECT_END, 0, 0, 0 },
    };
    s32 hit;

    func_02035cb8(&work->touchX, &work->touchY);
    hit = func_02035c08(rects);
    switch (hit) {
    case 0:
        func_ov133_021b96d8(work);
        GFL_SndSEPlay(SEQ_SE_DECIDE3);
        func_ov133_021ba098(work);
        return 2;
    case 1: {
        WordSet *wordSet;

        work->state = DUP_FITTING_STATE_CONFIRM_OUT;
        work->transition = 192;
        GFL_SndSEPlay(SEQ_SE_MSCL_21);
        func_ov133_021b94c0(work);
        wordSet = GFL_WordSetSystemCreateDefault(work->heapId);
        loadPokemonNicknameToStrbuf(wordSet, 0, work->param->poke->pkm);
        func_ov133_021b9c08(work, Title3_Text_000, wordSet);
        GFL_WordSetSystemFree(wordSet);
        func_ov133_021ba098(work);
        break;
    }
    case TOUCH_RECT_NONE:
        func_ov133_021b952c(work);
        break;
    }
    func_ov133_021b8fa8(work);
    return 0;
}

void func_ov133_021b94c0(DupFittingWork *work) {
    DupFittingItemNode *node;
    u8 i;

    for (i = 0; i < 9; i++) {
        work->param->poke->equips[i].unk2 = 0;
    }
    node = func_ov133_021ba184(work->wornList);
    while (node != NULL) {
        u16 pos = func_ov133_021ba258(node);
        MusPokeDrawEquipPos *equip = func_ov112_021f5450(work->poke, pos);

        func_ov112_021f4ed4(work->itemSys, func_ov133_021ba218(node), equip->info.cellRotation);
        node = func_ov133_021ba208(node);
    }
}


// pokebw2's DupFitting_UpdateConfirmTouch
void func_ov133_021b952c(DupFittingWork *work) {
    work->prevTouchX = work->touchX;
    work->prevTouchY = work->touchY;
    work->touchStart = func_02035c60();
    work->touching = func_02035cb8(&work->touchX, &work->touchY);
    if (work->touchStart == TRUE) {
        DupFittingItemNode *best = NULL;
        u32 bestDistance = 256;
        DupFittingItemNode *node;

        for (node = func_ov133_021ba184(work->wornList); node != NULL; node = func_ov133_021ba208(node)) {
            u32 distance = func_ov133_021ba324(node, work->touchX, work->touchY);

            if (bestDistance > distance) {
                best = node;
                bestDistance = distance;
            }
        }
        if (best != NULL) {
            work->heldNode = best;
        }
    }
    if (work->touching == TRUE && work->heldNode != NULL) {
        func_ov133_021b95ac(work);
    }
    if (work->touching == FALSE) {
        work->dragging = FALSE;
        work->heldNode = NULL;
    }
}

void func_ov133_021b95ac(DupFittingWork *work) {
    u16 pos = func_ov133_021ba258(work->heldNode);
    MusPokeDrawEquipPos *equip = func_ov112_021f5450(work->poke, pos);
    DupFittingPoint *nodePos;
    s32 y;
    u16 angle;
    fx32 fx;
    fx32 fy;

    func_ov133_021ba218(work->heldNode);
    nodePos = func_ov133_021ba228(work->heldNode);
    y = nodePos->y;
    fx = FX32_CONST((s32)(work->touchX - nodePos->x));
    fy = FX32_CONST((s32)(work->touchY - y));
    angle = FX_Atan2Idx(fx, fy);
    if (work->dragging == TRUE) {
        s16 oldRotation = work->param->poke->equips[pos].unk2;
        s16 rotation = oldRotation + (s16)(work->prevAngle - angle);

        if (rotation < -0xaaa) {
            rotation = -0xaaa;
        }
        if (rotation > 0xaaa) {
            rotation = 0xaaa;
        }
        work->rotateAccum += (s16)(oldRotation - rotation);
        if (MATH_ABS(work->rotateAccum) > 0x71c) {
            work->rotateAccum = 0;
            GFL_SndSEPlay(SEQ_SE_MSCL_08);
        }
        func_ov112_021f4ed4(work->itemSys, func_ov133_021ba218(work->heldNode),
                            equip->info.cellRotation + rotation);
        work->param->poke->equips[pos].unk2 = rotation;
    }
    work->prevAngle = angle;
    work->dragging = TRUE;
}


void func_ov133_021b96d8(DupFittingWork *work) {
    MusicalSaveUnk1B0 *saved = func_0200a628(work->param->save);
    DupFittingItemNode *node;
    u8 i = 0;

    func_0200a604(work->param->save);
    node = func_ov133_021ba184(work->wornList);
    while (node != NULL && i < 8) {
        u16 pos = func_ov133_021ba258(node);

        func_ov112_021f5450(work->poke, pos);
        saved[i].unk0 = pos;
        saved[i].itemId = func_ov133_021ba20c(node)->itemId;
        saved[i].rotation = work->param->poke->equips[pos].unk2;
        work->param->poke->equips[pos].itemId = func_ov133_021ba20c(node)->itemId;
        work->param->poke->equips[pos].slot = i;
        node = func_ov133_021ba208(node);
        i++;
    }
}


// pokebw2's DupFitting_UpdateButtons
void func_ov133_021b9784(DupFittingWork *work) {
    ClActorPos pos;
    u8 palette;

    pos.x = 32;
    pos.y = work->confirmTimer * 2 + 160;
    if (pos.y >= 224) {
        pos.y = 224;
    }
    func_0204b404(work->buttons[0], &pos, CLACT_SURFACE_MAIN);
    pos.x = 224;
    pos.y = work->confirmTimer * 2 + 160;
    if (pos.y >= 224) {
        pos.y = 224;
    }
    func_0204b404(work->buttons[1], &pos, CLACT_SURFACE_MAIN);
    palette = 24 * work->confirmTimer / 60 + 4;
    if (palette >= 11) {
        palette = 3;
    }
    GFL_BGSysSetScrPaletteNo(2, 10, 44, 12, 12, palette);
    GFL_BGSysQueueScrLoad(2);
}

// pokebw2's DupFitting_StartDemo
void func_ov133_021b9818(DupFittingWork *work) {
    u16 i;

    work->demo = TRUE;
    work->demoFrame = 0;
    work->demoStage = 1;
    i = 5;
    do {
        if (work->sorted[i]->used != TRUE) {
            break;
        }
        i++;
        if (i == work->entryCount) {
            i = 0;
        }
    } while (i != 5);
    if (i == 0) {
        work->demoItem = work->entryCount - 1;
    } else {
        work->demoItem = i - 1;
    }
    work->state = DUP_FITTING_STATE_DEMO;
}

// pokebw2's DupFitting_EndDemo
void func_ov133_021b987c(DupFittingWork *work) {
    work->demo = FALSE;
    work->state = DUP_FITTING_STATE_DRESS;
    func_0204b3dc(work->hand, FALSE);
    func_ov133_021b9c90(work);
}

// pokebw2's DupFitting_UpdateDemo
void func_ov133_021b98a0(DupFittingWork *work) {
    switch (work->demoStage) {
    case 1: {
        u8 item = work->demoItem;

        func_ov133_021b9990(work, item * DUP_FITTING_RING_STEP, item * DUP_FITTING_RING_STEP, item * 15);
        break;
    }
    case 2:
        func_ov133_021b9a88(work, 30);
        break;
    case 3: {
        u8 item = work->demoItem;
        u16 target = item * DUP_FITTING_RING_STEP;

        func_ov133_021b9990(work, -target, -(item * DUP_FITTING_RING_STEP), item * 15);
        break;
    }
    case 4:
        func_ov133_021b9a88(work, 30);
        break;
    case 5: {
        DupFittingPoint from = { 162, 120 };
        DupFittingPoint to = { 192, 160 };

        func_ov133_021b9ac0(work, &from, &to, 60);
        break;
    }
    case 6:
        func_ov133_021b9a88(work, 30);
        break;
    case 7: {
        DupFittingPoint from = { 192, 160 };
        DupFittingPoint to = { 162, 120 };

        func_ov133_021b9ac0(work, &from, &to, 60);
        break;
    }
    case 8:
        func_ov133_021b9a88(work, 30);
        break;
    case 9:
        func_ov133_021b987c(work);
        return;
    }
    {
        ClActorPos pos;

        pos.x = work->touchX;
        pos.y = work->touchY;
        func_0204b404(work->hand, &pos, CLACT_SURFACE_MAIN);
    }
    func_ov133_021b75e0(work);
}

// pokebw2's DupFitting_DemoRotate
void func_ov133_021b9990(DupFittingWork *work, u16 target, s32 total, u16 frames) {
    s32 step;
    u16 angle;

    work->touching = FALSE;
    work->touchStart = FALSE;
    func_0204b3dc(work->hand, TRUE);
    step = total / frames;
    angle = (target - step * work->demoFrame + 0x10000) % 0x10000;
    work->touchX = FX_FX32_TO_F32(72 * FX_SinIdx(angle) + 128 * FX32_ONE);
    work->touchY = FX_FX32_TO_F32(48 * FX_CosIdx(angle) + 80 * FX32_ONE);
    work->angle -= (u16)step;
    func_ov133_021b765c(work, work->angle, step, 72, 48);
    work->demoFrame++;
    if (work->demoFrame > frames) {
        work->demoFrame = 0;
        work->demoStage++;
    }
}

// pokebw2's DupFitting_DemoWait
void func_ov133_021b9a88(DupFittingWork *work, u16 frames) {
    work->touching = FALSE;
    work->touchStart = FALSE;
    work->demoFrame++;
    func_0204b3dc(work->hand, FALSE);
    if (work->demoFrame > frames) {
        work->demoFrame = 0;
        work->demoStage++;
    }
}

void func_ov133_021b9ac0(DupFittingWork *work, const DupFittingPoint *from, const DupFittingPoint *to,
                                    u16 frames) {
    s32 y;

    work->touching = TRUE;
    if (work->demoFrame == 0) {
        work->touchStart = TRUE;
    } else {
        work->touchStart = FALSE;
    }
    y = from->y + (to->y - from->y) * work->demoFrame / frames;
    work->touchX = from->x + (to->x - from->x) * work->demoFrame / frames;
    work->touchY = y;
    func_0204b3dc(work->hand, TRUE);
    work->demoFrame++;
    if (work->demoFrame > frames) {
        work->demoFrame = 0;
        work->demoStage++;
    }
}

// pokebw2's DupFitting_InitMessages
void func_ov133_021b9b48(DupFittingWork *work) {
    work->font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, work->heapId);
    work->dressMsg = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_TITLE_3, work->heapId);
    work->itemMsg = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_TITLE_13, work->heapId);
    work->categoryMsg =
        GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_TITLE_S005D_WORD_SET_MUSICAL_INFO, work->heapId);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, PALTYPE_SUB_BG, 0x1c0, 0x20, work->heapId);
    work->printQueue = func_0201c440(work->heapId);
    LoadSysMsgBox(7, 0x80, 12, 0, work->heapId);
}

// pokebw2's DupFitting_FreeMessages
void func_ov133_021b9bd0(DupFittingWork *work) {
    func_0201c4c0(work->printQueue);
    GFL_MsgDataFree(work->itemMsg);
    GFL_MsgDataFree(work->categoryMsg);
    GFL_MsgDataFree(work->dressMsg);
    GFL_FontFree(work->font);
}

// pokebw2's DupFitting_PrintMessage
void func_ov133_021b9c08(DupFittingWork *work, u32 messageId, WordSet *wordSet) {
    StrBuf *str = GFL_MsgDataLoadStrbufNew(work->dressMsg, messageId);

    func_0201c6ec(work->printQueue);
    GFL_BitmapFill(BmpWin_GetBitmap(work->bmpWin), 15);
    if (wordSet != NULL) {
        StrBuf *formatted = GFL_StrBufCreate(128, work->heapId);

        GFL_WordSetFormatStrbuf(wordSet, formatted, str);
        GFL_StrBufFree(str);
        str = formatted;
    }
    func_0201c724(work->printQueue, BmpWin_GetBitmap(work->bmpWin), 2, 2, str, work->font, PRINT_COLOR(1, 2, 0));
    GFL_StrBufFree(str);
    work->printing = TRUE;
}

// pokebw2's DupFitting_PrintPokeName
void func_ov133_021b9c90(DupFittingWork *work) {
    WordSet *wordSet = GFL_WordSetSystemCreateDefault(work->heapId);

    loadPokemonNicknameToStrbuf(wordSet, 0, work->param->poke->pkm);
    func_ov133_021b9c08(work, Title3_Text_000, wordSet);
    GFL_WordSetSystemFree(wordSet);
}

void func_ov133_021b9cbc(DupFittingWork *work, DupFittingItemNode *node) {
    MusItemData *items = func_ov112_021f4f10(work->itemSys);
    DupFittingItemEntry *entry = func_ov133_021ba20c(node);
    s32 category = func_ov112_021f5158(items, entry->itemId);
    WordSet *wordSet = GFL_WordSetSystemCreateDefault(work->heapId);
    StrBuf *itemName = GFL_MsgDataLoadStrbufNew(work->itemMsg, entry->itemId + Title13_Text_003);
    StrBuf *categoryName;

    category += TitleS005dWordSetMusicalInfo_Text_016;
    categoryName = GFL_MsgDataLoadStrbufNew(work->categoryMsg, category);
    func_0201ee0c(wordSet, 0, itemName, 0, 1, 2);
    func_0201ee0c(wordSet, 1, categoryName, 0, 1, 2);
    func_ov133_021b9c08(work, Title3_Text_003, wordSet);
    GFL_StrBufFree(categoryName);
    GFL_StrBufFree(itemName);
    GFL_WordSetSystemFree(wordSet);
}


// pokebw2's DupFitting_PrintCategory
void func_ov133_021b9d54(DupFittingWork *work, s32 category) {
    u32 messageId;
    WordSet *wordSet;
    StrBuf *name;

    if (category == DUP_FITTING_CATEGORY_ORIGINAL) {
        func_ov133_021b9c08(work, Title3_Text_005, NULL);
        return;
    }
    messageId = 0;
    switch (category) {
    case 0:
    case 1:
        messageId = TitleS005dWordSetMusicalInfo_Text_016;
        break;
    case 2:
        messageId = TitleS005dWordSetMusicalInfo_Text_017;
        break;
    case 3:
        messageId = TitleS005dWordSetMusicalInfo_Text_023;
        break;
    case 4:
        messageId = TitleS005dWordSetMusicalInfo_Text_020;
        break;
    case 5:
        messageId = TitleS005dWordSetMusicalInfo_Text_021;
        break;
    case 6:
    case 7:
        messageId = TitleS005dWordSetMusicalInfo_Text_022;
        break;
    }
    wordSet = GFL_WordSetSystemCreateDefault(work->heapId);
    name = GFL_MsgDataLoadStrbufNew(work->categoryMsg, messageId);
    func_0201ee0c(wordSet, 0, name, 0, 1, 2);
    func_ov133_021b9c08(work, Title3_Text_004, wordSet);
    GFL_StrBufFree(name);
    GFL_WordSetSystemFree(wordSet);
}

// pokebw2's DupFitting_InitSparkles
void func_ov133_021b9de8(DupFittingWork *work) {
    ClActorSetup sparkle;
    ClActorSetup setup;
    u8 i;

    for (i = 0; i < 8; i++) {
        sparkle.x = 0;
        sparkle.y = 0;
        sparkle.sequence = 0;
        sparkle.priority = 0;
        sparkle.bgPriority = 0;
        work->sparkles[i] = func_0204b294(work->clactUnit, work->clactRes[2], work->clactRes[5], work->clactRes[8],
                                          &sparkle, CLACT_SURFACE_SUB, work->heapId);
        func_0204b3dc(work->sparkles[i], TRUE);
        func_0204b92c(work->sparkles[i], TRUE);
        func_0204b688(work->sparkles[i], 0);
        work->sparkleTimer[i] = 0;
    }
    setup.x = 128;
    setup.y = 96;
    setup.sequence = 0;
    setup.priority = 0;
    setup.bgPriority = 0;
    work->okMark = func_0204b294(work->clactUnit, work->clactRes[1], work->clactRes[4], work->clactRes[7], &setup,
                                 CLACT_SURFACE_MAIN, work->heapId);
    func_0204b3dc(work->okMark, FALSE);
    func_0204b92c(work->okMark, TRUE);
    setup.sequence = 2;
    work->touchEffect = func_0204b294(work->clactUnit, work->clactRes[1], work->clactRes[4], work->clactRes[7],
                                      &setup, CLACT_SURFACE_MAIN, work->heapId);
    func_0204b3dc(work->touchEffect, FALSE);
    func_0204b92c(work->touchEffect, TRUE);
}

// pokebw2's DupFitting_FreeSparkles
void func_ov133_021b9f24(DupFittingWork *work) {
    u8 i;

    for (i = 0; i < 8; i++) {
        func_0204b3b4(work->sparkles[i]);
    }
    func_0204b3b4(work->touchEffect);
    func_0204b3b4(work->okMark);
}

// pokebw2's DupFitting_UpdateSparkles
void func_ov133_021b9f5c(DupFittingWork *work) {
    ClActorPos pos;
    u8 i;

    for (i = 0; i < 8; i++) {
        func_0204b84c(work->sparkles[i], GFL_BGSysGetBGPriority(4));
        func_0204b688(work->sparkles[i], 0);
        if (work->sparkleTimer[i] == 0) {
            pos.x = GFL_RandomMTRange(56);
            if (GFL_RandomMTRange(2) == 1) {
                pos.x = 256 - pos.x;
            }
            pos.y = GFL_RandomMTRange(192);
            func_0204b404(work->sparkles[i], &pos, CLACT_SURFACE_SUB);
            if (pos.y > 90) {
                func_0204b878(work->sparkles[i], GFL_RandomMTRange(2));
            } else {
                func_0204b878(work->sparkles[i], GFL_RandomMTRange(2) + 2);
            }
            work->sparkleTimer[i] = GFL_RandomMTRange(200) + 400;
        } else {
            work->sparkleTimer[i]--;
        }
    }
    if (func_0204b3f4(work->touchEffect) == TRUE && func_0204b98c(work->touchEffect) == FALSE) {
        func_0204b3dc(work->touchEffect, FALSE);
    }
}

// pokebw2's DupFitting_ShowTouchEffect
void func_ov133_021ba098(DupFittingWork *work) {
    ClActorPos pos;

    pos.x = work->touchX;
    pos.y = work->touchY;
    func_0204b9a0(work->touchEffect);
    func_0204b3dc(work->touchEffect, TRUE);
    func_0204b404(work->touchEffect, &pos, CLACT_SURFACE_MAIN);
}

// pokebw2's DupFitting_Trace
void func_ov133_021ba0c8(DupFittingWork *work, u8 id) {
}
