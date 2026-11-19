#include "global.h"

#include "heap.h"
#include "menu_input_state.h"
#include "overlay_109_photo_album_app.h"

void ov109_021E5B40(PhotoAlbumApp *app) {
    ov109_021E5D5C(app);
    Heap_Free(app->photos);
    MenuInputStateMgr_SetState(app->args->menuInputStatePtr, app->menuInputState);
}
