#include "global.h"

#include "overlay_109_photo_album_app.h"

#include "system.h"
#include "unk_02005D10.h"

s32 ov109_021E63E8(PhotoAlbumApp *app) {
    u8 idx;
    u8 row;
    u8 col;

    if ((app->menuInputState == MENU_INPUT_STATE_TOUCH) && (0xF3 & gSystem.newKeys)) {
        ov109_021E75B4(app);
    }
    if (1 & gSystem.newKeys) {
        row = app->cursorY;
        col = app->cursorX;
        if (row >= 3U) {
            idx = 0xC;
        } else {
            idx = col + row * 4;
        }
        return ov109_021E628C(app, idx);
    }
    if (2 & gSystem.newKeys) {
        return ov109_021E628C(app, 0xC);
    }
    if (0x20 & gSystem.newKeys) {
        row = app->cursorY;
        col = app->cursorX;
        if (row >= 3U) {
            idx = 0xC;
        } else {
            idx = col + row * 4;
        }
        if (idx == 0xC) {
            return 1;
        }
        if (col != 0) {
            PlaySE(0x5DC);
            app->cursorX = app->cursorX - 1;
            ov109_021E7474(app, app->cursorX, app->cursorY, 1);
        } else if (app->page != 0) {
            app->cursorX = (col + 3) % 4;
            return ov109_021E628C(app, 0xD);
        }
    } else if (0x10 & gSystem.newKeys) {
        row = app->cursorY;
        col = app->cursorX;
        if (row >= 3U) {
            idx = 0xC;
        } else {
            idx = col + row * 4;
        }
        if (idx == 0xC) {
            return 1;
        }
        if (col < 3U) {
            PlaySE(0x5DC);
            app->cursorX = app->cursorX + 1;
            ov109_021E7474(app, app->cursorX, app->cursorY, 1);
        } else if (app->page < app->numPages - 1) {
            app->cursorX = (col + 1) % 4;
            return ov109_021E628C(app, 0xE);
        }
    } else if (0x40 & gSystem.newKeys) {
        if ((app->cursorY == 4) && (app->cursorX < 3U)) {
            app->cursorX = 3;
        }
        PlaySE(0x5DC);
        app->cursorY = (app->cursorY + 3) % 4;
        ov109_021E7474(app, app->cursorX, app->cursorY, 1);
    } else if (gSystem.newKeys & 0x80) {
        PlaySE(0x5DC);
        app->cursorY = (app->cursorY + 1) % 4;
        ov109_021E7474(app, app->cursorX, app->cursorY, 1);
    }
    return 5;
}
