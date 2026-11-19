#include "global.h"

#include "bg_window.h"
#include "sprite_system.h"
#include "vram_transfer_manager.h"
#include "overlay_108_021E8850.h"
#include "overlay_108_safari_decoration_app.h"

void ov108_021E979C(void *data) {
    SafariDecorationApp *app = data;
    s32 iVar1;
    u32 uVar2;
    if (app->viewDirty != 0) {
        uVar2 = (u32)((u8)(app->cursor + (app->page * 6)));
        if ((app->cursor >= 6) || (uVar2 >= app->numObjects)) {
            ov108_021E8EA4(&app->view, 0x18, NULL);
        } else {
            iVar1 = uVar2 * 5;
            ov108_021E8EA4(&app->view, app->objects[uVar2].id, &app->objects[uVar2].config);
        }
        app->viewDirty = 0;
    }
    if (app->spriteSystem != NULL) {
        SpriteSystem_TransferOam();
    }
    GF_RunVramTransferTasks();
    DoScheduledBgGpuUpdates(app->bgConfig);
#if defined(SDK_PORT) || defined(NONMATCHING)
    *(u32 *)HW_INTR_CHECK_BUF |= 1;
#else
    {
        u8 *base = (u8 *)0x027E0000;
        *((u32 *)(base + 0x3FF8)) = (*((u32 *)(base + 0x3FF8))) | 1;
    }
#endif
    return;
}
