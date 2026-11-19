#include "global.h"
struct HallOfFameShowcase;

#include "system.h"
#include "touchscreen.h"
#include "unk_02005D10.h"
#include "unk_02019BA4.h"
#include "overlay_64_hall_of_fame_showcase.h"

void ov64_021E62A8(void);

void ov64_021E64F8(void *arg0);

s32 ov64_021E652C(void *, s32);

s32 ov64_021E677C(void *, s32);

s32 ov64_021E6B84(void *);

extern TouchscreenHitbox _021E6E7C;

s32 ov64_021E62C8(struct HallOfFameShowcase *arg0);

s32 ov64_021E62C8(HallOfFameShowcase *arg0) {
    s32 temp_r0;
    s32 temp_r0_3;
    s32 temp_r3;
    s32 temp_r3_2;
    u32 temp_r0_2;
    u32 temp_r1;

    temp_r1 = *((u32 *)&arg0->state);
    switch (temp_r1) {
    case 0:
        temp_r0 = TouchscreenHitbox_FindRectAtTouchNew(&_021E6E7C);
        if (temp_r0 == 0) {
            if ((s32) *((s32 *)&arg0->record) > 0) {
                *((s32 *)&arg0->pressedButton) = 0xF;
                *((u32 *)&arg0->nextState) = 2U;
                *((u32 *)&arg0->state) = 1U;
                ov64_021E64F8(arg0);
                PlaySE(0x5DCU);
            }
        } else if (temp_r0 == 1) {
            temp_r3 = *((s32 *)&arg0->numRecords);
            if ((temp_r3 >= 1) && (*((s32 *)&arg0->record) != (temp_r3 - 1))) {
                *((s32 *)&arg0->pressedButton) = 0x10;
                *((u32 *)&arg0->nextState) = 3U;
                *((u32 *)&arg0->state) = 1U;
                ov64_021E64F8(arg0);
                PlaySE(0x5DCU);
            }
        } else {
            temp_r0_2 = (u32)GridInputHandler_HandleInput_AllowHold(arg0->gridInput);
            switch (temp_r0_2) {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
                ov64_021E677C(arg0, temp_r0_2);
                if ((gSystem.newKeys & 1) == 0) {
                    if (temp_r0_2 < arg0->numMons) {
                        PlaySE(0x5DCU);
                    }
                }
                break;
            case -4:
                break;
            case 6:
            case -2:
                PlaySE(0x5DCU);
                *((s32 *)&arg0->pressedButton) = 0x11;
                *((u32 *)&arg0->nextState) = 4U;
                *((u32 *)&arg0->state) = (u32)(*((u32 *)&arg0->state) + 1);
                break;
            case -3:
                ov64_021E677C(arg0, GridInputHandler_GetNextInput(arg0->gridInput));
                PlaySE(0x5DCU);
                break;
            case -1:
                temp_r0_3 = GridInputHandler_GetNextInput(arg0->gridInput);
                if (0x10 & gSystem.newAndRepeatedKeys) {
                    if ((temp_r0_3 == 2) || (temp_r0_3 == 5)) {
                        temp_r3_2 = *((s32 *)&arg0->numRecords);
                        if ((temp_r3_2 >= 1) && (*((s32 *)&arg0->record) != (temp_r3_2 - 1))) {
                            *((s32 *)&arg0->pressedButton) = 0x10;
                            *((u32 *)&arg0->nextState) = 3U;
                            *((u32 *)&arg0->state) = 1U;
                            ov64_021E64F8(arg0);
                            PlaySE(0x5DCU);
                        }
                    }
                } else if ((0x20 & gSystem.newAndRepeatedKeys) && ((temp_r0_3 == 0) || (temp_r0_3 == 3)) && ((s32) *((s32 *)&arg0->record) > 0)) {
                    *((s32 *)&arg0->pressedButton) = 0xF;
                    *((u32 *)&arg0->nextState) = 2U;
                    *((u32 *)&arg0->state) = 1U;
                    ov64_021E64F8(arg0);
                    PlaySE(0x5DCU);
                }
                break;
            }
        }
        break;
    case 1:
        if (ov64_021E6B84(arg0) == 0) {
            *((u32 *)&arg0->state) = (u32) *((u32 *)&arg0->nextState);
        }
        break;
    case 2:
        ov64_021E652C(arg0, -1);
        *((u32 *)&arg0->state) = 0U;
        break;
    case 3:
        ov64_021E652C(arg0, 1);
        *((u32 *)&arg0->state) = 0U;
        break;
    case 4:
        ov64_021E62A8();
        PlaySE(0x60DU);
        return 0;
    }
    return 1;
}
