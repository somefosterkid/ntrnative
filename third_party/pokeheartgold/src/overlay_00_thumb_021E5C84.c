#include "global.h"
#include "overlay_00_nintendo_wfcmanager.h"

extern NintendoWFCManager *_0221A680;

void ov00_021E5C84(void (*serverCallback)(u16 aid, u16 *data, u16 length), void (*clientCallback)(u16 aid, u16 *data, u16 length));

void ov00_021E5C84(void (*serverCallback)(u16 aid, u16 *data, u16 length), void (*clientCallback)(u16 aid, u16 *data, u16 length)) {
    _0221A680->dataTransferServerCallback = serverCallback;
    _0221A680->dataTransferClientCallback = clientCallback;
}

void ov00_021E5CA0(void *arg0, void *arg1);

void ov00_021E5CA0(void *arg0, void *arg1) {
    _0221A680->connectionClosedCallback = arg0;
    _0221A680->connectionClosedCBArg = arg1;
}

void ov00_021E5CBC(BOOL (*arg0)(s32 hostFriendIdx));

void ov00_021E5CBC(BOOL (*arg0)(s32 hostFriendIdx)) {
    _0221A680->hostMatchCallback = arg0;
}

void ov00_021E5CD0(void *arg0, void *arg1);

void ov00_021E5CD0(void *arg0, void *arg1) {
    _0221A680->newClientCallback = arg0;
    _0221A680->newClientCallbackArg = arg1;
}
