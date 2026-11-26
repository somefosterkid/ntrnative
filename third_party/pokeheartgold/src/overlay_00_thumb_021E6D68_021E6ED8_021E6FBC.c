#include "global.h"

#include "overlay_00_nintendo_wfcmanager.h"

void ov00_021E6388(u32);

extern NintendoWFCManager *_0221A680;

void ov00_021E6FBC(s32 param_1, s32 param_2, u32 param_3, u32 param_4, u32 param_5);

void ov00_021E6FBC(s32 param_1, s32 param_2, u32 param_3, u32 param_4, u32 param_5) {
    _0221A680->unused_1094 = 0;
    if ((param_1 == 0) && (param_2 == 0)) {
        ov00_021E6388(param_5);
    }
    return;
}
