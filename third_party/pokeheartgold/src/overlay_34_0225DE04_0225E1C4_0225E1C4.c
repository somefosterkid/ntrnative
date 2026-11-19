#include "global.h"

#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "overlay_34.h"

extern const int _0225E694[3];

void ov34_0225E1C4(BgConfig *param0, NNSG2dScreenData *param1, Ov34Row *param2, int param3, int param4, int *param5) {
    int v0, v1 = 0;

    if (param4 > 3) {
        param4 = 3;
    }

    for (v0 = 0; v0 < param4; v0++) {
        if (param3 != v0) {
            if (param2[v0].unk34 != 0) {
                param2[v0].unk34--;
            }
        }

        v1 += param2[v0].unk34;
    }

    if ((v1 == 0) && (*param5 == 0)) {
        *param5 = v1;
        return;
    }

    *param5 = v1;

    for (v0 = 0; v0 < param4; v0++) {
        int v2 = param2[v0].unk34 / 2;

        CopyToBgTilemapRect(param0, 7, 0, v0 * 7 + 2, 32, 7, param1->rawData, 0, 24 * param2[v0].unk30 + 8 * v2, 32, 48);
        BgSetPosTextAndCommit(param0, GF_BG_LYR_SUB_0 + v0, BG_POS_OP_SET_Y, _0225E694[v2]);
    }

    DC_FlushRange((void *)GetBgTilemapBuffer(param0, 7), 32 * 24 * 2);
    BgCopyOrUncompressTilemapBufferRangeToVram(param0, 7, GetBgTilemapBuffer(param0, 7), 32 * 24 * 2, 0);
}
