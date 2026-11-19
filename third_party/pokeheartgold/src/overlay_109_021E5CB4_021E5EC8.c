#include "global.h"

#include "bg_window.h"
#include "overlay_109_photo_album_app.h"

void ov109_021E5EC8(PhotoAlbumApp *app) {
    ov109_021E70C4(app, 3, 3, app->page);
    ov109_021E7524(app);
    ov109_021E7388(app, 0);
    ov109_021E73F8(app, 0, 1, 0);
    ov109_021E7474(app, app->cursorX, app->cursorY, 1);
    ScheduleBgTilemapBufferTransfer(app->bgConfig, 3U);
    ScheduleBgTilemapBufferTransfer(app->bgConfig, 2U);
}

s32 ov109_021E5F10(PhotoAlbumApp *app) {
    s32 v0;
    s32 v1;

    v0 = 0;
    if (app->busy != 0) {
        return 1;
    }
    v1 = ov109_021E5FFC(app, &v0);
    if (v0 == 0) {
        v1 = ov109_021E6058(app);
    }
    return v1;
}
