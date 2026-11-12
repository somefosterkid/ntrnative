// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov194/pokemontrade_3d.c, ported by tools/port_file.py.
#include "types.h"
#include "app/pokemon_trade_local.h"
#include "constants/pokemon.h"
#include "constants/species.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/dwc_rap.h"
#include "gfl/g3d.h"
#include "gfl/graphics.h"
#include "gfl/heap.h"
#include "gfl/particle.h"
#include "gfl/std.h"
#include "gfl/tcb.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_trig.h"
#include "nitro/fx/fx_mtx.h"
#include "nitro/gx.h"
#include "nitro/os.h"
#include "pml/personal.h"
#include "pml/poke_party.h"
#include "system/mcss.h"
#include "work_area.h"

// The trade's 3D: the scenes, camera paths and particles of the trade demo, the Pokémon's sprites, and the ring of
// cubes in the box strip, one for the partner's party and one for each of their boxes, textured with the colours of
// their slots

// The work that each particle system draws in
// The SPL manager's work, whose emitters and particles hold pointers
#define PARTICLE_WORK_SIZE WORK_AREA_SIZE(0x4800)

// The cubes' textures, 32 by 32 pixels of 16 colours, from this address of texture VRAM on, and their palette
#define CUBE_TEX_SIZE 0x200
#define CUBE_TEX_ADDR 0x3000
#define CUBE_PLTT_ADDR 0x2000

// No scene is loaded
#define SCENE_NONE 0xffff

// A scene's camera setup, and what it draws each frame
typedef struct {
    void (*init)(PokemonTradeWork *wk);
    void (*draw)(PokemonTradeWork *wk, SRTMatrix *mtx);
} TradeSceneFuncs;

// Where a species' sprite moves to, for the sexes and directions the entry takes
typedef struct {
    u32 species;
    u32 form;
    // A sex, or 3 for any
    u32 sex;
    // 0 for a sprite facing front, 1 for one facing back, or 2 for either
    u32 dir;
    f32 x;
    f32 y;
    f32 z;
} TradeSpriteOffset;

// How the box cubes are turned and how far from the ring's centre they are, and the step of the scenes' animations
typedef struct {
    u16 rotY;
    u16 rotX;
    fx32 step;
    fx32 distance;
} TradeCubeParams;

static void func_ov119_021e2b14(HeapID heapId);
static void func_ov119_021e2b68(void);
void func_ov119_021e2b78(PokemonTradeWork *wk);
void func_ov119_021e2bd0(PokemonTradeWork *wk);
void func_ov119_021e2c34(PokemonTradeWork *wk, SRTMatrix *mtx);
void func_ov119_021e2c3c(PokemonTradeWork *wk, SRTMatrix *mtx);
static void func_ov119_021e2ea0(PokemonTrade3DWork *work);
void func_ov119_021e324c(PokemonTradeWork *wk, int sceneId);
void func_ov119_021e32c4(PokemonTradeWork *wk);
static void func_ov119_021e32ec(PokemonTradeWork *wk);
static void func_ov119_021e33fc(MCSS *mcss, const VecFx32 *scale, BOOL front, PartyPkm *pkm);
void func_ov119_021e3700(int vertex);
void func_ov119_021e3730(int normal);
static void func_ov119_021e3744(int texCoord);
static void func_ov119_021e3758(u16 *tex, int col, int row, u8 color);
void func_ov119_021e37d8(u16 *tex, const u8 (*boxes)[30], int box, int unused);
static void func_ov119_021e380c(u16 *tex, const u8 *party, int unused);
void func_ov119_021e3838(PokemonTradeWork *wk, int count);
static void func_ov119_021e38d8(BOOL vtxColor, BOOL shininess);
static void func_ov119_021e3910(fx32 distance, int index, u16 angle, u16 rotation);
void func_ov119_021e3a40(PokemonTradeWork *wk);
static void func_ov119_021e3b2c(PokemonTradeWork *wk);

// Nothing in the ROM reads it (pokebw2's data_ov194_021c679c): as an unused global it is the file's first .rodata
const u32 data_ov119_021e7998 = 16;

// pokebw2's sLightSetups
static const LightSetup data_ov119_021e7a9c[4] = {
    { 0, { { -(FX16_ONE - 1), -(FX16_ONE - 1), -(FX16_ONE - 1) }, GX_RGB(31, 31, 31) } },
    { 1, { { FX16_ONE - 1, -(FX16_ONE - 1), -(FX16_ONE - 1) }, GX_RGB(31, 31, 31) } },
    { 2, { { -(FX16_ONE - 1), -(FX16_ONE - 1), -(FX16_ONE - 1) }, GX_RGB(31, 31, 31) } },
    { 3, { { -(FX16_ONE - 1), -(FX16_ONE - 1), -(FX16_ONE - 1) }, GX_RGB(31, 31, 31) } },
};

// pokebw2's sLightSetupList
static const LightSetupList data_ov119_021e799c = { data_ov119_021e7a9c, NELEMS(data_ov119_021e7a9c) };

