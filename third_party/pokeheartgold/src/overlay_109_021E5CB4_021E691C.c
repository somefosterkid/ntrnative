#include "global.h"

#include "overlay_109_photo_album_app.h"

void ov109_021E691C(PhotoAlbumApp *app, s32 scale) {
    app->affineScale = scale;
    app->affineUpdate = 1;
}
