#include "global.h"

#include "constants/heap.h"

#include "battle/trainer_intro_data.h"
#include "bg_window.h"
#include "error_handling.h"
#include "font.h"
#include "gf_gfx_planes.h"
#include "options.h"
#include "overlay_05.h"
#include "overlay_05_unk_struct_ov10_0221_fb28_t.h"
#include "palette.h"
#include "render_window.h"

void ov05_0221D140(UnkStruct_ov10_0221FB28 *arg0) {
    u32 uVar1;
    BattleSetup *iVar2;

    if ((((arg0->unk_BA8 != 0) || (arg0->trainerIntroData == NULL)) || (iVar2 = arg0->trainerIntroData->setup, iVar2 == 0)) || (iVar2->options == 0)) {
        GF_AssertFail();
    }
    uVar1 = Options_GetFrame(arg0->trainerIntroData->setup->options);
    LoadUserFrameGfx2(arg0->unk_0C, GF_BG_LYR_MAIN_0, 1, 0xf, (u8)uVar1, arg0->trainerIntroData->heapID);
    PaletteData_LoadPaletteSlotFromHardware(arg0->unk_08, PLTTBUF_MAIN_BG, 0xf0, 0x20);
    LoadUserFrameGfx1(arg0->unk_0C, GF_BG_LYR_MAIN_0, 0x1f, 0xe, 0, arg0->trainerIntroData->heapID);
    PaletteData_LoadPaletteSlotFromHardware(arg0->unk_08, PLTTBUF_MAIN_BG, 0xe0, 0x20);
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_13_OFFSET, arg0->trainerIntroData->heapID);
    PaletteData_LoadPaletteSlotFromHardware(arg0->unk_08, PLTTBUF_MAIN_BG, 0xd0, 0x20);
    AddWindowParameterized(arg0->unk_0C, &arg0->unk_B88, 0, 2, 0x13, 0x1b, 4, 0xd, 0x28);
    AddWindowParameterized(arg0->unk_0C, &arg0->unk_B98, 0, 2, 0x13, 0x1b, 4, 0xd, 0x28);
    return;
}

void ov05_0221D228(void) {
    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG1, GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ, 8, 8);
}