// pokebw2's sSceneAnimations
static const G3DSceneAnimationSetup data_ov119_021e79a4[2] = { { 1, 0 }, { 2, 0 } };

// pokebw2's sCameraUp
static const VecFx32 data_ov119_021e79ac = { 0, FX32_ONE, 0 };

// pokebw2's sSceneActor
static const G3DSceneActorSetup data_ov119_021e79b8 = { 0, 0, 0, 0, data_ov119_021e79a4, NELEMS(data_ov119_021e79a4) };

// pokebw2's sScene1Resources
static const G3DSceneResourceSetup data_ov119_021e79e8[3] = {
    { 0x67, 0x17, 0, G3D_SCENE_RES_ARCSYS },
    { 0x67, 0x16, 0, G3D_SCENE_RES_ARCSYS },
    { 0x67, 0x18, 0, G3D_SCENE_RES_ARCSYS },
};

// pokebw2's sScene2Resources
static const G3DSceneResourceSetup data_ov119_021e7a0c[3] = {
    { 0x68, 0x6, 0, G3D_SCENE_RES_ARCSYS },
    { 0x68, 0x5, 0, G3D_SCENE_RES_ARCSYS },
    { 0x68, 0x7, 0, G3D_SCENE_RES_ARCSYS },
};

// pokebw2's sScene3Resources
static const G3DSceneResourceSetup data_ov119_021e7a30[3] = {
    { 0x67, 0x2f, 0, G3D_SCENE_RES_ARCSYS },
    { 0x67, 0x2e, 0, G3D_SCENE_RES_ARCSYS },
    { 0x67, 0x30, 0, G3D_SCENE_RES_ARCSYS },
};

// pokebw2's sScene4Resources
static const G3DSceneResourceSetup data_ov119_021e7a54[3] = {
    { 0x67, 0x2c, 0, G3D_SCENE_RES_ARCSYS },
    { 0x67, 0x2b, 0, G3D_SCENE_RES_ARCSYS },
    { 0x67, 0x2d, 0, G3D_SCENE_RES_ARCSYS },
};

// pokebw2's sScene5Resources
static const G3DSceneResourceSetup data_ov119_021e7a78[3] = {
    { 0x67, 0x21, 0, G3D_SCENE_RES_ARCSYS },
    { 0x67, 0x20, 0, G3D_SCENE_RES_ARCSYS },
    { 0x67, 0x22, 0, G3D_SCENE_RES_ARCSYS },
};

static const Light data_ov119_021e79c8[4] = {
    { { 0, -FX16_ONE, 0 }, GX_RGB(16, 16, 16) },
    { { 0, FX16_ONE, 0 }, GX_RGB(16, 16, 16) },
    { { 0, -FX16_ONE, 0 }, GX_RGB(16, 16, 16) },
    { { 0, -FX16_ONE, 0 }, GX_RGB(16, 16, 16) },
};

// Scene 0 has nothing to load, and its camera looks at the box cubes
static const TradeSceneFuncs data_ov119_021e7ac4[6] = {
    { func_ov119_021e2b78, func_ov119_021e2c34 }, { func_ov119_021e2bd0, func_ov119_021e2c3c },
    { func_ov119_021e2bd0, func_ov119_021e2c3c }, { func_ov119_021e2bd0, func_ov119_021e2c3c },
    { func_ov119_021e2bd0, func_ov119_021e2c3c }, { func_ov119_021e2bd0, func_ov119_021e2c3c },
};

static const G3DSceneSetup data_ov119_021e7b3c[6] = {
    { NULL, 0, NULL, 0 },
    { data_ov119_021e79e8, NELEMS(data_ov119_021e79e8), &data_ov119_021e79b8, 1 },
    { data_ov119_021e7a0c, NELEMS(data_ov119_021e7a0c), &data_ov119_021e79b8, 1 },
    { data_ov119_021e7a30, NELEMS(data_ov119_021e7a30), &data_ov119_021e79b8, 1 },
    { data_ov119_021e7a54, NELEMS(data_ov119_021e7a54), &data_ov119_021e79b8, 1 },
    { data_ov119_021e7a78, NELEMS(data_ov119_021e7a78), &data_ov119_021e79b8, 1 },
};

// pokebw2's sSpriteOffsets
static const TradeSpriteOffset data_ov119_021e7b9c[4] = {
    { SPECIES_FURRET, 0, 3, 1, -2.08f, 0.0f, 0.0f },
    { SPECIES_DRIFLOON, 0, 3, 0, 6.66f, 0.0f, 0.0f },
    { SPECIES_MILOTIC, 0, 3, 0, 4.36f, 0.1f, 0.0f },
    // The ROM's x is 0x40533334, one step above the float nearest 3.3
    { SPECIES_KADABRA, 0, 3, 0, 3.3000002f, -0.1f, 0.0f },
};

// pokebw2's sCameraPos
static VecFx32 data_ov119_021e815c = { 0, 0x100, 0x3000 };

// pokebw2's sCameraTarget
static VecFx32 data_ov119_021e8150 = { 0, 0xaa0, 0 };

