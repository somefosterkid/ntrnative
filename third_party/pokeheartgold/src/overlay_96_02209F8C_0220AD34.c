#include "global.h"

#include "error_handling.h"
#include "overlay_96_app_0220a87c.h"

u32 ov96_0220AD34(Ov96App_0220A87C *arg0);

u32 ov96_0220AD34(Ov96App_0220A87C *arg0) {
    if (arg0 == NULL) {
        GF_AssertFail();
    }
    return (u32)(arg0->unk_180 << 0x1C) >> 0x1C;
}

u16 ov96_0220AD4C(Ov96App_0220A87C *arg0);

u16 ov96_0220AD4C(Ov96App_0220A87C *arg0) {
    if (arg0 == NULL) {
        GF_AssertFail();
    }
    return (u16)((u32) arg0->unk_17C >> 0x10);
}
