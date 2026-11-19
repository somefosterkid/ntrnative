#include "global.h"

#include "gf_gfx_planes.h"
#include "overlay_109_photo_album_app.h"
#include "touchscreen_list_menu.h"

void ov109_021E6E9C(PhotoAlbumApp *app) {
    ov109_021E6DE4(app);
    ov109_021E6EE4(app);
    app->listMenuSpawner = TouchscreenListMenuSpawner_Create(app->heapID, NULL);
    GfGfx_EngineATogglePlanes(0x10U, 1U);
}

void ov109_021E6EC0(PhotoAlbumApp *app) {
    GfGfx_EngineATogglePlanes(0x10U, 0U);
    ov109_021E6F60(app);
    TouchscreenListMenuSpawner_Destroy(app->listMenuSpawner);
    ov109_021E6E64(app);
}
