// Black's code at ov221 0x021b9dd4-0x021ba140. No name is known: the module and address.

#include "types.h"
#include "app/chihou_zukan_award_graphic.h"
#include "app/psel_graphic.h"
#include "gfl/clact.h"
#include "gfl/g3d.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/tcb.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_trig.h"
#include "nitro/gx.h"
#include "nitro/mi.h"
#include "struct_decls.h"
#include "system/gf_font.h"
#include "gfl/bg_sys.h"
#include "gfl/bmpwin.h"

static const BGSysVRAMConfig data_ov221_021ba220 = { 1, 0, 4, 0, 2, 0, 256, 0, 8, 32, 16, 16 };

// The BGs, cell actors and 3D camera of the regional Pokedex diploma and its vblank task, pokebw2's
// chihou_zukan_award_graphic.c
typedef struct {
    u32 unk0;
} ChihouZukanAwardBG;

typedef struct {
    ClActUnit *unit;
} ChihouZukanAwardClAct;

typedef struct {
    G3DCamera *camera;
} ChihouZukanAwardG3dSys;

struct ChihouZukanAwardGraphic {
    ChihouZukanAwardBG bg;
    ChihouZukanAwardClAct clact;
    ChihouZukanAwardG3dSys g3d;
    TCB *vblankTask;
};

void func_ov221_021b9fb0(ChihouZukanAwardBG *bg, HeapID heapId);
void func_ov221_021ba054(ChihouZukanAwardClAct *clact, const BGSysVRAMConfig *vramConfig, HeapID heapId);
void func_ov221_021ba0c4(ChihouZukanAwardG3dSys *g3d, HeapID heapId);
void func_ov221_021b9f9c(TCB *tcb, void *data);
void func_ov221_021ba130(ChihouZukanAwardG3dSys *g3d);
void func_ov221_021ba094(ChihouZukanAwardClAct *clact);
void func_ov221_021ba010(ChihouZukanAwardBG *bg);
void func_ov221_021ba0b0(ChihouZukanAwardClAct *clact);
void func_ov221_021ba048(ChihouZukanAwardBG *bg);
ClActUnit *func_ov221_021ba0c0(ChihouZukanAwardClAct *clact);
void func_ov221_021ba04c(ChihouZukanAwardBG *bg);
void func_ov221_021ba0b8(ChihouZukanAwardClAct *clact);
static const ClActSysSetup data_ov221_021ba1e4 = { 0, 0, 0, 512, 4, 124, 4, 124, 0, 32, 32, 32, 32, 16, 16 };
void func_ov221_021b9dd4(void);
static const VecFx32 data_ov221_021ba1bc = { 0x6b33, 0x6b33, 0x114cd };
static const VecFx32 data_ov221_021ba1b0 = { 0, 0x299a, 0 };
static const VecFx32 data_ov221_021ba1c8 = { 0, 0x1000, 0 };

typedef struct {
    u32 unk_0;
    BGSetup setup;
    u32 mode;
    u32 enabled;
} Ov221Entry021ba250;
const Ov221Entry021ba250 data_ov221_021ba250[2] = {
    { 1, { 0, 0, 0x1000, 0, 2, 1, 2, 1, 0x10000, 0, 1, 0, 0 }, 0, 1 },
    { 2, { 0, 0, 0x1000, 0, 2, 0, 4, 5, 0x8000, 0, 2, 0, 0 }, 0, 1 },
};
static const BGSysLCDConfig data_ov221_021ba1d4 = { 1, 0, 0, 1 };
static const Light data_ov221_021ba200[4] = {
    { { 0, -0x1000, 0 }, 0x4210 },
    { { 0, 0x1000, 0 }, 0x4210 },
    { { 0, -0x1000, 0 }, 0x4210 },
    { { 0, -0x1000, 0 }, 0x4210 },
};


void func_ov221_021b9dd4(void) {
    u32 i;

    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG0, TRUE);
    G2_SetBG0Priority(0);
    G3X_SetShading(GX_SHADING_HIGHLIGHT);
    G3X_AntiAlias(FALSE);
    G3X_AlphaTest(FALSE, 0);
    G3X_AlphaBlend(TRUE);
    G3X_EdgeMarking(FALSE);
    G3X_SetFog(FALSE, 0, 0, 0);
    G3X_SetClearColor(GX_RGB(0, 0, 0), 0, 0x7fff, 63, FALSE);
    G3_ViewPort(0, 0, 255, 191);
    for (i = 0; i < 4; i++) {
        GFL_G3DSysLightSet(i, &data_ov221_021ba200[i]);
    }
    GFL_G3DSysSetSwapBufferParams(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
}


