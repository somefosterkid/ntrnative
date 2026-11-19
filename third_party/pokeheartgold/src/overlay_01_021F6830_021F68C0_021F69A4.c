#include "global.h"

#include "overlay_01_021F6830.h"
#include "poke_overlay.h"

void ov01_021F69A4(SubAppTaskData *data);

void ov01_021F69A4(SubAppTaskData *data) {
    u32 temp_r0;

    temp_r0 = ov01_02206C60[data->appIdx].overlayId;
    if (temp_r0 != -1U) {
        UnloadOverlayByID(temp_r0);
    }
}
