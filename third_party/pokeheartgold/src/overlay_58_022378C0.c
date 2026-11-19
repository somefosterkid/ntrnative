#include "global.h"

#include "overlay_manager.h"

BOOL ov58_022378C0(OverlayManager **appManPtr);

BOOL ov58_022378C0(OverlayManager **appManPtr) {
    if (*appManPtr) {
        if (OverlayManager_Run(*appManPtr)) {
            OverlayManager_Delete(*appManPtr);
            *appManPtr = NULL;
            return 1;
        }
    }

    return 0;
}
