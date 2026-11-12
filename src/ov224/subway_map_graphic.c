// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov317/subway_map_graphic.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/subway_map.h"
#include "gfl/bg_sys.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/g3d.h"
#include "gfl/graphics.h"
#include "gfl/gx_layers.h"
#include "gfl/std.h"
#include "gfl/tcb.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_trig.h"
#include "nitro/gx.h"
#include "system/gf_font.h"

// The BGs, cell actors and 3D system of the subway map

typedef struct {
    u32 unk0;
} SubwayMapBG;

typedef struct {
    ClActUnit *unit;
} SubwayMapClAct;

typedef struct {
    G3DCamera *camera;
} SubwayMapG3dSys;

struct SubwayMapGraphic {
    SubwayMapBG bg;
    SubwayMapClAct clact;
    SubwayMapG3dSys g3d;
    TCB *vblankTask;
};

typedef struct {
    u32 bg;
    BGSetup setup;
    u32 mode;
    u32 enabled;
} SubwayMapBGSetup;

static void func_ov224_021b9a58(TCB *tcb, void *data);
static void func_ov224_021b9a6c(SubwayMapBG *bg, HeapID heapId);
static void func_ov224_021b9acc(SubwayMapBG *bg);
static void func_ov224_021b9b04(SubwayMapBG *bg);
static void func_ov224_021b9b08(SubwayMapBG *bg);
static void func_ov224_021b9b10(SubwayMapClAct *clact, const BGSysVRAMConfig *vramConfig, HeapID heapId);
static void func_ov224_021b9b50(SubwayMapClAct *clact);
static void func_ov224_021b9b6c(SubwayMapClAct *clact);
static void func_ov224_021b9b74(SubwayMapClAct *clact);
static void func_ov224_021b9b7c(SubwayMapG3dSys *g3d, HeapID heapId);
static void func_ov224_021b9be8(SubwayMapG3dSys *g3d);

// pokebw2's sSubwayMapCameraTarget
static const VecFx32 data_ov224_021b9c48 = { 0, FX32_ONE, 0 };
// pokebw2's sSubwayMapCameraPosition
static const VecFx32 data_ov224_021b9c3c = { FX32_CONST(6.7), FX32_CONST(6.7), FX32_CONST(17.3) };
// pokebw2's sSubwayMapCameraUpVector
static const VecFx32 data_ov224_021b9c30 = { 0, FX32_CONST(2.6), 0 };

// pokebw2's sSubwayMapLCDConfig
static const BGSysLCDConfig data_ov224_021b9c54 = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_3D };
// pokebw2's sSubwayMapClActSetup
static const ClActSysSetup data_ov224_021b9c64 = { 0, 0, 0, 512, 4, 124, 4, 124, 0, 32, 32, 32, 32, 16, 16 };

// pokebw2's sSubwayMapLights
static const Light data_ov224_021b9c80[] = {
    { { 0, -FX16_ONE, 0 }, GX_RGB(16, 16, 16) },
    { { 0, FX16_ONE, 0 }, GX_RGB(16, 16, 16) },
    { { 0, -FX16_ONE, 0 }, GX_RGB(16, 16, 16) },
    { { 0, -FX16_ONE, 0 }, GX_RGB(16, 16, 16) },
};

// pokebw2's sSubwayMapVRAMConfig
static const BGSysVRAMConfig data_ov224_021b9ca0 = {
    GX_VRAM_BG_128_A,  GX_VRAM_BGEXTPLTT_NONE,  GX_VRAM_SUB_BG_128_C,        GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_128_B, GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_16_I,        GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_0_D,   GX_VRAM_TEXPLTT_0_F,     GX_OBJVRAMMODE_CHAR_1D_128K, GX_OBJVRAMMODE_CHAR_1D_32K,
};

