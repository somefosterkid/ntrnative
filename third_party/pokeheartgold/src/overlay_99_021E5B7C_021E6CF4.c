#include "global.h"

#include "error_handling.h"
#include "overlay_99_obj99.h"
#include "overlay_98_ov98text_sys.h"

void ov99_021E6CF4(Obj99 *arg0, u8 arg1);

void ov99_021E6CF4(Obj99 *arg0, u8 arg1) {
    if (arg1 > 0x28U) {
        GF_AssertFail();
    }
    ov98_0221EBD8(arg0->textSys, 6, arg1 / 2U + 0x21, 0);
}

void ov99_021E6D14(Obj99 *arg0, u32 arg1);

void ov99_021E6D14(Obj99 *arg0, u32 arg1) {
    if (arg1 > 0xAU) {
        GF_AssertFail();
    }
    ov98_0221EBD8(arg0->textSys, 9, arg1 + 0x11, 0);
}
