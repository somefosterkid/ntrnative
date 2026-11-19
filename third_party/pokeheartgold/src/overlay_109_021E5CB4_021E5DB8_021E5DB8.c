#include "global.h"

#include "overlay_109_photo_album_app.h"

void ov109_021E5DB8(PhotoAlbumApp *app, s32 a, s32 b) {
    PhotoAlbumEntry tmp;

    MI_CpuCopy8(&app->entries[a], &tmp, sizeof(PhotoAlbumEntry));
    MI_CpuCopy8(&app->entries[b], &app->entries[a], sizeof(PhotoAlbumEntry));
    MI_CpuCopy8(&tmp, &app->entries[b], sizeof(PhotoAlbumEntry));
}
