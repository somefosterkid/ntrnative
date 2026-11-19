#include "global.h"

#include "overlay_109_photo_album_app.h"

void ov109_021E68B8(PhotoAlbumApp *app, u32 value) {
    app->bgToggleValue = (u8)value;
    app->bgTogglePending = 1;
}
