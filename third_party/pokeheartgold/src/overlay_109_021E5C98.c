#include "global.h"

#include "overlay_109_photo_album_app.h"
#include "sys_task.h"
#include "system.h"

s32 ov109_021E5C98(PhotoAlbumApp *app) {
    if (ov109_021E61DC(app) != 0) {
        return 1;
    }
    return 6;
}

s32 ov109_021E5CAC(PhotoAlbumApp *app) {
    return ov109_021E682C(app);
}

void ov109_021E5CB4(PhotoAlbumApp *app) {
    ov109_021E694C(app);
    ov109_021E6B5C(app);
    ov109_021E6C9C(app);
    ov109_021E6D84(app);
    ov109_021E6E9C(app);
    Main_SetVBlankIntrCB(ov109_021E685C, app);
}

void ov109_021E5CE4(PhotoAlbumApp *app) {
    ov109_021E6EC0(app);
    ov109_021E6DBC(app);
    ov109_021E6D2C(app);
    ov109_021E6C7C(app);
    ov109_021E6AFC(app);
}
