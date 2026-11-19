#include "global.h"
#include "overlay_109_photo_album_app.h"

extern s32 ov109_021E78DC[];

s32 ov109_021E656C(PhotoAlbumApp *app) {
    u16 v0;

    v0 = app->subState;
    switch (v0) {
    case 0:
        ov109_021E7474(app, app->cursorX, app->cursorY, 0);
        ov109_021E74D4(app, 0);
        ov109_021E68B8(app, 1);
        ov109_021E691C(app, 0x1000);
        ov109_021E6F7C(app, 1, 1);
        ov109_021E70C4(app, 2, 1, app->page);
        ov109_021E70C4(app, 3, 1, (u8)(app->page - 1));
        app->scrollStep = 0;
        app->subState = (u16)(app->subState + 1);
    default:
        break;
    case 1:
        app->scrollStep = app->scrollStep + 1;
        ov109_021E691C(app, ov109_021E78DC[app->scrollStep]);
        if (app->scrollStep >= 4) {
            ov109_021E6F7C(app, 2, 1);
            ov109_021E70C4(app, 2, 2, (u8)(app->page - 1));
            app->subState = (u16)(app->subState + 1);
        }
        break;
    case 2:
        app->scrollStep = app->scrollStep - 1;
        ov109_021E691C(app, ov109_021E78DC[app->scrollStep]);
        if (app->scrollStep == 0) {
            ov109_021E70C4(app, 3, 2, (u8)(app->page - 1));
            ov109_021E68B8(app, 0);
            ov109_021E691C(app, 0x1000);
            app->page = (u8)(app->page - 1);
            ov109_021E7474(app, app->cursorX, app->cursorY, 1);
            ov109_021E7524(app);
            if (app->mode == 1) {
                ov109_021E74D4(app, 1);
            }
            app->subState = 0U;
            return 1;
        }
        break;
    }
    return 0;
}

s32 ov109_021E66C4(PhotoAlbumApp *app) {
    u16 v0;

    v0 = app->subState;
    switch (v0) {
    case 0:
        ov109_021E7474(app, app->cursorX, app->cursorY, 0);
        ov109_021E74D4(app, 0);
        ov109_021E68B8(app, 1);
        ov109_021E691C(app, 0x1000);
        ov109_021E6F7C(app, 2, 1);
        ov109_021E70C4(app, 2, 2, app->page);
        ov109_021E70C4(app, 3, 2, (u8)(app->page + 1));
        app->scrollStep = 0;
        app->subState = (u16)(app->subState + 1);
    default:
        break;
    case 1:
        app->scrollStep = app->scrollStep + 1;
        ov109_021E691C(app, ov109_021E78DC[app->scrollStep]);
        if (app->scrollStep >= 4) {
            ov109_021E6F7C(app, 1, 1);
            ov109_021E70C4(app, 2, 1, (u8)(app->page + 1));
            app->subState = (u16)(app->subState + 1);
        }
        break;
    case 2:
        app->scrollStep = app->scrollStep - 1;
        ov109_021E691C(app, ov109_021E78DC[app->scrollStep]);
        if (app->scrollStep == 0) {
            ov109_021E70C4(app, 3, 1, (u8)(app->page + 1));
            ov109_021E68B8(app, 0);
            ov109_021E691C(app, 0x1000);
            app->page = (u8)(app->page + 1);
            ov109_021E7474(app, app->cursorX, app->cursorY, 1);
            ov109_021E7524(app);
            if (app->mode == 1) {
                ov109_021E74D4(app, 1);
            }
            app->subState = 0U;
            return 1;
        }
        break;
    }
    return 0;
}

s32 ov109_021E681C(PhotoAlbumApp *app) {
    s32 v0;

    v0 = ov109_021E7640(app);
    if (v0 < 0) {
        v0 = 4;
    }
    return v0;
}
