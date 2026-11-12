// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov162/title.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/title.h"
#include "constants/arc.h"
#include "constants/species.h"
#include "constants/text_banks.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/fade.h"
#include "gfl/g3d.h"
#include "gfl/graphics.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/overlay.h"
#include "gfl/proc.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "gfl/tcbl.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_trig.h"
#include "nitro/fx/fx_cp.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "nitro/os.h"
#include "system/gf_font.h"
#include "system/printsys.h"
#include "system/version.h"
#include "app/mictest.h"
#include "app/startmenu.h"
#include "gfl/overlay.h"
#include "text/system/title_16.h"
#include "constants/version.h"

// The title screen: a 3D scene under a camera that follows a curve, with 2D layers over it. The main engine draws to
// VRAM D through the display capture, blending each frame with the last for a motion blur, and shows VRAM D.
// A or Start goes on to the start menu, Up, Select and B to deleting the save, and Down, X and Y to the microphone
#define TIMEOUT_FRAMES 6780
// test. If none of them is pressed, the title ends after TIMEOUT_FRAMES without starting anything

// The cry of the version's Kyurem, when A or Start is pressed
#ifdef BLACK2
#define KYUREM_FORM 2
#else
#define KYUREM_FORM 1
#endif

#define DELETE_SAVE_KEYS (PAD_KEY_UP | PAD_BUTTON_SELECT | PAD_BUTTON_B)
#define MIC_TEST_KEYS (PAD_KEY_DOWN | PAD_BUTTON_X | PAD_BUTTON_Y)

// After this many frames, the title flashes white, puts the 3D scene on the top screen and hides main BG 3 and the
// sub engine's BG 2 and OBJ. A, B or Start then puts them back
#define HIDE_LAYERS_FRAMES 1740
// The title ends after this many frames

// Frames of the camera curve: where it goes when a button brings the layers back, and loops from its end after that,
// and where it goes when the title is left
#define CURVE_LOOP_FRAME 7300
#define CURVE_END_FRAME 7780
#define CURVE_LEAVE_FRAME 7001

enum {
    STATE_INIT_DISPLAY,
    STATE_INIT,
    STATE_WAIT_FADE_IN,
    STATE_RUN,
    STATE_WAIT_CRY,
    STATE_FADE_OUT,
    STATE_WAIT_FADE_OUT,
    STATE_FREE,
    STATE_END,
};

// What the title goes on to
enum {
    NEXT_START_MENU,
    NEXT_NONE,
    NEXT_DELETE_SAVE,
    NEXT_MIC_TEST,
};

enum {
    LAYERS_SHOWN,
    LAYERS_HIDDEN,
    LAYERS_BACK,
};

typedef struct {
    TCBExManager *tcbManager;
    TCB *vblankTask;
    // The display capture's blend factors, out of 16, for the new frame and the last
    s16 captureEva;
    s16 captureEvb;
} TitleSys;

typedef struct {
    BmpWin *window;
    GFLBitmap *bitmap;
    PrintWindow print;
    BOOL unk10;
    Font *font;
    PrintQueue *printQueue;
    u32 unk1C;
    MsgData *msgData;
    StrBuf *strbuf;
    // Scrolls sub BG 2, one way in Black 2 and the other in White 2
    s32 scroll;
} TitleBG;

typedef struct {
    G3DManager *manager;
    u16 scene;
    G3DLight *light;
    G3DCurve *curve;
    G3DCamera *camera;
} TitleG3D;

typedef struct {
    ClActUnit *unit;
    ClActor *actor;
    u8 unk8[0x24];
    u32 palette;
    u32 chars;
    u32 cellAnims;
} TitleObj;

typedef struct {
    u16 state;
    u16 heapId;
    u32 next;
    TitleSys sys;
    TitleBG bg;
    TitleG3D g3d;
    TitleObj obj;
    u32 frames;
    u32 cry;
    u32 layers;
} TitleWork;

