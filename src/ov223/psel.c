// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov316/psel.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/psel.h"
#include "app/psel_graphic.h"
#include "constants/arc.h"
#include "constants/script_text_banks.h"
#include "constants/sound.h"
#include "constants/species.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/fade.h"
#include "gfl/g3d.h"
#include "gfl/graphics.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/proc.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "gfl/tcbl.h"
#include "gfl/touchpanel.h"
#include "gfl/ui.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_mtx.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "nnsys/g2d.h"
#include "nnsys/gfd.h"
#include "pml/poke_graphic.h"
#include "system/app_taskmenu.h"
#include "system/gf_font.h"
#include "system/printsys.h"
#include "system/text_speed.h"
#include "system/wordset.h"
#include "text/script/aspertia_city.h"

// The choice of a starter Pokémon, named after the ROM's "psel.c": the gift box opens on the main screen, a 3D scene
// whose camera follows a curve, and the player picks one of its three Poké Balls with the keys or by touching it. The
// sub screen shows the three Pokémon, the picked one jumping to the front.

#define PSEL_POKE_COUNT 3
// No ball is picked
#define PSEL_CHOICE_NONE 4

// The members of the archive (ARCID_PSEL) that the code names
enum {
    PSEL_ARC_SUB_BG_NCLR = 7,
    PSEL_ARC_CURSOR_NCLR = 11,
    PSEL_ARC_SUB_BG_NCGR = 16,
    PSEL_ARC_CURSOR_NCGR = 17,
    PSEL_ARC_SUB_BG_BACK_NSCR = 19,
    PSEL_ARC_SUB_BG_FRONT_NSCR = 20,
    PSEL_ARC_CURSOR_NCER = 24,
    PSEL_ARC_CURSOR_NANR = 28,
    PSEL_ARC_BOX_NSBMD = 34,
    PSEL_ARC_BOX_ANIM = 35,
    PSEL_ARC_BALL_NSBMD = 37,
    PSEL_ARC_BALL_ANIM_1 = 38,
    PSEL_ARC_BALL_ANIM_2 = 39,
};

// What the VBlank task is asked to show or hide: the main screen's text BGs, the sub screen's, and the message BG alone
#define PSEL_BG_REQ_BG3_ON 0x1 // Black's
#define PSEL_BG_REQ_BG3_OFF 0x2 // Black's
#define PSEL_BG_REQ_MAIN_ON 0x4
#define PSEL_BG_REQ_MAIN_OFF 0x8
#define PSEL_BG_REQ_SUB_ON 0x10
#define PSEL_BG_REQ_SUB_OFF 0x20
#define PSEL_BG_REQ_MSG_ON 0x40
#define PSEL_BG_REQ_MSG_OFF 0x80

// The frames of the intro, the box opening: when the screen flashes white, when the balls appear, and its length
#define PSEL_INTRO_FLASH_FRAME 160
#define PSEL_INTRO_BALLS_FRAME 162
#define PSEL_INTRO_SE_FRAME 202
#define PSEL_INTRO_FRAMES 278

// The 3D objects' and the camera curve's animation speed: half a frame each frame
#define PSEL_ANIME_SPEED FX32_HALF

// A Pokémon's palette, and the background's: at its own colors, fading to the other ones, at them, and fading back
enum {
    PSEL_PAL_NORMAL,
    PSEL_PAL_TO_DARK,
    PSEL_PAL_DARK,
    PSEL_PAL_TO_NORMAL,
};

// The states of a Pokémon's picture: small at the back, jumping to the middle, waiting there, jumping to the front,
// and big at the front
enum {
    PSEL_POKE_BACK,
    PSEL_POKE_JUMP_1,
    PSEL_POKE_MIDDLE,
    PSEL_POKE_JUMP_2,
    PSEL_POKE_FRONT,
};

// What a Pokémon's picture is asked to do: come to the front, or go back
enum {
    PSEL_POKE_REQ_NONE,
    PSEL_POKE_REQ_FRONT,
    PSEL_POKE_REQ_BACK,
};

// The 3D objects
enum {
    PSEL_OBJ_BOX,
    PSEL_OBJ_BALL_1,
    PSEL_OBJ_BALL_2,
    PSEL_OBJ_BALL_3,
    PSEL_OBJ_COUNT,
};

typedef struct {
    u16 actor;
    SRTMatrix status;
    BOOL draw;
} PselObj;

typedef struct {
    u32 chars;
    u32 pltt;
    u32 cellAnim;
} PselPokeRes;

// A Pokémon on the sub screen: its still picture (0), shown while it fades in, and the scaled one (1) that jumps
typedef struct {
    PselPokeRes res[2];
    ClActor *clact[2];
    u16 palNormal[16];
    u16 palDark[16];
    u16 palNow[16];
    u16 palState;
    u16 palStep;
    u16 frame;
    u32 state;
    u32 request;
    // Set with a request, and cleared once the picture moves
    BOOL unmoved;
} PselPoke;

typedef struct {
    HeapID heapId;
    PselParam *param;
    BOOL touchMode;
    s32 step;
    u32 frame;
    PselGraphic *graphic;
    Font *font;
    PrintQueue *printQueue;
    TCB *vblankTask;
    u32 bgRequest;
    G3DManager *g3dMgr;
    // The first scene holds the box alone, the second the box and the three balls
    u16 introScene;
    u16 scene;
    u16 introObjIdx;
    // The box, then the three balls, as indices into objs
    u16 objIdx[PSEL_OBJ_COUNT];
    u16 objCount;
    PselObj *objs;
    G3DCurve *cameraCurve;
    s32 cameraBall;
    BOOL cameraReverse;
    u32 cameraFrame;
    u32 introFrame;
    PselPoke poke[PSEL_POKE_COUNT];
    s32 fadeCount;
    u32 fadeState;
    u32 cursorChars;
    u32 cursorPltt;
    u32 cursorCellAnim;
    ClActor *cursor;
    s32 choice;
    // The ball whose animation plays
    s32 ball;
    // Set when the picked Pokémon reaches the front
    BOOL pokeArrived;
    MsgData *msgData;
    BmpWin *win[3];
    BOOL winDone[3];
    PrintStream *stream;
    TCBExManager *tcbMgr;
    StrBuf *strbuf;
    AppTaskMenuRes *menuRes;
    AppTaskMenuWin *button;
    AppTaskMenu *yesNo;
    u8 bgAlpha;
    // The sub screen's BG palettes of the three Pokémon and of no choice, the one the fade started from and the shown one
    u16 bgPal[4][0x80];
    u16 bgPalFrom[0x80];
    u16 bgPalNow[0x80];
    BOOL bgPalActive;
    u8 bgPalStep;
    u8 bgPalTarget;
    s32 fadeFreq;
} PselWork;

// The camera curve's frames of a ball: the zoom on it and its end, the picked ball's flight and its end, and when the
// screen starts to fade out
typedef struct {
    u16 zoom[2];
    u16 decideStart;
    u16 decideEnd;
    u16 fadeOut;
} PselCameraFrames;

// A Pokémon's picture at the back, in the middle and at the front: its position, its scale in 16ths, how long it waits
// in the middle, and the length and height of its two jumps
typedef struct {
    u8 pos[3][2];
    u8 scale[3];
    u8 wait;
    u8 duration[2];
    u8 height[2];
} PselPokeMove;

// A Pokémon's files: the scaled picture's characters, cells and animation, and its two palettes
typedef struct {
    u8 chars;
    u8 pltt;
    u8 cell;
    u8 anim;
    u8 plttDark;
} PselPokeFiles;

typedef struct {
    u8 bg;
    u8 x;
    u8 y;
    u8 width;
    u8 height;
    u8 palette;
    u8 fromEnd;
    u8 unk7;
    u8 unk8;
} PselWinSetup;

typedef struct {
    u8 x;
    u8 y;
} PselCursorPos;

typedef s32 (*PselSeqFunc)(PselWork *wk, s32 *step);

