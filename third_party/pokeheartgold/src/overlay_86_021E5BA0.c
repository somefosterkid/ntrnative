#include "global.h"

#include "overlay_86_frontier_records_app.h"
#include "palette.h"
#include "screen_fade.h"
#include "system.h"
#include "touchscreen.h"
#include "unk_02005D10.h"

s32 ov86_021E7094(FrontierRecordsApp *app);

s32 ov86_021E71FC(FrontierRecordsApp *app);

s32 ov86_021E7710(FrontierRecordsApp *app);

u32 ov86_021E7984(FrontierRecordsApp *app);

s32 ov86_021E7B54(FrontierRecordsApp *app);

extern u32 _021E7E98;

BOOL ov86_021E5BA0(FrontierRecordsApp *app);

BOOL ov86_021E5C94(FrontierRecordsApp *app);

BOOL ov86_021E5BA0(FrontierRecordsApp *app) {
    u8 uVar1;
    BOOL BVar2;
    s32 iVar3;

    switch (app->state) {
    case 0:
        iVar3 = ov86_021E71FC(app);
        if (iVar3 == 1) {
            app->state = 3;
        } else {
            app->state = 1;
        }
        break;
    case 1:
        BVar2 = TouchscreenHitbox_TouchNewIsIn((TouchscreenHitbox *)&_021E7E98);
        if ((BVar2 == 1) || ((gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) != 0)) {
            PlaySE(0x5dd);
            ov86_021E71C0(app);
            app->nextState = 10;
            app->state = 9;
        }
        break;
    case 2:
        ov86_021E7258(app, 1);
        app->state = 3;
        break;
    case 3:
        uVar1 = ov86_021E7710(app);
        app->state = uVar1;
        break;
    case 4:
        ov86_021E7258(app, 2);
        app->state = 5;
        break;
    case 5:
        uVar1 = ov86_021E7984(app);
        app->state = uVar1;
        break;
    case 6:
        uVar1 = ov86_021E7B54(app);
        app->state = uVar1;
        break;
    case 7:
        ov86_021E7258(app, 0);
        app->state = 8;
        break;
    case 8:
        BVar2 = TouchscreenHitbox_TouchNewIsIn((TouchscreenHitbox *)&_021E7E98);
        if ((BVar2 == 1) || ((gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) != 0)) {
            PlaySE(0x5dd);
            ov86_021E71C0(app);
            app->nextState = 4;
            app->state = 9;
        }
        break;
    case 9:
        iVar3 = ov86_021E7094(app);
        if (iVar3 == 0) {
            app->state = app->nextState;
        }
        break;
    case 10:
        return 1;
    }
    return 0;
}

BOOL ov86_021E5C94(FrontierRecordsApp *app) {
    switch (app->state) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0, 6, 1, HEAP_ID_121);
        app->state++;
        break;
    case 1:
        if (IsPaletteFadeFinished() == 1) {
            return 1;
        }
        break;
    }

    return 0;
}