BOOL func_ov088_021b95a0(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov088_021b9750(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov088_021b9874(GameProc *proc, u32 *state, void *param, void *work);
void func_ov088_021b95cc(TitleWork *wk);
void func_ov088_021b965c(TitleWork *wk);
void func_ov088_021b98bc(void);
void func_ov088_021b9910(void);
void func_ov088_021b9928(TitleWork *wk);
void func_ov088_021b997c(TitleSys *sys, HeapID heapId);
void func_ov088_021b9c2c(TitleG3D *g3d, HeapID heapId);
void func_ov088_021b9ec4(TitleObj *obj, HeapID heapId);
void func_ov088_021b9f70(TitleObj *obj);
void func_ov088_021b9e54(TitleG3D *g3d);
void func_ov088_021b9c08(TitleBG *bg);
void func_ov088_021b99ec(TitleSys *sys);
void func_ov088_021b9954(TCB *tcb, void *data);
void func_ov088_021b99e0(TitleSys *sys, HeapID heapId);
void func_ov088_021b9a14(TitleSys *sys, s16 eva, s16 evb);
void func_ov088_021b9a1c(TitleBG *bg, HeapID heapId);
static void func_ov088_021b9bd4(TitleBG *bg, HeapID heapId);
static void func_ov088_021b9e7c(TitleG3D *g3d, u32 frame);
static void func_ov088_021b9f68(TitleObj *obj, HeapID heapId);

// Declared in an order that gives the original layout of the data (tools/decomp/rodata_order.py)

// pokebw2's sBGSetupSub3
static const BGSetup data_ov088_021bd430 = {
    0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_256, GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x04000, 0x8000,
    GX_BG_EXTPLTT_01, 2, GX_BG_AREAOVER_XLU, FALSE,
};

// pokebw2's sBGSetupSub2
static const BGSetup data_ov088_021bd450 = {
    0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_256, GX_BG_SCRBASE_0x1000, GX_BG_CHARBASE_0x04000, 0x8000,
    GX_BG_EXTPLTT_01, 3, GX_BG_AREAOVER_XLU, FALSE,
};

// pokebw2's sBGSetupMain3
static const BGSetup data_ov088_021bd470 = {
    0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x0800, GX_BG_CHARBASE_0x04000, 0x4000,
    GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE,
};

// pokebw2's sBGSetupSub1
static const BGSetup data_ov088_021bd490 = {
    0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x0800, GX_BG_CHARBASE_0x0c000, 0x4000,
    GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE,
};

// pokebw2's sLightSetups
static const LightSetup data_ov088_021bd4b0[] = {
    { 0, { { FX16_ONE - 1, -(FX16_ONE - 1) / 2, -(FX16_ONE - 1) }, GX_RGB(31, 31, 31) } },
    { 1, { { -(FX16_ONE - 1), -(FX16_ONE - 1), -(FX16_ONE - 1) }, GX_RGB(31, 31, 31) } },
    { 2, { { -(FX16_ONE - 1), -(FX16_ONE - 1), -(FX16_ONE - 1) }, GX_RGB(31, 31, 31) } },
    { 3, { { -(FX16_ONE - 1), -(FX16_ONE - 1), -(FX16_ONE - 1) }, GX_RGB(31, 31, 31) } },
};

// pokebw2's sLightSetupList
static const LightSetupList data_ov088_021bd3dc = { data_ov088_021bd4b0, NELEMS(data_ov088_021bd4b0) };

// pokebw2's sAnimations1
static const G3DSceneAnimationSetup data_ov088_021bd3d4[] = { { 3, 0 } };

// pokebw2's sAnimations2
static const G3DSceneAnimationSetup data_ov088_021bd3d8[] = { { 5, 0 } };

// pokebw2's sCameraUpVector
static const VecFx32 data_ov088_021bd3f8 = { 0, FX32_ONE, 0 };

// pokebw2's sCameraTarget
static const VecFx32 data_ov088_021bd404 = { 0, 0, 0 };

// pokebw2's sCameraPosition
static const VecFx32 data_ov088_021bd3ec = { 0, 0, 0 };

// pokebw2's sAnimations0
static const G3DSceneAnimationSetup data_ov088_021bd3e4[] = { { 1, 0 }, { 6, 0 } };

// pokebw2's sActors
static const G3DSceneActorSetup data_ov088_021bd4d8[] = {
    { 0, 0, 0, 0, data_ov088_021bd3e4, NELEMS(data_ov088_021bd3e4) },
    { 2, 0, 2, 0, data_ov088_021bd3d4, NELEMS(data_ov088_021bd3d4) },
    { 4, 0, 4, 0, data_ov088_021bd3d8, NELEMS(data_ov088_021bd3d8) },
};

// The model and animations of the version's Kyurem and of the other actors
// pokebw2's sResources
static const G3DSceneResourceSetup data_ov088_021bd580[] = {
#ifdef BLACK2
    { ARCID_DEMO3D_RESOURCE, 447, 0 }, { ARCID_DEMO3D_RESOURCE, 446, 0 }, { ARCID_DEMO3D_RESOURCE, 449, 0 },
    { ARCID_DEMO3D_RESOURCE, 448, 0 }, { ARCID_DEMO3D_RESOURCE, 451, 0 }, { ARCID_DEMO3D_RESOURCE, 452, 0 },
    { ARCID_DEMO3D_RESOURCE, 450, 0 },
#elif GAME_VERSION == VERSION_WHITE
    { ARCID_DEMO3D_RESOURCE, 235, 0 }, { ARCID_DEMO3D_RESOURCE, 234, 0 }, { ARCID_DEMO3D_RESOURCE, 238, 0 },
    { ARCID_DEMO3D_RESOURCE, 237, 0 }, { ARCID_DEMO3D_RESOURCE, 240, 0 }, { ARCID_DEMO3D_RESOURCE, 239, 0 },
    { ARCID_DEMO3D_RESOURCE, 236, 0 },
#else
    { ARCID_DEMO3D_RESOURCE, 227, 0 }, { ARCID_DEMO3D_RESOURCE, 226, 0 }, { ARCID_DEMO3D_RESOURCE, 230, 0 },
    { ARCID_DEMO3D_RESOURCE, 229, 0 }, { ARCID_DEMO3D_RESOURCE, 232, 0 }, { ARCID_DEMO3D_RESOURCE, 231, 0 },
    { ARCID_DEMO3D_RESOURCE, 228, 0 },
#endif
};

// pokebw2's sSceneSetup
static const G3DSceneSetup data_ov088_021bd410 = { data_ov088_021bd580, NELEMS(data_ov088_021bd580), data_ov088_021bd4d8, NELEMS(data_ov088_021bd4d8) };

// pokebw2's sLCDConfig
static const BGSysLCDConfig data_ov088_021bd420 = { GX_DISPMODE_VRAM_D, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_3D };

// pokebw2's sTransform
static const SRTMatrix data_ov088_021bd538 = {
    { 0, 0, 0 },
    { FX32_ONE, FX32_ONE, FX32_ONE },
    { { { FX32_ONE, 0, 0 }, { 0, FX32_ONE, 0 }, { 0, 0, FX32_ONE } } },
};

// pokebw2's sVRAMConfig
static const BGSysVRAMConfig data_ov088_021bd508 = {
    GX_VRAM_BG_32_FG,
    GX_VRAM_BGEXTPLTT_NONE,
    GX_VRAM_SUB_BG_128_C,
    GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_NONE,
    GX_VRAM_OBJEXTPLTT_NONE,
    GX_VRAM_SUB_OBJ_16_I,
    GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_01_AB,
    GX_VRAM_TEXPLTT_0123_E,
    GX_OBJVRAMMODE_CHAR_1D_64K,
    GX_OBJVRAMMODE_CHAR_1D_32K,
};

// pokebw2's Title_Exit
// pokebw2's Title_Init
// pokebw2's Title_Main
const GameProcFunctions data_ov088_021bd574 = { func_ov088_021b95a0, func_ov088_021b9750, func_ov088_021b9874 };

// pokebw2's Title_UpdateTimer

BOOL func_ov088_021b95a0(GameProc *proc, u32 *state, void *param, void *work) {
    TitleWork *wk;

    GFL_HeapCreateChild(HEAPID_USER, 0x16, 0x120000);
    wk = GFL_ProcInitSubsystem(proc, sizeof(TitleWork), 0x16);
    MI_CpuFill8(wk, 0, sizeof(TitleWork));
    wk->heapId = 0x16;
    return TRUE;
}

#if GAME_VERSION == VERSION_WHITE
#define TITLE_CURVE_END_FRAME 5779
#define TITLE_SPECIES SPECIES_ZEKROM
#define TITLE_CURVE_FILE 0xf1
#else
#define TITLE_CURVE_END_FRAME 5619
#define TITLE_SPECIES SPECIES_RESHIRAM
#define TITLE_CURVE_FILE 0xe9
#endif

void func_ov088_021b95cc(TitleWork *wk) {
    if (wk->frames == 666) {
        GFL_FadeSet(FADE_ENGINE_A_WHITE | FADE_ENGINE_B_WHITE, 16, 0, 3);
        GFL_BGSysSetDisplayLayout(1);
        GFL_BGSysSetBGEnabled(6, FALSE);
        GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, FALSE);
        GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG3, FALSE);
        wk->layers = LAYERS_HIDDEN;
    }
    if (wk->layers == LAYERS_BACK && GFL_G3DCurveGetNowFrame(wk->g3d.curve) == TITLE_CURVE_END_FRAME * FX32_ONE) {
        func_ov088_021b9e7c(&wk->g3d, 5301);
    }
    if (++wk->frames > 4680) {
        wk->next = NEXT_NONE;
        wk->state = STATE_FADE_OUT;
    }
}

