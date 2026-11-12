// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov012/building_enter_effect.c, which is the same
// code in Black 2/White 2's overlay 12.
#include "types.h"
#include "field/building_enter_effect.h"
#include "gfl/tcb.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_mtx.h"
#include "nitro/gx.h"

// The zoom into a building as the player enters it, an affine scale of BG 2 each VBlank (a descriptive name).
// setBuildingEnterVBlankCallback is swan's name (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

typedef struct {
    f32 scale;
    u32 unk4;
} BuildingEnterEffect;

static void func_ov010_02166a6c(void *data);

// pokebw2's sScaleStep
static f32 data_ov010_021729fc = 0.0044f;
// pokebw2's sEffect
static BuildingEnterEffect data_ov010_02172ba4;

// Doesn't match: Black keeps the zero that FX32_CONST compares with in r6 from the start (pokebw2 parks its twin too)
static void func_ov010_02166a6c(void *data) {
    MtxFx22 mtx;
    f32 zoom;
    fx32 scale;
    f32 zero = 0.0f;

    if (reg_GX_DISPSTAT & REG_GX_DISPSTAT_VBLK_MASK) {
        zoom = data_ov010_02172ba4.scale + data_ov010_021729fc;
        data_ov010_02172ba4.scale = zoom;
        if (1.0f / zoom > zero) {
            scale = (fx32)(1.0f / zoom * FX32_ONE + 0.5f);
        } else {
            scale = (fx32)(1.0f / zoom * FX32_ONE - 0.5f);
        }
        MTX_Scale22_(&mtx, scale, scale);
        G2_SetBG2Affine(&mtx, 128, 92, 0, 0);
    }
}

void setBuildingEnterVBlankCallback(void) {
    data_ov010_02172ba4.scale = 1.0f;
    data_ov010_02172ba4.unk4 = 0;
    GFL_VBlankSetCallback(func_ov010_02166a6c, NULL);
}

void func_ov010_02166b2c(void) {
    MtxFx22 mtx;

    MTX_Identity22_(&mtx);
    G2_SetBG2Affine(&mtx, 128, 92, 0, 0);
    GFL_VBlankResetCallback();
}
