#include "global.h"

#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "overlay_86_frontier_records_app.h"
#include "palette.h"

u8 ov86_021E5E98(u8 facility);

void ov86_021E5E0C(FrontierRecordsApp *arg0) {
    ov86_021E5D10();
    ov86_021E5D30(arg0->bgConfig);
    arg0->plttData = PaletteData_Init(HEAP_ID_121);
    PaletteData_AllocBuffers(arg0->plttData, PLTTBUF_MAIN_OBJ, 0x200U, HEAP_ID_121);
    PaletteData_AllocBuffers(arg0->plttData, PLTTBUF_MAIN_BG, 0x200U, HEAP_ID_121);
    ov86_021E5ECC(arg0, GF_BG_LYR_MAIN_2);
    ov86_021E5F58(arg0, GF_BG_LYR_SUB_2);
}

void ov86_021E5E54(BgConfig *bgConfig) {
    GfGfx_EngineATogglePlanes(0x1FU, 0U);
    GfGfx_EngineBTogglePlanes(0x1FU, 0U);
    FreeBgTilemapBuffer(bgConfig, 0U);
    FreeBgTilemapBuffer(bgConfig, 2U);
    FreeBgTilemapBuffer(bgConfig, 4U);
    FreeBgTilemapBuffer(bgConfig, 6U);
    Heap_Free(bgConfig);
}

void ov86_021E5E90(FrontierRecordsApp *arg0, s32 *arg1, s32 arg2) {
    arg0->state = 0;
    *arg1 = arg2;
}

u8 ov86_021E5E98(u8 facility) {
    u8 windowGroup = 0;

    switch (facility) {
    case FACILITY_FACTORY:
    case FACILITY_FACTORY_OPEN:
        windowGroup = 0;
        break;
    case FACILITY_HALL:
        windowGroup = 1;
        break;
    case FACILITY_CASTLE:
        windowGroup = 2;
        break;
    case FACILITY_ARCADE:
        windowGroup = 3;
        break;
    case FACILITY_TOWER:
        windowGroup = 4;
        break;
    }

    return windowGroup;
}
