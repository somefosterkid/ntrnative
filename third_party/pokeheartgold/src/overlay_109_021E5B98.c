#include "global.h"

#include "overlay_109_photo_album_app.h"
#include "unk_020210A0.h"

s32 ov109_021E5B98(PhotoAlbumApp *app) {
    DisableTouchPad();
    ov109_021E5CE4(app);
    return 1;
}
