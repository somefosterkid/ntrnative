#include "global.h"
#include "overlay_102_pmsinput_work.h"
struct PMSInputWork;

s32 ov102_021E7A58(void *);

s32 ov102_021E8294(void *, void *);

s32 ov102_021E848C(void *, void *);

void ov102_021E8580(struct PMSInputWork *arg0, void *arg1);

void ov102_021E8580(PMSInputWork *arg0, void *arg1) {
    if (ov102_021E7A58(arg0) == 0) {
        if (*((s32 *)&arg0->ktChangeFunc) == 0) {
            ov102_021E8294(arg0, arg1);
            return;
        }
        ov102_021E848C(arg0, arg1);
    }
}

s32 sub_02091C38(void *, s32);

s32 sub_02091C74(void *, s32);

s32 ov102_021E85A8(void *arg0, s32 arg1, s32 arg2);

s32 ov102_021E85A8(void *arg0, s32 arg1, s32 arg2) {
    if ((u32)(u16)(arg2 + 0xFF02) <= 1U) {
        return 0;
    }
    if (arg1 == 0) {
        if (sub_02091C38(arg0, arg2) != 0) {
            return 1;
        }
        return 0;
    }
    if (sub_02091C74(arg0, arg2) != 0) {
        return 1;
    }
    return 0;
}