static TradeCubeParams data_ov119_021e8144 = { 0x8000, 0x299a, FX32_ONE, 0x13cd };

// pokebw2's sCubeVertices
static fx16 data_ov119_021e8190[8 * 3] = {
    FX16_ONE, FX16_ONE,  FX16_ONE,  FX16_ONE,  FX16_ONE,  -FX16_ONE, FX16_ONE,  -FX16_ONE,
    FX16_ONE, FX16_ONE,  -FX16_ONE, -FX16_ONE, -FX16_ONE, FX16_ONE,  FX16_ONE,  -FX16_ONE,
    FX16_ONE, -FX16_ONE, -FX16_ONE, -FX16_ONE, FX16_ONE,  -FX16_ONE, -FX16_ONE, -FX16_ONE,
};

// pokebw2's sCubeTexCoords
static u32 data_ov119_021e8168[4] = {
    GX_ST(0, 0),
    GX_ST(0, FX32_CONST(32)),
    GX_ST(FX32_CONST(32), 0),
    GX_ST(FX32_CONST(32), FX32_CONST(32)),
};

static u32 data_ov119_021e8178[6] = {
    GX_VECFX10(0, 0, GX_FX16_FX10(FX16_ONE - 1)), GX_VECFX10(0, GX_FX16_FX10(FX16_ONE - 1), 0),
    GX_VECFX10(GX_FX16_FX10(FX16_ONE - 1), 0, 0), GX_VECFX10(0, 0, GX_FX16_FX10(-FX16_ONE)),
    GX_VECFX10(0, GX_FX16_FX10(-FX16_ONE), 0),    GX_VECFX10(GX_FX16_FX10(-FX16_ONE), 0, 0),
};

static G3DLight *data_ov119_021f4560;

static void func_ov119_021e2b14(HeapID heapId) {
    data_ov119_021f4560 = GFL_G3DLightCreate(&data_ov119_021e799c, heapId);
    GFL_G3DLightFlush(data_ov119_021f4560);
    {
        G3DCameraProjection projection = {
            G3DCAM_PROJECTION_ORTHO,
            FX_SinIdx(DEG_TO_IDX(16)),
            FX_CosIdx(DEG_TO_IDX(16)),
            FX32_CONST(4.0 / 3.0),
            0,
            FX32_ONE,
            FX32_CONST(400),
            0,
        };
        GFL_G3DSysMtxSetProjection(&projection);
    }
}

static void func_ov119_021e2b68(void) {
    GFL_G3DLightFree(data_ov119_021f4560);
}

void func_ov119_021e2b78(PokemonTradeWork *wk) {
    VecFx32 pos;
    VecFx32 vec;

    pos.x = 0;
    pos.y = FX32_CONST(43);
    pos.z = FX32_CONST(241);
    vec.x = 0;
    vec.y = FX32_CONST(43);
    vec.z = 0;
    GFL_G3DCameraSetLookatPos(wk->camera, &pos);
    GFL_G3DCameraSetLookatTarget(wk->camera, &vec);
    pos.z -= FX32_ONE;
    GFL_G3DCameraSetProjectionZFar(wk->camera, &pos.z);
    vec.x = 0;
    vec.y = FX32_ONE;
    vec.z = 0;
    GFL_G3DCameraSetLookatUpVector(wk->camera, &vec);
}

void func_ov119_021e2bd0(PokemonTradeWork *wk) {
    VecFx32 pos;
    VecFx32 vec;
    fx32 far;
    fx32 near;

    near = FX32_ONE;
    far = FX32_CONST(8000);
    pos.x = 0;
    pos.y = FX32_CONST(3);
    pos.z = FX32_CONST(15);
    vec.x = 0;
    vec.y = 0;
    vec.z = 0;
    GFL_G3DCameraSetLookatPos(wk->camera, &pos);
    GFL_G3DCameraSetLookatTarget(wk->camera, &vec);
    GFL_G3DCameraSetProjectionZNear(wk->camera, &near);
    GFL_G3DCameraSetProjectionZFar(wk->camera, &far);
    vec.x = 0;
    vec.y = FX32_CONST(16);
    vec.z = FX32_ONE;
    GFL_G3DCameraSetLookatUpVector(wk->camera, &vec);
}



void func_ov119_021e2c34(PokemonTradeWork *wk, SRTMatrix *mtx) {
    func_ov119_021e3a40(wk);
}

