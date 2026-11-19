#include "global.h"

#include "bg_window.h"
#include "vram_transfer_manager.h"
#include "overlay_108_safari_area_customizer.h"

void ov108_021E8490(SafariAreaCustomizer *arg0);

void ov108_021E6BA0(void *cbArg);

void ov108_021E6BA0(void *cbArg) {
    SafariAreaCustomizer *param_1 = cbArg;
    ov108_021E8490(param_1);
    if (param_1->unk_184E2_0 != 0) {
        BgSetPosTextAndCommit(param_1->bgConfig, 1, BG_POS_OP_SET_XSCALE, (s32)(param_1->unk_184E4));
        BgSetPosTextAndCommit(param_1->bgConfig, 2, BG_POS_OP_SET_XSCALE, (s32)(param_1->unk_184E4));
        BgSetPosTextAndCommit(param_1->bgConfig, 4, BG_POS_OP_SET_XSCALE, (s32)(param_1->unk_184E6));
        BgSetPosTextAndCommit(param_1->bgConfig, 6, BG_POS_OP_SET_XSCALE, (s32)(param_1->unk_184E6));
        BgSetPosTextAndCommit(param_1->bgConfig, 5, BG_POS_OP_SET_XSCALE, (s32)(param_1->unk_184E6));
        param_1->unk_184E2_0 = 0;
    }
    GF_RunVramTransferTasks();
    DoScheduledBgGpuUpdates(param_1->bgConfig);
    OS_SetIrqCheckFlag(1UL << 0);
    return;
}
