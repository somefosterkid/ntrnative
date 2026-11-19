#include "global.h"

#include "bg_window.h"
#include "overlay_86_frontier_records_app.h"

s32 ov86_021E5FBC(FrontierRecordsApp *, s32, s32);

s32 ov86_021E6024(FrontierRecordsApp *, s32, s32, s32, s32, s32, s32, s32);

s32 ov86_021E6064(FrontierRecordsApp *, s32, s32, s32, s32, s32, s32, s32);

void ov86_021E7CF8(FrontierRecordsApp *arg0);

void ov86_021E64E0(FrontierRecordsApp *);
void ov86_021E7DA8(FrontierRecordsApp *param_1);
void ov86_021E7DD4(FrontierRecordsApp *param_1);

void ov86_021E7CF8(FrontierRecordsApp *arg0) {
    FillWindowPixelBuffer(&arg0->windows[22], 0U);
    ov86_021E5FBC(arg0, 0, arg0->page + 1);
    ov86_021E6064(arg0, 0x16, 0x28, 8, 0, 0, 0x10200, 1);
    ov86_021E6024(arg0, 0x16, 0x29, 0x10, 0, 0, 0x10200, 2);
    ov86_021E5FBC(arg0, 0, arg0->lastPage + 1);
    ov86_021E6064(arg0, 0x16, 0x28, 0x20, 0, 0, 0x10200, 1);
    ScheduleWindowCopyToVram(&arg0->windows[22]);
}

s32 ov86_021E705C(FrontierRecordsApp *, s32, s32);

s32 ov86_021E707C(FrontierRecordsApp *, s32, s16, s16);

void ov86_021E7D8C(FrontierRecordsApp *arg0);

void ov86_021E7D8C(FrontierRecordsApp *arg0) {
    ov86_021E707C(arg0, 1, 0xE0, 0xB0);
    ov86_021E705C(arg0, 1, 1);
}

void ov86_021E7DA8(FrontierRecordsApp *param_1) {
    param_1->species = param_1->lists[param_1->category].entries[(u32)param_1->cursor + param_1->page * 8];
    ov86_021E64E0(param_1);
    return;
}

void ov86_021E7DD4(FrontierRecordsApp *param_1) {
    ClearWindowTilemapAndScheduleTransfer(param_1->windows);
    ClearWindowTilemapAndScheduleTransfer(&param_1->windows[1]);
    ClearWindowTilemapAndScheduleTransfer(&param_1->windows[2]);
    ClearWindowTilemapAndScheduleTransfer(&param_1->windows[3]);
    return;
}
