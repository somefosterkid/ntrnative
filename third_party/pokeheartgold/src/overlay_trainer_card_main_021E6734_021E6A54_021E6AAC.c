#include "global.h"

#include "touchscreen.h"
#include "unk_02005D10.h"

extern u32 _021E7DB8;

extern u8 gSystem;

extern u32 ov51_021E7DBC;

extern u32 ov51_021E7DC0;

u32 ov51_021E6AAC(u8 *param_1, u32 *param_2);

u32 ov51_021E6AAC(u8 *param_1, u32 *param_2) {
    BOOL BVar1;
    BVar1 = System_GetTouchNew();
    if (BVar1 == 0) {
        return 0;
    }
    BVar1 = TouchscreenHitbox_TouchNewIsIn((TouchscreenHitbox *)(&_021E7DB8));
    if (BVar1 != 0) {
        *param_2 = 1;
        PlaySE(0x940);
        return 5;
    }
    if ((((*((s32 *)(param_1 + 0x30f4))) != 0) && (((((u32)(*((u8 *)(param_1 + 0x343a)))) << 0x1e) >> 0x1f) == 0)) && (BVar1 = TouchscreenHitbox_TouchNewIsIn((TouchscreenHitbox *)(&ov51_021E7DBC)), BVar1 != 0)) {
        *param_2 = 1;
        PlaySE(0x5dc);
        return 4;
    }
    BVar1 = TouchscreenHitbox_TouchNewIsIn((TouchscreenHitbox *)(&ov51_021E7DC0));
    if (BVar1 != 0) {
        *((s8 *)(param_1 + 0x3440)) = (s8)((u16 *)(&gSystem))[0x30];
        *((s8 *)(param_1 + 0x3441)) = (s8)((u16 *)(&gSystem))[0x31];
        *param_2 = 1;
        return 3;
    }
    return 0;
}
