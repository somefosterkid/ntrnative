#include "global.h"
struct UnkStruct_ov96_021EF610;

#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "overlay_96_unk_struct_ov96_021_ef610.h"

void ov96_021EFA28(void);

void ov96_021EFA28(void) {
    GfGfx_EngineBTogglePlanes(4U, 0U);
    GfGfx_EngineBTogglePlanes(8U, 0U);
}

void ov96_021EFA3C(struct UnkStruct_ov96_021EF610 *param_1);

void ov96_021EFA3C(UnkStruct_ov96_021EF610 *param_1) {
    s32 value;
    MtxFx22 MStack_1c;

    value = 0;
    if (*((s32 *)&param_1->unk_28) == 0) {
        value = 0x20;
    }
    MStack_1c._00 = 0x125;
    MStack_1c._01 = 0;
    MStack_1c._10 = 0;
    MStack_1c._11 = 0x125;
    OS_WaitVBlankIntr();
    SetBgAffine(*((BgConfig * *)&param_1->unk_4), 7, &MStack_1c, 0x80, 0x60);
    SetBgAffine(*((BgConfig * *)&param_1->unk_4), 6, &MStack_1c, 0x80, 0x60);
    Bg_SetTextDimAndAffineParams(*((BgConfig * *)&param_1->unk_4), 7, BG_POS_OP_SUB_XSCALE, value, &MStack_1c, 0x80, 0x60);
    Bg_SetTextDimAndAffineParams(*((BgConfig * *)&param_1->unk_4), 6, BG_POS_OP_SUB_XSCALE, value, &MStack_1c, 0x80, 0x60);
    GfGfx_EngineBTogglePlanes(4, 1);
    GfGfx_EngineBTogglePlanes(8, 1);
    *((u32 *)&param_1->unk_1C) = 0;
    *((u32 *)&param_1->unk_2C) = 1;
    return;
}
