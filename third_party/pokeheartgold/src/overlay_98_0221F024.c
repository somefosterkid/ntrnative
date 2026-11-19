#include "global.h"

#include "error_handling.h"
#include "overlay_98.h"
#include "gf_gfx_planes.h"
#include "screen_fade.h"
#include "system.h"


s32 ov98_0221EFE8(s32, s32, s32);
s32 ov98_0221F174(void);

u16 ov98_0221F024(const u8 *arg0) {
    s32 v0;
    u16 v1;

    v1 = 0;
    if (arg0 == NULL) {
        GF_AssertFail();
    }
    v0 = 0;
    do {
        if (MATH_CountPopulation((u32)arg0[v0]) == 5) {
            v1 += 1;
        }
        v0 += 1;
    } while (v0 < 0x1ED);
    return v1;
}

u8 ov98_0221F058(const Pokeathlon_RecordsSolo *arg0) {
    u8 v0;
    u8 v1;

    v1 = 0;
    v0 = 0;
    do {
        if (ov98_0221EFE8(v0, 1, ov98_0221F01C(&arg0[v0], 0)) != 0) {
            v1 += 1;
        }
        v0 += 1;
    } while ((u32)v0 < 0xAU);
    return v1;
}

void ov98_0221F090(void) {
    sub_0200FBF4(PM_LCD_TOP, 0U);
    sub_0200FBF4(PM_LCD_BOTTOM, 0U);
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    reg_GX_DISPCNT &= 0xFFFFE0FF;
    reg_GXS_DB_DISPCNT &= 0xFFFFE0FF;
    reg_GX_POWCNT &= 0xFFFF7FFF;
    ov98_0221F174();
}
