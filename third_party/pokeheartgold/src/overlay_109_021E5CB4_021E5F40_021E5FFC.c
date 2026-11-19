#include "global.h"

#include "overlay_109_photo_album_app.h"
#include "touchscreen.h"
#include "unk_02005D10.h"

extern TouchscreenHitbox ov109_021E7A18;

s32 ov109_021E5FFC(PhotoAlbumApp *app, s32 *arg1) {
    u32 temp_r0;

    temp_r0 = TouchscreenHitbox_FindRectAtTouchNew(&ov109_021E7A18);
    if (temp_r0 == -1) {
        return 1;
    }
    if (app->menuInputState == MENU_INPUT_STATE_BUTTONS) {
        ov109_021E75B4(app);
    }
    *arg1 = 1;
    if ((u32)temp_r0 < 0xCU) {
        app->cursorX = (u8)(3 & temp_r0);
        app->cursorY = (u8)((u32)temp_r0 >> 2);
        ov109_021E7474(app, app->cursorX, app->cursorY, 1);
        PlaySE(0x5DCU);
    }
    return ov109_021E5F40(app, (u8)temp_r0);
}
