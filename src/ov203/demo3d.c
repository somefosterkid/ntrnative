// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov293/demo3d.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/demo3d.h"
#include "app/demo3d/demo3d_data.h"
#include "app/demo3d/demo3d_engine.h"
#include "app/demo3d/demo3d_exp.h"
#include "app/demo3d/demo3d_graphic.h"
#include "app/ui/ui_scene.h"
#include "constants/arc.h"
#include "constants/text_banks.h"
#include "gfl/bg_sys.h"
#include "gfl/fade.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/overlay.h"
#include "gfl/proc.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "nitro/gx.h"
#include "nitro/os.h"
#include "nnsys/snd.h"
#include "system/brightness.h"
#include "system/printsys.h"
#include "system/wipe.h"

// The 3D demos' process, named after the ROM's "demo3d.c": it plays a camera animation through scenes of models,
// with commands at set frames. The Royal Unova's cruise, the ferris wheel and the ships and flights are such demos

#define DEMO3D_ID_C_CRUISER 1
#define DEMO3D_SOUND_GROUP_COUNT 2

typedef struct {
    HeapID heapId;
    Demo3DParam *param;
    u32 unk8;
    Demo3DGraphic *graphic;
    PrintQueue *printQueue;
    MsgData *msgData;
    Demo3DEngine *engine;
    Demo3DExp *exp;
    u32 lastVBlank;
    // The frames the last one took beyond its own, which the engine catches up on
    u32 droppedFrames;
    u32 droppedTotal;
} Demo3DWork;

// A sound group that a demo loads
typedef struct {
    u32 demoId;
    u32 group;
} Demo3DSoundGroup;

