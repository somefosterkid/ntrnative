#include "global.h"

#include "overlay_86_frontier_records_app.h"
#include "unk_02005D10.h"
#include "unk_02019BA4.h"

void ov86_021E71C0(void *);

s32 ov86_021E723C(void *);

s32 ov86_021E7688(void *, u32);

s32 ov86_021E7710(FrontierRecordsApp *arg0);

s32 ov86_021E7710(FrontierRecordsApp *arg0) {
    s32 temp_r7;
    u32 temp_r0;
    void *temp_r6;
    temp_r0 = (u32)GridInputHandler_HandleInput_NoHold(arg0->inputHandler);
    switch (temp_r0) {
    case 0xFFFFFFFEU:
    case 0x1AU:
        PlaySE(0x5DDU);
        ov86_021E71C0(arg0);
        ov86_021E723C(arg0);
        arg0->nextState = 0xA;
        return 9;

    case 0xFFFFFFFDU:
        PlaySE(0x5DCU);
        break;

    case 0xFFFFFFFCU:
    case 0xFFFFFFFFU:
        break;

    default:
        temp_r6 = &arg0->lists[0].count;

        temp_r7 = temp_r0 * 8;
        if ((*((s32 *)(temp_r6 + temp_r7))) == 0) {
            PlaySE(0x5F2U);
            break;
        }
        PlaySE(0x5DDU);
        arg0->category = (u16)temp_r0;
        arg0->cursor = 0;
        arg0->page = 0;
        arg0->lastPage = (s8)(((u32)((*((s32 *)(temp_r6 + temp_r7))) - 1)) >> 3);
        ov86_021E7688(arg0, arg0->category);
        arg0->nextState = 4;
        return 9;
    }
    return 3;
}
