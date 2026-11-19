#include "global.h"

#include "overlay_12_022378C0.h"
#include "unk_02034B0C.h"
#include "unk_02037C94.h"
#include "unk_0203A3B0.h"

void ov12_0223A7A0(void) {
    sub_0203A880();
    if (sub_02039998() != 0) {
        sub_0203A930(3 - DWC_GetLinkLevel());
        return;
    }
    if (sub_02035650() != 0) {
        sub_0203A930(3 - WM_GetLinkLevel());
    }
}
