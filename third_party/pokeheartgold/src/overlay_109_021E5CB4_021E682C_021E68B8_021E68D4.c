#include "global.h"

#include "bg_window.h"
#include "overlay_109_photo_album_app.h"

void ov109_021E68D4(PhotoAlbumApp *app) {
    MtxFx22 MStack_1c;

    if (app->affineUpdate) {
        MStack_1c._01 = 0;
        MStack_1c._10 = 0;
        MStack_1c._11 = FX_Inv(0x1000);
        MStack_1c._00 = FX_Inv(app->affineScale);
        SetBgAffine(app->bgConfig, 2, &MStack_1c, 0x80, 0x78);
        app->affineUpdate = 0;
    }
    return;
}