// pokebw2's sSubwayMapBGSetups
static const SubwayMapBGSetup data_ov224_021b9cd0[2] = {
    { 1,
      { 0, 0, 0x1000, 0, BGRES_256x512, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x1000, GX_BG_CHARBASE_0x04000, 0x8000,
        GX_BG_EXTPLTT_01, 1, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT,
      TRUE },
    { 2,
      { 0, 0, 0x1000, 0, BGRES_256x512, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x2000, GX_BG_CHARBASE_0x04000, 0x8000,
        GX_BG_EXTPLTT_01, 2, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT,
      TRUE },
};

// The 3D system calls this once it is set up
// pokebw2's SubwayMapGraphic_G3DInit
static void func_ov224_021b989c(void) {
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
    for (i = 0; i < NELEMS(data_ov224_021b9c80); i++) {
        GFL_G3DSysLightSet(i, &data_ov224_021b9c80[i]);
    }
    GFL_G3DSysSetSwapBufferParams(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
}

// pokebw2's SubwayMapGraphic_Create
SubwayMapGraphic *func_ov224_021b9940(u32 layout, HeapID heapId) {
    SubwayMapGraphic *graphic = GFL_HeapAllocate(heapId, sizeof(SubwayMapGraphic), FALSE, "subway_map_graphic.c", 460);

    MI_CpuFill8(graphic, 0, sizeof(SubwayMapGraphic));
    G2_BlendNone();
    G2S_BlendNone();
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    GFL_BGSysInitVRAM(GX_VRAM_LCDC_NONE);
    GFL_BGSysSetVRAMBanks(&data_ov224_021b9ca0);
    GFL_BGSysSetDisplayLayout(layout);
    GFL_BGSysEnableEngines();
    GFL_BGSysDisableAllA();
    GFL_BGSysDisableAllB();
    func_0201dd60();
    func_ov224_021b9a6c(&graphic->bg, heapId);
    func_ov224_021b9b10(&graphic->clact, &data_ov224_021b9ca0, heapId);
    func_ov224_021b9b7c(&graphic->g3d, heapId);
    graphic->vblankTask = GFL_VBlankTCBAdd(func_ov224_021b9a58, graphic, 0);
    return graphic;
}

// pokebw2's SubwayMapGraphic_Free
void func_ov224_021b99e8(SubwayMapGraphic *graphic) {
    GFL_TCBRemove(graphic->vblankTask);
    func_ov224_021b9be8(&graphic->g3d);
    func_ov224_021b9b50(&graphic->clact);
    func_ov224_021b9acc(&graphic->bg);
    func_0201dd68();
    G2_BlendNone();
    G2S_BlendNone();
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    MI_CpuFill8(graphic, 0, sizeof(SubwayMapGraphic));
    GFL_HeapFree(graphic);
}

// pokebw2's SubwayMapGraphic_Update
void func_ov224_021b9a44(SubwayMapGraphic *graphic) {
    func_ov224_021b9b6c(&graphic->clact);
    func_ov224_021b9b04(&graphic->bg);
}

// pokebw2's SubwayMapGraphic_VBlank
static void func_ov224_021b9a58(TCB *tcb, void *data) {
    SubwayMapGraphic *graphic = data;

    func_ov224_021b9b08(&graphic->bg);
    func_ov224_021b9b74(&graphic->clact);
}

// pokebw2's SubwayMapBG_Init
static void func_ov224_021b9a6c(SubwayMapBG *bg, HeapID heapId) {
    u32 i;

    MI_CpuFill8(bg, 0, sizeof(SubwayMapBG));
    GFL_BGSysCreate(heapId);
    BmpWin_InitAllocator(heapId);
    GFL_BGSysSetLCDConfig(&data_ov224_021b9c54);
    for (i = 0; i < NELEMS(data_ov224_021b9cd0); i++) {
        GFL_BGSysCreateBG(data_ov224_021b9cd0[i].bg, &data_ov224_021b9cd0[i].setup, data_ov224_021b9cd0[i].mode);
        GFL_BGSysClearBG(data_ov224_021b9cd0[i].bg);
        GFL_BGSysSetBGEnabled(data_ov224_021b9cd0[i].bg, data_ov224_021b9cd0[i].enabled);
    }
}

// pokebw2's SubwayMapBG_Free
static void func_ov224_021b9acc(SubwayMapBG *bg) {
    u32 i;

    for (i = 0; i < NELEMS(data_ov224_021b9cd0); i++) {
        GFL_BGSysReleaseBG(data_ov224_021b9cd0[i].bg);
    }
    BmpWin_FreeAllocator();
    GFL_BGSysFree();
    MI_CpuFill8(bg, 0, sizeof(SubwayMapBG));
}

// pokebw2's SubwayMapBG_Update
static void func_ov224_021b9b04(SubwayMapBG *bg) {
}

// pokebw2's SubwayMapBG_VBlank
static void func_ov224_021b9b08(SubwayMapBG *bg) {
    GFL_BGSysUpdate();
}

// pokebw2's SubwayMapClAct_Init
static void func_ov224_021b9b10(SubwayMapClAct *clact, const BGSysVRAMConfig *vramConfig, HeapID heapId) {
    MI_CpuFill8(clact, 0, sizeof(SubwayMapClAct));
    ClActSys_Create(&data_ov224_021b9c64, vramConfig, heapId);
    clact->unit = func_0204b100(128, 0, heapId);
    func_0204b270(clact->unit);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_OBJ, TRUE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
}

// pokebw2's SubwayMapClAct_Free
static void func_ov224_021b9b50(SubwayMapClAct *clact) {
    func_0204b1cc(clact->unit);
    func_0204a5ac();
    MI_CpuFill8(clact, 0, sizeof(SubwayMapClAct));
}

// pokebw2's SubwayMapClAct_Update
static void func_ov224_021b9b6c(SubwayMapClAct *clact) {
    func_0204a600();
}

// pokebw2's SubwayMapClAct_VBlank
static void func_ov224_021b9b74(SubwayMapClAct *clact) {
    func_0204a648();
}

// pokebw2's SubwayMapG3dSys_Init
static void func_ov224_021b9b7c(SubwayMapG3dSys *g3d, HeapID heapId) {
    GFL_G3DSysCreate(FALSE, 1, 0, 1, 0, heapId, func_ov224_021b989c);
    // A 40 degree field of view
    g3d->camera = GFL_G3DCameraCreate(G3DCAM_PROJECTION_PERSPECTIVE, FX_SinIdx(DEG_TO_IDX(20)),
                                      FX_CosIdx(DEG_TO_IDX(20)), FX32_CONST(4.0 / 3.0), 0, FX32_ONE, FX32_CONST(1024),
                                      0, &data_ov224_021b9c3c, &data_ov224_021b9c30, &data_ov224_021b9c48, heapId);
}

// pokebw2's SubwayMapG3dSys_Free
static void func_ov224_021b9be8(SubwayMapG3dSys *g3d) {
    GFL_G3DCameraFree(g3d->camera);
    GFL_G3DSysFree();
}
