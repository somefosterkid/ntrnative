#include "global.h"

#include "heap.h"
#include "overlay_109_photo_album_app.h"
#include "sprite.h"
#include "sys_task_api.h"

void ov109_021E7810(SysTask *task, void *param) {
    PhotoAlbumTaskWork *work = param;
    PhotoAlbumApp *app;

    if (Sprite_IsAnimated(work->app->sprites[4]) == 0) {
        ov109_021E7388(work->app, 0);
        app = work->app;
        app->busy = 0;
        MI_CpuFill8(work, 0U, sizeof(PhotoAlbumTaskWork));
        Heap_Free(work);
        SysTask_Destroy(task);
    }
}

u8 ov109_021E7850(Photo *photo) {
    u16 v0;
    u8 v1;
    u8 v2;

    v1 = 0;
    v2 = 0;
    do {
        v0 = photo->party[v2].species;
        if (((s32)v0 > 0) && ((s32)v0 <= 0x1ED)) {
            v1 += 1;
        }
        v2 += 1;
    } while ((u32)v2 < 6U);
    return v1;
}
