#include "global.h"

#include "gf_gfx_planes.h"
#include "overlay_113.h"

void ov113_021E677C(UnownReportData *param_1);

s32 ov113_021E69CC(UnownReportData *);

void ov113_021E6988(UnownReportData *arg0);

void ov113_021E6988(UnownReportData *arg0) {
    ov113_021E677C(arg0);
    ov113_021E69CC(arg0);
    GfGfx_EngineATogglePlanes(0x10U, 1U);
    GfGfx_EngineBTogglePlanes(0x10U, 1U);
}

s32 ov113_021E6930(UnownReportData *);

s32 ov113_021E6AB0(UnownReportData *);

void ov113_021E69A8(UnownReportData *arg0);

void ov113_021E69A8(UnownReportData *arg0) {
    GfGfx_EngineBTogglePlanes(0x10U, 0U);
    GfGfx_EngineATogglePlanes(0x10U, 0U);
    ov113_021E6AB0(arg0);
    ov113_021E6930(arg0);
}
