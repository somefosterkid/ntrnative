#include "global.h"

#include "sprite.h"
#include "sprite_system.h"
#include "overlay_109_photo_album_app.h"

void ov109_021E74D4(PhotoAlbumApp *app, s32 arg1) {
    s32 flag;
    u32 v0;

    flag = arg1;
    if (app->page != (u8)((s32) app->heldIndex / 12)) {
        flag = 0;
    }
    v0 = app->heldCursorX;
    Sprite_SetPositionXY(app->sprites[1], (s16)((v0 * 0x30) + 0x1C + ((v0 >> 1) * 8)), (s16)((0x28 * app->heldCursorY) + 8));
    Sprite_SetDrawFlag(app->sprites[1], flag);
}

void ov109_021E7524(PhotoAlbumApp *app) {
    u8 v0;

    Sprite_SetAnimActiveFlag(app->sprites[2], 0);
    Sprite_SetAnimActiveFlag(app->sprites[3], 0);
    if (app->page == 0) {
        Sprite_SetAnimCtrlSeq(app->sprites[2], 2);
    } else {
        Sprite_SetAnimCtrlSeq(app->sprites[2], 0);
    }
    v0 = app->numPages;
    if ((v0 == 0) || (app->page == (v0 - 1))) {
        Sprite_SetAnimCtrlSeq(app->sprites[3], 5);
        return;
    }
    Sprite_SetAnimCtrlSeq(app->sprites[3], 3);
}
