#include "global.h"
#include "overlay_96_unk_struct_ov96_021_f5018.h"

u8 ov96_021F65D8(UnkStruct_ov96_021F5018_Ball *arg0);

u8 ov96_021F65D8(UnkStruct_ov96_021F5018_Ball *arg0) {
    u16 v0;

    v0 = arg0->unk_24;
    if ((u32)v0 < 5U) {
        arg0->unk_24 = (u16)(v0 + 1);
    }
    arg0->unk_20 = (u16)(arg0->unk_20 + arg0->unk_24);
    if ((u32)arg0->unk_20 > 0x3E7U) {
        arg0->unk_20 = 0x3E7U;
    }
    return (u8)arg0->unk_24;
}