void func_ov119_021e2c3c(PokemonTradeWork *wk, SRTMatrix *mtx) {
    u32 i;

    mtx->translation.x = 0;
    mtx->translation.y = 0;
    mtx->translation.z = 0;
    mtx->scale.x = FX32_ONE;
    mtx->scale.y = FX32_ONE;
    mtx->scale.z = FX32_ONE;
    MTX_Identity33_(&mtx->rotation);
    if (wk->work3D != NULL) {
        if (!wk->work3D->cameraPosDone) {
            GFL_G3DCurveApplyCameraPosTranslation(wk->camera, wk->work3D->cameraPosPath);
        }
        if (!wk->work3D->cameraTargetDone) {
            GFL_G3DCurveApplyCameraTgtTranslation(wk->camera, wk->work3D->cameraTargetPath);
        }
    }
    GFL_G3DCameraFlush(wk->camera);
    G3X_SetShading(GX_SHADING_HIGHLIGHT);
    G3X_AntiAlias(TRUE);
    G3X_AlphaTest(FALSE, 0);
    G3X_AlphaBlend(FALSE);
    G3X_EdgeMarking(FALSE);
    G3X_SetFog(TRUE, 0, 0, 0);
    G3X_SetClearColor(GX_RGB(0, 0, 0), 0, 0x7fff, 63, FALSE);
    G3_ViewPort(0, 0, 255, 191);
    for (i = 0; i < NELEMS(data_ov119_021e79c8); i++) {
        GFL_G3DSysLightSet(i, &data_ov119_021e79c8[i]);
    }
    if (wk->work3D != NULL) {
        if (wk->work3D->emitters[0] != NULL) {
            VecFx32 pos;

            GFL_G3DCurveGetNowTranslation(wk->work3D->emitterPaths[0], &pos);
            func_02051838(wk->work3D->emitters[0], &pos);
        }
        if (wk->work3D->emitters[1] != NULL) {
            VecFx32 pos;

            GFL_G3DCurveGetNowTranslation(wk->work3D->emitterPaths[1], &pos);
            func_02051838(wk->work3D->emitters[1], &pos);
        }
    }
}


void func_ov119_021e2d70(PokemonTrade3DWork *work, int path) {
    switch (path) {
    case 0:
        work->cameraPosPath = GFL_G3DCurveLoadFileStream(work->heapId, 0x67, 0x38, 10);
        work->cameraTargetPath = GFL_G3DCurveLoadFileStream(work->heapId, 0x67, 0x39, 10);
        work->emitterPaths[0] = GFL_G3DCurveLoadFileStream(work->heapId, 0x67, 0x36, 10);
        work->emitterPaths[1] = GFL_G3DCurveLoadFileStream(work->heapId, 0x67, 0x37, 10);
        break;
    case 1:
        work->cameraPosPath = GFL_G3DCurveLoadFileStream(work->heapId, 0x67, 0x3f, 10);
        work->cameraTargetPath = GFL_G3DCurveLoadFileStream(work->heapId, 0x67, 0x3e, 10);
        work->emitterPaths[0] = GFL_G3DCurveLoadFileStream(work->heapId, 0x67, 0x36, 10);
        break;
    case 2:
        work->cameraPosPath = GFL_G3DCurveLoadFileStream(work->heapId, 0x67, 0x3d, 10);
        work->cameraTargetPath = GFL_G3DCurveLoadFileStream(work->heapId, 0x67, 0x3c, 10);
        break;
    case 3:
        work->cameraPosPath = GFL_G3DCurveLoadFileStream(work->heapId, 0x67, 0x3b, 10);
        work->cameraTargetPath = GFL_G3DCurveLoadFileStream(work->heapId, 0x67, 0x3a, 10);
        work->emitterPaths[1] = GFL_G3DCurveLoadFileStream(work->heapId, 0x67, 0x37, 10);
        GFL_G3DCurveFrameSet(work->emitterPaths[1], FX32_CONST(693));
        break;
    }
}

static void func_ov119_021e2ea0(PokemonTrade3DWork *work) {
    if (work->cameraPosPath != NULL) {
        GFL_G3DCurveFree(work->cameraPosPath);
        work->cameraPosPath = NULL;
    }
    if (work->cameraTargetPath != NULL) {
        GFL_G3DCurveFree(work->cameraTargetPath);
        work->cameraTargetPath = NULL;
    }
    if (work->emitterPaths[0] != NULL) {
        GFL_G3DCurveFree(work->emitterPaths[0]);
        work->emitterPaths[0] = NULL;
    }
    if (work->emitterPaths[1] != NULL) {
        GFL_G3DCurveFree(work->emitterPaths[1]);
        work->emitterPaths[1] = NULL;
    }
}

void func_ov119_021e2efc(PokemonTrade3DWork *work) {
    work->cameraPosPath = GFL_G3DCurveLoadFileStream(work->heapId, 0x68, 0xb, 10);
    work->cameraTargetPath = GFL_G3DCurveLoadFileStream(work->heapId, 0x68, 0xc, 10);
}

void func_ov119_021e2f2c(PokemonTrade3DWork *work) {
    func_ov119_021e2ea0(work);
}

void func_ov119_021e2f34(PokemonTrade3DWork *work) {
    int i;
    BOOL camera;

    func_02050b80(work->heapId);
    work->resource = NULL;
    for (i = 0; i < 9; i++) {
        work->systemWork[i] = GFL_HeapAllocate(work->heapId, PARTICLE_WORK_SIZE, TRUE, "pokemontrade_3d.c", 513);
        switch (i) {
        case 0:
        case 1:
        case 2:
        case 5:
        case 6:
        case 8:
            camera = FALSE;
            break;
        default:
            camera = TRUE;
            break;
        }
        work->systems[i] = func_02050c04(work->systemWork[i], PARTICLE_WORK_SIZE, camera, work->heapId);
    }
}

