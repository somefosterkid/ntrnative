// TwlSDK's MI library, mi_init.c: Black's main 0x0208361c-0x02083644. The file is libntr's (MIT,
// https://github.com/cybervisi0n/libntr) for SDK 5; the names are kh-recoded-decomp's twins' (MIT) or libntr's.

#include "types.h"
#include "nitro/os.h"
#include "nitro/mi.h"
#include "twl/mi.h"

void MI_SetWramBank(MIWram cnt);

void MI_Init(void) {
    BOOL BVar1;

    MI_SetWramBank(MI_WRAM_C|MI_WRAM_B);
    BVar1 = OS_IsRunOnTwl();
    if (BVar1 != 0) {
        MI_InitNDma();
    }
    MI_StopDma(0);
}
