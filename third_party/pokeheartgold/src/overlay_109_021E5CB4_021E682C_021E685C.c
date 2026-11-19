#include "global.h"

#include "bg_window.h"
#include "overlay_109_photo_album_app.h"
#include "sprite_system.h"

void ov109_021E685C(void *param_1_) {
    PhotoAlbumApp *app = param_1_;
#if !defined(SDK_PORT) && !defined(NONMATCHING)
    u8 *base;
#endif

    if (app->spriteSystem != NULL) {
        SpriteSystem_TransferOam();
    }
    ov109_021E6898(app);
    ov109_021E68D4(app);
    NNS_GfdDoVramTransfer();
    DoScheduledBgGpuUpdates(app->bgConfig);
#if defined(SDK_PORT) || defined(NONMATCHING)
    *(u32 *)HW_INTR_CHECK_BUF |= 1;
#else
    base = (u8 *)0x027E0000;
    *((u32 *)(base + 0x3ff8)) = 1 | *((u32 *)(base + 0x3ff8));
#endif
    return;
}
