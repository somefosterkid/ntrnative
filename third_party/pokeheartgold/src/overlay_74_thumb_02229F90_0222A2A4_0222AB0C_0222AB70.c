#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "font.h"
#include "gf_gfx_planes.h"
#include "options.h"
#include "render_window.h"
#include "text.h"

void ov74_0222A81C(u32);

void ov74_0222A94C(void *, u32, u32);

void ov74_0222AA18(void *, u32 *, u32);

u32 ov74_0222AB70(void *param_1, u32 *param_2);

u32 ov74_0222AB70(void *param_1, u32 *param_2) {
    u32 uVar1;
    BOOL BVar2;

    ResetAllTextPrinters();
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, HEAP_ID_84);
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_1_OFFSET, HEAP_ID_84);
    uVar1 = Options_GetFrame((Options *)param_2[2]);
    LoadUserFrameGfx2((BgConfig *)*param_2, GF_BG_LYR_MAIN_0, 1, 2, (u8)uVar1, HEAP_ID_84);
    LoadUserFrameGfx1((BgConfig *)*param_2, GF_BG_LYR_MAIN_0, 0x1f, 3, 1, HEAP_ID_84);
    *(u16 *)HW_BG_PLTT = 0x7d8c;
    BVar2 = WindowIsInUse((Window *)(param_2 + 6));
    if (BVar2 == 0) {
        AddWindowParameterized((BgConfig *)*param_2, (Window *)(param_2 + 6), 0, 2, 0x13, 0x1b, 4, 0, 0x28);
    }
    ov74_0222AA18(param_1, param_2 + 6, 0);
    ov74_0222A94C(param_1, 0xc4, 0);
    ov74_0222A81C(*param_2);
    return 1;
}