// pokebw2's Title_HandleInput
void func_ov088_021b965c(TitleWork *wk) {
    if ((GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B | PAD_BUTTON_START)) &&
        wk->layers == LAYERS_HIDDEN) {
        GFL_BGSysSetDisplayLayout(0);
        GFL_BGSysSetBGEnabled(6, TRUE);
        GFL_BGSysSetBGEnabled(3, TRUE);
        GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
        func_ov088_021b9e7c(&wk->g3d, 0x14b5);
        wk->layers = LAYERS_BACK;
        return;
    }
    if (GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_START)) {
        wk->next = NEXT_START_MENU;
        wk->state = STATE_WAIT_CRY;
        wk->cry = PokeVoice_Play(TITLE_SPECIES, 0, 64, 0, 0, 0, 0, 0);
        func_ov088_021b9e7c(&wk->g3d, 0x1389);
        GFL_BGSysSetDisplayLayout(0);
        GFL_BGSysSetBGEnabled(6, TRUE);
        GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, FALSE);
        GFL_FadeSet(FADE_ENGINE_A_WHITE | FADE_ENGINE_B_WHITE, 16, 0, 3);
        GFL_SndBGMFadeOut(4);
        func_ov088_021b9a14(&wk->sys, 8, 8);
        return;
    }
    if (GCTX_HIDGetHeldKeys() == DELETE_SAVE_KEYS) {
        wk->next = NEXT_DELETE_SAVE;
        wk->state = STATE_FADE_OUT;
        return;
    }
    if (GCTX_HIDGetHeldKeys() == MIC_TEST_KEYS) {
        wk->next = NEXT_MIC_TEST;
        wk->state = STATE_FADE_OUT;
    }
}

