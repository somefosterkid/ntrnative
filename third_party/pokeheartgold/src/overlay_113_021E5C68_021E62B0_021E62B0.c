#include "global.h"

#include "bg_window.h"
#include "overlay_113.h"
#include "sprite_system.h"
#include "vram_transfer_manager.h"

void ov113_021E62B0(void *param);

void ov113_021E62B0(void *param) {
    UnownReportData *param_1 = param;

    if (param_1->spriteSystem != NULL) {
        SpriteSystem_TransferOam();
    }
    GF_RunVramTransferTasks();
    DoScheduledBgGpuUpdates(param_1->bgConfig);
#if defined(SDK_PORT) || defined(NONMATCHING)
    *(u32 *)HW_INTR_CHECK_BUF |= 1;
#else
    {
        u8 *base = (u8 *)0x027E0000;
        u32 off = 0x3FF8;
        *((u32 *)(base + off)) |= 1;
    }
#endif
    return;
}
