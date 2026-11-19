#include "global.h"

#include "font.h"
#include "list_menu_items.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "constants/heap.h"
#include "bg_window.h"
#include "yes_no_prompt.h"
#include "sprite_system.h"
#include "unk_0200B150.h"
#include "vram_transfer_manager.h"
#include "overlay_109_photo_album_app.h"

extern WindowTemplate ov109_021E78F4[];

extern ResdatIdList ov109_021E78A8;
extern OamCharTransferParam ov109_021E78C8;
extern OamManagerParam ov109_021E79D0;

void ov109_021E6D2C(PhotoAlbumApp *app) {
    s32 v0;
    String **v1;
    s32 v3;

    ListMenuItems_Delete(app->listMenuItems);
    v0 = 0;
    app->listMenuItems = NULL;
    do {
        String_Delete(app->photoStrings[v0]);
        v0 += 1;
    } while (v0 < 2);
    v3 = 0;
#if defined(SDK_PORT) || defined(NONMATCHING)
    v1 = app->msgStrings;
    do {
        String_Delete(*v1);
        v3 += 1;
        v1 += 1;
    } while (v3 < 5);
#else
    v1 = (String **)app;
    do {
        String_Delete(*(String **)((u8 *)v1 + offsetof(PhotoAlbumApp, msgStrings)));
        v3 += 1;
        v1 += 1;
    } while (v3 < 5);
#endif
    String_Delete(app->nameString);
    String_Delete(app->tmpString);
    MessageFormat_Delete(app->msgFmt);
    DestroyMsgData(app->msgData);
    FontID_Release(FONT_UNK_4);
}

void ov109_021E6D84(PhotoAlbumApp *app) {
    YesNoPrompt *pYVar1;
    WindowTemplate *template;
    Window *window;
    s32 iVar2;

    template = ov109_021E78F4;
    iVar2 = 0;
    window = app->windows;
    do {
        AddWindow(app->bgConfig, window, template);
        FillWindowPixelBuffer(window, 0);
        iVar2 = iVar2 + 1;
        template = template + 1;
        window = window + 1;
    } while (iVar2 < 3);
    pYVar1 = YesNoPrompt_Create(app->heapID);
    app->yesNoPrompt = pYVar1;
    return;
}

void ov109_021E6DBC(PhotoAlbumApp *app) {
    s32 iVar1;
    Window *window;

    iVar1 = 0;
    window = app->windows;
    do {
        ClearWindowTilemapAndCopyToVram(window);
        RemoveWindow(window);
        iVar1 = iVar1 + 1;
        window = window + 1;
    } while (iVar1 < 3);
    YesNoPrompt_Destroy(app->yesNoPrompt);
    return;
}

void ov109_021E6DE4(PhotoAlbumApp *app) {
    GF_CreateVramTransferManager(0x20U, app->heapID);
    app->spriteSystem = SpriteSystem_Alloc(app->heapID);
    SpriteSystem_Init(app->spriteSystem, &ov109_021E79D0, &ov109_021E78C8, 3);
    thunk_ClearMainOAM(app->heapID);
    thunk_ClearSubOAM(app->heapID);
    app->spriteManager = SpriteManager_New(app->spriteSystem);
    SpriteSystem_InitSprites(app->spriteSystem, app->spriteManager, 5);
    sub_0200D2A4(app->spriteSystem, app->spriteManager, &ov109_021E78A8, 2, 1);
}

void ov109_021E6E64(PhotoAlbumApp *app) {
    SpriteSystem_DestroySpriteManager(app->spriteSystem, app->spriteManager);
    app->spriteManager = NULL;
    SpriteSystem_Free(app->spriteSystem);
    app->spriteSystem = NULL;
    GF_DestroyVramTransferManager();
    thunk_ClearMainOAM(app->heapID);
}
