#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "gf_gfx_planes.h"
#include "options.h"
#include "pm_string.h"
#include "screen_fade.h"
#include "system.h"
#include "unk_020210A0.h"
#include "overlay_108_safari_area_customizer.h"

void ov108_021E6C68(SafariAreaCustomizer *appData);

void ov108_021E6D80(SafariAreaCustomizer *arg0);

void ov108_021E6F74(SafariAreaCustomizer *arg0);

void ov108_021E7080(SafariAreaCustomizer *work);

void ov108_021E7224(SafariAreaCustomizer *arg0);

void ov108_021E72CC(SafariAreaCustomizer *arg0);

void ov108_021E733C(SafariAreaCustomizer *arg0);

void ov108_021E7BFC(SafariAreaCustomizer *arg0);

void ov108_021E6BA0(void *cbArg);

s32 ov108_021E5C54(SafariAreaCustomizer *menuData);

s32 ov108_021E5C54(SafariAreaCustomizer *menuData) {
    s32 iVar1;

    iVar1 = menuData->unk_8;
    switch (iVar1) {
    case 0:
        Main_SetVBlankIntrCB((GFIntrCB)0x0, (void *)0x0);
        HBlankInterruptDisable();
        GfGfx_DisableEngineAPlanes();
        GfGfx_DisableEngineBPlanes();
        reg_GX_DISPCNT = reg_GX_DISPCNT & 0xffffe0ff;
        reg_GXS_DB_DISPCNT = reg_GXS_DB_DISPCNT & 0xffffe0ff;
        sub_0200FBF4(PM_LCD_TOP, 0);
        sub_0200FBF4(PM_LCD_BOTTOM, 0);
        ResetVisibleHardwareWindows(PM_LCD_TOP);
        ResetVisibleHardwareWindows(PM_LCD_BOTTOM);
        EnableTouchPad();
        InitializeTouchPad(4);
        break;
    case 1:
        ov108_021E6C68(menuData);
        ov108_021E6D80(menuData);
        ov108_021E6F74(menuData);
        ov108_021E7080(menuData);
        ov108_021E7224(menuData);
        ov108_021E72CC(menuData);
        ov108_021E733C(menuData);
        break;
    case 2:
        ov108_021E7BFC(menuData);
        Main_SetVBlankIntrCB(ov108_021E6BA0, menuData);
        menuData->unk_8 = 0;
        return 1;
    }
    menuData->unk_8 = menuData->unk_8 + 1;
    return 0;
}
