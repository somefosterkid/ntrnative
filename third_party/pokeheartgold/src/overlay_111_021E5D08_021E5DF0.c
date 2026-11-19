#include "global.h"

#include "bg_window.h"
#include "error_handling.h"
#include "sprite_system.h"
#include "overlay_111.h"

void ov111_021E5DF0(void *param_1);

void ov111_021E5DF0(void *param_1_) {
    BugContestSwapMon *param_1 = param_1_;
#if !defined(SDK_PORT) && !defined(NONMATCHING)
    int new_var3;
#endif
    if (param_1 == 0) {
        GF_AssertFail();
    }
    if (param_1->spriteManager == NULL) {
        GF_AssertFail();
    }
    if (param_1->bgConfig == NULL) {
        GF_AssertFail();
    }
    SpriteSystem_DrawSprites(param_1->spriteManager);
    SpriteSystem_TransferOam();
    DoScheduledBgGpuUpdates(param_1->bgConfig);
#if defined(SDK_PORT) || defined(NONMATCHING)
    *(u32 *)HW_INTR_CHECK_BUF |= 1;
#else
    new_var3 = 0x3FF8;
    *((u32 *)((u8 *)0x027E0000 + new_var3)) = 1 | *((u32 *)((u8 *)0x027E0000 + new_var3));
#endif
    return;
}
