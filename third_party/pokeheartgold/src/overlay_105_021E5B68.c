#include "global.h"

#include "gf_3d_render.h"
#include "gf_gfx_planes.h"
#include "system.h"

s32 ov105_021E5C84(void);

extern u16 ov105_021E5DE0;

void ov105_021E5B68(void);

void ov105_021E5B68(void) {
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    reg_GX_DISPCNT &= 0xFFFFE0FF;
    reg_GXS_DB_DISPCNT &= 0xFFFFE0FF;
    ov105_021E5C84();
    GF3dRender_InitSimpleManager(HEAP_ID_151);
    reg_G3X_DISP3DCNT = 0x20 | (reg_G3X_DISP3DCNT & 0xFFFFCFFF);
    G3X_SetEdgeColorTable(&ov105_021E5DE0);
    GfGfx_SwapDisplay();
}
