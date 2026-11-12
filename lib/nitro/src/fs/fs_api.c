// TwlSDK's FS library, fs_api.c: Black's main 0x0207ad04-0x0207ad40. The file is libntr's (MIT,
// https://github.com/cybervisi0n/libntr) for SDK 5; the names are kh-recoded-decomp's twins' (MIT) or libntr's in the
// same place.

#include "types.h"

void FSi_InitRomArchive(u32 param_1);
static u32 data_02150e04[2];
void func_02078c40(void);

void FS_Init(u32 dma) {
    if (data_02150e04[0] != 0) {
        return;
    }
    data_02150e04[0] = 1;
    FSi_InitRomArchive(dma);
    func_02078c40();
}

BOOL FS_IsAvailable(void) {
    return data_02150e04[0];
}
