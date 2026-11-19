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
void ov108_021E7BB4(SafariAreaCustomizer *arg0, u8 arg1, u8 arg2);

void ov108_021E61E8(SafariAreaCustomizer *arg0);

extern void ov108_021E6238(SafariAreaCustomizer *app);

void ov108_021E767C(SafariAreaCustomizer *param_1, s8 param_2);

extern void ov108_021E7700(SafariAreaCustomizer *app, int a1, int a2, int a3);

void ov108_021E79A8(SafariAreaCustomizer *arg0, u8 arg1, s32 arg2, s32 arg3);

int ov108_021E5F38(SafariAreaCustomizer *app);

int ov108_021E5F38(SafariAreaCustomizer *app) {
    BOOL ret;
    int touch;
    int res = YesNoPrompt_HandleInput(app->yesNoPrompt);

    switch (res) {
    case 1:
        ret = 1;
        u8 areaNo = (u8)(app->cursor + app->page * 6);
        if (areaNo != app->areaSet.areas[app->areaIdx].area_no) {
            app->areaChanged = 1;
            SafariZone_InitAreaInSet(&app->areaSet, app->areaIdx, areaNo);
        }
        break;
    case 2:
        u32 idx = app->areaIdx;
        ov108_021E7BB4(app, idx, app->areaSet.areas[idx].area_no);
        ret = 0;
        break;
    default:
        return 1;
    }

    touch = YesNoPrompt_IsInTouchMode(app->yesNoPrompt);
    if (touch != app->inTouchMode) {
        if (touch == 0) {
            ov108_021E61E8(app);
        } else {
            ov108_021E6238(app);
        }
    }
    app->inTouchMode = touch;
    YesNoPrompt_Reset(app->yesNoPrompt);
    ClearWindowTilemapAndCopyToVram(&app->windows[1]);
    ov108_021E767C(app, 0);
    ov108_021E7700(app, 0, 1, 1);
    ov108_021E79A8(app, 1, 0, 0);
    return ret ? 2 : 0;
}
