#include "global.h"

#include "overlay_74_wh_work.h"

void ov74_02236168(int state) {
    if (ov74_0223E2FC.print != NULL) {
        ov74_0223E2FC.print(ov74_0223D00C, ov74_0223CFE4[ov74_0223E2FC.work->state]);
    }
    ov74_0223E2FC.work->state = state;
    if (ov74_0223E2FC.print != NULL) {
        ov74_0223E2FC.print(ov74_0223D014, ov74_0223CFE4[ov74_0223E2FC.work->state]);
    }
    return;
}