ChihouZukanAwardGraphic *func_ov221_021b9e78(u32 layout, HeapID heapId) {
    ChihouZukanAwardGraphic *graphic;
    TCB *task;

    graphic = GFL_HeapAllocate(heapId, sizeof(ChihouZukanAwardGraphic), FALSE, "chihou_zukan_award_graphic.c", 0x1cc);
    MI_CpuFill8(graphic, 0, sizeof(ChihouZukanAwardGraphic));
    G2_BlendNone();
    G2S_BlendNone();
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    GFL_BGSysInitVRAM(0);
    GFL_BGSysSetVRAMBanks(&data_ov221_021ba220);
    GFL_BGSysSetDisplayLayout(layout);
    GFL_BGSysEnableEngines();
    GFL_BGSysDisableAllA();
    GFL_BGSysDisableAllB();
    func_0201dd60();
    func_ov221_021b9fb0(&graphic->bg, heapId);
    func_ov221_021ba054(&graphic->clact, &data_ov221_021ba220, heapId);
    func_ov221_021ba0c4(&graphic->g3d, heapId);
    task = GFL_VBlankTCBAdd(func_ov221_021b9f9c, graphic, 0);
    graphic->vblankTask = task;
    return graphic;
}

void func_ov221_021b9f20(ChihouZukanAwardGraphic *graphic) {
    GFL_TCBRemove(graphic->vblankTask);
    func_ov221_021ba130(&graphic->g3d);
    func_ov221_021ba094(&graphic->clact);
    func_ov221_021ba010(&graphic->bg);
    func_0201dd68();
    G2_BlendNone();
    G2S_BlendNone();
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    MI_CpuFill8(graphic, 0, sizeof(ChihouZukanAwardGraphic));
    GFL_HeapFree(graphic);
}

void func_ov221_021b9f7c(ChihouZukanAwardGraphic *graphic) {
    func_ov221_021ba0b0(&graphic->clact);
    func_ov221_021ba048(&graphic->bg);
}

ClActUnit *func_ov221_021b9f90(ChihouZukanAwardGraphic *graphic) {
    return func_ov221_021ba0c0(&graphic->clact);
}

void func_ov221_021b9f9c(TCB *tcb, void *data) {
    ChihouZukanAwardGraphic *graphic = data;

    func_ov221_021ba04c(&graphic->bg);
    func_ov221_021ba0b8(&graphic->clact);
}

void func_ov221_021b9fb0(ChihouZukanAwardBG *bg, HeapID heapId) {
    u32 i;
    const Ov221Entry021ba250 *entry;

    MI_CpuFill8(bg, 0, sizeof(ChihouZukanAwardBG));
    GFL_BGSysCreate(heapId);
    BmpWin_InitAllocator(heapId);
    GFL_BGSysSetLCDConfig(&data_ov221_021ba1d4);
    for (i = 0; i < 2; i++) {
        entry = &data_ov221_021ba250[i];
        u32 frame;
        frame = entry->unk_0;
        GFL_BGSysCreateBG(frame, &entry->setup, entry->mode);
        GFL_BGSysClearBG(frame);
        GFL_BGSysSetBGEnabled(frame, entry->enabled);
    }
}


void func_ov221_021ba010(ChihouZukanAwardBG *bg) {
    u32 i;

    for (i = 0; i < 2; i++) {
        GFL_BGSysReleaseBG(data_ov221_021ba250[i].unk_0);
    }
    BmpWin_FreeAllocator();
    GFL_BGSysFree();
    MI_CpuFill8(bg, 0, sizeof(ChihouZukanAwardBG));
}


void func_ov221_021ba048(ChihouZukanAwardBG *bg) {
}

void func_ov221_021ba04c(ChihouZukanAwardBG *bg) {
    GFL_BGSysUpdate();
}

void func_ov221_021ba054(ChihouZukanAwardClAct *clact, const BGSysVRAMConfig *vramConfig, HeapID heapId) {
    ClActUnit *unit;

    MI_CpuFill8(clact, 0, sizeof(ChihouZukanAwardClAct));
    ClActSys_Create(&data_ov221_021ba1e4, vramConfig, heapId);
    unit = func_0204b100(0x80, 0, heapId);
    clact->unit = unit;
    func_0204b270(unit);
    GFL_BGSysSetBGEnabledA(0x10, 1);
    GFL_BGSysSetBGEnabledB(0x10, 1);
}

void func_ov221_021ba094(ChihouZukanAwardClAct *clact) {
    func_0204b1cc(clact->unit);
    func_0204a5ac();
    MI_CpuFill8(clact, 0, sizeof(ChihouZukanAwardClAct));
}

void func_ov221_021ba0b0(ChihouZukanAwardClAct *clact) {
    func_0204a600();
}

void func_ov221_021ba0b8(ChihouZukanAwardClAct *clact) {
    func_0204a648();
}



ClActUnit *func_ov221_021ba0c0(ChihouZukanAwardClAct *clact) {
    return clact->unit;
}

void func_ov221_021ba0c4(ChihouZukanAwardG3dSys *g3d, HeapID heapId) {
    GFL_G3DSysCreate(0, 1, 0, 1, 0, heapId, func_ov221_021b9dd4);
    g3d->camera = GFL_G3DCameraCreate(G3DCAM_PROJECTION_PERSPECTIVE, FX_SinIdx(DEG_TO_IDX(20)), FX_CosIdx(DEG_TO_IDX(20)),
                                      0x1555, 0, 0x1000, 0x400000, 0, &data_ov221_021ba1bc, &data_ov221_021ba1b0,
                                      &data_ov221_021ba1c8, heapId);
}

void func_ov221_021ba130(ChihouZukanAwardG3dSys *g3d) {
    GFL_G3DCameraFree(g3d->camera);
    GFL_G3DSysFree();
}
