#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"

#include "overlay_96_app_02210C98.h"

extern u8 ov96_0221D1F8[];

extern u8 ov96_0221D360[];

void ov96_0221362C(Ov96App_02210C98 *param_1);

void ov96_0221362C(Ov96App_02210C98 *param_1) {
    WindowTemplate *template;
    Window *window;
    s32 iVar1;

    AddWindow(param_1->unk_4, &param_1->unk_8[0], (WindowTemplate *)ov96_0221D1F8);
    template = (WindowTemplate *)ov96_0221D360;
    iVar1 = 0;
    window = &param_1->unk_8[1];
    do {
        AddWindow(param_1->unk_4, window, template);
        iVar1 = iVar1 + 1;
        template = template + 1;
        window = window + 1;
    } while (iVar1 < 4);
    BG_FillCharDataRange(param_1->unk_4, GF_BG_LYR_MAIN_3, 0, 1, 0);
    BG_FillCharDataRange(param_1->unk_4, GF_BG_LYR_MAIN_0, 0, 1, 0);
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_15_OFFSET, param_1->unk_58);
    GfGfxLoader_GXLoadPal(NARC_a_2_3_4, 0x13, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_14_OFFSET, 0x20, param_1->unk_58);
    return;
}
