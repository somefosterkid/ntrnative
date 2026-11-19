#include "global.h"
struct SafariAreaCustomizer;

#include "bg_window.h"
#include "overlay_108_safari_area_customizer.h"

void ov108_021E767C(SafariAreaCustomizer *param_1, s8 param_2);

void ov108_021E767C(SafariAreaCustomizer *param_1, s8 param_2) {
    NNSG2dScreenData *scrn;
    FillBgTilemapRect(param_1->bgConfig, 0, 0, 0, 0x13, 0x18, 5, 0x11);
    scrn = param_1->unk_4D8;
    CopyToBgTilemapRect(param_1->bgConfig, 0, 0, 0x15 - (param_2 * 2), 0x18, (param_2 * 2) + 3, scrn->rawData, 0, (param_2 * 3), (u8)(((scrn->screenWidth) & ((s8)0x7ff)) >> 3), (u8)(scrn->screenHeight >> 3));
    ScheduleBgTilemapBufferTransfer(param_1->bgConfig, 0);
    return;
}
