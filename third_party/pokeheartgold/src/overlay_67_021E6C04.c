#include "global.h"

#include "overlay_67_app.h"
#include "unk_02019BA4.h"
#include "unk_02020A0C.h"

void ov67_021E6C5C(void);

void ov67_021E6C04(Ov67App *app) {
    GridInputHandler_Free(app->unk_4A4);
}

void ov67_021E6C14(Ov67App *app, s32 target) {
    u8 x;
    u8 y;

    DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(app->unk_4A4, target), &x, &y);
    ov67_021E6A28(app, 0, x, y);
    if (target == 0xC) {
        ov67_021E6A08(app, 0, 2);
        return;
    }
    ov67_021E6A08(app, 0, 7);
}

void ov67_021E6C5C(void) {
}
