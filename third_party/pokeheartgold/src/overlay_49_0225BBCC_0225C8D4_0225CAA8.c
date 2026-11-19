#include "global.h"

#include "bg_window.h"

void ov49_0225CAA8(void *param_1, void **param_2);

void ov49_0225CAA8(void *param_1, void **param_2) {
    BgClearTilemapBufferAndCommit((BgConfig *)*param_2, 4);
    BgClearTilemapBufferAndCommit((BgConfig *)*param_2, 5);
    BgClearTilemapBufferAndCommit((BgConfig *)*param_2, 6);
    BgSetPosTextAndCommit((BgConfig *)*param_2, 6, BG_POS_OP_SET_XSCALE, 0);
    return;
}