BOOL func_ov088_021b9750(GameProc *proc, u32 *state, void *param, void *work) {
    TitleWork *wk = work;

    switch (wk->state) {
    case STATE_INIT_DISPLAY:
        func_ov088_021b98bc();
        func_ov088_021b9910();
        wk->state = STATE_INIT;
        break;
    case STATE_INIT:
        func_ov088_021b997c(&wk->sys, wk->heapId);
        func_ov088_021b9a1c(&wk->bg, wk->heapId);
        func_ov088_021b9c2c(&wk->g3d, wk->heapId);
        func_ov088_021b9ec4(&wk->obj, wk->heapId);
        GFL_FadeSet(FADE_ENGINE_A_BLACK | FADE_ENGINE_B_BLACK, 0x10, 0, 2);
        wk->frames = 0;
        wk->state = STATE_WAIT_FADE_IN;
        GFL_SndBGMStop(0x3ef);
        break;
    case STATE_WAIT_FADE_IN:
        if (!GFL_FadeIsRunning()) {
            wk->state = STATE_RUN;
        }
        func_ov088_021b9928(wk);
        break;
    case STATE_RUN:
        func_ov088_021b9928(wk);
        func_ov088_021b965c(wk);
        func_ov088_021b95cc(wk);
        break;
    case STATE_WAIT_CRY:
        if (!PokeVoice_IsPlaying(wk->cry)) {
            wk->state = STATE_FADE_OUT;
        }
        func_ov088_021b9928(wk);
        break;
    case STATE_FADE_OUT:
        GFL_FadeSet(FADE_ENGINE_A_BLACK | FADE_ENGINE_B_BLACK, 0, 0x10, 2);
        wk->state = STATE_WAIT_FADE_OUT;
        func_ov088_021b9928(wk);
        GFL_SndBGMFadeOut(0x10);
        break;
    case STATE_WAIT_FADE_OUT:
        if (!GFL_FadeIsRunning() && !GFL_SndBGMIsFading()) {
            func_02005d28();
            wk->state = STATE_FREE;
        }
        func_ov088_021b9928(wk);
        break;
    case STATE_FREE:
        func_ov088_021b9f70(&wk->obj);
        func_ov088_021b9e54(&wk->g3d);
        func_ov088_021b9c08(&wk->bg);
        func_ov088_021b99ec(&wk->sys);
        return TRUE;
    case STATE_END:
        return TRUE;
    }
    return FALSE;
}