void func_ov119_021e2fb0(PokemonTrade3DWork *work) {
    int i;

    work->resource = func_0205121c(0x67, 0x35, HEAPID_TAIL(work->heapId));
    for (i = 0; i < 4; i++) {
        OS_WaitInterrupt(TRUE, OS_IE_V_BLANK);
        func_02051360(work->systems[i], work->resource, TRUE, GFL_VBlankGetTCBMgr());
    }
}

void func_ov119_021e3004(PokemonTrade3DWork *work) {
    int i;

    for (i = 4; i < 9; i++) {
        OS_WaitInterrupt(TRUE, OS_IE_V_BLANK);
        func_02051360(work->systems[i], work->resource, TRUE, GFL_VBlankGetTCBMgr());
    }
}

void func_ov119_021e3034(PokemonTrade3DWork *work, int count) {
    int i;

    func_02050ed4();
    if (work->resource != NULL) {
        GFL_HeapFree(work->resource);
        work->resource = NULL;
    }
    for (i = 0; i < 9; i++) {
        if (work->systemWork[i] != NULL) {
            GFL_HeapFree(work->systemWork[i]);
            work->systemWork[i] = NULL;
        }
    }
}

void func_ov119_021e3070(PokemonTrade3DWork *work) {
    int i;
    void *resource;

    func_02050b80(work->heapId);
    for (i = 0; i < 6; i++) {
        work->systemWork[i] = GFL_HeapAllocate(work->heapId, PARTICLE_WORK_SIZE, TRUE, "pokemontrade_3d.c", 627);
        work->systems[i] = func_02050c04(work->systemWork[i], PARTICLE_WORK_SIZE, FALSE, work->heapId);
    }
    resource = func_0205121c(0x68, 0xa, work->heapId);
    for (i = 0; i < 6; i++) {
        func_02051230(work->systems[i], resource, TRUE, NULL);
    }
}

void func_ov119_021e30ec(PokemonTradeWork *wk) {
    wk->scene = SCENE_NONE;
    wk->sceneId = -1;
    func_ov119_021e2b14(0x91);
    wk->sceneMgr = GFL_G3DMgrCreate(20, 20, 0x91);
    if (wk->camera == NULL) {
        VecFx32 pos = { 0, 0, 0 };
        VecFx32 target = { 0, 0, 0 };
        HeapID heapId = wk->heapId;
        VecFx32 up = { 0, FX32_ONE, 0 };

        wk->camera = GFL_G3DCameraCreate(G3DCAM_PROJECTION_PERSPECTIVE, FX_SinIdx(DEG_TO_IDX(20)),
                                         FX_CosIdx(DEG_TO_IDX(20)), FX32_CONST(4.0 / 3.0), 0, FX32_ONE, FX32_CONST(1024),
                                         0, &pos, &up, &target, heapId);
    }
    if (wk->sceneId != -1) {
        data_ov119_021e7ac4[wk->sceneId].init(wk);
    }
}


void func_ov119_021e31b4(PokemonTradeWork *wk) {
    func_ov119_021e32ec(wk);
}

void func_ov119_021e31bc(PokemonTradeWork *wk) {
    if (wk->camera != NULL) {
        GFL_G3DCameraFree(wk->camera);
    }
    GFL_G3DMgrFree(wk->sceneMgr);
    if (wk->work3D != NULL) {
        func_ov119_021e3034(wk->work3D, 6);
        func_ov119_021e2f2c(wk->work3D);
        GFL_HeapFree(wk->work3D);
        wk->work3D = NULL;
    }
    func_ov119_021e2b68();
}


void func_ov119_021e31fc(PokemonTradeWork *wk) {
    func_ov119_021e3838(wk, 25);
}

void func_ov119_021e3208(PokemonTradeWork *wk, int sceneId) {
    func_ov119_021e324c(wk, sceneId);
    if (wk->sceneId != -1) {
        data_ov119_021e7ac4[wk->sceneId].init(wk);
    }
}


void func_ov119_021e3230(PokemonTradeWork *wk) {
    if (wk->sceneId != -1) {
        func_ov119_021e32c4(wk);
        wk->sceneId = -1;
    }
}

void func_ov119_021e324c(PokemonTradeWork *wk, int sceneId) {
    if (data_ov119_021e7b3c[sceneId].actorCount != 0) {
        wk->scene = GFL_G3DMgrNewScene(wk->sceneMgr, &data_ov119_021e7b3c[sceneId]);
    } else {
        wk->scene = SCENE_NONE;
    }
    if (wk->scene != SCENE_NONE) {
        G3DActor *actor = GFL_G3DMgrGetActor(wk->sceneMgr, wk->scene);
        int i;
        int count = GFL_G3DActorGetAnmCount(actor);

        for (i = 0; i < count; i++) {
            GFL_G3DActorBindAnm(actor, i);
        }
    }
    wk->sceneId = sceneId;
}



