#include "global.h"

/* HDR-OVERLAY {"ToggleBgLayer": {"file": "include/bg_window.h", "new": "void ToggleBgLayer(u8 bgId, u32 toggle);", "old": "void ToggleBgLayer(u8 bgId, u8 toggle);"}} */
#define ToggleBgLayer ToggleBgLayer__hdr

/* HDR-OVERLAY defines done */
#include "bg_window.h"

/* HDR-OVERLAY end */
#undef ToggleBgLayer

#include "overlay_109_photo_album_app.h"

void ToggleBgLayer(u8 bgId, u8 toggle);

void ov109_021E6898(PhotoAlbumApp *app) {
    if (app->bgTogglePending != 0) {
        ToggleBgLayer(2U, app->bgToggleValue);
    }
    app->bgTogglePending = 0;
}
