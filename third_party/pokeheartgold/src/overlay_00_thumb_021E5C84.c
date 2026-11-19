#include "global.h"
#include "overlay_00_nintendo_wfcmanager.h"

extern void *_0221A680;

void ov00_021E5C84(void (*serverCallback)(u16 aid, u16 *data, u16 length), void (*clientCallback)(u16 aid, u16 *data, u16 length));

void ov00_021E5C84(void (*serverCallback)(u16 aid, u16 *data, u16 length), void (*clientCallback)(u16 aid, u16 *data, u16 length)) {
    ((NintendoWFCManager *)_0221A680)->dataTransferServerCallback = serverCallback;
    ((NintendoWFCManager *)_0221A680)->dataTransferClientCallback = clientCallback;
}

void ov00_021E5CA0(void *arg0, void *arg1);

void ov00_021E5CA0(void *arg0, void *arg1) {
    ((NintendoWFCManager *)_0221A680)->connectionClosedCallback = arg0;
    ((NintendoWFCManager *)_0221A680)->connectionClosedCBArg = arg1;
}

void ov00_021E5CBC(s32 (*arg0)(s32));

void ov00_021E5CBC(s32 (*arg0)(s32)) {
    *(s32(**)(s32))((u8 *)_0221A680 + 0xFBC) = arg0;
}

void ov00_021E5CD0(void *arg0, void *arg1);

void ov00_021E5CD0(void *arg0, void *arg1) {
    *(void **)((u8 *)_0221A680 + 0xFB4) = arg0;
    *(void **)((u8 *)_0221A680 + 0xFB8) = arg1;
}
