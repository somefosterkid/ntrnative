#include "global.h"

#include "bg_window.h"
#include "trainer_card_main.h"
#include "unk_0200B150.h"
#include "vram_transfer_manager.h"

void ov51_021E6B88(void *data);

void ov51_021E6B88(void *data) {
    TrainerCardMain *cardMain = data;
    s32 val;
    val = -cardMain->scrollOffset;
    BgSetPosTextAndCommit(cardMain->bgConfig, 5, BG_POS_OP_SET_ROT, val);
    BgSetPosTextAndCommit(cardMain->bgConfig, 5, BG_POS_OP_SET_XSCALE, val);
    BgSetPosTextAndCommit(cardMain->bgConfig, 3, BG_POS_OP_SET_ROT, val);
    BgSetPosTextAndCommit(cardMain->bgConfig, 3, BG_POS_OP_SET_XSCALE, val);
    if (cardMain->scaleChanged) {
        ov51_021E69EC(cardMain);
    }
    DoScheduledBgGpuUpdates(cardMain->bgConfig);
    GF_RunVramTransferTasks();
    OamManager_ApplyAndResetBuffers();
    OS_SetIrqCheckFlag(1UL << 0);
    return;
}
