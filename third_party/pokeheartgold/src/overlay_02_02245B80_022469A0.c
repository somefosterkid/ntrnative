#include "global.h"

#include "overlay_02_02245B80.h"
#include "system.h"

BOOL ov02_022469A0(void) {
    if (gSystem.newKeys & 1) {
        return 1;
    }

    return 0;
}
