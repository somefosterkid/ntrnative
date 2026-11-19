#include "global.h"

#include "gf_gfx_planes.h"
#include "overlay_67_app.h"
#include "screen_fade.h"
#include "system.h"

void ov67_021E5BC0(void);

void ov67_021E5BE0(void *data);

void ov67_021E681C(Ov67App *app);

u32 ov67_021E5A6C(Ov67App *app) {
    switch (app->state) {
    case 0:
        Main_SetVBlankIntrCB((GFIntrCB)0x0, (void *)0x0);
        HBlankInterruptDisable();
        GfGfx_DisableEngineAPlanes();
        GfGfx_DisableEngineBPlanes();
        reg_GX_DISPCNT = reg_GX_DISPCNT & 0xffffe0ff;
        reg_GXS_DB_DISPCNT = reg_GXS_DB_DISPCNT & 0xffffe0ff;
        reg_GX_POWCNT = reg_GX_POWCNT & 0xFFFF7FFF;
        ov67_021E5BC0();
        ResetVisibleHardwareWindows(PM_LCD_TOP);
        ResetVisibleHardwareWindows(PM_LCD_BOTTOM);
        ov67_021E5DA0(app);
        break;

    case 1:
        ov67_021E5EB0(app);
        ov67_021E60B4(app);
        break;

    case 2:
        ov67_021E6820(app);
        break;

    case 3:
        ov67_021E6008(app);
        if (app->isRankingsScreen == 0) {
            ov67_021E6220(app);
        } else {
            ov67_021E63E8(app);
            ov67_021E65C0(app);
            ov67_021E681C(app);
            ov67_021E6BC4(app);
        }
        break;

    case 4:
        Main_SetVBlankIntrCB(ov67_021E5BE0, app);
        app->state = 0;
        return 1;
    }

    app->state = (app->state) + 1;
    return 0;
}
