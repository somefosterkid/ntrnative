#include "global.h"

#include "bg_window.h"
#include "unk_0200B150.h"
#include "vram_transfer_manager.h"

void ov51_021E69EC(u32 *);

void ov51_021E6B88(u32 *param_1);

void ov51_021E6B88(u32 *param_1) {
    s32 val;
    val = -((u32)(*((u8 *)(((u8 *)param_1) + 0x343f))));
    BgSetPosTextAndCommit((BgConfig *)(*param_1), 5, BG_POS_OP_SET_ROT, val);
    BgSetPosTextAndCommit((BgConfig *)(*param_1), 5, BG_POS_OP_SET_XSCALE, val);
    BgSetPosTextAndCommit((BgConfig *)(*param_1), 3, BG_POS_OP_SET_ROT, val);
    BgSetPosTextAndCommit((BgConfig *)(*param_1), 3, BG_POS_OP_SET_XSCALE, val);
    if ((((u32)(*((u8 *)(((u8 *)param_1) + 0x343a)))) << 0x1d) >> 0x1f) {
        ov51_021E69EC(param_1);
    }
    DoScheduledBgGpuUpdates((BgConfig *)(*param_1));
    GF_RunVramTransferTasks();
    OamManager_ApplyAndResetBuffers();
    OS_SetIrqCheckFlag(1UL << 0);
    return;
}
