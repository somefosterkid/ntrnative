#include "global.h"

#include "constants/heap.h"

#include "menu_input_state.h"
#include "options.h"
#include "overlay_109_photo_album_app.h"
#include "photo_album.h"
#include "player_data.h"
#include "save.h"

extern s32 ov109_021E7890;

void ov109_021E5A70(PhotoAlbumApp *app) {
    Options *temp_r0;
    u8 temp_r4;
    u8 temp_r6;

    app->menuInputState = MenuInputStateMgr_GetState(app->args->menuInputStatePtr);
    temp_r0 = Save_PlayerData_GetOptionsAddr(app->args->saveData);
    app->textFrameDelay = Options_GetTextFrameDelay(temp_r0);
    app->frame = (s8)Options_GetFrame(temp_r0);
    temp_r6 = app->args->cursorPos;
    temp_r4 = *(u8 *)(((u8 *)&ov109_021E7890) + ((s32)temp_r6 % 12));
    app->page = (s8)((s32)temp_r6 / 12);
    app->cursorX = (s8)(temp_r4 % 4);
    app->cursorY = (s8)((u32)temp_r4 >> 2);
    app->photoAlbum = Save_PhotoAlbum_Get(app->args->saveData);
    app->photos = PhotoAlbum_LoadAllInUsePhotos(app->photoAlbum, app->heapID);
    app->numPhotos = PhotoAlbum_GetNumSaved(app->photoAlbum);
    app->numEntries = (u8)app->numPhotos;
    app->numPages = (u8)((s32)app->numEntries / 12);
    if (((s32)app->numEntries % 12) != 0) {
        app->numPages = (u8)(app->numPages + 1);
    }
    if (app->numEntries == 0) {
        app->numPages = 1U;
    }
    ov109_021E5D08(app);
}
