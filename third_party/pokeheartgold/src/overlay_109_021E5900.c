#include "global.h"

#include "heap.h"
#include "overlay_109_photo_album_app.h"
#include "overlay_manager.h"

BOOL PhotoAlbum_Init(OverlayManager *man, int *state) {
    PhotoAlbumApp *app;

    switch (*state) {
    case 0:
        ov109_021E5A20();
        Heap_Create(HEAP_ID_3, HEAP_ID_96, 0x20000U);
        app = OverlayManager_CreateAndGetData(man, sizeof(PhotoAlbumApp), HEAP_ID_96);
        MI_CpuFill8(app, 0U, sizeof(PhotoAlbumApp));
        app->heapID = HEAP_ID_96;
        app->args = OverlayManager_GetArgs(man);
        ov109_021E5A70(app);
        *state += 1;
    default:
        break;
    case 1:
        if (ov109_021E5B60(OverlayManager_GetData(man)) != 0) {
            return 1;
        }
        break;
    }
    return 0;
}
