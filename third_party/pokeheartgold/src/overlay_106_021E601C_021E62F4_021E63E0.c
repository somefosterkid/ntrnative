#include "global.h"

#include "bg_window.h"
#include "overlay_106.h"
#include "overlay_106_arceus_cinematic.h"
#include "sprite_system.h"

void ov106_021E63E0(void *data);

void ov106_021E63E0(void *data) {
    ArceusCinematic *arceus = data;

    ov106_021E6A34(arceus);
    DoScheduledBgGpuUpdates(arceus->bgConfig);
    SpriteSystem_TransferOam();
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}