extern const GameProcFunctions data_ov088_021bd974;
BOOL func_ov088_021b9874(GameProc *proc, u32 *state, void *param, void *work) {
    TitleWork *wk = work;
    u32 next = wk->next;

    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(0x16);
    if (next == NEXT_START_MENU) {
        GCTX_ProcMgrReplaceProc(OVERLAY_ID(88), &data_ov088_021bd66c, NULL);
    } else if (next == NEXT_DELETE_SAVE) {
        GCTX_ProcMgrReplaceProc(OVERLAY_ID(88), &data_ov088_021bd974, NULL);
    } else if (next == NEXT_MIC_TEST) {
        GCTX_ProcMgrReplaceProc(OVERLAY_ID(168), &data_ov168_02184904, NULL);
    }
    return TRUE;
}


// pokebw2's Title_ResetDisplay
void func_ov088_021b98bc(void) {
    GX_SetMasterBrightness(-16);
    GXS_SetMasterBrightness(-16);
    GFL_BGSysSetEnabledBGsA(0);
    GFL_BGSysSetEnabledBGsB(0);
    G2_BlendNone();
    G2S_BlendNone();
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
}

// pokebw2's Title_SetBanks
void func_ov088_021b9910(void) {
    GFL_BGSysSetVRAMBanks(&data_ov088_021bd508);
    GFL_BGSysSetLCDConfig(&data_ov088_021bd420);
}

// Steps the camera once for each VBlank since the last update, so that it keeps its speed when frames are dropped
// pokebw2's Title_Update
void func_ov088_021b9dd0(TitleG3D *g3d, HeapID heapId);
void func_ov088_021b9928(TitleWork *wk) {
    func_ov088_021b99e0(&wk->sys, wk->heapId);
    func_ov088_021b9bd4(&wk->bg, wk->heapId);
    func_ov088_021b9dd0(&wk->g3d, wk->heapId);
    func_ov088_021b9f68(&wk->obj, wk->heapId);
}

