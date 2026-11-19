#include "global.h"

#include "error_handling.h"
#include "math_util.h"
#include "overlay_96_ov96rival.h"

extern s32 ov96_0221CE08[];

s32 ov96_0220CCBC(Ov96Rival *arg0);

s32 ov96_0220CCBC(Ov96Rival *arg0) {
    s32 sum = 0;
    u8 roll;
    u8 idx;
    roll = (u8)(MTRandom() % 100U);
    idx = (u8)(arg0->stat[arg0->unk_14 >> 30] >> 26);
    if (idx >= 3U) {
        GF_AssertFail();
    }
    sum += ov96_0221CE08[idx];
    if (roll < sum) {
        return 1;
    }
    return 0;
}
