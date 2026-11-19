#include "global.h"
#include "overlay_102_pmsinput_work.h"
struct EasyChatApp;
struct PMSInputWork;

s32 ov102_021E7A58(void *, u32 *);

s32 ov102_021E8178(void *, u32 *);

int ov102_021E821C(struct EasyChatApp *arg0, int *arg1);

s32 ov102_021E7A4C(void *, void *);
s32 ov102_021E8580(void *, s32 *);
s32 ov102_021E9464(void *);
s32 ov102_021E947C(void *, s32);
s32 ov102_021E8240(struct PMSInputWork *arg0, s32 *arg1);

int ov102_021E821C(struct EasyChatApp *arg0_, int *arg1_) {
    void *arg0 = arg0_;
    u32 *arg1 = arg1_;
    if (((u32)*arg1 <= 1U) && (ov102_021E7A58(arg0, arg1) != 0)) {
        return 0;
    }
    return ov102_021E8178(arg0, arg1);
}

s32 ov102_021E8240(PMSInputWork *arg0, s32 *arg1) {
    s32 v0;

    v0 = *arg1;
    switch (v0) {
    case 0:
        if (ov102_021E947C(*((void **)&arg0->vwk), 0x12) != 0) {
            ov102_021E8580(arg0, arg1);
        }
        break;
    case 1:
        if (ov102_021E9464(*((void **)&arg0->vwk)) != 0) {
            ov102_021E7A4C(arg0, *((void **)&arg0->mainProc));
        }
        break;
    case 2:
        if (ov102_021E947C(*((void **)&arg0->vwk), 0x1B) != 0) {
            *arg1 = 0;
        }
        break;
    }
    return 0;
}
