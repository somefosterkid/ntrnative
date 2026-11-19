#include "global.h"

#include "touchscreen_list_menu.h"
#include "overlay_109_photo_album_app.h"

extern const TouchscreenListMenuTemplate ov109_021E789C;

void ov109_021E75C8(PhotoAlbumApp *app) {
    TouchscreenListMenuHeader header;

    MI_CpuFill8(&header, 0, sizeof(TouchscreenListMenuHeader));
    header.template = ov109_021E789C;
    header.listMenuItems = app->listMenuItems;
    header.bgConfig = app->bgConfig;
    header.numWindows = 4;
    app->listMenu = TouchscreenListMenu_Create(app->listMenuSpawner, &header, app->menuInputState, 24, 11, 6, 0);
    ov109_021E7474(app, app->cursorX, app->cursorY, 0);
}
