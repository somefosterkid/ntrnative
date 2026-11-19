#include "global.h"

#include "menu_input_state.h"
#include "overlay_109_photo_album_app.h"
#include "sprite.h"

void ov109_021E7584(PhotoAlbumApp *app, s32 idx) {
    Sprite_ResetAnimCtrlState(app->sprites[idx]);
    Sprite_SetAnimActiveFlag(app->sprites[idx], 1);
    if (idx == 2) {
        Sprite_SetAnimCtrlSeq(app->sprites[idx], 1);
        return;
    }
    Sprite_SetAnimCtrlSeq(app->sprites[idx], 4);
    return;
}

s32 ov109_021E75B4(PhotoAlbumApp *app) {
    MenuInputState v0;

    if (app->menuInputState == MENU_INPUT_STATE_BUTTONS) {
        v0 = MENU_INPUT_STATE_TOUCH;
    } else {
        v0 = MENU_INPUT_STATE_BUTTONS;
    }
    app->menuInputState = v0;
    return 0;
}
