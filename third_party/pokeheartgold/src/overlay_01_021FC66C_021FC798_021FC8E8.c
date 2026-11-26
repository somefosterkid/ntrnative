#include "global.h"

#include "overlay_01_fishing_task.h"

#include "overlay_01_021FC66C.h"

s32 ov01_021FCAE8(void);

s32 ov01_021FC8E8(FishingTask *arg0, void *arg1, void *arg2) {
    arg0->fishHookedCounter = (s32)(arg0->fishHookedCounter - 1);
    if (ov01_021FCAE8() == 1) {
        arg0->fishingTask = 6;
        return 1;
    }
    if (arg0->fishHookedCounter > 0) {
        return 0;
    }
    arg0->fishingTask = 0xB;
    return 0;
}