// pokebw2's TitleSys_VBlank
void func_ov088_021b9954(TCB *tcb, void *data) {
    TitleSys *sys = data;

    func_0204a648();
    GX_SetCapture(GX_CAPTURE_SIZE_256x192, GX_CAPTURE_MODE_AB, GX_CAPTURE_SRCA_2D3D, GX_CAPTURE_SRCB_VRAM_0x00000,
                  GX_CAPTURE_DEST_VRAM_D_0x00000, sys->captureEva, sys->captureEvb);
}

// pokebw2's TitleSys_Init
void func_ov088_021b997c(TitleSys *sys, HeapID heapId) {
    GFL_BGSysCreate(heapId);
    BmpWin_InitAllocator(heapId);
    GFL_G3DSysCreate(FALSE, 2, 0, 2, 0x1000, heapId, NULL);
    func_0201dd60();
    sys->tcbManager = GFL_TCBExMgrCreate(heapId, heapId, 4, 32);
    sys->vblankTask = GFL_VBlankTCBAdd(func_ov088_021b9954, sys, 5);
    GFL_BGSysSetDisplayLayout(0);
    GFL_BGSysEnableEngines();
    sys->captureEva = 12;
    sys->captureEvb = 4;
}

// pokebw2's TitleSys_Update
void func_ov088_021b99e0(TitleSys *sys, HeapID heapId) {
    GFL_TCBExMgrUpdate(sys->tcbManager);
}

// pokebw2's TitleSys_Free
void func_ov088_021b99ec(TitleSys *sys) {
    GFL_BGSysSetDisplayLayout(1);
    GFL_BGSysEnableEngines();
    GFL_TCBRemove(sys->vblankTask);
    GFL_TCBExMgrFree(sys->tcbManager);
    GFL_G3DSysFree();
    BmpWin_FreeAllocator();
    GFL_BGSysFree();
}

// pokebw2's TitleSys_SetCaptureBlend
void func_ov088_021b9a14(TitleSys *sys, s16 eva, s16 evb) {
    sys->captureEva = eva;
    sys->captureEvb = evb;
}

// The sub engine's BG 1 has a line of text, blank in this version, which func_ov088_021b9bd4 hides
// pokebw2's TitleBG_Init
void func_ov088_021b9a1c(TitleBG *bg, HeapID heapId) {
    s32 width;

    GFL_BGSysCreateBG(3, &data_ov088_021bd470, BGMODE_TEXT);
    GFL_BGSysCreateBG(5, &data_ov088_021bd490, BGMODE_TEXT);
    GFL_BGSysCreateBG(7, &data_ov088_021bd430, BGMODE_TEXT);
    GFL_BGSysCreateBG(6, &data_ov088_021bd450, BGMODE_TEXT);
    GFL_BGSysLoadNCGRStatic(ARCID_TITLE, 9, 3, 0, 0, TRUE, heapId);
    loadBGScrToVramByNarcNoReserveNegAlign(ARCID_TITLE, 10, 3, 0, 0, TRUE, heapId);
    GFL_BGSysLoadNCLRDefault(ARCID_TITLE, 8, 0, 0, 0, heapId);
    GFL_BGSysLoadScr(3);
    GFL_BGSysSetBGEnabled(3, TRUE);
    GFL_BGSysLoadNCGRStatic(ARCID_TITLE, 0, 7, 0, 0x8000, TRUE, heapId);
    loadBGScrToVramByNarcNoReserveNegAlign(ARCID_TITLE, 1, 7, 0, 0, TRUE, heapId);
    GFL_BGSysLoadNCLRDefault(ARCID_TITLE, 2, 4, 0, 0, heapId);
    GFL_BGSysLoadScr(7);
    GFL_BGSysSetBGEnabled(7, TRUE);
    loadBGScrToVramByNarcNoReserveNegAlign(ARCID_TITLE, 3, 6, 0, 0, TRUE, heapId);
    GFL_BGSysSetBGEnabled(6, TRUE);
    GFL_BGSysFillChar(5, 0, 1, 0);
    GFL_BGSysFillScrArea(5, 0, 0, 0, 32, 32, 17);
    bg->window = BmpWin_CreateDynamic(5, 0, 19, 32, 2, 10, 0);
    bg->bitmap = BmpWin_GetBitmap(bg->window);
    GFL_BitmapFill(bg->bitmap, 0);
    BmpWin_FlushMap(bg->window);
    BmpWin_FlushChar(bg->window);
    bg->print.window = bg->window;
    bg->print.flushPending = FALSE;
    GFL_BGSysLoadScr(5);
    bg->font = GFL_FontCreate(ARCID_FONT, 0, 0, 0, heapId);
    bg->printQueue = func_0201c440(heapId);
    bg->msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_TITLE_16, heapId);
    bg->strbuf = GFL_StrBufCreate(64, heapId);
    GFL_MsgDataLoadStrbuf(bg->msgData, Title16_Text_001, bg->strbuf);
    width = GFL_FontGetBlockWidth(bg->strbuf, bg->font, 0);
    func_0201c724(bg->printQueue, bg->bitmap, 128 - width / 2, 0, bg->strbuf, bg->font, PRINT_COLOR(1, 2, 0));
    bg->unk10 = TRUE;
}

