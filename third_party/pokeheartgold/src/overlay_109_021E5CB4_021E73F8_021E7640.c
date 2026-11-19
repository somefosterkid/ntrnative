#include "global.h"

#include "touchscreen_list_menu.h"
#include "overlay_109_photo_album_app.h"

s32 ov109_021E7640(PhotoAlbumApp *app) {
    s32 v0;
    s32 v2;
    u8 v1;

    v0 = TouchscreenListMenu_HandleInput(app->listMenu);
    v2 = -1;
    if (v0 != -1) {
        v1 = TouchscreenListMenu_WasLastInputTouch(app->listMenu);
        TouchscreenListMenu_Destroy(app->listMenu);
        if (app->menuInputState != v1) {
            ov109_021E75B4(app);
        }
        switch (v0) {
        case 0:
            *(s8 *)((u8 *)(app->args) + 1) = 1;
            *(u8 *)((u8 *)(app->args) + 2) = (u8) app->heldIndex;
            return 8;
        case 1:
            ov109_021E73F8(app, 4, 0, 0);
            app->mode = 2;
            return 7;
        case 2:
            ov109_021E7474(app, app->cursorX, app->cursorY, 1);
            ov109_021E73F8(app, 2, 0, 0);
            app->mode = 1;
            return 5;
        default:
            ov109_021E7248(app, 0);
            ov109_021E74D4(app, 0);
            ov109_021E7474(app, app->cursorX, app->cursorY, 1);
            v2 = 1;
            return v2;
        }
    } else {
        return v2;
    }
}
