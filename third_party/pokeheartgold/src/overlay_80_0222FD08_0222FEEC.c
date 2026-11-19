#include "global.h"

#include "overlay_80_0222FD08.h"

void ov80_0222FF00(void *factory);

void ov80_02230270(void *factory);

void ov80_0222FEEC(void *param_1, s32 param_2) {
    if (param_2 == 0) {
        ov80_0222FF00(param_1);
        return;
    }
    ov80_02230270(param_1);
    return;
}
