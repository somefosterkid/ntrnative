#include "global.h"
#include "overlay_96_app_021ed1d0.h"

u32 ov96_021EE264(Ov96RankEntry *arg0, s32 arg1);

u32 ov96_021EE264(Ov96RankEntry *arg0, s32 arg1) {
    s32 v0;
    Ov96RankEntry *v1;

    v0 = 0;
    v1 = arg0;
loop_1:
    if (arg1 == v1->idx) {
        return arg0[v0].rank;
    }
    v0 += 1;
    v1 += 1;
    if (v0 >= 4) {
        return -1U;
    }
    goto loop_1;
}
