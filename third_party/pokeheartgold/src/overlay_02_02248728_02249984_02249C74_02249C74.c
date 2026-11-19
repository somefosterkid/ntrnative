#include "global.h"

#include "overlay_02_hmcut_in.h"

void ov02_02249420(void);

s32 ov02_0224AB8C(HMCutIn *);

void ov02_0224AB9C(HMCutIn *);

u32 ov02_02249C74(HMCutIn *param_1);

u32 ov02_02249C74(HMCutIn *param_1) {
    s32 iVar1;

    if (param_1->mode == 3) {
        iVar1 = ov02_0224AB8C(param_1);
        if (iVar1 == 2) {
            ov02_0224AB9C(param_1);
            param_1->state = param_1->state + 2;
        }
        return 0;
    }
    if ((param_1->screenFaded == 0) && (iVar1 = ov02_0224AB8C(param_1), iVar1 == 4)) {
        param_1->screenFaded = 1;
        ov02_02249420();
    }
    iVar1 = ov02_0224AB8C(param_1);
    if (iVar1 != 2) {
        return 0;
    }
    if (param_1->screenFaded == 0) {
        param_1->screenFaded = 1;
        ov02_02249420();
    }
    ov02_0224AB9C(param_1);
    param_1->state = param_1->state + 1;
    return 0;
}
