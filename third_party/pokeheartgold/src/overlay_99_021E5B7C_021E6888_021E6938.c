#include "global.h"

#include "bg_window.h"
#include "sprite_system.h"
#include "overlay_99_obj99.h"

void ov99_021E6938(void *param0);

void ov99_021E6938(void *param0) {
    Obj99 *v0 = param0;

    SpriteSystem_TransferOam();

    DoScheduledBgGpuUpdates(v0->bgConfig);

    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}
