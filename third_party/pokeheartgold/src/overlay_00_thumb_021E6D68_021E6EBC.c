#include "global.h"
#include "overlay_00_nintendo_wfcmanager.h"

extern void *_0221A680;

s32 ov00_021E6EBC(void);

s32 ov00_021E6EBC(void) {
    if (_0221A680 != NULL) {
        return *((s32 *)&((NintendoWFCManager *)_0221A680)->hostFriendIdx);
    }
    return -1;
}
