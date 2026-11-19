#include "global.h"

#include "overlay_67_app.h"

void ov67_021E6D20(Ov67App *app, s32 arg1);

void ov67_021E6D20(Ov67App *app, s32 arg1) {
    s32 v0;

    if (arg1 == 0xC) {
        v0 = 0x1E;
    } else {
        v0 = arg1 + (0xC * app->unk_4A0);
    }
    ov67_021E6688(app, v0);
    ov67_021E6C14(app, arg1);
}
