#include "global.h"

#include "overlay_72_wfc_connect.h"

extern s32 (*ov72_0223B744[])(NintendoWfcConnect *);

s32 ov72_0223A588(NintendoWfcConnect *wfc);

s32 ov72_0223A588(NintendoWfcConnect *wfc) {
    if (ov72_0223B744[wfc->numberInput.state](wfc) == 1) {
        return wfc->numberInput.value;
    }
    return 0;
}
