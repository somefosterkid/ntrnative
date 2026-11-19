#include "global.h"
struct SafariAreaCustomizer;

#include "touchscreen_list_menu.h"
#include "overlay_108_safari_area_customizer.h"

void ov108_021E79A8(SafariAreaCustomizer *arg0, u8 arg1, s32 arg2, s32 arg3);

extern const TouchscreenListMenuTemplate _021EA720[];

extern const u16 ov108_021EA748[][2];

extern const u16 ov108_021EA74A[][2];

void ov108_021E6A58(SafariAreaCustomizer *arg0);

void ov108_021E6A58(SafariAreaCustomizer *arg0) {
    TouchscreenListMenuHeader header;

    MI_CpuFill8(&header, 0, sizeof(TouchscreenListMenuHeader));
    header.template = _021EA720[1];
    header.listMenuItems = arg0->unk_4B4;
    header.bgConfig = arg0->bgConfig;
    header.numWindows = 3;
    arg0->listMenu = TouchscreenListMenu_Create(arg0->listMenuSpawner, &header, arg0->inTouchMode, ov108_021EA748[arg0->areaIdx][0], ov108_021EA74A[arg0->areaIdx][0], 0, 0);
    ov108_021E79A8(arg0, 1, arg0->areaIdx, 1);
    ov108_021E79A8(arg0, 4, arg0->areaIdx, 1);
}
