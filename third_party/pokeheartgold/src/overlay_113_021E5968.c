#include "global.h"

#include "overlay_manager.h"
#include "sprite_system.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "screen_fade.h"
#include "system.h"
#include "pokedex.h"
#include "overlay_113.h"

s32 ov113_021E5BC4(UnownReportData *arg0);

s32 ov113_021E5C68(UnownReportData *arg0);

s32 ov113_021E5C0C(UnownReportData *arg0);

s32 ov113_021E5C60(UnownReportData *arg0);

BOOL UnownReport_Main(OverlayManager *man, int *state);

void ov113_021E59F8(void);
s32 ov113_021E5B60(UnownReportData *);
s32 ov113_021E5BB0(UnownReportData *);
BOOL UnownReport_Exit(OverlayManager *man, int *state);

s32 ov113_021E5A48(Pokedex *arg0, u8 *arg1);

BOOL UnownReport_Main(OverlayManager *man, int *state) {
    UnownReportData *arg0;
    s32 iVar1;
    s32 sVar2;

    arg0 = OverlayManager_GetData(man);
    switch (*state) {
    case 0:
        sVar2 = ov113_021E5BC4(arg0);
        *state = sVar2;
        break;
    case 1:
        iVar1 = ov113_021E5C0C(arg0);
        *state = iVar1;
        break;
    case 2:
        iVar1 = ov113_021E5C60(arg0);
        *state = iVar1;
        break;
    case 3:
        sVar2 = ov113_021E5C68(arg0);
        *state = sVar2;
        break;
    case 4:
        return 1;
    }
    SpriteSystem_DrawSprites(arg0->spriteManager1);
    SpriteSystem_DrawSprites(arg0->spriteManager2);
    return 0;
}

BOOL UnownReport_Exit(OverlayManager *man, int *state) {
    UnownReportData *v0;

    v0 = OverlayManager_GetData(man);
    if (ov113_021E5BB0(v0) == 0) {
        return 0;
    }
    ov113_021E59F8();
    ov113_021E5B60(v0);
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_152);
    return 1;
}

void ov113_021E59F8(void) {
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

s32 ov113_021E5A48(Pokedex *pokedex, u8 *arg1) {
    s32 idx;
    s32 v1;
    u8 v0;

    v1 = 0;
    idx = 0;
    do {
        v0 = (u8)Pokedex_GetSeenFormByIdx_Unown(pokedex, idx, 1U);
        if ((u32)(u8)(v0 + 0xE6) <= 1U) {
            *(u8 *)(arg1 + v1) = v0;
            v1 += 1;
        }
        idx += 1;
    } while (idx < 0x1C);
    return v1;
}
