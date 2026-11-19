#include "global.h"

#include "bg_window.h"
#include "overlay_109_photo_album_app.h"

void ov109_021E6F7C(PhotoAlbumApp *app, u32 arg1, s32 arg2) {
    u32 temp_r2;
    NNSG2dScreenData *temp_r0;
    BgClearTilemapBufferAndCommit(app->bgConfig, 2U);
    if (arg2 != 0) {
        temp_r0 = app->scrn[2].data;
        temp_r2 = (((s32)(arg1 + (arg1 >> 0x1F))) >> 1) * 0x10;
        CopyToBgTilemapRect(app->bgConfig, 2U, temp_r2, 0U, 0x10U, 0x18U, temp_r0->rawData, (u8)((s32)temp_r2), 0U, (u8)((u32)temp_r0->screenWidth >> 3), (u8)((u32)temp_r0->screenHeight >> 3));
    }
}

void ov109_021E6FD4(PhotoAlbumApp *app, u16 arg1, s32 arg2, s32 arg3) {
    u8 var_r4;
    u8 var_r2;
    NNSG2dScreenData *temp_r2;

    if (arg2 == 1) {
        var_r4 = 3;
        var_r2 = (arg3 * 2) + 1;
    } else {
        var_r4 = 0x1C;
        var_r2 = (arg3 * 2) + 2;
    }
    temp_r2 = app->scrn[2].data;
    CopyToBgTilemapRect(app->bgConfig, arg1, var_r4, 0xFU, 1U, 2U, temp_r2->rawData, (u8)(s32)var_r2, 0x18U, (u8)((u32)temp_r2->screenWidth >> 3), (u8)((u32)temp_r2->screenHeight >> 3));
}
