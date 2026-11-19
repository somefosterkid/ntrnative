#include "global.h"

#include "touchscreen.h"
#include "unk_02005D10.h"
#include "unk_02019BA4.h"
#include "overlay_67_app.h"

extern TouchscreenHitbox ov67_021E6D50;

s32 ov67_021E5C44(Ov67App *app) {
    s32 cnt;
    s32 cursor;
    s32 hit;
    u16 *cur;
    u32 input;

    input = (u32)GridInputHandler_HandleInput_NoHold(app->unk_4A4);
    switch (input) {
    case -2:
    case 12:
        PlaySE(0x5DDU);
        ov67_021E6BB8(app);
        app->unk_49C = 6;
        return 5;
    case -3:
        PlaySE(0x5DCU);
        cnt = app->unk_4A8;
        if (cnt == 1) {
            ov67_021E6B6C(app, 2);
            app->unk_49C = 4;
            return 5;
        }
        if (cnt == 2) {
            ov67_021E6B6C(app, 3);
            app->unk_49C = 4;
            return 5;
        }
        break;
    case -4:
    case -1:
        break;
    default:
        if (System_GetTouchNew() == 1) {
            PlaySE(0x5DCU);
        }
        break;
    }
    hit = TouchscreenHitbox_FindRectAtTouchNew(&ov67_021E6D50);
    switch (hit) {
    case 0:
        cur = &app->unk_4A0;
        cursor = *cur;
        if (cursor != 0) {
            cursor -= 1;
        } else {
            cursor = 2;
        }
        *cur = cursor;
        PlaySE(0x5E0U);
        ov67_021E6490(app);
        ov67_021E6688(app, (0xC * app->unk_4A0) + GridInputHandler_GetNextInput(app->unk_4A4));
        ov67_021E6530(app);
        ov67_021E6B6C(app, 2);
        app->unk_49C = 4;
        return 5;
    case 1:
        cur = &app->unk_4A0;
        cursor = *cur;
        if ((u32)cursor < 2U) {
            cursor += 1;
        } else {
            cursor = 0;
        }
        *cur = cursor;
        PlaySE(0x5E0U);
        ov67_021E6490(app);
        ov67_021E6688(app, (0xC * app->unk_4A0) + GridInputHandler_GetNextInput(app->unk_4A4));
        ov67_021E6530(app);
        ov67_021E6B6C(app, 3);
        app->unk_49C = 4;
        return 5;
    default:
        return 4;
    }
}
