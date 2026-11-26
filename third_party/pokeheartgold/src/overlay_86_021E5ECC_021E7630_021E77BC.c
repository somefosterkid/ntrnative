#include "global.h"

#include "bg_window.h"
#include "overlay_86_frontier_records_app.h"
#include "unk_02019BA4.h"
#include "unk_02020A0C.h"

void ov86_021E77BC(FrontierRecordsApp *arg0) {
    u32 v1;

    v1 = 0;
    do {
        if (arg0->lists[v1].count == 0) {
            ov86_021E7688(arg0, v1);
            BgTilemapRectChangePalette(arg0->bgConfig, 2U, arg0->flash.x, arg0->flash.y, (u8)(s32) arg0->flash.width, (u8)(s32) arg0->flash.height, 1U);
        }
        v1 += 1;
    } while (v1 < 0x1AU);
    ScheduleBgTilemapBufferTransfer(arg0->bgConfig, 2U);
}

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
