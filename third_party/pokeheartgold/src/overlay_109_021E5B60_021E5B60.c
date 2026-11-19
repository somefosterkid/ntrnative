#include "global.h"

#include "overlay_109_photo_album_app.h"
#include "unk_020210A0.h"

s32 ov109_021E5B60(PhotoAlbumApp *app) {
    s32 state;

    state = app->state;
    switch (state) {
    case 0:
        ov109_021E5CB4(app);
        app->state = app->state + 1;
        break;
    case 1:
        ov109_021E5EC8(app);
        EnableTouchPad();
        InitializeTouchPad(4);
        app->state = 0;
        return 1;
    }
    return 0;
}
