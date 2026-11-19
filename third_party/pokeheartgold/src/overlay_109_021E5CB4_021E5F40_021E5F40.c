#include "global.h"

#include "overlay_109_photo_album_app.h"
#include "unk_02005D10.h"

extern void *_021E7884;

s32 ov109_021E5F40(PhotoAlbumApp *app, s32 arg1) {
    u8 temp_r4;

    switch (arg1) {
    case 12:
        app->args->photoWasSelected = 0;
        PlaySE(0x5DCU);
        ov109_021E7388(app, 1);
        return 8;
    case 13:
        if (app->page == 0) {
            return 1;
        }
        ov109_021E7584(app, 2);
        PlaySE(0x920U);
        return 2;
    case 14:
        if (app->page == (app->numPages - 1)) {
            return 1;
        }
        ov109_021E7584(app, 3);
        PlaySE(0x920U);
        return 3;
    default:
        temp_r4 = *(u8 *)(((u8 *)&_021E7884) + arg1) + (0xC * app->page);
        if ((u32)temp_r4 >= (u32)app->numEntries) {
            return 1;
        }
        app->heldIndex = temp_r4;
        app->heldCursorX = (u8)app->cursorX;
        app->heldCursorY = (u8)app->cursorY;
        PlaySE(0x5DCU);
        ov109_021E75C8(app);
        ov109_021E7248(app, app->entries[temp_r4].photo);
        ov109_021E74D4(app, 1);
        return 4;
    }
}
