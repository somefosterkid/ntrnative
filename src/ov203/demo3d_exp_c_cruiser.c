// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov293/demo3d_exp_c_cruiser.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/demo3d/demo3d_engine.h"
#include "app/demo3d/demo3d_exp.h"
#include "app/demo3d/demo3d_graphic.h"
#include "app/ui/ui_scene.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/clact.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "nitro/fx/fx.h"
#include "nitro/gx.h"

// The Royal Unova's cruise's own part of its 3D demo, named after the ROM's "demo3d_exp_c_cruiser.c": a map of the
// bay on the lower screen, with a marker that follows the ship's course

// The archive of the map and its marker. Our name
#define ARCID_DEMO3D_C_CRUISER 85
// The marker moves every 100 frames of the camera's animation
#define POSITION_FRAMES 100
#define POSITION_COUNT 54

typedef struct {
    // The next of the marker's positions
    u8 position;
    UIObjRes objRes;
    ClActor *marker;
} Demo3DExpCCruiser;

static void func_ov203_021bf9d0(HeapID heapId);
void func_ov203_021bfa20(Demo3DExp *exp);
void func_ov203_021bfae4(Demo3DExp *exp);
void func_ov203_021bfb28(Demo3DExp *exp);

// pokebw2's gDemo3DExpCCruiserFuncs, which demo3d.c gives the cruise's demo
const Demo3DExpFuncs data_ov203_021c3bc4 = {
    func_ov203_021bfa20,
    func_ov203_021bfae4,
    func_ov203_021bfb28,
};

static const ClActorPos data_ov203_021c3bd0[POSITION_COUNT] = {
#include "asset/demo3d_exp_c_cruiser_data_ov203_021c3bd0.inc"
};

// pokebw2's Demo3DExpCCruiser_LoadBG
static void func_ov203_021bf9d0(HeapID heapId) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_DEMO3D_C_CRUISER, heapId);

    GFL_G2DIOLoadArcNCLRDefault(arc, 3, 4, 0, 0, heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 9, 6, 0, 0, FALSE, heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 18, 6, 0, 0, FALSE, heapId);
    GFL_ArcToolFree(arc);
}

// pokebw2's Demo3DExpCCruiser_Init
void func_ov203_021bfa20(Demo3DExp *exp) {
    Demo3DExpCCruiser *work = GFL_HeapAllocate(exp->heapId, sizeof(Demo3DExpCCruiser), TRUE, "demo3d_exp_c_cruiser.c", 179);
    UIObjResSetup setup;
    ClActUnit *unit;

    exp->work = work;
    func_ov203_021bf9d0(exp->heapId);
    work->position = (func_ov203_021be620(exp->engine) >> FX32_SHIFT) / POSITION_FRAMES;
    if (work->position >= POSITION_COUNT) {
        work->position = POSITION_COUNT - 1;
    }
    unit = func_ov203_021bdf30(exp->graphic);
    setup.vramType = 1;
    setup.flags = 0;
    setup.arcId = ARCID_DEMO3D_C_CRUISER;
    setup.paletteFile = 4;
    setup.charFile = 10;
    setup.cellFile = 13;
    setup.animFile = 20;
    setup.paletteOffset = 0;
    setup.paletteStart = 0;
    setup.paletteCount = 1;
    func_ov067_021b6208(&work->objRes, &setup, unit, exp->heapId);
    work->marker = func_ov067_021b629c(&work->objRes, unit, data_ov203_021c3bd0[work->position].x,
                                        data_ov203_021c3bd0[work->position].y, 0, exp->heapId);
    func_0204b3dc(work->marker, TRUE);
    func_0204b92c(work->marker, TRUE);
    GFL_BGSysSetBGEnabled(6, TRUE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
}

// pokebw2's Demo3DExpCCruiser_Main
void func_ov203_021bfae4(Demo3DExp *exp) {
    Demo3DExpCCruiser *work = exp->work;
    int frame = func_ov203_021be620(exp->engine) >> FX32_SHIFT;

    if (work->position < POSITION_COUNT && work->position * POSITION_FRAMES <= frame) {
        func_0204b404(work->marker, &data_ov203_021c3bd0[work->position], 1);
        work->position++;
        if (work->position == POSITION_COUNT) {
            func_0204b3dc(work->marker, FALSE);
        }
    }
}

// pokebw2's Demo3DExpCCruiser_Exit
void func_ov203_021bfb28(Demo3DExp *exp) {
    func_ov067_021b6284(&((Demo3DExpCCruiser *)exp->work)->objRes);
    GFL_HeapFree(exp->work);
}
