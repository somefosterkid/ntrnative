#include "global.h"
#include "overlay_96_ov96_rower_work.h"

f32 ov96_021FF6DC(Ov96Rower *arg0, s32 arg1);

f32 ov96_021FF6DC(Ov96Rower *arg0, s32 arg1) {
    s32 temp_r4;
    if ((arg0->unkA5) != 0) {
        return (f32)(arg0->subs[arg1].unk14);
    }
    temp_r4 = arg1 * 0x1C;
    if (((double)(arg0->subs[arg1].unk0C)) <= 40.0) {
        return (f32)((u8 *)arg0)[(arg1 * 0x1C) + 0x2D];
    }
    return (f32)((u8 *)arg0)[(arg1 * 0x1C) + 0x2E];
}
