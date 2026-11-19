#include "global.h"

#include "overlay_109_photo_album_app.h"
#include "photo_album.h"

void ov109_021E5D5C(PhotoAlbumApp *app) {
    u8 idx;

    idx = 0;
    while ((u32)idx < (u32)app->numEntries) {
        PhotoAlbum_SetPhotoAtIndex(app->photoAlbum, app->entries[idx].photo, idx);
        idx += 1;
    }
    if ((u32)idx < (u32)app->numPhotos) {
        do {
            PhotoAlbum_DeletePhotoByIndex(app->photoAlbum, idx);
            idx += 1;
        } while ((u32)idx < (u32)app->numPhotos);
    }
}
