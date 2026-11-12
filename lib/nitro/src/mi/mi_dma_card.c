// TwlSDK's MI library, mi_dma_card.c: Black's main 0x020835a0-0x0208361c. The file is libntr's (MIT,
// https://github.com/cybervisi0n/libntr) for SDK 5; the names are kh-recoded-decomp's twins' (MIT) or libntr's.

#include "types.h"
#include "nitro/mi.h"


void MIi_CardDmaCopy32(u32 param_1, u32 param_2, u32 param_3, u32 param_4) {
    vu32 *ctrl;

    MIi_CheckAnotherAutoDMA(param_1, 0xffffffff);
    MIi_CheckDma0SourceAddress(param_1, param_2, param_4, 0x1000000);
    if (param_4 == 0) {
        return;
    }
    ctrl = (vu32 *)(0x040000b0 + (param_1 * 3 + 2) * 4);
    while (*ctrl & 0x80000000) {
    }
    MIi_DmaSetParameters(param_1, param_2, param_3, 0xaf000001, 0);
}