void func_ov119_021e32c4(PokemonTradeWork *wk) {
    if (wk->scene != SCENE_NONE) {
        GFL_G3DMgrDeleteScene(wk->sceneMgr, wk->scene);
    }
    wk->scene = SCENE_NONE;
}

static void func_ov119_021e32ec(PokemonTradeWork *wk) {
    SRTMatrix mtx;

    if (wk->sceneId != -1) {
        if (data_ov119_021e7ac4[wk->sceneId].draw != NULL) {
            data_ov119_021e7ac4[wk->sceneId].draw(wk, &mtx);
        }
        if (wk->scene != SCENE_NONE) {
            G3DActor *actor = GFL_G3DMgrGetActor(wk->sceneMgr, wk->scene);
            int count = GFL_G3DActorGetAnmCount(actor);
            int i;

            for (i = 0; i < count; i++) {
                GFL_G3DActorStepAnmFrameLoop(actor, i, data_ov119_021e8144.step);
            }
        }
        GFL_G3DSysReset();
        GFL_G3DCameraFlush(wk->camera);
        GFL_G3DSysMtxViewFlush();
        if (wk->work3D != NULL) {
            func_02051534();
            func_02051574();
        }
        if (wk->scene != SCENE_NONE) {
            GFL_G3DSysDrawObj(GFL_G3DMgrGetActor(wk->sceneMgr, wk->scene), &mtx);
        }
        if (wk->work3D != NULL) {
            if (wk->work3D->cameraPosPath != NULL) {
                if (!wk->work3D->cameraPosDone) {
                    wk->work3D->cameraPosDone = GFL_G3DCurveFrameStepLoop(wk->work3D->cameraPosPath, data_ov119_021e8144.step);
                }
                if (!wk->work3D->cameraTargetDone) {
                    wk->work3D->cameraTargetDone =
                        GFL_G3DCurveFrameStepLoop(wk->work3D->cameraTargetPath, data_ov119_021e8144.step);
                }
            }
            if (wk->work3D->emitterPaths[1] != NULL) {
                GFL_G3DCurveFrameStepLoop(wk->work3D->emitterPaths[1], data_ov119_021e8144.step);
            }
            if (wk->work3D->emitterPaths[0] != NULL) {
                GFL_G3DCurveFrameStepLoop(wk->work3D->emitterPaths[0], data_ov119_021e8144.step);
            }
        }
    }
}



// Moves the sprite to where its species is drawn off centre, or else by a third of its width toward the middle
static void func_ov119_021e33fc(MCSS *mcss, const VecFx32 *scale, BOOL front, PartyPkm *pkm) {
    VecFx32 offset;
    BOOL found;
    int dir = 0;
    u32 species = PokeParty_GetParam(pkm, PKM_PARAM_SPECIES, NULL);
    u32 form = PokeParty_GetParam(pkm, PKM_PARAM_FORM, NULL);
    u32 sex = PokeParty_GetSex(pkm);
    u8 i;

    if (!front) {
        dir = 1;
    }
    found = FALSE;
    for (i = 0; i < NELEMS(data_ov119_021e7b9c); i++) {
        if (species == data_ov119_021e7b9c[i].species && form == data_ov119_021e7b9c[i].form &&
            (data_ov119_021e7b9c[i].sex == 3 || sex == data_ov119_021e7b9c[i].sex) &&
            (data_ov119_021e7b9c[i].dir == 2 || dir == data_ov119_021e7b9c[i].dir)) {
            offset.x = FX32_CONST(data_ov119_021e7b9c[i].x);
            if (scale->x >= 0) {
                offset.x = -offset.x;
            }
            offset.y = FX32_CONST(data_ov119_021e7b9c[i].y);
            offset.z = FX32_CONST(data_ov119_021e7b9c[i].z);
            found = TRUE;
            break;
        }
    }
    if (!found) {
        int width;
        f32 x;

        // The result is left unused
        func_02015f04(mcss);
        width = func_02015f14(mcss);
        if (scale->x < 0) {
            x = width * 0.33f;
        } else {
            x = -width * 0.33f;
        }
        offset.x = FX32_CONST(x);
        offset.y = 0;
        offset.z = 0;
    }
    func_02015c78(mcss, &offset);
}

void func_ov119_021e35a0(PokemonTradeWork *wk, int side, BOOL front, PartyPkm *pkm, BOOL mirror) {
    VecFx32 scale = { FX32_CONST(16), FX32_CONST(16), FX32_ONE };
    fx32 x[4] = { FX32_CONST(-55), FX32_CONST(55), FX32_CONST(-55), FX32_CONST(55) };

    if (mirror) {
        u16 species = PokeParty_GetParam(pkm, PKM_PARAM_SPECIES, NULL);
        u16 form = PokeParty_GetParam(pkm, PKM_PARAM_FORM, NULL);
        void *personal = PML_PersonalLoad(species, form, wk->heapId);
        // The species' flag that its sprite is never mirrored
        u32 noMirror = PML_PersonalGetParam(personal, 34);

        PML_PersonalFree(personal);
        if (noMirror == FALSE) {
            scale.x = -scale.x;
        }
    }
    if (wk->type == 2) {
        func_ov009_0215642c(TRUE);
    }
    if (front) {
        wk->mcss[side] = func_02017270(wk->mcssSys, pkm, 0, x[side], FX32_CONST(-28), 0);
    } else {
        wk->mcss[side] = func_02017270(wk->mcssSys, pkm, 1, x[side], FX32_CONST(-28), 0);
    }
    if (wk->type == 2) {
        func_ov009_0215642c(FALSE);
    }
    func_020173b4(wk->mcss[side]);
    func_02015cac(wk->mcss[side], &scale);
    func_02015ff0(wk->mcss[side], 1);
    func_ov119_021e33fc(wk->mcss[side], &scale, front, pkm);
}