BOOL func_ov203_021bda40(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov203_021bdb20(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov203_021bdb64(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov203_021bdc20(u8 demoId, int type, HeapID heapId);
BOOL func_ov203_021bdc88(Demo3DWork *work);
Demo3DExp *func_ov203_021bdcc0(u32 demoId, Demo3DGraphic *graphic, Demo3DEngine *engine, HeapID heapId);
void func_ov203_021bdd18(Demo3DExp *exp);
static void func_ov203_021bdd30(Demo3DExp *exp);
static void Demo3D_LoadSoundGroups(u32 demoId);
static void Demo3D_FreeSoundGroups(u32 demoId);

// pokebw2's Demo3D_Exit
// pokebw2's Demo3D_Init
// pokebw2's Demo3D_Main
// pokebw2's gDemo3DProcFunctions
const GameProcFunctions data_ov203_021bfb3c = { func_ov203_021bda40, func_ov203_021bdb64, func_ov203_021bdb20 };

static const Demo3DSoundGroup sDemo3DSoundGroups[DEMO3D_SOUND_GROUP_COUNT] = {
    { 18, 3 },
    { 21, 4 },
};



BOOL func_ov203_021bda40(GameProc *proc, u32 *state, void *param, void *work) {
    Demo3DParam *demoParam = param;
    Demo3DWork *demo;
    s32 frames;
    u8 white;
    u8 fadeFrames;

    GFL_OvlLoad(OVERLAY_ID(67));
    GFL_HeapCreateChild(HEAPID_USER, HEAPID_DEMO3D, 0x130000);
    demo = GFL_ProcInitSubsystem(proc, sizeof(Demo3DWork), HEAPID_DEMO3D);
    MI_CpuFill8(demo, 0, sizeof(Demo3DWork));
    demo->heapId = HEAPID_DEMO3D;
    demo->param = demoParam;
    demo->graphic = func_ov203_021bddd8(1, demoParam->demoId, demo->heapId);
    demo->msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_INTRO_BTL_SERVER_FLOW, demo->heapId);
    demo->printQueue = func_0201c440(demo->heapId);
    GFL_BGSysSetBGEnabled(6, FALSE);
    GFL_BGSysSetBGEnabled(4, FALSE);
    GFL_BGSysSetBGEnabled(3, FALSE);
    GFL_BGSysSetBGEnabled(2, FALSE);
    GFL_BGSysSetBGEnabled(1, FALSE);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_OBJ, FALSE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, FALSE);
    demo->engine = func_ov203_021be160(demo->graphic, demoParam, demo->heapId);
    demo->exp = func_ov203_021bdcc0(demoParam->demoId, demo->graphic, demo->engine, demo->heapId);
    frames = func_ov203_021be62c(demo->engine) - demoParam->unk08;
    func_ov203_021be948(demoParam->demoId, FALSE, &white, &fadeFrames);
    if (frames > fadeFrames) {
        func_ov203_021bdc20(demoParam->demoId, WIPE_TYPE_FADE_IN, demo->heapId);
    }
    return TRUE;
}

BOOL func_ov203_021bdb20(GameProc *proc, u32 *state, void *param, void *work) {
    Demo3DWork *demo = work;

    func_02023f98(3);
    func_ov203_021bdd18(demo->exp);
    GFL_MsgDataFree(demo->msgData);
    func_0201c4c0(demo->printQueue);
    func_ov203_021be4b4(demo->engine);
    func_ov203_021bde98(demo->graphic);
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(HEAPID_DEMO3D);
    GFL_OvlUnload(OVERLAY_ID(67));
    return TRUE;
}

BOOL func_ov203_021bdb64(GameProc *proc, u32 *state, void *param, void *work) {
    Demo3DWork *demo = work;
    BOOL ended;
    u32 vblank;
    u32 elapsed;

    func_ov203_021bdd30(demo->exp);
    func_0201c4e4(demo->printQueue);
    func_ov203_021bdf00(demo->graphic);
    ended = func_ov203_021be538(demo->engine, demo->droppedFrames);
    vblank = OS_GetVBlankCount();
    elapsed = vblank - demo->lastVBlank;
    demo->droppedFrames = 0;
    if (demo->lastVBlank != 0 && elapsed > 1) {
        demo->droppedFrames = elapsed - 1;
    }
    demo->lastVBlank = vblank;
    demo->droppedTotal += demo->droppedFrames;
    switch (*state) {
    case 0:
        if (func_ov203_021bdc88(demo) || ended) {
            demo->param->unk10 = func_ov203_021be620(demo->engine) >> FX32_SHIFT;
            demo->param->result = ended + 1;
            func_ov203_021be638(demo->engine, demo->param->result);
            (*state)++;
        }
        break;
    case 1:
        if (GFL_WipeIsFinished() == TRUE) {
            if (func_ov203_021bdc20(demo->param->demoId, WIPE_TYPE_FADE_OUT, demo->heapId)) {
                return TRUE;
            }
            (*state)++;
        }
        break;
    case 2:
        if (GFL_WipeIsFinished() == TRUE) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

// Starts the demo's fade-in or fade-out. Returns TRUE when the demo has none
// pokebw2's Demo3D_StartFade
static BOOL func_ov203_021bdc20(u8 demoId, int type, HeapID heapId) {
    u8 white;
    u8 frames;
    u16 color;

    func_ov203_021be948(demoId, type == WIPE_TYPE_FADE_OUT ? TRUE : FALSE, &white, &frames);
    color = WIPE_COLOR_BLACK;
    if (white) {
        color = WIPE_COLOR_WHITE;
    }
    func_02022268(color);
    if (frames) {
        u8 freq = GFL_FadeGetUpdateFreq();
        GFL_WipeSet(WIPE_MODE_BOTH, type, type, color, frames * freq, 1, heapId);
        return FALSE;
    }
    return TRUE;
}

// pokebw2's Demo3D_IsSkipRequested
BOOL func_ov203_021bdc88(Demo3DWork *work) {
    if (work->param->result != 0 || !GFL_WipeIsFinished()) {
        return FALSE;
    }
    if (work->exp->skippable && (GCTX_HIDGetPressedKeys() &
                                 (PAD_BUTTON_A | PAD_BUTTON_B | PAD_KEY_RIGHT | PAD_KEY_LEFT | PAD_KEY_UP |
                                  PAD_KEY_DOWN | PAD_BUTTON_X | PAD_BUTTON_Y))) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's Demo3DExp_Create
Demo3DExp *func_ov203_021bdcc0(u32 demoId, Demo3DGraphic *graphic, Demo3DEngine *engine, HeapID heapId) {
    Demo3DExp *exp = GFL_HeapAllocate(heapId, sizeof(Demo3DExp), TRUE, "demo3d.c", 481);

    exp->graphic = graphic;
    exp->engine = engine;
    exp->heapId = heapId;
    exp->funcs = NULL;
    exp->skippable = FALSE;
    if (demoId == DEMO3D_ID_C_CRUISER) {
        exp->funcs = &data_ov203_021c3bc4;
        exp->skippable = TRUE;
    }
    if (exp->funcs != NULL) {
        exp->funcs->init(exp);
    }
    return exp;
}

// pokebw2's Demo3DExp_Free
void func_ov203_021bdd18(Demo3DExp *exp) {
    if (exp->funcs != NULL) {
        exp->funcs->exit(exp);
    }
    GFL_HeapFree(exp);
}

// pokebw2's Demo3DExp_Main
static void func_ov203_021bdd30(Demo3DExp *exp) {
    if (exp->funcs != NULL) {
        exp->funcs->main(exp);
    }
}

