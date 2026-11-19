#include "global.h"
#include "overlay_59_apricorn_app.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "pm_string.h"
#include "sprite.h"
#include "sprite_system.h"
#include "list_menu.h"


void GF_RunVramTransferTasks(void);

void ov59_022393D4(void *data);

void ov59_022393D4(void *data) {
    Ov59ApricornApp *app = data;

    if (app->spriteSystem != NULL) {
        SpriteSystem_TransferOam();
    }

    NNS_GfdDoVramTransfer();
    DoScheduledBgGpuUpdates(app->bgConfig);

    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}
