#include "global.h"

#include "bg_window.h"
#include "pokepic.h"
#include "sprite_system.h"
#include "vram_transfer_manager.h"
#include "overlay_68_move_relearner.h"

void ov68_021E5B6C(void *data);

void ov68_021E5B6C(void *data) {
    MoveRelearner *work = data;

    DoScheduledBgGpuUpdates(work->bgConfig);
    PokepicManager_HandleLoadImgAndOrPltt(work->monPic.pokepicManager);
    GF_RunVramTransferTasks();
    SpriteSystem_TransferOam();

    OS_SetIrqCheckFlag(1UL << 0);
}
