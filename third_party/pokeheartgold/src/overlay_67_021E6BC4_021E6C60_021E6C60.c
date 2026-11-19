#include "global.h"

#include "overlay_67_app.h"

void ov67_021E6C60(Ov67App *app, s32 param_2, s32 param_3);

void ov67_021E6C60(Ov67App *app, s32 param_2, s32 param_3) {
    u16 *idx;
    app->unk_4A8 = 0;
    if ((param_2 == 2 && param_3 == 0) || (param_2 == 5 && param_3 == 3) || (param_2 == 8 && param_3 == 6) || (param_2 == 0xb && param_3 == 9)) {
        idx = &app->unk_4A0;
        if (*idx != 0) {
            *idx = *idx - 1;
        } else {
            *idx = 2;
        }
        ov67_021E6490(app);
        ov67_021E6530(app);
        app->unk_4A8 = 1;
    }
    if ((param_2 == 0 && param_3 == 2) || (param_2 == 3 && param_3 == 5) || (param_2 == 6 && param_3 == 8) || (param_2 == 9 && param_3 == 0xb)) {
        idx = &app->unk_4A0;
        if (*idx < 2) {
            *idx = *idx + 1;
        } else {
            *idx = 0;
        }
        ov67_021E6490(app);
        ov67_021E6530(app);
        app->unk_4A8 = 2;
    }
    ov67_021E6688(app, param_2 == 0xc ? 0x1e : param_2 + *(&app->unk_4A0) * 0xc);
    ov67_021E6C14(app, param_2);
}
