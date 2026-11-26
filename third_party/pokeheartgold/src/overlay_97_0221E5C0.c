#include "global.h"
#include "overlay_manager.h"

s32 ov97_0221E6DC(OverlayManager *arg0);

s32 ov97_0221E5C0(OverlayManager *arg0);

s32 ov97_0221E5C0(OverlayManager *arg0) {
    if (ov97_0221E6DC(arg0) != 0) {
        return 1;
    }
    return 0;
}
