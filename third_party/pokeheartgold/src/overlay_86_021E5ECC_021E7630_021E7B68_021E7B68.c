#include "global.h"

#include "bg_window.h"
#include "overlay_86_frontier_records_app.h"

s32 ov86_021E6064(FrontierRecordsApp *, s32, s32, s32, s32, s32, s32, s32);

s32 ov86_021E668C(void *, s32);

s32 ov86_021E78B8(void *, s32);

void ov86_021E7B68(FrontierRecordsApp *arg0);

void ov86_021E7B68(FrontierRecordsApp *arg0) {
    int base;
    u8 flag;
    int start;
    u32 off2;
    Window *wins;
    u8 *ent;
    u8 i;

    if (arg0->windowSet == 0) {
        base = 5;
    } else {
        base = 0xD;
    }
    wins = (Window *)arg0;
    i = 0;
    start = (u16)(arg0->page * 8);
    off2 = start * 2;
    wins = (Window *)((u8 *)wins + 0x10);
    do {
        FillWindowPixelBuffer(&wins[base + i], 0);
        ent = (u8 *)arg0 + arg0->category * 8;
        if (*(u32 *)(ent + 0x264) > (u32)(start + i)) {
            ov86_021E668C(arg0, *(u16 *)((off2 + *(int *)(ent + 0x260)) + i * 2));
            ov86_021E6064(arg0, i + base, 0x13, (GetWindowWidth(&wins[base + i]) * 8) / 2, 0, 4, 0xF0100, 2);
            flag = 0;
        } else {
            flag = 1;
        }
        ScheduleWindowCopyToVram(&wins[i + base]);
        ov86_021E78B8(arg0, i);
        BgTilemapRectChangePalette(arg0->bgConfig, 2, arg0->flash.x, arg0->flash.y, arg0->flash.width, arg0->flash.height, flag);
        i++;
    } while (i < 8);
    ScheduleBgTilemapBufferTransfer(arg0->bgConfig, 2);
    arg0->windowSet ^= 1;
}
