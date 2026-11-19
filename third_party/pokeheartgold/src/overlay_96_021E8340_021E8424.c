#include "global.h"

#include "error_handling.h"

extern u8 ov96_0221AA20[];

u16 ov96_021E8424(u32 param_1);

u16 ov96_021E8424(u32 param_1) {
    if ((param_1 != 0) && (param_1 <= (0x65 - 1))) {
        return *((u16 *)(ov96_0221AA20 + (((param_1 - 1) & 0xff) * 2)));
    }
    GF_AssertFail();
    return 0;
}
