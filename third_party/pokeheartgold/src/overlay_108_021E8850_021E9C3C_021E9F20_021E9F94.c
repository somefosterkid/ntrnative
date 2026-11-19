#include "global.h"

#include "bg_window.h"
#include "gf_gfx_loader.h"
#include "overlay_108_021E8850.h"
#include "overlay_108_safari_decoration_app.h"

void ov108_021E9F94(SafariDecorationApp *param_1, s32 param_2) {
    NNSG2dScreenData *scrn;

    if (param_2 == 0) {
        scrn = param_1->scrn1;
    } else {
        scrn = param_1->scrn2;
    }
    FillBgTilemapRect(param_1->bgConfig, 5, 0, 0, 5, 0x20, 0xe, 0x11);
    FillBgTilemapRect(param_1->bgConfig, 6, 0, 0, 5, 0x20, 0xe, 0x11);
    CopyToBgTilemapRect(param_1->bgConfig, 7, 0, 0, 0x20, 0x18, scrn->rawData, 0, 0, (u8)(scrn->screenWidth >> 3), (u8)(scrn->screenHeight >> 3));
    ScheduleBgTilemapBufferTransfer(param_1->bgConfig, 5);
    ScheduleBgTilemapBufferTransfer(param_1->bgConfig, 6);
    ScheduleBgTilemapBufferTransfer(param_1->bgConfig, 7);
    return;
}
