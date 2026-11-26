#include "global.h"
#include "overlay_14.h"

#include "palette.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F2A18(UnkStruct_ov14_boxsys *, s32, s32);

s32 ov14_021ED9AC(UnkStruct_ov14_boxapp *arg0) {
    PaletteData_LoadPaletteSlotFromHardware(arg0->sys->unk_18, PLTTBUF_MAIN_BG, 0U, 0x200U);
    PaletteData_BeginPaletteFade(arg0->sys->unk_18, 1U, 0xC000U, 0, 0U, 0x10U, 0x7FFFU);
    arg0->unk30 = 0x46;
    return 3;
}

s32 ov14_021ED9EC(UnkStruct_ov14_boxapp *arg0) {
    PaletteData_BeginPaletteFade(arg0->sys->unk_18, 1U, 0xC000U, 0, 0x10U, 0U, 0x7FFFU);
    arg0->unk30 = 0x42;
    return 3;
}

s32 ov14_021EDA1C(void *arg0) {
    ov14_021F2A18(((UnkStruct_ov14_boxapp *)arg0)->sys, 9, 1);
    if (((UnkStruct_ov14_boxapp *)arg0)->unk_0->unk8 == 3) {
        return 0x75;
    }
    return 0xC;
}

s32 ov14_021EDA3C(UnkStruct_ov14_boxapp *arg0) {
    ov14_021F2A18(arg0->sys, 9, 1);
    return 0x24;
}
