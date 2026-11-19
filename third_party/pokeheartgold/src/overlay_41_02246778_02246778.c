#include "global.h"
#include "bg_window.h"

struct GraphicsModes;

void SetBothScreensModesAndDisable(const GraphicsModes *graphicsModes);

void GfGfx_DisableEngineAPlanes(void);

void GfGfx_EngineATogglePlanes(u8 layerMask, u8 enable);

void GfGfx_DisableEngineBPlanes(void);

void GfGfx_EngineBTogglePlanes(u8 layerMask, u8 enable);

void ov41_02246778(void);

extern const GraphicsModes _0224BF94;

void ov41_02246778(void) {
    {
        GraphicsModes v0 = _0224BF94;

        SetBothScreensModesAndDisable(&v0);
    }

    GX_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_1D_32K);
    NNS_G2dInitOamManagerModule();

    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0 | GX_PLANEMASK_BG1 | GX_PLANEMASK_BG2 | GX_PLANEMASK_BG3 | GX_PLANEMASK_OBJ, 1);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0 | GX_PLANEMASK_BG1 | GX_PLANEMASK_OBJ, 1);
}
