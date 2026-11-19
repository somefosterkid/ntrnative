#include "global.h"
struct EasyChatApp;

s32 ov102_021E7A58(void *, s32 *);

s32 ov102_021E7C84(void *, s32 *);

int ov102_021E7D0C(struct EasyChatApp *arg0, int *arg1);

int ov102_021E7D0C(struct EasyChatApp *arg0_, int *arg1_) {
    void *arg0 = arg0_;
    s32 *arg1 = arg1_;
    if ((*arg1 == 0) && (ov102_021E7A58(arg0, arg1) != 0)) {
        return 0;
    }
    return ov102_021E7C84(arg0, arg1);
}
