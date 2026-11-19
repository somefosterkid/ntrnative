#include "global.h"

#include "overlay_74_wh_work.h"

BOOL ov74_022368D4(void) {
    {
        u16 v0[4];
        u16 *v1;

        v1 = (u16 *)&ov74_0223E2FC.work->parentBssid[0];
        v0[0] = v1[0];
        v0[3] = v1[1];
        v0[2] = v1[2];
        v0[1] = ov74_0223E2FC.work->checksum;

        {
            int v2;
            u16 v5 = 0x3fa2;

            for (v2 = 0; v2 < 4; v2++) {
                v0[v2] = v0[v2] ^ v5;
                v5 = v0[v2];
            }
        }

        {
            u8 *v3 = (u8 *)&(v0[0]);
            int v4;

            for (v4 = 0; v4 < 8; v4++) {
                (void)0;
            }
        }

        CRYPTO_RC4Init(&ov74_0223E2FC.work->rc4, &(v0[0]), 8);
        CRYPTO_RC4Encrypt(&ov74_0223E2FC.work->rc4, ov74_0223E2FC.work->recvBuf, ov74_0223E2FC.work->dataSize, ov74_0223E2FC.work->recvBuf);

        MI_CpuClear32(&ov74_0223E2FC.work->rc4, sizeof(ov74_0223E2FC.work->rc4));
    }

    return ov74_022368A4(ov74_0223E2FC.work->recvBuf, ov74_0223E2FC.work->dataSize) == ov74_0223E2FC.work->checksum;
}

int ov74_02236980(void) {
    return sizeof(WHWork) + 32;
}
