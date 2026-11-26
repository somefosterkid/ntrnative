#include "global.h"

#include "bg_window.h"
#include "overlay_86_frontier_records_app.h"

void ov86_021E7B68(FrontierRecordsApp *arg0) {
    int base;
    u8 flag;
    int start;
    u8 i;

    if (arg0->windowSet == 0) {
        base = 5;
    } else {
        base = 0xD;
    }
    i = 0;
    start = (u16)(arg0->page * 8);
    do {
        FillWindowPixelBuffer(&arg0->windows[base + i], 0);
        if ((u32)arg0->lists[arg0->category].count > (u32)(start + i)) {
            ov86_021E668C(arg0, arg0->lists[arg0->category].entries[start + i]);
            ov86_021E6064(arg0, i + base, 0x13, (GetWindowWidth(&arg0->windows[base + i]) * 8) / 2, 0, FONT_UNK_4, 0xF0100, 2);
            flag = 0;
        } else {
            flag = 1;
        }
        ScheduleWindowCopyToVram(&arg0->windows[i + base]);
        ov86_021E78B8(arg0, i);
        BgTilemapRectChangePalette(arg0->bgConfig, 2, arg0->flash.x, arg0->flash.y, arg0->flash.width, arg0->flash.height, flag);
        i++;
    } while (i < 8);
    ScheduleBgTilemapBufferTransfer(arg0->bgConfig, 2);
    arg0->windowSet ^= 1;
}
