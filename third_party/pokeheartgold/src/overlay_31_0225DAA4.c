#include "global.h"

#include "error_handling.h"
#include "overlay_31.h"
#include "touchscreen.h"
#include "bg_window.h"
#include "font.h"
#include "gf_gfx_loader.h"

extern u32 _0225EF40;

extern BgTemplate ov31_0225EED0;
extern BgTemplate ov31_0225EEEC;
extern BgTemplate ov31_0225EF08;


s32 ov31_0225DAA4(MartSubScreen *param_1, s32 param_2) {
    if ((param_2 != 0) && (param_2 != 1)) {
        GF_AssertFail();
    }
    return TouchscreenHitbox_FindRectAtTouchNew(*(TouchscreenHitbox **)(&_0225EF40 + param_2));
}

void ov31_0225DAC4(MartSubScreen *arg0) {
    GXS_SetGraphicsMode(GX_BGMODE_0);
    GX_SetBankForSubBG(GX_VRAM_SUB_BG_32_H);
    GX_SetBankForSubOBJ(GX_VRAM_SUB_OBJ_16_I);
    reg_GXS_DB_DISPCNT = 0x10 | (reg_GXS_DB_DISPCNT & 0xFFCFFFEF);
    InitBgFromTemplate(arg0->bgConfig, 4U, &ov31_0225EED0, 0U);
    InitBgFromTemplate(arg0->bgConfig, 5U, &ov31_0225EEEC, 0U);
    InitBgFromTemplate(arg0->bgConfig, 6U, &ov31_0225EF08, 0U);
    BG_ClearCharDataRange(6U, 0x20U, 0U, HEAP_ID_FIELD1);
    BgClearTilemapBufferAndCommit(arg0->bgConfig, 6U);
}

void ov31_0225DB38(MartSubScreen *arg0) {
    reg_G2S_DB_BLDCNT = 0;
    GfGfxLoader_GXLoadPal(NARC_graphic_shop_gra, 0xF, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0U, HEAP_ID_8);
    GfGfxLoader_LoadCharData(NARC_graphic_shop_gra, 0x10, arg0->bgConfig, GF_BG_LYR_SUB_1, 0U, 0U, 0, HEAP_ID_8);
    ov31_0225D9D4(arg0, 0);
    LoadFontPal1(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_11_OFFSET, HEAP_ID_8);
    LoadFontPal0(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_12_OFFSET, HEAP_ID_8);
    ov31_0225DBD4(arg0);
    ov31_0225DCA8(arg0);
}

void ov31_0225DBA0(MartSubScreen *param_1) {
    ov31_0225DCF4(param_1);
    RemoveWindow(&param_1->unk_34);
    RemoveWindow(&param_1->msgWindow);
    RemoveWindow(&param_1->moneyWindow);
    RemoveWindow(&param_1->pageWindow);
    RemoveWindow(&param_1->unk_64);
    return;
}
