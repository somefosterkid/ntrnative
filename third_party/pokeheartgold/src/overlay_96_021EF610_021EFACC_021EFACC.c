#include "global.h"
struct UnkStruct_ov96_021EF610;

#include "bg_window.h"
#include "overlay_96_unk_struct_ov96_021_ef610.h"

void ov96_021EFACC(struct UnkStruct_ov96_021EF610 *arg0);

void ov96_021EFACC(UnkStruct_ov96_021EF610 *arg0) {
    s32 spC[4];
    s32 temp_r0;
    temp_r0 = (arg0->unk_1C) + 1;
    arg0->unk_1C = temp_r0;
    temp_r0 = ((s32)(temp_r0 << 0xC)) / 14;
    spC[0] = temp_r0;
    spC[1] = 0;
    spC[2] = 0;
    spC[3] = temp_r0;
    OS_WaitVBlankIntr();
    SetBgAffine(*((BgConfig * *)&arg0->unk_4), 7U, (MtxFx22 *)spC, 0x80, 0x60);
    SetBgAffine(*((BgConfig * *)&arg0->unk_4), 6U, (MtxFx22 *)spC, 0x80, 0x60);
    if (((s32)(arg0->unk_1C)) >= 0xE) {
        arg0->unk_1C = 0;
        arg0->unk_2C = 0;
    }
}
