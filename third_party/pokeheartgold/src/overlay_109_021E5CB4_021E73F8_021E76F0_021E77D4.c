#include "global.h"

#include "heap.h"
#include "overlay_109_photo_album_app.h"
#include "sys_task_api.h"

void ov109_021E77D4(PhotoAlbumApp *app) {
    PhotoAlbumTaskWork *v0;

    v0 = Heap_AllocAtEnd(app->heapID, sizeof(PhotoAlbumTaskWork));
    MI_CpuFill8(v0, 0U, sizeof(PhotoAlbumTaskWork));
    v0->app = app;
    SysTask_CreateOnMainQueue(ov109_021E7810, v0, 0U);
    ov109_021E7388(app, 1);
    app->busy = 1;
}
