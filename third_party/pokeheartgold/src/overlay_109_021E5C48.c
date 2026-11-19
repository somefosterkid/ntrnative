#include "global.h"

#include "overlay_109_photo_album_app.h"
#include "sys_task.h"

s32 ov109_021E5C48(PhotoAlbumApp *app) {
    if (ov109_021E656C(app) == 0) {
        return 2;
    }
    if (app->mode == 1) {
        return 5;
    }
    return 1;
}

s32 ov109_021E5C68(PhotoAlbumApp *app) {
    if (ov109_021E66C4(app) == 0) {
        return 3;
    }
    if (app->mode == 1) {
        return 5;
    }
    return 1;
}

s32 ov109_021E5C88(PhotoAlbumApp *app) {
    return ov109_021E681C(app);
}

short ov109_021E5C90(PhotoAlbumApp *app) {
    return ov109_021E61BC(app);
}
