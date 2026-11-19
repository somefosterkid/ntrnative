#include "global.h"

#include "bg_window.h"
#include "error_handling.h"
#include "sprite_system.h"
#include "overlay_112_sprite_app.h"

void ov112_021F2EB0(Ov112SpriteApp *param_1);

void ov112_021F2EB0(Ov112SpriteApp *param_1) {
    if (param_1 == 0) {
        GF_AssertFail();
    }
    if ((param_1->spriteManager) == 0) {
        GF_AssertFail();
    }
    if ((param_1->bgConfig) == 0) {
        GF_AssertFail();
    }
    SpriteSystem_DrawSprites(param_1->spriteManager);
    SpriteSystem_TransferOam();
    DoScheduledBgGpuUpdates(param_1->bgConfig);
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
    return;
}
