// TwlSDK's OS library, os_systemWork.c: Black's main 0x02087b04-0x02087b78. The file is libntr's (MIT,
// https://github.com/cybervisi0n/libntr) for SDK 5; the names are kh-recoded-decomp's twins' (MIT) or libntr's.

#include "types.h"
#include "nitro/os.h"

u32 OS_GetBootInfo(void);

u16 OS_GetBootType(void) {
    return *((u16 *)OS_GetBootInfo());
}

u32 OS_GetBootInfo(void) {
    return 0x2fffc40;
}

static s32 data_02151360[2];

BOOL OSi_IsCodecTwlMode(void) {
    if (data_02151360[1] == 0) {
        data_02151360[0] = OS_IsRunOnTwl() == 1 && (*(u8 *)0x02ffe1bf & 1) == 1;
    }
    data_02151360[1] = 1;
    return data_02151360[0];
}
