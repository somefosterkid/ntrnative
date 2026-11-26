#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "font.h"
#include "gf_gfx_planes.h"
#include "options.h"
#include "overlay_manager.h"
#include "overlay_74_mystery_gift_app_data.h"
#include "render_window.h"
#include "text.h"

void ov74_0222A81C(BgConfig *);

void ov74_0222A94C(OverlayManager *, u32, u32);

void ov74_0222AA18(OverlayManager *, Window *, u32);

u32 ov74_0222AB70(OverlayManager *param_1, MysteryGiftAppData *param_2);

u32 ov74_0222AB70(OverlayManager *param_1, MysteryGiftAppData *param_2) {
    u32 uVar1;
    BOOL BVar2;

    ResetAllTextPrinters();
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, HEAP_ID_84);
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_1_OFFSET, HEAP_ID_84);
    uVar1 = Options_GetFrame(param_2->options);
    LoadUserFrameGfx2(param_2->bgConfig, GF_BG_LYR_MAIN_0, 1, 2, (u8)uVar1, HEAP_ID_84);
    LoadUserFrameGfx1(param_2->bgConfig, GF_BG_LYR_MAIN_0, 0x1f, 3, 1, HEAP_ID_84);
    *(u16 *)HW_BG_PLTT = 0x7d8c;
    BVar2 = WindowIsInUse(&param_2->messageBox);
    if (BVar2 == 0) {
        AddWindowParameterized(param_2->bgConfig, &param_2->messageBox, 0, 2, 0x13, 0x1b, 4, 0, 0x28);
    }
    ov74_0222AA18(param_1, &param_2->messageBox, 0);
    ov74_0222A94C(param_1, 0xc4, 0);
    ov74_0222A81C(param_2->bgConfig);
    return 1;
}
