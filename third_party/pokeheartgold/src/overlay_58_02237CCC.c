#include "global.h"

#include "gf_gfx_planes.h"
#include "screen_fade.h"
#include "system.h"

void ov58_02237CCC(void);

void ov58_02237CCC(void) {
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    reg_GX_DISPCNT &= 0xFFFFE0FF;
    reg_GXS_DB_DISPCNT &= 0xFFFFE0FF;
    sub_0200FBF4(PM_LCD_TOP, 0U);
    sub_0200FBF4(PM_LCD_BOTTOM, 0U);
    ResetVisibleHardwareWindows(PM_LCD_TOP);
    ResetVisibleHardwareWindows(PM_LCD_BOTTOM);
}
