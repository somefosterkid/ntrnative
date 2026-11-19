#include "global.h"

#include "overlay_109_photo_album_app.h"
#include "system.h"
#include "unk_02005D10.h"

s32 ov109_021E6058(PhotoAlbumApp *app) {
    u8 temp_r0;
    u8 temp_r0_2;
    u8 temp_r0_3;
    u8 temp_r1;
    u8 temp_r1_2;
    u8 var_r0;
    u8 var_r0_2;
    u8 var_r1;

    if ((app->menuInputState == MENU_INPUT_STATE_TOUCH) && (0xF3 & gSystem.newKeys)) {
        ov109_021E75B4(app);
    }
    if (1 & gSystem.newKeys) {
        temp_r0 = app->cursorY;
        var_r0_2 = app->cursorX;
        if (((u32)temp_r0) >= 3U) {
            var_r1 = 0xC;
        } else {
            var_r1 = var_r0_2 + (temp_r0 * 4);
        }
        return ov109_021E5F40(app, var_r1);
    }
    if (2 & gSystem.newKeys) {
        return ov109_021E5F40(app, 0xCU);
    }
    if (0x20 & gSystem.newKeys) {
        temp_r0_2 = app->cursorY;
        temp_r1 = app->cursorX;
        if (((u32)temp_r0_2) >= 3U) {
            var_r0 = 0xC;
        } else {
            var_r0 = temp_r1 + (temp_r0_2 * 4);
        }
        if (var_r0 == 0xC) {
            return 1;
        }
        if (temp_r1 != 0) {
            PlaySE(0x5DCU);
            app->cursorX = (u8)(app->cursorX - 1);
            ov109_021E7474(app, app->cursorX, app->cursorY, 1);
        } else if (app->page != 0) {
            return ov109_021E5F40(app, 0xDU);
        }
    } else {
        if (0x10 & gSystem.newKeys) {
            temp_r0_3 = app->cursorY;
            temp_r1_2 = app->cursorX;
            if (((u32)temp_r0_3) >= 3U) {
                var_r0_2 = 0xC;
            } else {
                var_r0_2 = temp_r1_2 + (temp_r0_3 * 4);
            }
            if (var_r0_2 == 0xC) {
                return 1;
            }
            if (((u32)temp_r1_2) < 3U) {
                PlaySE(0x5DCU);
                app->cursorX = (u8)(app->cursorX + 1);
                ov109_021E7474(app, app->cursorX, app->cursorY, 1);
            } else if (((s32)app->page) < ((s32)(app->numPages - 1))) {
                return ov109_021E5F40(app, 0xEU);
            }
        } else if (0x40 & gSystem.newKeys) {
            if ((app->cursorY == 4) && (((u32)app->cursorX) < 3U)) {
                app->cursorX = 3U;
            }
            PlaySE(0x5DCU);
            app->cursorY = (u8)((app->cursorY + 3) % 4);
            ov109_021E7474(app, app->cursorX, app->cursorY, 1);
        } else if (gSystem.newKeys & 0x80) {
            PlaySE(0x5DCU);
            app->cursorY = (u8)((app->cursorY + 1) % 4);
            ov109_021E7474(app, app->cursorX, app->cursorY, 1);
        }
    }
    return 1;
}
