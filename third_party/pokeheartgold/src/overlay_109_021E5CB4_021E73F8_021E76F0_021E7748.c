#include "global.h"

#include "yes_no_prompt.h"
#include "overlay_109_photo_album_app.h"

s32 ov109_021E7748(PhotoAlbumApp *app) {
    enum MenuInputState temp_r0_2;
    enum YesNoResponse temp_r0;
    temp_r0 = YesNoPrompt_HandleInput(app->yesNoPrompt);
    switch (temp_r0) {
    case YESNORESPONSE_YES:
        ov109_021E5DEC(app, app->heldIndex);
        break;

    default:
        return 7;

    case YESNORESPONSE_NO:
        break;
    }
    temp_r0_2 = YesNoPrompt_IsInTouchMode(app->yesNoPrompt);
    if (temp_r0_2 != (app->menuInputState)) {
        app->menuInputState = temp_r0_2;
    }
    YesNoPrompt_Reset(app->yesNoPrompt);
    ov109_021E74D4(app, 0);
    if (temp_r0 == YESNORESPONSE_YES) {
        ov109_021E70C4(app, 3, 3, app->page);
        ov109_021E7524(app);
    }
    ov109_021E7474(app, app->cursorX, app->cursorY, 1);
    ov109_021E7248(app, 0);
    ov109_021E73F8(app, 0, 0, 0);
    app->mode = 0;
    return 1;
}
