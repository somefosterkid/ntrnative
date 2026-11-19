#include "global.h"

#include "overlay_109_photo_album_app.h"
#include "sprite_system.h"

void ov109_021E6F60(PhotoAlbumApp *app) {
    s32 v0;

    v0 = 0;
    do {
        thunk_Sprite_Delete(app->sprites[v0]);
        v0 += 1;
    } while (v0 < 5);
}
