#include "global.h"

#include "overlay_86_frontier_records_app.h"
#include "unk_02019BA4.h"
#include "unk_02020A0C.h"

s32 ov86_021E705C(FrontierRecordsApp *, s32, s32);

s32 ov86_021E707C(FrontierRecordsApp *, s32, s16, s16);

void ov86_021E75EC(FrontierRecordsApp *arg0, s32 arg1);

void ov86_021E75EC(FrontierRecordsApp *arg0, s32 target) {
    u8 py[2];

    DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(arg0->inputHandler, target), &py[1], &py[0]);
    ov86_021E707C(arg0, 1, py[1], py[0]);
    if (target == 0x1A) {
        ov86_021E705C(arg0, 1, 1);
        return;
    }
    ov86_021E705C(arg0, 1, 2);
}
