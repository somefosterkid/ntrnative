#include "global.h"

#include "overlay_74_wh_work.h"

void ov74_022361B8(int errCode) {
    if ((u32)(ov74_0223E2FC.work->state - 9) > 1U) {
        ov74_0223E2FC.work->errCode = errCode;
    }
}
