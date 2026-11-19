#include "global.h"

#include "bg_window.h"
#include "msgdata.h"
#include "pm_string.h"
#include "safari_zone.h"
#include "sprite_system.h"
#include "yes_no_prompt.h"
#include "overlay_108_safari_area_customizer.h"

// one layout for the Safari area customizer app (OverlayManager data, 0x184EC bytes: SafariAreaCustomizer_Init);
// every member's offset is used by ov108_021E5F38 / 021E64C0 / 021E7F7C / 021E81A8 (or 021E6450 / 021E7EB0 in src)
void ov108_021E77D4(SafariAreaCustomizer *param_1);

void ov108_021E78C0(SafariAreaCustomizer *param_1, s32 param_2, s32 param_3, s32 param_4);

void ov108_021E78F4(SafariAreaCustomizer *arg0, s32 arg1, u32 arg2);

void ov108_021E79A8(SafariAreaCustomizer *arg0, u8 arg1, s32 arg2, s32 arg3);

s32 ov108_021E84F8(void *, s32, s32);

void ov108_021E7F7C(SafariAreaCustomizer *app);

void ov108_021E7F7C(SafariAreaCustomizer *app) {
    int i;
    int base;

    if (app->unk_184E2_3 == 0) {
        FillBgTilemapRect(app->bgConfig, 1, 0, 0, 0x15, 0x20, 3, 0x11);
        ScheduleBgTilemapBufferTransfer(app->bgConfig, 1);
        app->unk_184E6 = -0xC0;
        app->unk_184E2_0 = 1;
        app->cursor = 0;
        ov108_021E78F4(app, 1, app->cursor);
        ov108_021E78F4(app, 2, app->areaIdx);
        ov108_021E77D4(app);
    } else {
        ov108_021E79A8(app, 1, 0xFF, 0);
        ov108_021E78C0(app, 1, 0, 0);
        base = app->unk_184E2_2 * 6;
        for (i = 0; i < 6; i++) {
            ManagedSprite_SetPositionXY(app->sprites[base + i], (i % 3) * 0x48 + 0x38, (i / 3) * 0x48 - 0x88);
            ManagedSprite_SetPriority(app->sprites[base + i], 2);
            ov108_021E84F8(app->unk_348, (u8)(i + base), app->areaSet.areas[i].area_no);
        }
    }
    app->unk_184E1 = 0;
    FillWindowPixelBuffer(&app->windows[4], 0xC);
    ScheduleWindowCopyToVram(&app->windows[4]);
    G2S_SetWnd0Position(0, 0, 240, 16);
    G2S_SetWnd0InsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3, 1);
    G2S_SetWndOutsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3 | GX_WND_PLANEMASK_OBJ, 1);
    GXS_SetVisibleWnd(GX_WNDMASK_W0);
}
