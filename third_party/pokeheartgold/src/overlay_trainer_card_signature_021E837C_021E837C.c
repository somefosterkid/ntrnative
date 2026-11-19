#include "global.h"

#include "bg_window.h"
#include "unk_0200B150.h"
#include "vram_transfer_manager.h"

void ov52_021E837C(void *param0);

void ov52_021E837C(void *param0) {
    GF_RunVramTransferTasks();
    OamManager_ApplyAndResetBuffers();
    DoScheduledBgGpuUpdates((BgConfig *)param0);

    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}
