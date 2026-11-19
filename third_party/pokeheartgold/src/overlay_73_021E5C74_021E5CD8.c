#include "global.h"

#include "bg_window.h"
#include "unk_0200B150.h"
#include "vram_transfer_manager.h"

struct BgConfig;

void ov73_021E5CD8(void *param0);

void ov73_021E5CD8(void *param0) {
    GF_RunVramTransferTasks();
    OamManager_ApplyAndResetBuffers();
    DoScheduledBgGpuUpdates(*(BgConfig **)param0);

    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}