BOOL func_ov223_021b9608(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov223_021b9674(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov223_021b96e0(GameProc *proc, u32 *state, void *param, void *work);
s32 func_ov223_021bb9dc(PselWork *wk, s32 *step);
s32 func_ov223_021bb9e0(PselWork *wk, s32 *step);
s32 func_ov223_021bba78(PselWork *wk, s32 *step);
s32 func_ov223_021bbadc(PselWork *wk, s32 *step);
s32 func_ov223_021bbb28(PselWork *wk, s32 *step);
s32 func_ov223_021bbc6c(PselWork *wk, s32 *step);
s32 func_ov223_021bbcbc(PselWork *wk, s32 *step);
s32 func_ov223_021bbcf0(PselWork *wk, s32 *step);
s32 func_ov223_021bbd74(PselWork *wk, s32 *step);
s32 func_ov223_021bbde0(PselWork *wk, s32 *step);
s32 func_ov223_021bc274(PselWork *wk, s32 *step);
s32 func_ov223_021bc2cc(PselWork *wk, s32 *step);
s32 func_ov223_021bc30c(PselWork *wk, s32 *step);
s32 func_ov223_021bc348(PselWork *wk, s32 *step);

void func_ov223_021bb974(PselWork *wk, s32 ball, s32 kind);
void func_ov223_021baf30(PselWork *wk);
void func_ov223_021b97f8(PselWork *wk);
void func_ov223_021b95a0(G3DCurve *curve, s32 frame);
void func_ov223_021b9740(TCB *unused0, void *work);
void func_ov223_021b9830(PselWork *wk);
void func_ov223_021baab8(PselWork *wk);
void func_ov223_021bae00(PselWork *wk);
void func_ov223_021baa5c(PselWork *wk, BOOL flag);
void func_ov223_021bb38c(PselWork *wk);
u32 func_ov223_021baf48(PselWork *wk);
void func_ov223_021bb540(PselWork *wk);
void func_ov223_021bb630(PselWork *wk);
void func_ov223_021bb7a4(PselWork *wk);
void func_ov223_021bb844(PselWork *wk);
void func_ov223_021baa84(PselWork *wk);
s16 func_ov223_021ba0ac(ClActorPos *pos, ClActorScale *scale, u8 x0, u8 y0, u8 x1, u8 y1, u8 scale0, u8 scale1,
                             u32 duration, u8 height, u32 frame);
void func_ov223_021bb3ac(PselWork *wk);
void func_ov223_021bb520(u16 color, u16 *r, u16 *g, u16 *b);

// The OBJ palettes of the sub screen that the Pokémon take
// pokebw2's sPselPokePaletteSlots
static const u8 data_ov223_021bc6fc[PSEL_POKE_COUNT] = { 0, 1, 2 };

// The sub screen's BG palettes of the three Pokémon, and of no choice
// pokebw2's sPselBGPaletteFiles
static const u8 data_ov223_021bc708[4] = { 9, 8, 10, 7 };

// pokebw2's sPselCursorPositions
static const PselCursorPos data_ov223_021bc716[PSEL_POKE_COUNT] = { { 66, 39 }, { 128, 13 }, { 189, 39 } };

// pokebw2's sPselSpecies
static const u16 data_ov223_021bc710[PSEL_POKE_COUNT] = { SPECIES_SNIVY, SPECIES_TEPIG, SPECIES_OSHAWOTT };

// pokebw2's sPselBallAnimations
static const G3DSceneAnimationSetup data_ov223_021bc71c[] = { { 3, 0 }, { 4, 0 } };

// pokebw2's sPselIntroBoxAnimations
static const G3DSceneAnimationSetup data_ov223_021bc704[] = { { 1, 0 } };

// pokebw2's sPselTypeMessages
static const u32 data_ov223_021bc730[PSEL_POKE_COUNT] = {
    AspertiaCity_Text_018,
    AspertiaCity_Text_017,
    AspertiaCity_Text_016,
};

// pokebw2's sPselBoxAnimations
static const G3DSceneAnimationSetup data_ov223_021bc724[] = { { 1, 0 }, { 2, 0 }, { 3, 0 } };

// pokebw2's sPselPokeFiles
static const PselPokeFiles data_ov223_021bc73c[PSEL_POKE_COUNT] = {
    { 12, 0, 21, 25, 1 },
    { 13, 2, 22, 26, 3 },
    { 14, 4, 23, 27, 5 },
};

const G3DSceneResourceSetup sPselIntroResources[] = {
    { ARCID_PSEL, 29, 0, G3D_SCENE_RES_ARCSYS },
    { ARCID_PSEL, 30, 0, G3D_SCENE_RES_ARCSYS },
    { ARCID_PSEL, 31, 0, G3D_SCENE_RES_ARCSYS },
    { ARCID_PSEL, 32, 0, G3D_SCENE_RES_ARCSYS },
};

const G3DSceneResourceSetup sPselSceneResources[] = {
    { ARCID_PSEL, PSEL_ARC_BOX_NSBMD, 0, G3D_SCENE_RES_ARCSYS },
    { ARCID_PSEL, PSEL_ARC_BOX_ANIM, 0, G3D_SCENE_RES_ARCSYS },
    { ARCID_PSEL, PSEL_ARC_BALL_NSBMD, 0, G3D_SCENE_RES_ARCSYS },
    { ARCID_PSEL, PSEL_ARC_BALL_ANIM_1, 0, G3D_SCENE_RES_ARCSYS },
    { ARCID_PSEL, PSEL_ARC_BALL_ANIM_2, 0, G3D_SCENE_RES_ARCSYS },
};

const G3DSceneActorSetup sPselSceneActors[] = {
    { 0, 0, 0, 0, data_ov223_021bc704, NELEMS(data_ov223_021bc704) },
    { 2, 0, 2, 0, data_ov223_021bc71c, NELEMS(data_ov223_021bc71c) },
    { 2, 0, 2, 0, data_ov223_021bc71c, NELEMS(data_ov223_021bc71c) },
    { 2, 0, 2, 0, data_ov223_021bc71c, NELEMS(data_ov223_021bc71c) },
};

// pokebw2's sPselBoxActors
static const G3DSceneActorSetup data_ov223_021bc74c[] = {
    { 0, 0, 0, 0, data_ov223_021bc724, NELEMS(data_ov223_021bc724) },
};

// The box alone, then the box and the three balls
// pokebw2's sPselScenes
static const G3DSceneSetup data_ov223_021bc7ac[2] = {
    { sPselIntroResources, 4, data_ov223_021bc74c, NELEMS(data_ov223_021bc74c) },
    { sPselSceneResources, 5, sPselSceneActors, 4 },
};

// The three balls, and the button that confirms the choice
const TouchRect data_ov223_021bc75c[] = {
    { TOUCH_RECT_CIRCLE, 66, 96, 32 },
    { TOUCH_RECT_CIRCLE, 128, 66, 26 },
    { TOUCH_RECT_CIRCLE, 189, 96, 32 },
    { 168, 191, 72, 183 },
    { TOUCH_RECT_END, 0, 0, 0 },
};

// The window behind the Pokémon's picture, the message window, and the sub screen's
// pokebw2's sPselWinSetups
static const PselWinSetup data_ov223_021bc770[3] = {
    { 3, 4, 5, 24, 14, 14, TRUE, 0, 0 },
    { 3, 1, 19, 30, 2, 14, TRUE, 0, 0 },
    { 7, 1, 18, 30, 4, 1, TRUE, 0, 0 },
};

// pokebw2's sPselCameraFrames
static const PselCameraFrames data_ov223_021bc78c[PSEL_POKE_COUNT] = {
    { { 80, 98 }, 140, 258, 200 },
    { { 100, 118 }, 260, 378, 320 },
    { { 120, 138 }, 380, 498, 440 },
};

// pokebw2's sPselBallPositions
static const VecFx32 data_ov223_021bc7cc[PSEL_POKE_COUNT] = {
    { -0xc800, 0x1d000, 0x399a },
    { 0, 0x1d000, -0x7000 },
    { 0xc800, 0x1d000, 0x399a },
};

// pokebw2's sPselSeqFuncs
static const PselSeqFunc data_ov223_021bc84c[] = {
    func_ov223_021bb9dc, func_ov223_021bb9e0, func_ov223_021bba78, func_ov223_021bbadc, func_ov223_021bbb28,
    func_ov223_021bbc6c, func_ov223_021bbcbc, func_ov223_021bbcf0, func_ov223_021bbd74, func_ov223_021bbde0,
    func_ov223_021bc274, func_ov223_021bc2cc, func_ov223_021bc30c, func_ov223_021bc348,
};

// pokebw2's sPselPokeMoves
static const PselPokeMove data_ov223_021bc7f0[PSEL_POKE_COUNT] = {
    { { { 53, 73 }, { 90, 83 }, { 128, 94 } }, { 8, 12, 16 }, 8, { 4, 4 }, { 32, 32 } },
    { { { 128, 68 }, { 128, 81 }, { 128, 94 } }, { 8, 12, 16 }, 8, { 4, 4 }, { 32, 32 } },
    { { { 202, 73 }, { 165, 83 }, { 128, 94 } }, { 8, 12, 16 }, 8, { 4, 4 }, { 32, 32 } },
};

// pokebw2's PselProcFunctions
const GameProcFunctions data_ov223_021bc8c4 = { func_ov223_021b9608, func_ov223_021b96e0, func_ov223_021b9674 };

// The background's palette of a choice
#define PSEL_BG_PAL_OF(choice) ((choice) == PSEL_CHOICE_NONE ? 3 : (choice))

// The camera curve advances half a frame for each frame of the game
// pokebw2's PselCurve_SetFrame
void func_ov223_021b95a0(G3DCurve *curve, s32 frame) {
    GFL_G3DCurveFrameSet(curve, frame / 2);
}

// A 0x44-byte entry of the array the sub screen's matrices are kept in; the fields are named by offset
typedef struct {
    u32 unk_0;
    u32 unk_4;
    u32 unk_8;
    u32 unk_c;
    u32 unk_10;
    u32 unk_14;
    u32 unk_18;
    MtxFx33 mtx;
    u32 unk_40;
} PselMtxEntry;

void *func_ov223_021b95b0(HeapID heapId, u32 count) {
    PselMtxEntry *ptr;
    u16 i;

    ptr = GFL_HeapAllocate(heapId, count * sizeof(PselMtxEntry), TRUE, "psel.c", 0x28e);
    for (i = 0; i < count; i++) {
        ptr[i].unk_4 = 0;
        ptr[i].unk_8 = 0;
        ptr[i].unk_c = 0;
        ptr[i].unk_10 = 0x1000;
        ptr[i].unk_14 = 0x1000;
        ptr[i].unk_18 = 0x1000;
        MTX_Identity33_(&ptr[i].mtx);
    }
    return ptr;
}


void func_ov223_021b9600(void *ptr) {
    GFL_HeapFree(ptr);
}

BOOL func_ov223_021b9608(GameProc *proc, u32 *state, void *param, void *work) {
    PselWork *wk;

    GFL_FadeSet(3, 0x10, 0x10, 0);
    GFL_HeapCreateChild(1, 0x87, 0x100000);
    wk = GFL_ProcInitSubsystem(proc, sizeof(PselWork), 0x87);
    MI_CpuFill8(wk, 0, sizeof(PselWork));
    wk->heapId = 0x87;
    wk->param = param;
    wk->fadeFreq = GFL_FadeGetUpdateFreq();
    GFL_FadeSetUpdateFreq(1);
    wk->touchMode = func_020355c4();
    wk->choice = PSEL_CHOICE_NONE;
    wk->ball = PSEL_CHOICE_NONE;
    wk->pokeArrived = FALSE;
    wk->step = 0;
    return TRUE;
}

BOOL func_ov223_021b9674(GameProc *proc, u32 *state, void *param, void *work) {
    PselWork *wk = work;

    GFL_FadeSet(0xc, 0x10, 0x10, 0);
    func_020355d8(wk->touchMode);
    GFL_FadeSetUpdateFreq(wk->fadeFreq);
    switch (wk->choice) {
    case 0:
        *wk->param->result = 0;
        break;
    case 1:
        *wk->param->result = 1;
        break;
    case 2:
        *wk->param->result = 2;
        break;
    default:
        *wk->param->result = 0;
        break;
    }
    wk->param->choice = wk->choice;
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(0x87);
    return TRUE;
}



BOOL func_ov223_021b96e0(GameProc *proc, u32 *state, void *param, void *work) {
    PselWork *wk = work;
    s32 cur;
    s32 next;
    s32 now;

    cur = *state;
    next = data_ov223_021bc84c[cur](wk, &wk->step);
    *state = next;
    if (next != 0xd) {
        if (next != cur) {
            wk->step = 0;
        }
    } else {
        return TRUE;
    }
    now = *state;
    if (now >= 2 && now <= 0xb) {
        func_0201c4e4(wk->printQueue);
        func_ov223_021baf30(wk);
        func_ov223_021bc4f4(wk->graphic);
        func_ov223_021bc508(wk->graphic);
        func_ov223_021b97f8(wk);
        func_ov223_021bc514(wk->graphic);
    }
    return FALSE;
}

void func_ov223_021b9740(TCB *unused0, void *work) {
    PselWork *wk = work;

    if (wk->bgRequest & 1) {
        GFL_BGSysSetBGEnabled(3, 1);
    }
    if (wk->bgRequest & 2) {
        GFL_BGSysSetBGEnabled(3, 0);
    }
    if (wk->bgRequest & 4) {
        GFL_BGSysSetBGEnabled(3, 1);
        GFL_BGSysSetBGEnabled(2, 1);
    }
    if (wk->bgRequest & 8) {
        GFL_BGSysSetBGEnabled(3, 0);
        GFL_BGSysSetBGEnabled(2, 0);
    }
    if (wk->bgRequest & 0x10) {
        GFL_BGSysSetBGEnabled(7, 1);
        GFL_BGSysSetBGEnabled(6, 1);
        GFL_BGSysSetBGEnabled(5, 1);
    }
    if (wk->bgRequest & 0x20) {
        GFL_BGSysSetBGEnabled(7, 0);
        GFL_BGSysSetBGEnabled(6, 0);
        GFL_BGSysSetBGEnabled(5, 0);
    }
    if (wk->bgRequest & 0x40) {
        GFL_BGSysSetBGEnabled(3, 1);
    }
    if (wk->bgRequest & 0x80) {
        GFL_BGSysSetBGEnabled(3, 0);
    }
    wk->bgRequest = 0;
}



void func_ov223_021b97f8(PselWork *wk) {
    u16 i;

    for (i = 0; i < wk->objCount; i++) {
        PselObj *obj = &wk->objs[i];

        if (obj->draw) {
            G3DActor *actor = GFL_G3DMgrGetActor(wk->g3dMgr, obj->actor);
            GFL_G3DSysDrawObj(actor, &obj->status);
        }
    }
}


void func_ov223_021b9830(PselWork *wk) {
    wk->g3dMgr = GFL_G3DMgrCreate(5, 4, wk->heapId);
    wk->objCount = 0;
}


void func_ov223_021b9848(PselWork *wk) {
    GFL_G3DMgrFree(wk->g3dMgr);
}

void func_ov223_021b9854(PselWork *wk) {
    u16 i;
    u16 count;
    u16 j;
    G3DActor *actor;
    u16 first;

    wk->introScene = GFL_G3DMgrNewScene(wk->g3dMgr, &data_ov223_021bc7ac[0]);
    wk->objCount = 1;
    wk->objs = func_ov223_021b95b0(wk->heapId, wk->objCount);
    first = GFL_G3DMgrGetSceneFirstActorIdx(wk->g3dMgr, wk->introScene);
    i = 0;
    wk->introObjIdx = 0;
    {
        PselObj *obj = wk->objs;

        obj->actor = first;
        obj->draw = 1;
    }
    for (i = 0; i < wk->objCount; i++) {
        actor = GFL_G3DMgrGetActor(wk->g3dMgr, wk->objs[i].actor);
        count = GFL_G3DActorGetAnmCount(actor);
        for (j = 0; j < count; j++) {
            GFL_G3DActorBindAnm(actor, j);
        }
    }
    wk->cameraCurve = GFL_G3DCurveLoadFileStream(wk->heapId, 0xcd, 0x21, 10);
    GFL_G3DCurveApplyCamera(func_ov223_021bc52c(wk->graphic), wk->cameraCurve);
    wk->introFrame = 0;
}


void func_ov223_021b98f4(PselWork *wk) {
    GFL_G3DMgrDeleteScene(wk->g3dMgr, wk->introScene);
    func_ov223_021b9600(wk->objs);
    wk->objCount = 0;
    GFL_G3DCurveFree(wk->cameraCurve);
}

void func_ov223_021b9914(PselWork *wk) {
    u16 i;
    u16 count;
    u16 j;
    G3DActor *actor;
    // The ROM keeps this constant in .rodata though the code has its value, as a function's static is kept (pokebw2's
    // reading of Black 2's); Black has a second copy, func_ov223_021b9a8c's
    static const fx32 speed = PSEL_ANIME_SPEED;

    for (i = 0; i < wk->objCount; i++) {
        actor = GFL_G3DMgrGetActor(wk->g3dMgr, wk->objs[i].actor);
        count = GFL_G3DActorGetAnmCount(actor);
        for (j = 0; j < count; j++) {
            GFL_G3DActorStepAnmFrameLoop(actor, j, speed);
        }
    }
    GFL_G3DCurveFrameStepLoop(wk->cameraCurve, speed);
    GFL_G3DCurveApplyCamera(func_ov223_021bc52c(wk->graphic), wk->cameraCurve);
    wk->introFrame++;
}

void func_ov223_021b99a0(PselWork *wk) {
    u16 firstActor;
    u16 i;
    u16 j;
    u16 count;
    u16 k;
    G3DActor *actor;

    wk->scene = GFL_G3DMgrNewScene(wk->g3dMgr, &data_ov223_021bc7ac[1]);
    wk->objCount = 4;
    wk->objs = func_ov223_021b95b0(wk->heapId, wk->objCount);
    j = 0;
    firstActor = GFL_G3DMgrGetSceneFirstActorIdx(wk->g3dMgr, wk->scene);
    for (i = 0; i < 4; i++) {
        PselObj *obj;

        wk->objIdx[i] = j;
        obj = &wk->objs[j];
        obj->actor = firstActor + i;
        obj->draw = 1;
        if (i + 1 >= 2 && i + 1 <= 4) {
            obj->status.translation = data_ov223_021bc7cc[i - 1];
        }
        j++;
    }
    actor = GFL_G3DMgrGetActor(wk->g3dMgr, wk->objs[wk->objIdx[0]].actor);
    count = GFL_G3DActorGetAnmCount(actor);
    for (k = 0; k < count; k++) {
        GFL_G3DActorBindAnm(actor, k);
    }
    wk->cameraCurve = GFL_G3DCurveLoadFileStream(wk->heapId, 0xcd, 0x24, 10);
    GFL_G3DCurveApplyCamera(func_ov223_021bc52c(wk->graphic), wk->cameraCurve);
    wk->introFrame = 0;
}



void func_ov223_021b9a6c(PselWork *wk) {
    GFL_G3DMgrDeleteScene(wk->g3dMgr, wk->scene);
    func_ov223_021b9600(wk->objs);
    wk->objCount = 0;
    GFL_G3DCurveFree(wk->cameraCurve);
}

void func_ov223_021b9a8c(PselWork *wk) {
    G3DActor *actor;
    u16 count;
    u16 i;
    // The ROM keeps this constant in .rodata too (its second copy): this function's own static
    static const fx32 speed = PSEL_ANIME_SPEED;

    actor = GFL_G3DMgrGetActor(wk->g3dMgr, wk->objs[wk->objIdx[0]].actor);
    count = GFL_G3DActorGetAnmCount(actor);
    for (i = 0; i < count; i++) {
        GFL_G3DActorStepAnmFrameLoop(actor, i, speed);
    }
    GFL_G3DCurveFrameStepLoop(wk->cameraCurve, speed);
    GFL_G3DCurveApplyCamera(func_ov223_021bc52c(wk->graphic), wk->cameraCurve);
    wk->introFrame++;
}

void func_ov223_021b9ae4(PselWork *wk, s32 ball, s32 flag) {
    u16 v;
    f32 pos;

    v = data_ov223_021bc78c[ball].zoom[flag != 0 ? 1 : 0];
    if (v != 0) {
        pos = 0.5f + (f32)(v << 12);
    } else {
        pos = (f32)(v << 12) - 0.5f;
    }
    func_ov223_021b95a0(wk->cameraCurve, (s32)pos);
    GFL_G3DCurveApplyCamera(func_ov223_021bc52c(wk->graphic), wk->cameraCurve);
    wk->cameraBall = ball;
    wk->cameraReverse = flag;
    wk->cameraFrame = v;
}



BOOL func_ov223_021b9b4c(PselWork *wk) {
    s32 index = 0;

    if (wk->cameraReverse == FALSE) {
        index = 1;
    }
    if (wk->cameraFrame == data_ov223_021bc78c[wk->cameraBall].zoom[index]) {
        return TRUE;
    }
    return FALSE;
}

void func_ov223_021b9b80(PselWork *wk) {
    u16 target;
    s32 step;
    s32 index;

    index = 0;
    if (wk->cameraReverse == 0) {
        index = 1;
    }
    target = data_ov223_021bc78c[wk->cameraBall].zoom[index];
    if (wk->cameraFrame == target) {
        return;
    }
    if (wk->cameraReverse != 0) {
        step = -0x800;
    } else {
        step = 0x800;
    }
    GFL_G3DCurveFrameStepLoop(wk->cameraCurve, step);
    GFL_G3DCurveApplyCamera(func_ov223_021bc52c(wk->graphic), wk->cameraCurve);
    if (wk->cameraReverse != 0) {
        wk->cameraFrame--;
    } else {
        wk->cameraFrame++;
    }
    if (wk->cameraFrame == target && wk->cameraReverse != 0) {
        func_ov223_021b95a0(wk->cameraCurve, 0x4e000);
        GFL_G3DCurveApplyCamera(func_ov223_021bc52c(wk->graphic), wk->cameraCurve);
    }
}


void func_ov223_021b9c00(PselWork *wk, s32 ball) {
    u16 v;
    f32 pos;

    v = data_ov223_021bc78c[ball].decideStart;
    if (v != 0) {
        pos = 0.5f + (f32)(v << 12);
    } else {
        pos = (f32)(v << 12) - 0.5f;
    }
    func_ov223_021b95a0(wk->cameraCurve, (s32)pos);
    GFL_G3DCurveApplyCamera(func_ov223_021bc52c(wk->graphic), wk->cameraCurve);
    wk->cameraBall = ball;
    wk->cameraReverse = 0;
    wk->cameraFrame = v;
}


void func_ov223_021b9c58(PselWork *wk) {
    if (wk->cameraFrame != data_ov223_021bc78c[wk->cameraBall].decideEnd) {
        G3DCamera *camera;

        GFL_G3DCurveFrameStepLoop(wk->cameraCurve, 0x800);
        camera = func_ov223_021bc52c(wk->graphic);
        GFL_G3DCurveApplyCamera(camera, wk->cameraCurve);
        wk->cameraFrame++;
    }
}

void func_ov223_021b9c90(PselWork *wk, s32 ball) {
    G3DActor *actor;
    u16 count;
    u16 i;
    fx32 frame;

    if (wk->ball != 4 && wk->ball != ball) {
        actor = GFL_G3DMgrGetActor(wk->g3dMgr, wk->objs[(&wk->introObjIdx)[wk->ball + 2]].actor);
        count = GFL_G3DActorGetAnmCount(actor);
        for (i = 0; i < count; i++) {
            GFL_G3DActorUnbindAnm(actor, i);
        }
    }
    if (ball != 4 && wk->ball != ball) {
        actor = GFL_G3DMgrGetActor(wk->g3dMgr, wk->objs[(&wk->introObjIdx)[ball + 2]].actor);
        count = GFL_G3DActorGetAnmCount(actor);
        frame = 0;
        for (i = 0; i < count; i++) {
            GFL_G3DActorBindAnm(actor, i);
            GFL_G3DActorSetAnmFrame(actor, i, &frame);
        }
        func_0204b908(wk->cursor, 0);
    }
    wk->ball = ball;
}


void func_ov223_021b9d4c(PselWork *wk) {
    G3DActor *actor;
    u16 count;
    u16 i;

    if (wk->ball != PSEL_CHOICE_NONE) {
        actor = GFL_G3DMgrGetActor(wk->g3dMgr, wk->objs[(&wk->introObjIdx)[wk->ball + 2]].actor);
        count = GFL_G3DActorGetAnmCount(actor);
        for (i = 0; i < count; i++) {
            GFL_G3DActorStepAnmFrameLoop(actor, i, 0x800);
        }
    }
}

void func_ov223_021b9d9c(PselWork *wk) {
    ArcTool *pokeArc;
    ArcTool *arc;
    u8 i;
    u16 species;
    ClActorSetup setup;
    ClActorSetupEx setupEx;
    u8 scale;

    pokeArc = MakePokeGraArcHandle(wk->heapId);
    for (i = 0; i < 3; i++) {
        species = data_ov223_021bc710[i];
        wk->poke[i].res[0].chars = func_0202d81c(pokeArc, species, 0, 0, 0, 0, 0, 0, 1, wk->heapId);
        wk->poke[i].res[0].pltt = func_0202d7d8(pokeArc, species, 0, 0, 0, 0, 0, 1, data_ov223_021bc6fc[i] << 5, wk->heapId);
        wk->poke[i].res[0].cellAnim = func_0202d898(species, 0, 0, 0, 0, 0, 0, 1, wk->heapId);
    }
    GFL_ArcToolFree(pokeArc);
    for (i = 0; i < 3; i++) {
        MI_CpuFill8(&setup, 0, 8);
        setup.x = data_ov223_021bc7f0[i].pos[0][0];
        setup.y = data_ov223_021bc7f0[i].pos[0][1];
        setup.sequence = 0;
        setup.priority = 2;
        setup.bgPriority = 3;
        wk->poke[i].clact[0] = func_0204b294(func_ov223_021bc520(wk->graphic), wk->poke[i].res[0].chars,
                                             wk->poke[i].res[0].pltt, wk->poke[i].res[0].cellAnim, &setup, 1,
                                             wk->heapId);
        func_0204b3dc(wk->poke[i].clact[0], FALSE);
    }
    arc = GFL_ArcSysCreateFileHandle(0xcd, wk->heapId);
    for (i = 0; i < 3; i++) {
        wk->poke[i].res[1].chars = func_0204a6c8(arc, data_ov223_021bc73c[i].chars, FALSE, 1, wk->heapId);
        wk->poke[i].res[1].cellAnim = func_0204af28(arc, data_ov223_021bc73c[i].cell, data_ov223_021bc73c[i].anim, wk->heapId);
    }
    GFL_ArcToolFree(arc);
    for (i = 0; i < 3; i++) {
        MI_CpuFill8(&setupEx, 0, 0x18);
        setupEx.base.x = data_ov223_021bc7f0[i].pos[0][0];
        setupEx.base.y = data_ov223_021bc7f0[i].pos[0][1];
        setupEx.base.sequence = 0;
        setupEx.base.priority = 2;
        setupEx.base.bgPriority = 3;
        setupEx.affineCenter.x = 0;
        setupEx.affineCenter.y = 0;
        scale = data_ov223_021bc7f0[i].scale[0];
        setupEx.scaleX = FX32_CONST(scale / 16.0f);
        setupEx.scaleY = FX32_CONST(scale / 16.0f);
        setupEx.rotation = 0;
        setupEx.affineMode = 1;
        wk->poke[i].clact[1] = func_0204b324(func_ov223_021bc520(wk->graphic), wk->poke[i].res[1].chars,
                                             wk->poke[i].res[0].pltt, wk->poke[i].res[1].cellAnim, &setupEx, 1,
                                             wk->heapId);
        func_0204b3dc(wk->poke[i].clact[1], FALSE);
    }
    for (i = 0; i < 3; i++) {
        wk->poke[i].frame = 0;
        wk->poke[i].state = 0;
        wk->poke[i].request = 0;
        wk->poke[i].unmoved = 0;
    }
    wk->fadeCount = 0;
    wk->fadeState = 0;
}


void func_ov223_021ba050(PselWork *wk) {
    u8 i;
    u8 j;

    for (i = 0; i < PSEL_POKE_COUNT; i++) {
        for (j = 0; j < 2; j++) {
            func_0204b3b4(wk->poke[i].clact[j]);
            func_0204afd8(wk->poke[i].res[j].cellAnim);
            func_0204a8d4(wk->poke[i].res[j].chars);
            if (j != 1) {
                func_0204ada4(wk->poke[i].res[j].pltt);
            }
        }
    }
}



















// A frame of a jump from one place and scale to another, along a parabola. Returns the y without the jump's height,
// by which the pictures are sorted
s16 func_ov223_021ba0ac(ClActorPos *pos, ClActorScale *scale, u8 x0, u8 y0, u8 x1, u8 y1, u8 scale0, u8 scale1,
                             u32 duration, u8 height, u32 frame) {
    f32 x = (f32)x0 + (f32)frame * ((f32)x1 - (f32)x0) / (f32)duration;
    f32 y = (f32)y0 + (f32)frame * ((f32)y1 - (f32)y0) / (f32)duration;
    f32 jump = (f32)height + -(f32)height * ((f32)frame - (f32)duration / 2.0f) *
                                 ((f32)frame - (f32)duration / 2.0f) * 4.0f / (f32)duration / (f32)duration;
    f32 size = (f32)scale0 + (f32)frame * ((f32)scale1 - (f32)scale0) / (f32)duration;
    s16 posX = x;
    s16 posY = y - jump;
    fx32 fxSize = FX32_CONST(size / 16.0f);

    pos->x = posX;
    pos->y = posY;
    scale->x = scale->y = fxSize;
    return (s32)y;
}

void func_ov223_021ba2b4(PselWork *wk) {
    s32 moved[3] = { 0, 0, 0 };
    s16 offs[3];
    PselPoke *poke;
    const PselPokeMove *move;
    u8 i;
    u8 j;
    s32 delta;

    for (i = 0; i < 3; i++) {
        poke = &wk->poke[i];
        move = &data_ov223_021bc7f0[i];
        delta = 0;
        if (poke->request == 0) {
            continue;
        }
        switch (poke->request) {
        case 0:
            break;
        case 1:
            delta = 1;
            break;
        case 2:
            delta = -1;
            break;
        }
        if (delta < 0) {
            poke->frame = 0;
            poke->state = 1;
        }
        switch (poke->state) {
        case 0:
            if (delta > 0) {
                func_0204b3dc(poke->clact[0], FALSE);
                func_0204b3dc(poke->clact[1], TRUE);
                poke->frame = 0;
                poke->state = 1;
                poke->unmoved = 0;
            } else {
                if (poke->request == 2) {
                    poke->request = 0;
                }
                break;
            }
            /* fall through */
        case 1:
            if (poke->frame == 0) {
                ClActorPos pos;
                ClActorScale scale;

                pos.x = move->pos[0][0];
                pos.y = move->pos[0][1];
                scale.x = scale.y = FX32_CONST(move->scale[0] / 16.0f);
                func_0204b404(poke->clact[1], &pos, 1);
                func_0204b5ac(poke->clact[1], &scale);
            } else {
                ClActorPos pos;
                ClActorScale scale;

                offs[i] = func_ov223_021ba0ac(&pos, &scale, move->pos[0][0], move->pos[0][1], move->pos[1][0], move->pos[1][1], move->scale[0], move->scale[1], move->duration[0],
                                              move->height[0], poke->frame);
                moved[i] = 1;
                func_0204b404(poke->clact[1], &pos, 1);
                func_0204b5ac(poke->clact[1], &scale);
            }
            if (poke->frame == 0 && delta < 0) {
                func_0204b3dc(poke->clact[0], TRUE);
                func_0204b3dc(poke->clact[1], FALSE);
                poke->frame = 0;
                poke->state = 0;
                poke->unmoved = 0;
            } else {
                poke->frame = poke->frame + delta;
                poke->unmoved = 0;
                if (poke->frame == move->duration[0]) {
                    poke->frame = 0;
                    poke->state = 2;
                }
            }
            break;
        case 2: {
            ClActorPos pos;
            ClActorScale scale;

            pos.x = move->pos[1][0];
            pos.y = move->pos[1][1];
            scale.x = scale.y = FX32_CONST(move->scale[1] / 16.0f);
            func_0204b404(poke->clact[1], &pos, 1);
            func_0204b5ac(poke->clact[1], &scale);
            if (poke->frame == 0 && delta < 0) {
                poke->frame = move->duration[0] - 1;
                poke->state = 1;
                poke->unmoved = 0;
            } else {
                poke->frame = poke->frame + delta;
                poke->unmoved = 0;
                if (poke->frame == move->wait) {
                    poke->frame = 0;
                    poke->state = 3;
                }
            }
            break;
        }
        case 3:
            if (poke->frame == 0) {
                ClActorPos pos;
                ClActorScale scale;

                pos.x = move->pos[1][0];
                pos.y = move->pos[1][1];
                scale.x = scale.y = FX32_CONST(move->scale[1] / 16.0f);
                func_0204b404(poke->clact[1], &pos, 1);
                func_0204b5ac(poke->clact[1], &scale);
            } else {
                ClActorPos pos;
                ClActorScale scale;

                offs[i] = func_ov223_021ba0ac(&pos, &scale, move->pos[1][0], move->pos[1][1], move->pos[2][0], move->pos[2][1], move->scale[1], move->scale[2], move->duration[1],
                                              move->height[1], poke->frame);
                moved[i] = 1;
                func_0204b404(poke->clact[1], &pos, 1);
                func_0204b5ac(poke->clact[1], &scale);
            }
            if (poke->frame == 0 && delta < 0) {
                poke->frame = move->wait - 1;
                poke->state = 2;
                poke->unmoved = 0;
            } else {
                poke->frame = poke->frame + delta;
                poke->unmoved = 0;
                if (poke->frame == move->duration[1]) {
                    poke->frame = 0;
                    poke->state = 4;
                }
            }
            break;
        case 4: {
            ClActorPos pos;
            ClActorScale scale;

            pos.x = move->pos[2][0];
            pos.y = move->pos[2][1];
            scale.x = scale.y = FX32_CONST(move->scale[2] / 16.0f);
            func_0204b404(poke->clact[1], &pos, 1);
            func_0204b5ac(poke->clact[1], &scale);
            if (delta < 0) {
                func_0204b84c(poke->clact[1], 3);
                poke->frame = move->duration[1] - 1;
                poke->state = 3;
                poke->unmoved = 0;
            } else if (poke->request == 1) {
                func_0204b84c(poke->clact[1], 2);
                poke->request = 0;
                if (poke->unmoved == 0) {
                    wk->pokeArrived = TRUE;
                }
            }
            break;
        }
        }
    }
    {
        u8 order[3] = { 0, 1, 2 };
        s16 y[3];
        ClActorPos pos;

        for (i = 0; i < 3; i++) {
            if (moved[i] != 0) {
                y[i] = offs[i];
            } else {
                func_0204b454(wk->poke[i].clact[1], &pos, 1);
                y[i] = pos.y;
            }
        }
        for (i = 0; i < 3; i++) {
            for (j = 2; j > i; j--) {
                u8 b = order[j - 1];
                u8 a = order[j];

                if (y[a] > y[b]) {
                    order[j - 1] = a;
                    order[j] = b;
                }
            }
        }
        {
            u8 first;
            u8 second;
            u8 k;

            second = order[1];
            first = order[0];

            for (k = 0; k < 3; k++) {
                u8 rank;

                if (first == k) {
                    rank = 0;
                } else {
                    rank = 1;
                    if (second != k) {
                        rank = 2;
                    }
                }
                func_0204b800(wk->poke[k].clact[1], rank);
            }
        }
    }
}


void func_ov223_021ba81c(PselWork *wk, u32 choice) {
    u8 i;

    for (i = 0; i < 3; i++) {
        if (i == choice) {
            func_ov223_021bb974(wk, i, 0);
        } else {
            func_ov223_021bb974(wk, i, 2);
        }
        if (i == choice) {
            wk->poke[i].request = 1;
            wk->poke[i].unmoved = 1;
        } else {
            wk->poke[i].request = 2;
            wk->poke[i].unmoved = 1;
        }
    }
}


void func_ov223_021ba878(PselWork *wk) {
    u8 i;

    for (i = 0; i < PSEL_POKE_COUNT; i++) {
        func_0204b688(wk->poke[i].clact[0], 1);
    }
    wk->fadeCount = 0;
    wk->fadeState = 1;
    G2S_SetBlendAlpha(wk->fadeCount, 0x3f, 0, 16 - wk->fadeCount);
}


BOOL func_ov223_021ba8bc(PselWork *wk) {
    return wk->fadeState == 0;
}

void func_ov223_021ba8d0(PselWork *wk) {
    u8 i;

    switch (wk->fadeState) {
    case 1:
        for (i = 0; i < 3; i++) {
            func_0204b3dc(wk->poke[i].clact[0], 1);
        }
        wk->fadeState = 2;
        break;
    case 2:
        if (wk->fadeCount < 0x10) {
            wk->fadeCount++;
            if (wk->fadeCount == 0x10) {
                for (i = 0; i < 3; i++) {
                    func_0204b688(wk->poke[i].clact[0], 0);
                }
                wk->fadeState = 3;
            }
            G2S_SetBlendAlpha(0, 0x3f, wk->fadeCount, 0x10 - wk->fadeCount);
        } else {
            wk->fadeState = 3;
        }
        break;
    case 3:
        G2S_SetBlendAlpha(4, 0x3f, 8, 8);
        wk->fadeState = 0;
        break;
    }
}


void func_ov223_021ba984(PselWork *wk) {
    ArcTool *arc;
    ClActorSetup setup;
    ClActUnit *unit;

    arc = GFL_ArcSysCreateFileHandle(0xcd, wk->heapId);
    wk->cursorPltt = func_0204ac18(arc, 0xb, 0, 0, 0, 1, wk->heapId);
    wk->cursorChars = func_0204a6c8(arc, 0x11, 0, 0, wk->heapId);
    wk->cursorCellAnim = func_0204af28(arc, 0x18, 0x1c, wk->heapId);
    GFL_ArcToolFree(arc);
    MI_CpuFill8(&setup, 0, 8);
    setup.x = 0x42;
    setup.y = 0x27;
    setup.sequence = 0;
    setup.priority = 0;
    setup.bgPriority = 2;
    unit = func_ov223_021bc520(wk->graphic);
    wk->cursor = func_0204b294(unit, wk->cursorChars, wk->cursorPltt, wk->cursorCellAnim, &setup, 0, wk->heapId);
    func_0204b92c(wk->cursor, 1);
    func_ov223_021baa5c(wk, 0);
}


void func_ov223_021baa30(PselWork *wk) {
    func_0204b3b4(wk->cursor);
    func_0204afd8(wk->cursorCellAnim);
    func_0204a8d4(wk->cursorChars);
    func_0204ada4(wk->cursorPltt);
}

void func_ov223_021baa5c(PselWork *wk, BOOL flag) {
    if (flag) {
        func_ov223_021baa84(wk);
    } else {
        ClActorPos pos;

        pos.x = 0x190;
        pos.y = 0x12c;
        func_0204b404(wk->cursor, &pos, 0);
    }
}


void func_ov223_021baa84(PselWork *wk) {
    ClActorPos pos;

    pos.x = data_ov223_021bc716[wk->choice].x;
    pos.y = data_ov223_021bc716[wk->choice].y;
    func_0204b404(wk->cursor, &pos, 0);
}

void func_ov223_021baab8(PselWork *wk) {
    u16 *palette;
    GFLBitmap *bitmap;

    palette = GFL_HeapAllocate(wk->heapId, 0x20, TRUE, "psel.c", 0x92b);
    palette[0] = 0;
    palette[1] = 0;
    palette[2] = 0x7fff;
    GFL_BGSysUploadStdPalette(2, palette, 0x20, 0x1a0);
    GFL_BGSysUploadStdPalette(6, palette, 0x20, 0);
    GFL_HeapFree(palette);
    bitmap = GFL_BitmapCreate(2, 1, 0x20, wk->heapId);
    GFL_BitmapFillArea(bitmap, 0, 0, 8, 8, 0);
    GFL_BitmapFillArea(bitmap, 8, 0, 8, 8, 1);
    GFL_BGSysLoadChar(2, GFL_BitmapGetPixelData(bitmap), GFL_BitmapCalcPixelDataSize(bitmap), 0);
    GFL_BGSysLoadChar(6, GFL_BitmapGetPixelData(bitmap), GFL_BitmapCalcPixelDataSize(bitmap), 0);
    GFL_BitmapFree(bitmap);
}





void func_ov223_021bab74(PselWork *wk) {
}

void func_ov223_021bab78(PselWork *wk) {
    u16 *scr;
    u8 col;
    u8 row;
    u16 n;
    u16 v;

    scr = GFL_HeapAllocate(wk->heapId, 0x600, TRUE, "psel.c", 0x951);
    n = 0;
    for (row = 0; row < 0x18; row++) {
        for (col = 0; col < 0x20; col++) {
            v = row >= 4 && row < 0x14;
            scr[n] = v | 0xd000;
            n++;
        }
    }
    GFL_BGSysLoadScrAreaAll(2, scr, 0, 0, 0x20, 0x18);
    GFL_BGSysQueueScrLoad(2);
    GFL_HeapFree(scr);
}


void func_ov223_021babfc(PselWork *wk) {
    GFL_BGSysFillScrAsync(2, 0xd000);
}

void func_ov223_021bac0c(PselWork *wk) {
    ArcTool *arc;

    arc = GFL_ArcSysCreateFileHandle(0xcd, wk->heapId);
    GFL_G2DIOLoadArcNCLR(arc, 6, 4, 0x100, 0x100, 0x100, wk->heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 0xf, 4, 0, 0, 0, wk->heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 0x12, 4, 0, 0, 0, wk->heapId);
    GFL_ArcToolFree(arc);
    GFL_BGSysLoadScr(4);
}



void func_ov223_021bac6c(PselWork *wk) {
}

void func_ov223_021bac70(PselWork *wk) {
    u16 *scr;
    u8 col;
    u8 j;
    u8 row;
    u8 row2;
    u16 n;
    u16 v;


    scr = GFL_HeapAllocate(wk->heapId, 0x600, TRUE, "psel.c", 0x99f);
    n = 0;
    for (row = 0; row < 0x18; row++) {
        for (col = 0; col < 0x20; col++) {
            v = row >= 0x11 && row < 0x17;
            scr[n] = v | 0xd000;
            n++;
        }
    }
    GFL_BGSysLoadScrAreaAll(2, scr, 0, 0, 0x20, 0x18);
    GFL_BGSysQueueScrLoad(2);
    GFL_HeapFree(scr);
    scr = GFL_HeapAllocate(wk->heapId, 0x600, TRUE, "psel.c", 0x9b8);
    n = 0;
    for (row2 = 0; row2 < 0x18; row2++) {
        for (j = 0; j < 0x20; j++) {
            if (row2 >= 0x11 && row2 < 0x17) {
                v = 1;
            } else {
                v = 0;
            }
            scr[n] = v;
            n++;
        }
    }
    GFL_BGSysLoadScrAreaAll(6, scr, 0, 0, 0x20, 0x18);
    GFL_BGSysQueueScrLoad(6);
    GFL_HeapFree(scr);
}


void func_ov223_021bad68(PselWork *wk) {
    GFL_BGSysFillScrAsync(2, 0xd000);
    GFL_BGSysFillScrAsync(6, 0);
}

void func_ov223_021bad80(PselWork *wk) {
    ArcTool *arc;

    arc = GFL_ArcSysCreateFileHandle(0xcd, wk->heapId);
    GFL_G2DIOLoadArcNCLR(arc, 7, 4, 0x100, 0x100, 0x100, wk->heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 0x10, 4, 0, 0, 0, wk->heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 0x13, 4, 0, 0, 0, wk->heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 0x14, 5, 0, 0, 0, wk->heapId);
    GFL_ArcToolFree(arc);
    GFL_BGSysLoadScr(4);
    GFL_BGSysLoadScr(5);
}



void func_ov223_021badfc(PselWork *wk) {
}

void func_ov223_021bae00(PselWork *wk) {
    u8 i;
    GFLBitmap *bitmap;

    GFL_BGSysLoadNCLRDefault(0x17, 5, 0, 0x1c0, 0x20, wk->heapId);
    GFL_BGSysLoadNCLRDefault(0x17, 5, 4, 0x20, 0x20, wk->heapId);
    wk->msgData = GFL_MsgSysLoadData(0, 3, 0x1ae, wk->heapId);
    wk->tcbMgr = GFL_TCBExMgrCreate(wk->heapId, wk->heapId, 1, 0);
    GFL_TextRndUpdateColorIndexLUT(0xf, 2, 0);
    for (i = 0; i < 3; i++) {
        wk->win[i] = BmpWin_CreateDynamic(data_ov223_021bc770[i].bg, data_ov223_021bc770[i].x,
            data_ov223_021bc770[i].y, data_ov223_021bc770[i].width, data_ov223_021bc770[i].height,
            data_ov223_021bc770[i].palette, data_ov223_021bc770[i].fromEnd);
        bitmap = BmpWin_GetBitmap(wk->win[i]);
        GFL_BitmapFill(bitmap, 0);
        BmpWin_FlushChar(wk->win[i]);
        wk->winDone[i] = 1;
    }
    wk->stream = NULL;
    wk->strbuf = NULL;
}


void func_ov223_021baee0(PselWork *wk) {
    u8 i;

    if (wk->stream != NULL) {
        func_0201ce48(wk->stream);
    }
    if (wk->strbuf != NULL) {
        GFL_StrBufFree(wk->strbuf);
    }
    for (i = 0; i < 3; i++) {
        BmpWin_Free(wk->win[i]);
    }
    GFL_TCBExMgrFree(wk->tcbMgr);
    GFL_MsgDataFree(wk->msgData);
}


void func_ov223_021baf30(PselWork *wk) {
    GFL_TCBExMgrUpdate(wk->tcbMgr);
    func_ov223_021baf48(wk);
}

u32 func_ov223_021baf48(PselWork *wk) {
    GFLBitmap *bitmap;
    BmpWin *win;
    u32 ret;
    u8 i;

    ret = 1;
    for (i = 0; i < 3; i++) {
        if (wk->winDone[i] == 0) {
            bitmap = BmpWin_GetBitmap(wk->win[i]);
            if (func_0201c6c4(wk->printQueue, bitmap) != 0) {
                ret = 0;
            } else {
                win = wk->win[i];
                BmpWin_FlushChar(win);
                BmpWin_FlushMap(win);
                GFL_BGSysQueueScrLoad(BmpWin_GetBGIndex(win));
                wk->winDone[i] = 1;
            }
        }
    }
    return ret;
}

void func_ov223_021bafc4(PselWork *wk, u32 messageId) {
    BmpWin *window;
    s16 x;
    s16 y;
    u16 width;
    u16 height;
    u16 blockWidth;
    u16 blockHeight;
    s32 wait;

    window = wk->win[1];
    x = 0;
    y = 0;
    GFL_BitmapFill(BmpWin_GetBitmap(window), 0);
    if (wk->stream != NULL) {
        func_0201ce48(wk->stream);
    }
    if (wk->strbuf != NULL) {
        GFL_StrBufFree(wk->strbuf);
    }
    wk->strbuf = GFL_MsgDataLoadStrbufNew(wk->msgData, messageId);
    blockWidth = GFL_FontGetBlockWidth(wk->strbuf, wk->font, 0);
    blockHeight = GFL_FontGetBlockHeight(wk->strbuf, wk->font);
    width = GFL_BitmapGetWidth(BmpWin_GetBitmap(window));
    height = GFL_BitmapGetHeight(BmpWin_GetBitmap(window));
    if (width > blockWidth) {
        x = (width - blockWidth) / 2;
    }
    if (height > blockHeight) {
        y = (height - blockHeight) / 2;
    }
    wait = func_02012ffc();
    wk->stream = func_0201cce4(window, x, y, wk->strbuf, wk->font, wait, wk->tcbMgr, 2, wk->heapId, 0);
    BmpWin_FlushChar(window);
    BmpWin_FlushMap(window);
    GFL_BGSysQueueScrLoad(BmpWin_GetBGIndex(window));
}


u32 func_ov223_021bb0b0(PselWork *wk) {
    u32 ret;

    ret = 0;
    switch (func_0201ce30(wk->stream)) {
    case 0:
        if ((GCTX_HIDGetHeldKeys() & 3) == 0 && func_02035c38() == 0) {
            break;
        }
        func_0201ce5c(wk->stream, 0);
        if ((GCTX_HIDGetHeldKeys() & 3) != 0) {
            wk->touchMode = 0;
        } else {
            wk->touchMode = 1;
        }
        break;
    case 1:
        if ((GCTX_HIDGetPressedKeys() & 3) == 0 && func_02035c60() == 0) {
            break;
        }
        func_0201ce38(wk->stream);
        if ((GCTX_HIDGetPressedKeys() & 3) != 0) {
            wk->touchMode = 0;
        } else {
            wk->touchMode = 1;
        }
        break;
    case 2:
        ret = 1;
        break;
    default:
        break;
    }
    return ret;
}



void func_ov223_021bb134(PselWork *wk) {
    GFLBitmap *bitmap;
    BmpWin *win;
    u8 bg;

    bitmap = BmpWin_GetBitmap(wk->win[1]);
    GFL_BitmapFill(bitmap, 0);
    win = wk->win[1];
    BmpWin_FlushChar(win);
    BmpWin_FlushMap(win);
    bg = BmpWin_GetBGIndex(win);
    GFL_BGSysQueueScrLoad(bg);
    if (wk->stream != NULL) {
        func_0201ce48(wk->stream);
    }
    wk->stream = NULL;
    if (wk->strbuf != NULL) {
        GFL_StrBufFree(wk->strbuf);
    }
    wk->strbuf = NULL;
}

void func_ov223_021bb18c(PselWork *wk, u32 id) {
    BmpWin *window;
    WordSet *wordSet;
    StrBuf *src;
    StrBuf *dest;
    u16 blockHeight;
    u16 height;
    u16 y;
    BOOL *done;

    window = wk->win[2];
    done = &wk->winDone[2];
    y = 0;
    wordSet = GFL_WordSetSystemCreateDefault(wk->heapId);
    src = GFL_MsgDataLoadStrbufNew(wk->msgData, data_ov223_021bc730[id]);
    dest = GFL_StrBufCreate(0x40, wk->heapId);
    WordSet_LoadSpeciesName(wordSet, 1, data_ov223_021bc710[id]);
    GFL_WordSetFormatStrbuf(wordSet, dest, src);
    GFL_StrBufFree(src);
    GFL_WordSetSystemFree(wordSet);
    GFL_BitmapFill(BmpWin_GetBitmap(window), 0);
    GFL_FontGetBlockWidth(dest, wk->font, 0);
    blockHeight = GFL_FontGetBlockHeight(dest, wk->font);
    GFL_BitmapGetWidth(BmpWin_GetBitmap(window));
    height = GFL_BitmapGetHeight(BmpWin_GetBitmap(window));
    if (height > blockHeight) {
        y = (height - blockHeight) / 2;
    }
    func_0201c724(wk->printQueue, BmpWin_GetBitmap(window), 0, y, dest, wk->font, 0x3c40);
    GFL_StrBufFree(dest);
    *done = FALSE;
    func_ov223_021baf48(wk);
}



void func_ov223_021bb274(PselWork *wk) {
    GFLBitmap *bitmap = BmpWin_GetBitmap(wk->win[2]);
    GFL_BitmapFill(bitmap, 0);
    BmpWin *win = wk->win[2];
    BmpWin_FlushChar(win);
    BmpWin_FlushMap(win);
    GFL_BGSysQueueScrLoad(BmpWin_GetBGIndex(win));
}

void func_ov223_021bb2a4(PselWork *wk) {
    StrBuf *strbuf;
    u16 blockWidth;
    u16 blockHeight;
    u16 width;
    u16 height;
    u16 x;
    u16 y;

    strbuf = GFL_MsgDataLoadStrbufNew(wk->msgData, 0xc);
    blockWidth = GFL_FontGetBlockWidth(strbuf, wk->font, 0);
    blockHeight = GFL_FontGetBlockHeight(strbuf, wk->font);
    width = GFL_BitmapGetWidth(BmpWin_GetBitmap(wk->win[0]));
    height = GFL_BitmapGetHeight(BmpWin_GetBitmap(wk->win[0]));
    x = 0;
    y = 0;
    if (width > blockWidth) {
        x = (width - blockWidth) / 2;
    }
    if (height > blockHeight) {
        y = (height - blockHeight) / 2;
    }
    func_0201c724(wk->printQueue, BmpWin_GetBitmap(wk->win[0]), x, y, strbuf, wk->font, 0x3c40);
    GFL_StrBufFree(strbuf);
    wk->winDone[0] = FALSE;
    func_ov223_021baf48(wk);
}


void func_ov223_021bb35c(PselWork *wk) {
    GFLBitmap *bitmap = BmpWin_GetBitmap(wk->win[0]);
    GFL_BitmapFill(bitmap, 0);
    BmpWin *win = wk->win[0];
    BmpWin_FlushChar(win);
    BmpWin_FlushMap(win);
    GFL_BGSysQueueScrLoad(BmpWin_GetBGIndex(win));
}




void func_ov223_021bb38c(PselWork *wk) {
    wk->menuRes = func_02027f60(3, 11, wk->font, wk->printQueue, wk->heapId);
}


void func_ov223_021bb3ac(PselWork *wk) {
    func_02027fd8(wk->menuRes);
}

// The button that confirms a ball picked by touch
void func_ov223_021bb3bc(PselWork *wk) {
    AppTaskMenuItem item;

    item.str = GFL_MsgDataLoadStrbufNew(wk->msgData, AspertiaCity_Text_021);
    item.color = PRINT_COLOR(14, 15, 3);
    item.type = 0;
    wk->button = func_02027fec(wk->menuRes, &item, 9, 21, 14, wk->heapId);
    GFL_StrBufFree(item.str);
}


void func_ov223_021bb404(PselWork *wk) {
    func_02028148(wk->button);
}

void func_ov223_021bb414(PselWork *wk) {
    func_02028178(wk->button);
}

void func_ov223_021bb424(PselWork *wk) {
    func_02028218(wk->button, TRUE);
    func_0202822c(wk->button, TRUE);
}


BOOL func_ov223_021bb440(PselWork *wk) {
    return func_02028230(wk->button);
}

void func_ov223_021bb450(PselWork *wk) {
    AppTaskMenuInit init;
    AppTaskMenuItem items[2];
    u32 i;

    for (i = 0; i < NELEMS(items); i++) {
        items[i].str = GFL_MsgDataLoadStrbufNew(wk->msgData, AspertiaCity_Text_022 + i);
        items[i].color = PRINT_COLOR(14, 15, 3);
        u32 type = 0;

        if (i != 0) {
            type = APP_TASKMENU_ITEM_RETURN;
        }
        items[i].type = type;
    }
    init.heapId = wk->heapId;
    init.itemCount = NELEMS(items);
    init.items = items;
    init.posType = APP_TASKMENU_POS_BOTTOM_RIGHT;
    init.x = 32;
    init.y = 17;
    init.width = 9;
    init.height = 3;
    wk->yesNo = func_02027778(&init, wk->menuRes);
    for (i = 0; i < NELEMS(items); i++) {
        GFL_StrBufFree(items[i].str);
    }
}

void func_ov223_021bb4d8(PselWork *wk) {
    func_02027858(wk->yesNo);
}

void func_ov223_021bb4e8(PselWork *wk) {
    func_02027974(wk->yesNo);
}

BOOL func_ov223_021bb4f8(PselWork *wk) {
    return func_020279e8(wk->yesNo);
}

BOOL func_ov223_021bb508(PselWork *wk) {
    if (func_02027a04(wk->yesNo) == 0) {
        return TRUE;
    }
    return FALSE;
}




















// The palette and the two characters of the black bands on BG1 and on the sub screen's BG2


// A black band over rows 17 to 22 of both screens, behind the text








// pokebw2's Psel_SplitColor
void func_ov223_021bb520(u16 color, u16 *r, u16 *g, u16 *b) {
    *r = color & 0x1f;
    *g = (color & 0x3e0) >> 5;
    *b = (color & 0x7c00) >> 10;
}

void func_ov223_021bb540(PselWork *wk) {
    ArcTool *arc;
    NNSG2dPaletteData *palette;
    void *data;
    u8 i;

    arc = GFL_ArcSysCreateFileHandle(0xcd, wk->heapId);
    for (i = 0; i < 3; i++) {
        data = GFL_G2DIOReadNCLRArc(arc, data_ov223_021bc73c[i].pltt, &palette, wk->heapId);
        MI_CpuCopy8(palette->pRawData, wk->poke[i].palNormal, 0x20);
        GFL_HeapFree(data);
        data = GFL_G2DIOReadNCLRArc(arc, data_ov223_021bc73c[i].plttDark, &palette, wk->heapId);
        MI_CpuCopy8(palette->pRawData, wk->poke[i].palDark, 0x20);
        GFL_HeapFree(data);
    }
    GFL_ArcToolFree(arc);
    for (i = 0; i < 3; i++) {
        wk->poke[i].palState = 2;
        wk->poke[i].palStep = 0x10;
    }
    for (i = 0; i < 3; i++) {
        MI_CpuCopy8(wk->poke[i].palDark, wk->poke[i].palNow, 0x20);
        NNS_GfdRegisterNewVramTransferTask(0x1e, data_ov223_021bc6fc[i] << 5, wk->poke[i].palNow, 0x20);
    }
}


void func_ov223_021bb62c(PselWork *wk) {
}

void func_ov223_021bb630(PselWork *wk) {
    u8 i;
    u8 j;
    BOOL changed;
    u16 fromR;
    u16 fromG;
    u16 fromB;
    u16 toR;
    u16 toG;
    u16 toB;
    u16 r;
    u16 g;
    u16 b;
    u16 step;
    u16 to;

    for (i = 0; i < 3; i++) {
        changed = FALSE;
        switch (wk->poke[i].palState) {
        case 1:
            if (wk->poke[i].palStep == 0x10) {
                wk->poke[i].palState = 2;
            } else {
                wk->poke[i].palStep++;
                wk->poke[i].palStep = 0x10;
                changed = TRUE;
            }
            break;
        case 3:
            if (wk->poke[i].palStep == 0) {
                wk->poke[i].palState = 0;
            } else {
                wk->poke[i].palStep--;
                changed = TRUE;
            }
            break;
        }
        if (changed) {
            if (wk->poke[i].palState == 1 || wk->poke[i].palState == 3) {
                for (j = 0; j < 0x10; j++) {
                    step = wk->poke[i].palStep;
                    to = wk->poke[i].palDark[j];
                    func_ov223_021bb520(wk->poke[i].palNormal[j], &fromR, &fromG, &fromB);
                    func_ov223_021bb520(to, &toR, &toG, &toB);
                    r = (fromR * (0x10 - step) + toR * step) / 0x10;
                    g = (fromG * (0x10 - step) + toG * step) / 0x10;
                    b = (fromB * (0x10 - step) + toB * step) / 0x10;
                    wk->poke[i].palNow[j] = r | (g << 5) | (b << 10);
                }
            }
            NNS_GfdRegisterNewVramTransferTask(0x1e, data_ov223_021bc6fc[i] << 5, wk->poke[i].palNow, 0x20);
        }
    }
}


void func_ov223_021bb7a4(PselWork *wk) {
    ArcTool *arc;
    NNSG2dPaletteData *palette;
    void *data;
    u8 i;

    arc = GFL_ArcSysCreateFileHandle(0xcd, wk->heapId);
    for (i = 0; i < 4; i++) {
        data = GFL_G2DIOReadNCLRArc(arc, data_ov223_021bc708[i], &palette, wk->heapId);
        MI_CpuCopy8(palette->pRawData + 0x100, wk->bgPal[i], 0x100);
        GFL_HeapFree(data);
    }
    GFL_ArcToolFree(arc);
    wk->bgPalActive = 0;
    wk->bgPalStep = 0x10;
    wk->bgPalTarget = 3;
    MI_CpuCopy8(wk->bgPal[3], wk->bgPalNow, 0x100);
    NNS_GfdRegisterNewVramTransferTask(0x1f, 0x100, wk->bgPalNow, 0x100);
}


void func_ov223_021bb840(PselWork *wk) {
}

void func_ov223_021bb844(PselWork *wk) {
    u8 i;
    u16 fromR;
    u16 fromG;
    u16 fromB;
    u16 toR;
    u16 toG;
    u16 toB;
    u16 r;
    u16 g;
    u16 b;
    u8 step;
    u16 to;
    BOOL update = FALSE;

    if (wk->bgPalActive == 1) {
        if (wk->bgPalStep == 0x10) {
            wk->bgPalActive = 0;
        } else {
            wk->bgPalStep++;
            update = TRUE;
        }
        if (update) {
            for (i = 0; i < 0x80; i++) {
                step = wk->bgPalStep;
                to = wk->bgPal[wk->bgPalTarget][i];
                func_ov223_021bb520(wk->bgPalFrom[i], &fromR, &fromG, &fromB);
                func_ov223_021bb520(to, &toR, &toG, &toB);
                r = (fromR * (0x10 - step) + toR * step) / 0x10;
                g = (fromG * (0x10 - step) + toG * step) / 0x10;
                b = (fromB * (0x10 - step) + toB * step) / 0x10;
                wk->bgPalNow[i] = r | (g << 5) | (b << 10);
            }
            NNS_GfdRegisterNewVramTransferTask(0x1f, 0x100, wk->bgPalNow, 0x100);
        }
    }
}


void func_ov223_021bb944(PselWork *wk) {
    func_ov223_021bb540(wk);
    func_ov223_021bb7a4(wk);
}

void func_ov223_021bb954(PselWork *wk) {
    func_ov223_021bb62c(wk);
    func_ov223_021bb840(wk);
}

void func_ov223_021bb964(PselWork *wk) {
    func_ov223_021bb630(wk);
    func_ov223_021bb844(wk);
}

void func_ov223_021bb974(PselWork *wk, s32 ball, s32 kind) {
    switch (kind) {
    case 0:
        if (wk->poke[ball].palState != 0) {
            wk->poke[ball].palState = 3;
        }
        break;
    case 2:
        if (wk->poke[ball].palState != 2) {
            wk->poke[ball].palState = 1;
        }
        break;
    }
}

void func_ov223_021bb9a4(PselWork *wk, u8 target) {
    if (wk->bgPalTarget != target) {
        wk->bgPalActive = TRUE;
        wk->bgPalStep = 0;
        wk->bgPalTarget = target;
        MI_CpuCopy8(wk->bgPalNow, wk->bgPalFrom, 0x100);
    }
}






s32 func_ov223_021bb9dc(PselWork *wk, s32 *step) {
    return TRUE;
}

s32 func_ov223_021bb9e0(PselWork *wk, s32 *step) {
    wk->graphic = func_ov223_021bc3f0(1, wk->heapId);
    wk->font = GFL_FontCreate(0x17, 0, 0, FALSE, wk->heapId);
    wk->printQueue = func_0201c440(wk->heapId);
    GFL_BGSysSetBGPriority(0, 2);
    GFL_BGSysSetBGPriority(1, 3);
    GFL_BGSysSetBGPriority(2, 1);
    GFL_BGSysSetBGPriority(3, 0);
    GFL_BGSysSetBGPriority(4, 3);
    GFL_BGSysSetBGPriority(5, 2);
    GFL_BGSysSetBGPriority(6, 1);
    GFL_BGSysSetBGPriority(7, 0);
    wk->vblankTask = GFL_VBlankTCBAdd(func_ov223_021b9740, wk, 1);
    wk->bgRequest = 0;
    func_ov223_021b9830(wk);
    func_ov223_021baab8(wk);
    func_ov223_021bae00(wk);
    func_ov223_021bb38c(wk);
    return 2;
}

s32 func_ov223_021bba78(PselWork *wk, s32 *step) {
    GFL_BGSysResetStdPalette(0, 0x2f3b);
    GFL_BGSysResetStdPalette(4, 0x2f3b);
    func_ov223_021b9854(wk);
    func_ov223_021bab78(wk);
    func_ov223_021bac0c(wk);
    func_ov223_021bb2a4(wk);
    wk->bgRequest |= PSEL_BG_REQ_BG3_OFF;
    wk->bgAlpha = 0;
    G2_SetBlendAlpha(5, 0x3f, wk->bgAlpha, 16 - wk->bgAlpha);
    wk->frame = 0;
    return 3;
}

s32 func_ov223_021bbadc(PselWork *wk, s32 *step) {
    if (wk->frame >= 1) {
        func_ov223_021b9914(wk);
    }
    switch (*step) {
    case 0:
        GFL_FadeSet(3, 16, 0, 0);
        (*step)++;
        break;
    case 1:
        if (!GFL_FadeIsRunning()) {
            wk->frame = 0;
            return 4;
        }
        break;
    }
    wk->frame++;
    return 3;
}

s32 func_ov223_021bbb28(PselWork *wk, s32 *step) {
    BOOL blend = FALSE;

    switch (*step) {
    case 0:
        func_ov223_021b9914(wk);
        if (wk->introFrame == 0x86) {
            (*step)++;
        }
        break;
    case 1:
        if (wk->bgAlpha < 8) {
            wk->bgAlpha++;
            blend = TRUE;
        }
        func_ov223_021b9914(wk);
        if (wk->introFrame == 0x8a) {
            (*step)++;
        }
        break;
    case 2:
        if (wk->bgAlpha < 8) {
            wk->bgAlpha++;
            blend = TRUE;
        } else {
            wk->bgRequest |= PSEL_BG_REQ_BG3_ON;
            (*step)++;
        }
        break;
    case 3:
        (*step)++;
        break;
    case 4:
        if ((GCTX_HIDGetPressedKeys() & 3) || func_02035c60()) {
            if (GCTX_HIDGetPressedKeys() & 3) {
                wk->touchMode = FALSE;
            } else {
                wk->touchMode = TRUE;
            }
            GFL_SndSEPlay(0x54c);
            (*step)++;
        }
        break;
    case 5:
        wk->bgRequest |= PSEL_BG_REQ_BG3_OFF;
        if (wk->bgAlpha != 0) {
            wk->bgAlpha--;
            blend = TRUE;
        }
        (*step)++;
        break;
    case 6:
        if (wk->bgAlpha != 0) {
            wk->bgAlpha--;
            blend = TRUE;
        }
        if (wk->bgAlpha == 4) {
            (*step)++;
        }
        break;
    case 7:
        if (wk->bgAlpha != 0) {
            wk->bgAlpha--;
            blend = TRUE;
        }
        func_ov223_021b9914(wk);
        if (wk->introFrame == 0xb4) {
            GFL_SndSEPlay(0x7a4);
        }
        if (wk->introFrame == 0xc7) {
            wk->frame = 0;
            return 5;
        }
        break;
    }
    if (blend) {
        G2_SetBlendAlpha(5, 0x3f, wk->bgAlpha, 16 - wk->bgAlpha);
    }
    wk->frame++;
    return 4;
}

s32 func_ov223_021bbc6c(PselWork *wk, s32 *step) {
    if (wk->introFrame != 0x102) {
        func_ov223_021b9914(wk);
    }
    switch (*step) {
    case 0:
        GFL_FadeSet(12, 0, 16, 3);
        (*step)++;
        break;
    case 1:
        if (!GFL_FadeIsRunning()) {
            wk->frame = 0;
            return 6;
        }
        break;
    }
    wk->frame++;
    return 5;
}

s32 func_ov223_021bbcbc(PselWork *wk, s32 *step) {
    G2_BlendNone();
    G2S_BlendNone();
    func_ov223_021bb35c(wk);
    func_ov223_021bac6c(wk);
    func_ov223_021babfc(wk);
    func_ov223_021b98f4(wk);
    wk->frame = 0;
    return 7;
}

s32 func_ov223_021bbcf0(PselWork *wk, s32 *step) {
    GFL_BGSysSetDisplayLayout(0);
    GFL_BGSysResetStdPalette(0, 0x2f3b);
    GFL_BGSysResetStdPalette(4, 0x2f3b);
    func_ov223_021b99a0(wk);
    func_ov223_021b9d9c(wk);
    func_ov223_021ba984(wk);
    func_ov223_021bac70(wk);
    func_ov223_021bad80(wk);
    func_ov223_021bb944(wk);
    wk->bgRequest |= PSEL_BG_REQ_MAIN_OFF;
    wk->bgRequest |= PSEL_BG_REQ_SUB_OFF;
    G2_SetBlendAlpha(5, 0x3f, 8, 8);
    G2S_SetBlendAlpha(4, 0x3f, 8, 8);
    wk->frame = 0;
    return 8;
}

s32 func_ov223_021bbd74(PselWork *wk, s32 *step) {
    if (wk->frame >= 16) {
        func_ov223_021b9a8c(wk);
    }
    if (wk->frame == 15) {
        GFL_SndSEPlay(0x7a5);
    }
    func_ov223_021b9d4c(wk);
    func_ov223_021ba2b4(wk);
    func_ov223_021bb964(wk);
    switch (*step) {
    case 0:
        GFL_FadeSet(12, 16, 0, 2);
        (*step)++;
        break;
    case 1:
        if (!GFL_FadeIsRunning()) {
            wk->frame = 0;
            return 9;
        }
        break;
    }
    wk->frame++;
    return 8;
}

s32 func_ov223_021bbde0(PselWork *wk, s32 *step) {
    BOOL changed = FALSE;

    func_ov223_021b9d4c(wk);
    func_ov223_021ba2b4(wk);
    func_ov223_021ba8d0(wk);
    func_ov223_021bb964(wk);
    switch (*step) {
    case 0:
        func_ov223_021b9a8c(wk);
        if (wk->introFrame == 0x4e) {
            *step = 1;
        }
        break;
    case 1:
        wk->bgRequest |= PSEL_BG_REQ_MAIN_ON;
        func_ov223_021bafc4(wk, AspertiaCity_Text_019);
        *step = 2;
        func_ov223_021ba878(wk);
        break;
    case 2:
        if (func_ov223_021bb0b0(wk) && func_ov223_021ba8bc(wk)) {
            *step = 3;
            wk->touchMode = TRUE;
        }
        break;
    case 3:
        if (!wk->touchMode) {
            if (wk->choice == PSEL_CHOICE_NONE) {
                changed = TRUE;
                wk->choice = 1;
            }
            func_ov223_021baa5c(wk, TRUE);
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            func_ov223_021baa84(wk);
            func_ov223_021b9c90(wk, wk->choice);
            func_ov223_021ba81c(wk, wk->choice);
            func_ov223_021bb9a4(wk, PSEL_BG_PAL_OF(wk->choice));
        } else {
            func_ov223_021baa5c(wk, FALSE);
            func_ov223_021b9c90(wk, PSEL_CHOICE_NONE);
            func_ov223_021ba81c(wk, PSEL_CHOICE_NONE);
            func_ov223_021bb9a4(wk, 3);
            changed = TRUE;
        }
        *step = 4;
        break;
    case 4:
        if (!wk->touchMode) {
            if (GCTX_HIDGetPressedKeys()) {
                BOOL moved = FALSE;

                if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_A) {
                    GFL_SndSEPlay(SEQ_SE_DECIDE1);
                    func_ov223_021baa5c(wk, FALSE);
                    *step = 8;
                } else if (GCTX_HIDGetPressedKeys() & PAD_KEY_LEFT) {
                    if (wk->choice > 0) {
                        wk->choice--;
                        moved = TRUE;
                    }
                } else if (GCTX_HIDGetPressedKeys() & PAD_KEY_RIGHT) {
                    if (wk->choice < 2) {
                        wk->choice++;
                        moved = TRUE;
                    }
                }
                if (moved) {
                    GFL_SndSEPlay(SEQ_SE_SELECT1);
                    func_ov223_021baa84(wk);
                    func_ov223_021b9c90(wk, wk->choice);
                    func_ov223_021ba81c(wk, wk->choice);
                    func_ov223_021bb9a4(wk, PSEL_BG_PAL_OF(wk->choice));
                    changed = TRUE;
                }
            } else if (func_02035c60()) {
                s32 hit = func_02035c08(data_ov223_021bc75c);

                if (hit >= 0 && hit <= 2) {
                    if (wk->choice != hit) {
                        changed = TRUE;
                    }
                    GFL_SndSEPlay(SEQ_SE_SELECT1);
                    func_ov223_021baa5c(wk, FALSE);
                    wk->choice = hit;
                    func_ov223_021b9c90(wk, hit);
                    func_ov223_021ba81c(wk, wk->choice);
                    func_ov223_021bb9a4(wk, PSEL_BG_PAL_OF(wk->choice));
                    wk->touchMode = TRUE;
                    *step = 5;
                }
            }
        } else {
            if (GCTX_HIDGetPressedKeys()) {
                wk->touchMode = FALSE;
                *step = 3;
            } else if (func_02035c60()) {
                s32 hit = func_02035c08(data_ov223_021bc75c);

                if (hit >= 0 && hit <= 2) {
                    GFL_SndSEPlay(SEQ_SE_SELECT1);
                    wk->choice = hit;
                    func_ov223_021b9c90(wk, hit);
                    func_ov223_021ba81c(wk, wk->choice);
                    func_ov223_021bb9a4(wk, PSEL_BG_PAL_OF(wk->choice));
                    changed = TRUE;
                    *step = 5;
                }
            }
        }
        break;
    case 5:
        func_ov223_021bb3bc(wk);
        *step = 6;
        break;
    case 6:
        func_ov223_021bb414(wk);
        if (GCTX_HIDGetPressedKeys()) {
            wk->touchMode = FALSE;
            if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_A) {
                GFL_SndSEPlay(SEQ_SE_DECIDE1);
                func_ov223_021bb424(wk);
                *step = 7;
            } else {
                func_ov223_021bb404(wk);
                GFL_BGSysQueueScrLoad(3);
                *step = 3;
            }
        } else if (func_02035c60()) {
            s32 hit = func_02035c08(data_ov223_021bc75c);

            if (hit == 3) {
                GFL_SndSEPlay(SEQ_SE_DECIDE1);
                func_ov223_021bb424(wk);
                *step = 7;
            } else if (hit >= 0 && hit <= 2) {
                if (wk->choice != hit) {
                    GFL_SndSEPlay(SEQ_SE_SELECT1);
                    wk->choice = hit;
                    func_ov223_021b9c90(wk, hit);
                    func_ov223_021ba81c(wk, wk->choice);
                    func_ov223_021bb9a4(wk, PSEL_BG_PAL_OF(wk->choice));
                    changed = TRUE;
                }
            }
        }
        break;
    case 7:
        func_ov223_021bb414(wk);
        if (func_ov223_021bb440(wk)) {
            func_ov223_021bb404(wk);
            GFL_BGSysQueueScrLoad(3);
            *step = 8;
        }
        break;
    case 8:
        func_ov223_021bafc4(wk, 0x14);
        func_ov223_021b9ae4(wk, wk->choice, FALSE);
        *step = 9;
        break;
    case 9:
        func_ov223_021b9b80(wk);
        if (func_ov223_021bb0b0(wk) && func_ov223_021b9b4c(wk)) {
            *step = 10;
        }
        break;
    case 10:
        func_020355d8(wk->touchMode);
        func_ov223_021bb450(wk);
        *step = 11;
        break;
    case 11:
        func_ov223_021bb4e8(wk);
        if (func_ov223_021bb4f8(wk)) {
            BOOL yes = func_ov223_021bb508(wk);

            func_ov223_021bb4d8(wk);
            wk->touchMode = func_020355c4();
            if (yes) {
                wk->bgRequest |= PSEL_BG_REQ_MAIN_OFF;
                func_ov223_021bb134(wk);
                *step = 14;
            } else {
                func_ov223_021bafc4(wk, AspertiaCity_Text_019);
                *step = 12;
            }
        }
        break;
    case 12:
        func_ov223_021b9ae4(wk, wk->choice, TRUE);
        *step = 13;
        break;
    case 13:
        func_ov223_021b9b80(wk);
        if (func_ov223_021b9b4c(wk)) {
            *step = 3;
        }
        break;
    case 14:
        func_ov223_021b9c00(wk, wk->choice);
        GFL_SndSEPlay(SEQ_SE_SDEMO_03);
        *step = 15;
        break;
    case 15:
        func_ov223_021b9c58(wk);
        if (wk->cameraFrame == data_ov223_021bc78c[wk->cameraBall].fadeOut - 2) {
            *step = 16;
        }
        break;
    case 16:
        func_ov223_021b9c58(wk);
        wk->frame = 0;
        return 10;
    }

    if (changed) {
        wk->bgRequest |= PSEL_BG_REQ_SUB_OFF;
        func_ov223_021bb274(wk);
    } else if (wk->pokeArrived) {
        wk->bgRequest |= PSEL_BG_REQ_SUB_ON;
        func_ov223_021bb18c(wk, wk->choice);
        PokeVoice_Play(data_ov223_021bc710[wk->choice], 0, 64, FALSE, 0, 0, FALSE, NULL);
    }
    wk->pokeArrived = FALSE;
    wk->frame++;
    return 9;
}

