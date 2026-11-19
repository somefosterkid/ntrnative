#include "global.h"

#include "bg_window.h"
#include "overlay_109_photo_album_app.h"
#include "text.h"
#include "unk_02005D10.h"

s32 ov109_021E61DC(PhotoAlbumApp *app) {
    u16 temp_r1;
    u16 temp_r1_2;
    u8 new_var;
    u16 temp_r2;

    temp_r1 = app->subState;
    switch (temp_r1) {
    case 0:
        temp_r2 = app->timer;
        app->timer = (u16)(temp_r2 + 1);
        if (((u32)temp_r2) < 0xFU) {
            return 0;
        }
        new_var = app->page;
        ov109_021E70C4(app, 3, 3, new_var);
        ov109_021E73F8(app, 3, 0, app->textFrameDelay);
        app->timer = 0U;
        app->subState = (u16)(app->subState + 1);
        break;
    case 1:
        temp_r1_2 = app->timer;
        app->timer = (u16)(temp_r1_2 + 1);
        if (((u32)temp_r1_2) < 0x1EU) {
            return 0;
        }
        if (TextPrinterCheckActive(app->textPrinterId) != 0) {
            return 0;
        }
        app->timer = 0U;
        app->subState = (u16)(app->subState + 1);
        break;
    default:
        app->mode = 0;
        PlaySE(0x5DCU);
        ov109_021E7474(app, app->cursorX, app->cursorY, 1);
        ov109_021E73F8(app, 0, 0, 0U);
        ScheduleBgTilemapBufferTransfer(app->bgConfig, 3U);
        app->timer = 0U;
        app->subState = 0U;
        return 1;
    }
    return 0;
}

extern void *_021E7884;

s32 ov109_021E628C(PhotoAlbumApp *app, s32 arg1) {
    u8 temp_r1;
    u8 temp_r4;

    switch (arg1) {
    case 12:
        PlaySE(0x5DCU);
        ov109_021E7248(app, 0);
        ov109_021E74D4(app, 0);
        ov109_021E77D4(app);
        ov109_021E73F8(app, 0, 0, 0);
        app->mode = 0;
        return 1;
    case 13:
        if (app->page == 0) {
            return 5;
        }
        ov109_021E7584(app, 2);
        PlaySE(0x920U);
        return 2;
    case 14:
        if (app->page == (app->numPages - 1)) {
            return 5;
        }
        ov109_021E7584(app, 3);
        PlaySE(0x920U);
        return 3;
    default:
        temp_r4 = *(u8 *)(((u8 *)&_021E7884) + arg1) + (0xC * app->page);
        if ((u32)temp_r4 >= (u32)app->numEntries) {
            return 5;
        }
        PlaySE(0x5DCU);
        ov109_021E74D4(app, 0);
        ov109_021E7248(app, 0);
        temp_r1 = app->heldIndex;
        if (temp_r4 != temp_r1) {
            ov109_021E5DB8(app, temp_r1, temp_r4);
            ov109_021E71BC(app, app->heldIndex, temp_r4);
            ov109_021E7474(app, app->cursorX, app->cursorY, 0);
            app->timer = 0;
            return 6;
        }
        app->mode = 0;
        ov109_021E73F8(app, 0, 0, 0);
        return 1;
    }
}
