#include "global.h"

#include "overlay_74_wh_work.h"

BOOL ov74_02236768(WHBlock *param0) {
    if ((ov74_0223E2FC.work->checksum != param0->checksum) || (ov74_0223E2FC.work->dataSize != param0->size)) {
        if (ov74_0223E2FC.work->mismatchCount++ >= 16) {
            ov74_022366E8(param0);

            if (ov74_0223E2FC.work->callback != NULL) {
                ov74_0223E2FC.work->callback(5);
            }
        } else {
            return 0;
        }
    }

    if (param0->index == 0xffff) {
        MI_CpuCopy32(param0->data, ov74_0223E2FC.work->parentInfo, (WM_SIZE_USER_GAMEINFO - 8));
        ov74_0223E2FC.work->hasParentInfo = 1;
    } else {
        ov74_0223E2FC.work->lastBlock = param0->index;

        if (param0->index >= ov74_0223E2FC.work->blockCount) {
            if (ov74_0223E2FC.work->callback != NULL) {
                ov74_0223E2FC.work->callback(4);
            }

            ov74_02236168(9);
        }

        if (ov74_0223E2FC.work->blockReceived[param0->index] == 0) {
            ov74_0223E2FC.work->blockReceived[param0->index] = 1;

            if (param0->index == ov74_0223E2FC.work->blockCount - 1) {
                MI_CpuCopy32(param0->data, &ov74_0223E2FC.work->recvBuf[param0->index * (WM_SIZE_USER_GAMEINFO - 8)], ov74_0223E2FC.work->dataSize - param0->index * (WM_SIZE_USER_GAMEINFO - 8));
            } else {
                MI_CpuCopy32(param0->data, &ov74_0223E2FC.work->recvBuf[param0->index * (WM_SIZE_USER_GAMEINFO - 8)], (WM_SIZE_USER_GAMEINFO - 8));
            }

            ov74_0223E2FC.work->blocksReceived++;

            if (ov74_0223E2FC.work->blocksReceived == ov74_0223E2FC.work->blockCount) {
                if (ov74_0223E2FC.work->callback != NULL) {
                    ov74_0223E2FC.work->callback(2);
                }

                return 1;
            }
        }
    }

    return 0;
}

u16 ov74_022368A4(u8 *data, u32 size) {
    u16 v0, v1;
    u16 *v2 = (u16 *)data;

    v0 = 0;

    for (v1 = 0; v1 < size / 2; v1++) {
        v0 += v2[v1];
        v0 = (u16)((v0 << 1) | (v0 >> 15));
    }

    return v0;
}