// pokebw2's TitleBG_Update
static void func_ov088_021b9bd4(TitleBG *bg, HeapID heapId) {
    GFL_BGSysSetBGEnabled(5, FALSE);
    GFL_BGSysMoveBGReq(6, BG_MOVE_SET_X, bg->scroll / 2);
    if (getGameVersion() == 0x15) {
        bg->scroll++;
    } else {
        bg->scroll--;
    }
}

// pokebw2's TitleBG_Free
void func_ov088_021b9c08(TitleBG *bg) {
    GFL_StrBufFree(bg->strbuf);
    GFL_MsgDataFree(bg->msgData);
    func_0201c4c0(bg->printQueue);
    GFL_FontFree(bg->font);
    BmpWin_Free(bg->window);
}

void func_ov088_021b9c2c(TitleG3D *g3d, HeapID heapId) {
    int firstActor;
    fx32 v;
    fx32 denom;
    fx32 numer;

    g3d->manager = GFL_G3DMgrCreate(7, 3, heapId);
    g3d->scene = GFL_G3DMgrNewScene(g3d->manager, &data_ov088_021bd410);
    firstActor = GFL_G3DMgrGetSceneFirstActorIdx(g3d->manager, g3d->scene);
    GFL_G3DActorBindAnm(GFL_G3DMgrGetActor(g3d->manager, firstActor), 0);
    GFL_G3DActorBindAnm(GFL_G3DMgrGetActor(g3d->manager, firstActor), 1);
    GFL_G3DActorBindAnm(GFL_G3DMgrGetActor(g3d->manager, firstActor + 1), 0);
    GFL_G3DActorBindAnm(GFL_G3DMgrGetActor(g3d->manager, firstActor + 2), 0);
    v = FX_SinIdx(DEG_TO_IDX(30));
    denom = FX_CosIdx(DEG_TO_IDX(30));
    numer = FX_Mul(v, 0x19a);
    fx32 top = FX_Div(numer, denom);
    fx32 bottom = -FX_Div(numer, denom);
    fx32 right = FX_Div(FX_Mul(FX_Div(0x223, 0x1000), v), denom);
    fx32 left = -FX_Div(FX_Mul(FX_Div(0x223, 0x1000), v), denom);
    g3d->camera = GFL_G3DCameraCreate(G3DCAM_PROJECTION_FRUSTUM, top, bottom, left, right, FX32_CONST(0.1),
                                      FX32_CONST(2048), FX32_CONST(0.1), &data_ov088_021bd3ec, &data_ov088_021bd3f8,
                                      &data_ov088_021bd404, heapId);
    g3d->curve = GFL_G3DCurveLoadFileStream(heapId, ARCID_DEMO3D_RESOURCE, TITLE_CURVE_FILE, 10);
    g3d->light = GFL_G3DLightCreate(&data_ov088_021bd3dc, heapId);
    GFL_G3DLightFlush(g3d->light);
    GFL_BGSysSet3DBGPriority(1);
    G3X_AntiAlias(TRUE);
    G3X_AlphaTest(FALSE, 0);
    G3X_AlphaBlend(TRUE);
}


