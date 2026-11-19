#include "global.h"

#include "overlay_106.h"
#include "overlay_106_legendary_cinematic.h"

s32 ov106_021E5D08(LegendaryCinematic *cinematic) {
    u32 i;
    for (i = 0; i < cinematic->numModels; i++) {
        if (ov106_021E5CD0(cinematic, i) == 1) {
            return 1;
        }
    }
    return 0;
}
