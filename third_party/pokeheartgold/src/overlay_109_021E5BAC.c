#include "global.h"

#include "overlay_109_photo_album_app.h"
#include "screen_fade.h"
#include "sys_task.h"

s32 ov109_021E5BAC(PhotoAlbumApp *app) {
    s32 v0;

    v0 = app->state;
    switch (v0) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0U, 6, 1, app->heapID);
        app->state = app->state + 1;
    default:
        break;
    case 1:
        if (IsPaletteFadeFinished() != 0) {
            app->state = 0;
            return 1;
        }
        break;
    }
    return 0;
}

s32 ov109_021E5BF4(PhotoAlbumApp *app) {
    s32 v0;

    v0 = app->state;
    switch (v0) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0U, 6, 1, app->heapID);
        app->state = app->state + 1;
    default:
        break;
    case 1:
        if (IsPaletteFadeFinished() != 0) {
            app->state = 0;
            return 9;
        }
        break;
    }
    return 8;
}

s32 ov109_021E5C40(PhotoAlbumApp *app) {
    return ov109_021E5F10(app);
}
