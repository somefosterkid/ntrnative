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

void ov108_021E81A8(SafariAreaCustomizer *app);

void ov108_021E81A8(SafariAreaCustomizer *app) {
    int i;
    int base;
    int dx;

    if (app->unk_184E2_1 == 0) {
        app->page++;
        dx = 0x100;
    } else {
        app->page--;
        dx = -0x100;
    }
    base = app->unk_184E2_2 * 6;
    for (i = 0; i < 6; i++) {
        ManagedSprite_SetPositionXY(app->sprites[base + i], dx + ((i % 3) * 0x50 + 0x30), (i / 3) * 0x48 + 0x38);
        ov108_021E84F8(app->unk_348, (u8)(i + base), (u8)(i + app->page * 6));
        ManagedSprite_SetPriority(app->sprites[base + i], 3);
    }
    app->unk_184E1 = 0;
}
