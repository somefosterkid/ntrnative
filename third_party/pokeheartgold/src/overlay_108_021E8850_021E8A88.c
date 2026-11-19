#include "global.h"
#include "overlay_108_021E8850.h"

#include "gf_gfx_planes.h"
#include "heap.h"
#include "menu_input_state.h"
#include "overlay_108_safari_decoration_app.h"
#include "safari_zone.h"
#include "screen_fade.h"
#include "system.h"

void ov108_021E8A88(SafariDecorationApp *arg0) {
    Heap_Free(arg0->objects);
    SafariZone_SetAreaSet(arg0->safariZone, 0, &arg0->areaSet);
    if ((u32) arg0->cursor >= 6U) {
        arg0->cursor = 5U;
    }
    *arg0->args->unk20 = arg0->cursor + (6 * arg0->page);
    MenuInputStateMgr_SetState(arg0->args->menuInputStatePtr, arg0->menuInputState);
}

void ov108_021E8AD4(void) {
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
