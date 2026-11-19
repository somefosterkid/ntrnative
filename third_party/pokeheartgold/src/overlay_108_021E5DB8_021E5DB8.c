#include "global.h"
struct SafariAreaCustomizer;

#include "touchscreen_list_menu.h"
#include "overlay_108_safari_area_customizer.h"

void ov108_021E61E8(SafariAreaCustomizer *arg0);

SysTask *ov108_021E6238(void *);

void ov108_021E6804(SafariAreaCustomizer *arg0);

void ov108_021E79A8(SafariAreaCustomizer *arg0, u8 arg1, s32 arg2, s32 arg3);

s32 ov108_021E5DB8(SafariAreaCustomizer *arg0);

s32 ov108_021E5DB8(SafariAreaCustomizer *arg0) {
    TouchscreenListMenu *temp_r0_2;
    s32 temp_r0;
    u32 temp_r4;

    temp_r0 = TouchscreenListMenu_HandleInput(arg0->listMenu);
    if (temp_r0 == -1) {
        return 1;
    }
    temp_r0_2 = arg0->listMenu;
    temp_r4 = (u32)(*(u8 *)((u8 *)temp_r0_2 + 0x27) << 0x18) >> 0x19;
    TouchscreenListMenu_Destroy(temp_r0_2);
    if (temp_r4 != arg0->inTouchMode) {
        if (temp_r4 == 0) {
            ov108_021E61E8(arg0);
        } else {
            ov108_021E6238(arg0);
        }
    }
    arg0->inTouchMode = temp_r4;
    switch (temp_r0) {
    case 0:
        arg0->cursor = 0U;
        return 2;
    case 1:
        ov108_021E79A8(arg0, 1, 0, 0);
        ov108_021E79A8(arg0, 4, 0, 0);
        arg0->cursor = (u8) arg0->areaIdx;
        ov108_021E6804(arg0);
        arg0->unk_C = 4;
        return 4;
    case 2:
    default:
        ov108_021E79A8(arg0, 1, 0, 0);
        ov108_021E79A8(arg0, 4, 0, 0);
        return 0;
    }
}
