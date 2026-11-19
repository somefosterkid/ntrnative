#include "global.h"

#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "overlay_42.h"
#include "palette.h"
#include "sprite.h"
#include "sprite_system.h"
#include "frontier_graphics.h"

struct BattleFrontier;

struct FrontierParticleSystem;

struct G3DPipelineBuffers;

struct UnkStruct_ov104_0223DA28_t;

typedef struct BattleFrontier BattleFrontier;

typedef struct FrontierParticleSystem FrontierParticleSystem;

typedef struct G3DPipelineBuffers G3DPipelineBuffers;

typedef struct UnkStruct_ov104_0223DA28_t UnkStruct_ov104_0223DA28;

UnkStruct_ov104_0223DA28 *ov80_02239D74(BgConfig *param0, PaletteData *param1);

void ov80_0222ADE8(FrontierGraphics *graphics, void **sceneData);

void ov80_0222ADE8(FrontierGraphics *graphics, void **sceneData) {
    *sceneData = ov80_02239D74(graphics->bgConfig, graphics->plttData);

    SetBgPriority(GF_BG_LYR_MAIN_0, 1);
    SetBgPriority(GF_BG_LYR_MAIN_2, 2);
    SetBgPriority(GF_BG_LYR_MAIN_3, 3);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, FALSE);
}

struct WFCFacilitySelectorEffects;

typedef struct WFCFacilitySelectorEffects WFCFacilitySelectorEffects;

void ov80_02239DB8(WFCFacilitySelectorEffects *effects);

void ov80_0222AE1C(FrontierGraphics *graphics, void **sceneData);

void ov80_0222AE1C(FrontierGraphics *graphics, void **sceneData) {
    ov80_02239DB8(*sceneData);
}
