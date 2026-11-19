#include "global.h"

#include "bg_window.h"
#include "battle/trainer_intro_data.h"
#include "overlay_05_unk_struct_ov10_0221_fb28_t.h"
#include "gf_gfx_planes.h"
#include "overlay_05.h"
#include "palette.h"
#include "system.h"

s32 ov05_0221BB00(void);

extern const UnkFuncPtr_ov10_02222AD0 ov05_0221EA98[];

extern const UnkFuncPtr_ov10_02222AD0 ov05_0221EAC0[];

extern const UnkFuncPtr_ov10_02222AD0 ov05_0221EC28[];

extern const UnkFuncPtr_ov10_02222AD0 ov05_0221EC98[];

s32 ov05_0221BD28(UnkStruct_ov10_0221FB28 *arg0) {
    PaletteData *data;
    u8 v0;
    TrainerIntroData *v1;

    reg_G2_BLDCNT = 0;
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    reg_GX_DISPCNT = (s32)(reg_GX_DISPCNT & 0xFFFFE0FF);
    reg_GXS_DB_DISPCNT = (s32)(0xFFFFE0FF & reg_GXS_DB_DISPCNT);
    reg_GX_POWCNT |= REG_GX_POWCNT_DSEL_MASK;
    reg_G2_BLDCNT = 0;
    reg_G2S_DB_BLDCNT = 0;
    ov05_0221BB00();
    arg0->unk_0C = BgConfig_Alloc(arg0->trainerIntroData->heapID);
    data = PaletteData_Init(arg0->trainerIntroData->heapID);
    arg0->unk_08 = data;
    PaletteData_AllocBuffers(data, PLTTBUF_MAIN_BG, 0x200, arg0->trainerIntroData->heapID);
    BG_SetMaskColor(4U, 0U);
    arg0->unk_B82 = 0;
    arg0->unk_B81 = 8;
    arg0->unk_B80 = 0;
    v1 = arg0->trainerIntroData;
    v0 = v1->mode;
    switch (v0) {
    case 0:
        arg0->unk_04 = ov05_0221EAC0;
        break;
    case 1:
        arg0->unk_04 = ov05_0221EA98;
        break;
    case 2:
        if (v1->battleResult == 3) {
            arg0->unk_04 = ov05_0221EC28;
        } else {
            arg0->unk_04 = ov05_0221EC98;
        }
        break;
    }
    return 1;
}
