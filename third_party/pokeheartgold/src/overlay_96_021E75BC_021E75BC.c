#include "global.h"

#include "bg_window.h"
#include "unk_0200B150.h"
#include "vram_transfer_manager.h"

struct BgConfig;

void ov96_021E75BC(void *param0);

void ov96_021E75BC(void *param0) {
    OamManager_ApplyAndResetBuffers();
    DoScheduledBgGpuUpdates((BgConfig *)param0);
    GF_RunVramTransferTasks();

    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}
