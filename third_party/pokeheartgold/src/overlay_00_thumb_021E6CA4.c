#include "global.h"

#include "overlay_00_thumb.h"
#include "overlay_44_02232E9C.h"
#include "overlay_00_nintendo_wfcmanager.h"

extern void *_0221A680;

void ov00_021E6CA4(void (*func)(u32)) {
    if (_0221A680 != NULL) {
        *(void (**)(u32))((u8 *)_0221A680 + 0xFC0) = func;
    }
}

s32 ov00_021E5AE8(void);

void ov00_021E6CB8(void) {
    DWC_ShutdownFriendsMatch();
    DWC_CleanupInet();
    ov00_021E6A4C();
    ov00_021E5AE8();
}

void ov00_021E6CCC(UnkStruct_ov44_02231958 *arg0, s32 arg1) {
    *((UnkStruct_ov44_02231958 **)&((NintendoWFCManager *)_0221A680)->friendStatusesBuffer) = arg0;
    *((s32 *)&((NintendoWFCManager *)_0221A680)->friendStatusSize) = arg1;
}
