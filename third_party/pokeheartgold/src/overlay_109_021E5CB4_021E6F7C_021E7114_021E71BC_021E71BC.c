#include "global.h"

#include "bg_window.h"
#include "overlay_109_photo_album_app.h"

extern s32 ov109_021E7890;

void ov109_021E71BC(PhotoAlbumApp *app, s32 arg1, s32 arg2) {
    u8 temp_r3;
    u8 temp_r5;

    if ((arg1 / 12) == app->page) {
        temp_r3 = *(u8 *)(((u8 *)&ov109_021E7890) + (arg1 % 12));
        ov109_021E7178(app, 0, 3, (u8)(temp_r3 % 4), (u32)(temp_r3 << 0x16) >> 0x18);
    }
    if ((arg2 / 12) == app->page) {
        temp_r5 = *(u8 *)(((u8 *)&ov109_021E7890) + (arg2 % 12));
        ov109_021E7178(app, 0, 3, (u8)(temp_r5 % 4), (u32)(temp_r5 << 0x16) >> 0x18);
    }
    ScheduleBgTilemapBufferTransfer(app->bgConfig, 3U);
}
