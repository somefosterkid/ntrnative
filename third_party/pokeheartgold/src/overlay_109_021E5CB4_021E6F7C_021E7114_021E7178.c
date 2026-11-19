#include "global.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
#include "overlay_109_photo_album_app.h"

void ov109_021E7178(PhotoAlbumApp *app, Photo *photo, u16 arg2, s32 arg3, u32 arg4) {
    u8 v1;

    v1 = 0;
    if (photo != NULL) {
        v1 = photo->iconId + 1;
    }
    ov109_021E7114(app, v1, arg2, (u8)(((u32)(arg3 << 0x17) >> 0x18) + ((6 * arg3) + 4)), (u8)((arg4 * 5) + 1));
}
#else
s32 ov109_021E7114(u8 *, u8, u8, u8);

void ov109_021E7178(void *arg0, void *arg1, u16 arg2, s32 arg3);

void ov109_021E7178(void *arg0, void *arg1, u16 arg2, s32 arg3) {
    s32 v0;
    u8 v1;

    v1 = 0;
    if (arg1 != NULL) {
        v1 = ((u32)(*(u8 *)((u8 *)arg1 + 4) << 0x18) >> 0x19) + 1;
    }
    v0 = (s32)(u8)((*(u8 *)((u8 *)(&v0) + 0x10) * 5) + 1);
    ov109_021E7114(arg0, v1, arg2, (u8)(((u32)(arg3 << 0x17) >> 0x18) + ((6 * arg3) + 4)));
}
#endif
