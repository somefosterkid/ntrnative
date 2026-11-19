#include "global.h"

#include "battle/trainer_intro_data.h"
#include "overlay_05.h"
#include "overlay_05_unk_struct_ov10_0221_fb28_t.h"
#include "palette.h"
#include "unk_02005D10.h"

BOOL ov05_0221CCF4(void);

s32 ov05_0221CB70(UnkStruct_ov10_0221FB28 *arg0) {
    s32 iVar1;

    iVar1 = ov05_0221CCF4();
    if (iVar1 == 1) {
        if (arg0->trainerIntroData->battleResult == 1) {
            PaletteData_CopyPalette(arg0->unk_08, 0, 0x40, 0, 0x30, 0x20);
        } else {
            PaletteData_CopyPalette(arg0->unk_08, 0, 0x40, 0, 0, 0x20);
        }
    } else if (arg0->trainerIntroData->battleResult == 1) {
        PaletteData_CopyPalette(arg0->unk_08, 0, 0x40, 0, 0, 0x20);
    } else {
        PaletteData_CopyPalette(arg0->unk_08, 0, 0x40, 0, 0x30, 0x20);
    }
    PaletteData_SetSelectedBufferAll(arg0->unk_08, 1);
    PlaySE(0x71b);
    arg0->unk_B81 = 0x40;
    return 1;
}
