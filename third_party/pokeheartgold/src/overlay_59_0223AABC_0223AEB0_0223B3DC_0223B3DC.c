#include "global.h"
#include "bg_window.h"
#include "overlay_59_juice_feed_app.h"
#include "sprite_system.h"
#include "vram_transfer_manager.h"

void ov59_0223C374(void *);
void ov59_0223B3DC(void *param_1);
void ov59_0223B3DC(void *param_1) {
    Ov59JuiceFeedApp *work = param_1;
    if (work->spriteSystem != NULL) {
        SpriteSystem_TransferOam();
    }
    ov59_0223C374(work->monSprite);
    GF_RunVramTransferTasks();
    DoScheduledBgGpuUpdates(work->bgConfig);
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
    return;
}