void func_ov119_021e36a8(PokemonTradeWork *wk, int side, BOOL front, PartyPkm *pkm, BOOL mirror, BOOL a5) {
    if (a5 && !front) {
        front = TRUE;
        mirror = FALSE;
    }
    func_ov119_021e35a0(wk, side, front, pkm, mirror);
}

void func_ov119_021e36c8(PokemonTradeWork *wk, int side, BOOL front, PartyPkm *pkm, BOOL mirror) {
    func_ov119_021e35a0(wk, side, front, pkm, mirror);
}

void func_ov119_021e36d8(PokemonTradeWork *wk, int side) {
    if (wk->mcss[side] != NULL) {
        func_02015ea0(wk->mcss[side]);
        func_02015ba4(wk->mcssSys, wk->mcss[side]);
        wk->mcss[side] = NULL;
    }
}


void func_ov119_021e3700(int vertex) {
    G3_Vtx(data_ov119_021e8190[vertex * 3], data_ov119_021e8190[vertex * 3 + 1], data_ov119_021e8190[vertex * 3 + 2]);
}

void func_ov119_021e3730(int normal) {
    reg_G3_NORMAL = data_ov119_021e8178[normal];
}

static void func_ov119_021e3744(int texCoord) {
    reg_G3_TEXCOORD = data_ov119_021e8168[texCoord];
}

// Paints a slot's 3 by 3 square of a box texture, the slots 5 pixels apart across and 6 down
static void func_ov119_021e3758(u16 *tex, int col, int row, u8 color) {
    int x;
    int y;
    int left = col * 5 + 2;
    int top = row * 6 + 3;

    for (y = top; y < top + 3; y++) {
        for (x = left; x < left + 3; x++) {
            int pixel = y * 32 + x;
            int shift = (pixel % 4) * 4;

            tex[pixel / 4] &= ~(0xf << shift);
            tex[pixel / 4] |= color << shift;
        }
    }
}

void func_ov119_021e37d8(u16 *tex, const u8 (*boxes)[30], int box, int unused) {
    int row;
    int col;

    for (row = 0; row < 5; row++) {
        for (col = 0; col < 6; col++) {
            func_ov119_021e3758(tex, col, row, boxes[box][row * 6 + col]);
        }
    }
}

static void func_ov119_021e380c(u16 *tex, const u8 *party, int unused) {
    int i;

    for (i = 0; i < 6; i++) {
        func_ov119_021e3758(tex, i % 2 + 2, i / 2 + 1, party[i]);
    }
}

void func_ov119_021e3838(PokemonTradeWork *wk, int count) {
    u16 *tex;
    int i;

    GX_BeginLoadTex();
    tex = GFL_HeapAllocate(wk->heapId, CUBE_TEX_SIZE, TRUE, "pokemontrade_3d.c", 1296);
    for (i = 0; i < count; i++) {
        MI_CpuFill8(tex, 0, CUBE_TEX_SIZE);
        if (i == 0) {
            func_ov119_021e380c(tex, wk->unk5B6, 2);
        } else {
            func_ov119_021e37d8(tex, wk->unk2E6, i - 1, 2);
        }
        DC_FlushRange(tex, CUBE_TEX_SIZE);
        GX_LoadTex(tex, i * CUBE_TEX_SIZE + CUBE_TEX_ADDR, CUBE_TEX_SIZE);
    }
    GFL_HeapFree(tex);
    GX_EndLoadTex();
    func_ov119_021e3b2c(wk);
}


static void func_ov119_021e38d8(BOOL vtxColor, BOOL shininess) {
    G3_MaterialColorDiffAmb(GX_RGB(23, 23, 23), GX_RGB(26, 26, 26), vtxColor);
    G3_MaterialColorSpecEmi(GX_RGB(31, 31, 31), GX_RGB(0, 0, 0), shininess);
}

