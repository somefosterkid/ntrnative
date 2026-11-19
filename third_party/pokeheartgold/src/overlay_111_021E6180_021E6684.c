#include "global.h"

#include "overlay_111.h"

s32 ov111_021E6684(SwapMonPanel *panel) {
    if (panel->sliding == 0) {
        return 1;
    }
    return 0;
}
