#include "global.h"

#include "overlay_manager.h"

void ov74_02229F60(void *, u8);

void ov74_0222A94C(OverlayManager *, u32, u32);

u32 ov74_0222A5AC(OverlayManager *param_1);

u32 ov74_0222A5AC(OverlayManager *param_1) {
    void *pvVar1;

    pvVar1 = OverlayManager_GetData(param_1);
    ov74_02229F60((u8 *)pvVar1 + 0x38, 0);
    ov74_0222A94C(param_1, 0xc4, 0);
    return 3;
}