// Draws the face of a cube with its texture, at the angle of the ring that the ring's rotation turns
static void func_ov119_021e3910(fx32 distance, int index, u16 angle, u16 rotation) {
    G3_PushMtx();
    G3_MtxMode(GX_MTXMODE_POSITION);
    G3_RotY(FX_SinIdx(rotation), FX_CosIdx(rotation));
    G3_PushMtx();
    G3_RotY(FX_SinIdx(angle), FX_CosIdx(angle));
    G3_Translate(0, 0, distance);
    G3_RotY(FX_SinIdx(data_ov119_021e8144.rotY), FX_CosIdx(data_ov119_021e8144.rotY));
    G3_RotX(FX_SinIdx(data_ov119_021e8144.rotX), FX_CosIdx(data_ov119_021e8144.rotX));
    G3_Scale(FX32_CONST(0.15), FX32_CONST(0.15), FX32_CONST(0.15));
    G3_MtxMode(GX_MTXMODE_TEXTURE);
    G3_Identity();
    G3_MtxMode(GX_MTXMODE_POSITION_VECTOR);
    func_ov119_021e38d8(TRUE, TRUE);
    G3_TexImageParam(GX_TEXFMT_PLTT16, GX_TEXGEN_TEXCOORD, GX_TEXSIZE_S32, GX_TEXSIZE_T32, GX_TEXREPEAT_NONE,
                     GX_TEXFLIP_NONE, GX_TEXPLTTCOLOR0_USE, index * CUBE_TEX_SIZE + CUBE_TEX_ADDR);
    G3_TexPlttBase(CUBE_PLTT_ADDR, GX_TEXFMT_PLTT16);
    G3_PolygonAttr(GX_LIGHTMASK_0, GX_POLYGONMODE_MODULATE, GX_CULL_FRONT, index, 31, 0);
    G3_Begin(GX_BEGIN_QUADS);
    func_ov119_021e3744(1);
    func_ov119_021e3730(0);
    func_ov119_021e3700(2);
    func_ov119_021e3744(0);
    func_ov119_021e3730(0);
    func_ov119_021e3700(0);
    func_ov119_021e3744(2);
    func_ov119_021e3730(0);
    func_ov119_021e3700(4);
    func_ov119_021e3744(3);
    func_ov119_021e3730(0);
    func_ov119_021e3700(6);
    G3_End();
    G3_PopMtx(1);
    G3_PopMtx(1);
}

void func_ov119_021e3a40(PokemonTradeWork *wk) {
    int boxCount;
    int i;
    int count;
    int width;
    int scroll;
    int x;
    int end;
    u32 pos;

    G3X_Reset();
    G3i_LookAt_(&data_ov119_021e815c, &data_ov119_021e79ac, &data_ov119_021e8150, TRUE, NULL);
    G3_LightVector(GX_LIGHTID_0, FX16_CONST(0.577), -FX16_CONST(0.577), -FX16_CONST(0.577));
    G3_LightColor(GX_LIGHTID_0, GX_RGB(31, 31, 31));
    boxCount = func_ov119_021d8a18(wk);
    count = boxCount + 1;
    width = count * 160;
    scroll = wk->unk107E;
    if (scroll < 96) {
        if (scroll < 48) {
            x = scroll * 112 / 48;
        } else {
            x = scroll + 64;
        }
    } else {
        x = scroll + 64;
    }
    end = boxCount * 160;
    if (x > end) {
        u32 over = x - end;

        if (over < 80) {
            x = end + over * 112 / 80;
        } else {
            x = end + (over - 80) * 48 / 80 + 112;
        }
    }
    x += 48;
    x %= width;
    pos = ((u64)x << 16) / width;
    for (i = 0; i < count; i++) {
        func_ov119_021e3910(data_ov119_021e8144.distance, i, (i << 16) / count, 0x10000 - pos);
    }
}


static void func_ov119_021e3b2c(PokemonTradeWork *wk) {
    NNSG2dPaletteData *palette;
    ArcTool *arc = GFL_ArcSysCreateFileHandle(0x74, wk->heapId);
    void *file = GFL_G2DIOReadNCLRArc(arc, 0x48, &palette, wk->heapId);
    u16 *colors = palette->pRawData;

    DC_FlushRange(&colors[16], 16 * sizeof(GXRgb));
    sys_memcpy_ex(&colors[16], &colors[17], 15 * sizeof(GXRgb));
    colors[16] = GX_RGB(5, 5, 5);
    DC_FlushRange(&colors[16], 16 * sizeof(GXRgb));
    GX_BeginLoadTexPltt();
    GX_LoadTexPltt(&colors[16], CUBE_PLTT_ADDR, 16 * sizeof(GXRgb));
    GX_EndLoadTexPltt();
    MI_CpuCopy8(&colors[16], wk->boxPalette, sizeof(wk->boxPalette));
    GFL_HeapFree(file);
    GFL_ArcToolFree(arc);
}


void func_ov119_021e3bb0(PokemonTradeWork *wk) {
    int i;
    int j;

    for (i = 0; i < 16; i++) {
        wk->boxPaletteDim[i] = 0;
        for (j = 0; j < 3; j++) {
            int c;
            int shift = j * 5;
            c = (wk->boxPalette[i] >> shift) & 0x1f;
            if (c != 0) {
                c--;
            }
            wk->boxPaletteDim[i] |= (c & 0x1f) << shift;
        }
    }
    GX_BeginLoadTexPltt();
    GX_LoadTexPltt(wk->boxPaletteDim, CUBE_PLTT_ADDR, sizeof(wk->boxPaletteDim));
    GX_EndLoadTexPltt();
}


