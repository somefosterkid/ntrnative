#include "global.h"

#include "overlay_109_photo_album_app.h"

void ov109_021E7114(PhotoAlbumApp *app, u8 param_2, u8 param_3, u8 param_4, u8 param_5) {
    unsigned int cVar1;
    NNSG2dScreenData *new_var2;
    NNSG2dScreenData *puVar2;
    s8 new_var;

    new_var2 = app->scrn[1].data;
    puVar2 = new_var2;
    new_var = (cVar1 = param_2 / 6);
    CopyToBgTilemapRect(app->bgConfig, param_3, param_4, param_5, 5, 5, app->scrn[1].data->rawData, (param_2 % 6) * 5, cVar1 * 5, (u8)((u32)new_var2->screenWidth >> 3), (u8)((u32)puVar2->screenHeight >> 3));
    return;
}
