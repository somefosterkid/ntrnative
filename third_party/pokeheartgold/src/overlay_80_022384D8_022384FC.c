#include "global.h"

#include "heap.h"
#include "sys_task_api.h"
#include "bg_window.h"
#include "overlay_80_022384D8.h"
#include "gf_gfx_planes.h"

void ov80_02238530(SysTask *task, void *data);

void ov80_02238530(SysTask *task, void *taskData);
s32 ov80_022385C4(void *);

WFCFacilitySelectorMonitorFlicker *ov80_022384FC(BgConfig *bgConfig) {
    WFCFacilitySelectorMonitorFlicker *flicker;

    flicker = Heap_Alloc(HEAP_ID_101, sizeof(WFCFacilitySelectorMonitorFlicker));
    MI_CpuFill8(flicker, 0U, sizeof(WFCFacilitySelectorMonitorFlicker));
    flicker->bgConfig = bgConfig;
    flicker->monitorState = 0;
    flicker->task = SysTask_CreateOnMainQueue(ov80_02238530, flicker, 0x1368CU);
    return flicker;
}

void ov80_02238530(SysTask *task, void *taskData) {
    WFCFacilitySelectorMonitorFlicker *flicker = taskData;

    if (flicker->delay < 2) {
        flicker->delay++;
        return;
    }

    flicker->delay = 0;
    flicker->monitorState ^= 1;

    u16 tiles[4];
    ov80_0223857C(tiles, flicker->monitorState);

    LoadRectToBgTilemapRect(flicker->bgConfig, GF_BG_LYR_MAIN_3, tiles, 14, 2, 2, 2);
    ScheduleBgTilemapBufferTransfer(flicker->bgConfig, GF_BG_LYR_MAIN_3);
}

void ov80_0223857C(u16 *tiles, u8 tileset) {
    u32 offset = tileset == 0 ? 12 : 14;

    for (u32 r = 0; r < 2; r++) {
        for (u32 c = 0; c < 2; c++) {
            tiles[(r * 2) + c] = ((16 * 6) + r * 16) + (offset + c);
        }
    }
}

void ov80_022385B0(WFCFacilitySelectorEffects *effects) {
    ov80_022385C4(effects->flicker);
    Heap_Free(effects);
}
