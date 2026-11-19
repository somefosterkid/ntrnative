#include "global.h"

#include "gf_3d_render.h"
#include "gf_gfx_planes.h"
#include "system.h"

s32 ov104_021E5CA8(void);

extern u16 ov104_021E5F14;

void ov104_021E5B88(void);

void ov104_021E5B88(void) {
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    reg_GX_DISPCNT &= 0xFFFFE0FF;
    reg_GXS_DB_DISPCNT &= 0xFFFFE0FF;
    ov104_021E5CA8();
    GF3dRender_InitSimpleManager(HEAP_ID_149);
    reg_G3X_DISP3DCNT = 0x20 | (reg_G3X_DISP3DCNT & 0xFFFFCFFF);
    G3X_SetEdgeColorTable(&ov104_021E5F14);
    GfGfx_SwapDisplay();
}
