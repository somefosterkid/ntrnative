#include "global.h"

#include "overlay_96_ov96rival.h"

s32 ov96_0220CD84(Ov96Rival *, s32);

s32 ov96_0220CC18(Ov96Rival *arg0);

s32 ov96_0220CC18(Ov96Rival *arg0) {
    if ((u32)((u32)(arg0->stat[arg0->unk_14 >> 0x1E] << 6) >> 0x18) >= 8U) {
        ov96_0220CD84(arg0, 0);
    }
    return 0;
}
