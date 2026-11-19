#include "global.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
#include "bg_window.h"
#include "overlay_109_photo_album_app.h"

void ov109_021E70C4(PhotoAlbumApp *app, u8 param_2, u32 param_3, u8 param_4) {
    unsigned long long new_var;
    u8 bgId;

    bgId = (u8)param_2;
    if ((param_3 & 1) != 0) {
        ov109_021E7030(app, param_2, param_4, 0);
        ov109_021E6FD4(app, bgId, 1, (s8)param_4);
    }
    if (0 != (param_3 & 2)) {
        ov109_021E7030(app, param_2, param_4, 2);
        ov109_021E6FD4(app, bgId, 2, (s8)param_4);
    }
    new_var = bgId;
    ScheduleBgTilemapBufferTransfer(app->bgConfig, new_var);
    return;
}
#else
#include "bg_window.h"

void ov109_021E6FD4(void *param_1, u8 param_2, s32 param_3, s8 param_4);

void ov109_021E7030(void *param_1, u32 param_2, s32 param_3, u32 param_4);

void ov109_021E70C4(u8 *param_1, u8 param_2, u32 param_3, s8 param_4);

void ov109_021E70C4(u8 *param_1, u8 param_2, u32 param_3, s8 param_4) {
    unsigned long long new_var;
    u8 bgId;
    bgId = (u8)param_2;
    if ((param_3 & 1) != 0) {
        ov109_021E7030(param_1, param_2, param_4, 0);
        ov109_021E6FD4(param_1, bgId, 1, (s8)param_4);
    }
    if (0 != (param_3 & 2)) {
        ov109_021E7030(param_1, param_2, param_4, 2);
        ov109_021E6FD4(param_1, bgId, 2, (s8)param_4);
    }
    new_var = bgId;
    ScheduleBgTilemapBufferTransfer(*((BgConfig **)(param_1 + 0x14)), new_var);
    return;
}
#endif