void func_ov088_021b9dd0(TitleG3D *g3d, HeapID heapId)
{
    u16 firstActorIdx;
    u32 i;
    G3DActor *actor;

    firstActorIdx = GFL_G3DMgrGetSceneFirstActorIdx(g3d->manager, g3d->scene);
    GFL_G3DCurveFrameStep(g3d->curve, 0x1000);
    GFL_G3DCurveApplyCamera(g3d->camera, g3d->curve);
    GFL_G3DCameraFlush(g3d->camera);
    GFL_G3DSysReset();
    GFL_G3DSysMtxViewFlush();

    for (i = 0; i < 3; i++) {
        actor = GFL_G3DMgrGetActor(g3d->manager, firstActorIdx + i);
        GFL_G3DSysDrawObj(actor, &data_ov088_021bd538);
    }

    GFL_G3DSysReqSwapBuffers();

    for (i = 0; i < 3; i++) {
        actor = GFL_G3DMgrGetActor(g3d->manager, firstActorIdx + i);
        GFL_G3DActorStepAnmFrameLoop(actor, 0, 0x1000);
        if (i == 0) {
            GFL_G3DActorStepAnmFrameLoop(actor, 1, 0x1000);
        }
    }
}


// pokebw2's TitleG3D_Free
void func_ov088_021b9e54(TitleG3D *g3d) {
    GFL_G3DCurveFree(g3d->curve);
    GFL_G3DCameraFree(g3d->camera);
    GFL_G3DLightFree(g3d->light);
    GFL_G3DMgrDeleteScene(g3d->manager, g3d->scene);
    GFL_G3DMgrFree(g3d->manager);
}

// Moves the camera and every animation to a frame
// pokebw2's TitleG3D_SetFrame
static void func_ov088_021b9e7c(TitleG3D *g3d, u32 frame) {
    fx32 frameFx = frame << 12;
    u16 firstActor;
    u32 i;
    G3DActor *actor;

    GFL_G3DCurveFrameSet(g3d->curve, frameFx);
    firstActor = GFL_G3DMgrGetSceneFirstActorIdx(g3d->manager, g3d->scene);
    for (i = 0; i < 3; i++) {
        actor = GFL_G3DMgrGetActor(g3d->manager, firstActor + i);
        GFL_G3DActorSetAnmFrame(actor, 0, &frameFx);
        if (i == 0)
            GFL_G3DActorSetAnmFrame(actor, 1, &frameFx);
    }
}

// pokebw2's TitleObj_Init
void func_ov088_021b9ec4(TitleObj *obj, HeapID heapId) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_TITLE, heapId);
    ClActorSetup setup;

    ClActSys_Create(&data_020a1448, &data_ov088_021bd508, heapId);
    obj->unit = func_0204b100(64, 0, heapId);
    func_0204b270(obj->unit);
    obj->chars = func_0204a6c8(arc, 5, 1, 1, heapId);
    obj->palette = func_0204abf0(arc, 4, 1, 0, heapId);
    obj->cellAnims = func_0204af28(arc, 6, 7, heapId);
    setup.x = 128;
    setup.y = 96;
    setup.sequence = 0;
    setup.bgPriority = 0;
    setup.priority = 0;
    obj->actor = func_0204b294(obj->unit, obj->chars, obj->palette, obj->cellAnims, &setup, 1, heapId);
    func_0204b92c(obj->actor, TRUE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
    GFL_ArcToolFree(arc);
}

// pokebw2's TitleObj_Update
static void func_ov088_021b9f68(TitleObj *obj, HeapID heapId) {
    func_0204a600();
}

// pokebw2's TitleObj_Free
void func_ov088_021b9f70(TitleObj *obj) {
    func_0204a8d4(obj->chars);
    func_0204ada4(obj->palette);
    func_0204afd8(obj->cellAnims);
    func_0204b1cc(obj->unit);
    func_0204a5ac();
}
