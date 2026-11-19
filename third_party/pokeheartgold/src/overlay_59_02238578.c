#include "global.h"
#include "overlay_59_apricorn_app.h"
#include "render_text.h"
#include "text.h"
#include "touchscreen.h"
#include "unk_02005D10.h"
#include "gf_gfx_planes.h"
#include "screen_fade.h"
#include "system.h"

s32 ov59_022385AC(Ov59ApricornApp *arg0);

s32 ov59_02238578(Ov59ApricornApp *arg0);

s32 ov59_02238578(Ov59ApricornApp *arg0) {
    if (ov59_022385AC(arg0) != 0) {
        return 4;
    }
    return 0xE;
}

s32 ov59_02238C24(Ov59ApricornApp *);

s32 ov59_02238D74(Ov59ApricornApp *);

s32 ov59_0223858C(Ov59ApricornApp *arg0);

s32 ov59_0223858C(Ov59ApricornApp *arg0) {
    if (ov59_022385AC(arg0) != 0) {
        ov59_02238D74(arg0);
        ov59_02238C24(arg0);
        return 4;
    }
    return 0xF;
}

s32 ov59_022385AC(Ov59ApricornApp *arg0) {
    u8 temp_r0;

    temp_r0 = arg0->printerId;
    if ((temp_r0 != 0xFF) && (TextPrinterCheckActive(temp_r0) != 0)) {
        return 0;
    }
    if (System_GetTouchNew() != 0) {
        PlaySE(0x5DCU);
        arg0->menuInputState = MENU_INPUT_STATE_TOUCH;
        TextFlags_SetCanTouchSpeedUpPrint(0);
        arg0->printerId = 0xFFU;
        return 1;
    }
    if (0xCF3 & gSystem.newKeys) {
        arg0->menuInputState = MENU_INPUT_STATE_BUTTONS;
    }
    if (3 & gSystem.newKeys) {
        PlaySE(0x5DCU);
        TextFlags_SetCanTouchSpeedUpPrint(0);
        arg0->printerId = 0xFFU;
        return 1;
    }
    return 0;
}

void ov59_02238624(void);

void ov59_02238624(void) {
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

void ov59_022393D4(void *data);

s32 ov59_02239428(Ov59ApricornApp *param0);

s32 ov59_02239604(Ov59ApricornApp *);

s32 ov59_02239704(Ov59ApricornApp *);

s32 ov59_022397E4(Ov59ApricornApp *);

s32 ov59_022398CC(Ov59ApricornApp *);

s32 ov59_022399F0(Ov59ApricornApp *);

void ov59_02238674(Ov59ApricornApp *arg0);

void ov59_02238674(Ov59ApricornApp *arg0) {
    ov59_02239428(arg0);
    ov59_02239604(arg0);
    ov59_02239704(arg0);
    ov59_022397E4(arg0);
    ov59_022398CC(arg0);
    ov59_022399F0(arg0);
    Main_SetVBlankIntrCB(ov59_022393D4, arg0);
}

s32 ov59_022395B8(Ov59ApricornApp *);

s32 ov59_022396E4(Ov59ApricornApp *);

s32 ov59_0223978C(Ov59ApricornApp *);

s32 ov59_022398A0(Ov59ApricornApp *);

s32 ov59_02239918(Ov59ApricornApp *);

s32 ov59_02239A08(Ov59ApricornApp *arg0);

void ov59_022386A8(Ov59ApricornApp *arg0);

void ov59_022386A8(Ov59ApricornApp *arg0) {
    ov59_02239A08(arg0);
    ov59_02239918(arg0);
    ov59_022398A0(arg0);
    ov59_0223978C(arg0);
    ov59_022396E4(arg0);
    ov59_022395B8(arg0);
}
