#include "global.h"
#include "overlay_00_nintendo_wfcmanager.h"

extern void *_0221A680;

void ov00_021E6FE0(int arg0, void *arg1);

void ov00_021E6FE0(int arg0, void *arg1) {
    s32 (*v0)(u16, s32);

    *((s32 *)&((NintendoWFCManager *)_0221A680)->latestNewClientFriendIdx) = arg0;
    v0 = *(s32(**)(u16, s32))((u8 *)_0221A680 + 0xFB4);
    if (v0 != NULL) {
        v0((u16)arg0, *((s32 *)&((NintendoWFCManager *)_0221A680)->newClientCallbackArg));
    }
}