s32 func_ov223_021bc274(PselWork *wk, s32 *step) {
    func_ov223_021b9c58(wk);
    func_ov223_021b9d4c(wk);
    func_ov223_021ba2b4(wk);
    func_ov223_021bb964(wk);
    switch (*step) {
    case 0:
        GFL_FadeSet(12, 0, 16, 2);
        (*step)++;
        break;
    case 1:
        if (!GFL_FadeIsRunning()) {
            wk->frame = 0;
            return 11;
        }
        break;
    }
    wk->frame++;
    return 10;
}

s32 func_ov223_021bc2cc(PselWork *wk, s32 *step) {
    G2_BlendNone();
    G2S_BlendNone();
    func_ov223_021bb954(wk);
    func_ov223_021badfc(wk);
    func_ov223_021bad68(wk);
    func_ov223_021baa30(wk);
    func_ov223_021ba050(wk);
    func_ov223_021b9a6c(wk);
    wk->frame = 0;
    return 12;
}

s32 func_ov223_021bc30c(PselWork *wk, s32 *step) {
    func_ov223_021bb3ac(wk);
    func_ov223_021baee0(wk);
    func_ov223_021bab74(wk);
    func_ov223_021b9848(wk);
    GFL_TCBRemove(wk->vblankTask);
    func_0201c6ec(wk->printQueue);
    func_0201c4c0(wk->printQueue);
    GFL_FontFree(wk->font);
    func_ov223_021bc498(wk->graphic);
    return 13;
}

s32 func_ov223_021bc348(PselWork *wk, s32 *step) {
    return 13;
}





// A color `step` 16ths of the way from one to another
#define PSEL_BLEND(from, to, step) (((from) * (16 - (step)) + (to) * (step)) / 16)










// Fades the sub screen's BG palette from what it shows to another one
