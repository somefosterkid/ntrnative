#include "global.h"
#include "overlay_108_021E8850.h"

u8 ov108_021EA700(s32 arg0, s32 arg1) {
    u8 v0;

    if (arg0 == 1) {
        if (arg1 != 1) {
            v0 = 1;
        } else {
            v0 = 0;
        }
        return v0;
    }
    if (arg1 == 1) {
        return 2U;
    }
    return 3U;
}
