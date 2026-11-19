#include "global.h"
#include "overlay_109_photo_album_app.h"

s32 ov109_021E682C(PhotoAlbumApp *app) {
    switch (app->subState) {
    case 0:
        ov109_021E76F0(app);
        app->subState = app->subState + 1;
        break;
    case 1:
        if (ov109_021E7748(app) != 7) {
            app->subState = 0;
            return 1;
        }
        break;
    }
    return 7;
}
