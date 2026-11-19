#include "global.h"

#include "overlay_72_wfc_connect.h"

s32 ov72_0223A8A0(NintendoWfcNumberInput *arg0);

u8 ov72_0223A8DC(NintendoWfcNumberInput *arg0);

u8 ov72_0223A8DC(NintendoWfcNumberInput *arg0) {
    if (ov72_0223A8A0(arg0) != 0) {
        return 0xBU;
    }
    return arg0->cursor;
}
