#include "global.h"

#include "overlay_109_photo_album_app.h"

void ov109_021E5D08(PhotoAlbumApp *app) {
    u8 i;
    PhotoAlbumEntry *entry;

    app->numEntries = app->numPhotos;
    MI_CpuFill8(app->entries, 0, sizeof(app->entries));
    for (i = 0; i < app->numEntries; i++) {
        entry = &app->entries[i];
        entry->index = i;
        entry->photo = &app->photos[i];
    }
}
