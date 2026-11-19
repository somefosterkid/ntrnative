#include "global.h"

#include "bg_window.h"
#include "unk_0200B150.h"
#include "unk_020932E0.h"
#include "vram_transfer_manager.h"

void sub_02093594(UnkStruct_020932E0 *param_1) {
    if (param_1->unk_4644 != NULL) {
        param_1->unk_4644(param_1->unk_8D4);
        param_1->unk_4644 = NULL;
        sub_020956B8(param_1);
    }
    GF_RunVramTransferTasks();
    OamManager_ApplyAndResetBuffers();
    DoScheduledBgGpuUpdates(param_1->unk_0);
    ((u32 *)&OS_IRQTable)[0xFFE] = ((u32 *)&OS_IRQTable)[0xFFE] | 1;
    return;
}
