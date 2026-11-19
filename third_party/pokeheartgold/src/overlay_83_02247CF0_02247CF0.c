#include "global.h"

#include "system.h"
#include "touchscreen.h"

s32 ov83_02247CF0(void);

s32 ov83_02247CF0(void) {
    if (3 & gSystem.newKeys) {
        return 1;
    }
    return System_GetTouchNew();
}
