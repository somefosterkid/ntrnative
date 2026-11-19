#include "global.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
#include "overlay_109_photo_album_app.h"

extern u8 _021E7884[];

void ov109_021E7030(PhotoAlbumApp *app, u32 param_2, u32 param_3, u8 param_4) {
    s32 i;
    s32 j;
    u32 idx;

    for (i = 0; i < 3; i++) {
        for (j = param_4; j < (s32)param_4 + 2; j++) {
            idx = (u8)(_021E7884[i * 4 + j] + param_3 * 12);
            if (idx < app->numEntries) {
                ov109_021E7178(app, app->entries[idx].photo, param_2, (u8)j, (u8)i);
            } else {
                ov109_021E7178(app, 0, param_2, (u8)j, (u8)i);
            }
        }
    }
}
#else
void ov109_021E7178(void *param_1, void *param_2, u32 param_3, u32 param_4, u8 param_5);

extern u8 _021E7884[];

void ov109_021E7030(u8 *param_1, u32 param_2, u32 param_3, u8 param_4);

void ov109_021E7030(u8 *param_1, u32 param_2, u32 param_3, u8 param_4) {
    s32 i;
    s32 j;
    u32 idx;
    for (i = 0; i < 3; i++) {
        for (j = param_4; j < (s32)param_4 + 2; j++) {
            idx = (u8)(_021E7884[i * 4 + j] + param_3 * 12);
            if (idx < param_1[0xc5]) {
                ov109_021E7178((u8 *)param_1, *(void **)(param_1 + idx * 8 + 0xd0), param_2, (u8)j, (u8)i);
            } else {
                ov109_021E7178((u8 *)param_1, 0, param_2, (u8)j, (u8)i);
            }
        }
    }
}
#endif
