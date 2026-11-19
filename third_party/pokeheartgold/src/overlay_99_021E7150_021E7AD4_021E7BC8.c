#include "global.h"
struct PokeathlonMedals;

#include "system.h"
#include "touchscreen.h"
#include "unk_02019BA4.h"
#include "overlay_99_pokeathlon_medals.h"

s32 ov99_021E7AD4(void *, s32, s32);

extern TouchscreenHitbox ov99_021E9F54;

s32 ov99_021E7BC8(struct PokeathlonMedals *arg0);

s32 ov99_021E7BC8(PokeathlonMedals *arg0) {
    s32 temp_r0;
    s32 temp_r4;
    s32 temp_r6;
    temp_r6 = GridInputHandler_HandleInput_AllowHold(arg0->grid);
    temp_r4 = GridInputHandler_GetNextInput(arg0->grid);
    temp_r0 = TouchscreenHitbox_FindHitboxAtTouchNew(&ov99_021E9F54);
    if (temp_r0 != -1) {
        if (temp_r0 == 0) {
            ov99_021E7AD4(arg0, 0, 1);
        } else if (temp_r0 == 1) {
            ov99_021E7AD4(arg0, 1, 1);
        }
    } else {
        if ((temp_r6 == (-1)) && (((u32)temp_r4) < 0x1EU)) {
            if (gSystem.newAndRepeatedKeys == 0x20) {
                ov99_021E7AD4(arg0, 0, 0);
            } else if (gSystem.newAndRepeatedKeys == 0x10) {
                ov99_021E7AD4(arg0, 1, 0);
            }
        } else if ((temp_r4 == 0x1E) && (gSystem.newKeys & 1)) {
            return 1;
        }
    }

    return 0;
}
