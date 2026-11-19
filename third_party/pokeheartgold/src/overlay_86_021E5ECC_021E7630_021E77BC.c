#include "global.h"

#include "bg_window.h"
#include "overlay_86_frontier_records_app.h"
#include "unk_02019BA4.h"
#include "unk_02020A0C.h"

s32 ov86_021E7688(void *, u32);

void ov86_021E77BC(FrontierRecordsApp *arg0);

void ov86_021E77BC(FrontierRecordsApp *arg0) {
    u32 v1;
    void *v0;

    v1 = 0;
    v0 = arg0;
    do {
        if (*(s32 *)((u8 *)v0 + 0x264) == 0) {
            ov86_021E7688(arg0, v1);
            BgTilemapRectChangePalette(arg0->bgConfig, 2U, arg0->flash.x, arg0->flash.y, (u8)(s32) arg0->flash.width, (u8)(s32) arg0->flash.height, 1U);
        }
        v1 += 1;
        v0 += 8;
    } while (v1 < 0x1AU);
    ScheduleBgTilemapBufferTransfer(arg0->bgConfig, 2U);
}

s32 ov86_021E705C(FrontierRecordsApp *, s32, s32);

s32 ov86_021E707C(FrontierRecordsApp *, s32, s16, s16);

void ov86_021E781C(FrontierRecordsApp *arg0, s32 arg1);

void ov86_021E781C(FrontierRecordsApp *arg0, s32 target) {
    u8 py[2];

    DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(arg0->inputHandler, target), &py[1], &py[0]);
    ov86_021E707C(arg0, 1, py[1], py[0]);
    if (target == 8) {
        ov86_021E705C(arg0, 1, 1);
        return;
    }
    ov86_021E705C(arg0, 1, 3);
}
