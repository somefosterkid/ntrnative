#include "global.h"
#include "overlay_00_nintendo_wfcmanager.h"

extern NintendoWFCManager *_0221A680;

void ov00_021E6FE0(int arg0, void *arg1);

void ov00_021E6FE0(int arg0, void *arg1) {
    NintendoWFCNewClientCallback v0;

    _0221A680->latestNewClientFriendIdx = arg0;
    v0 = _0221A680->newClientCallback;
    if (v0 != NULL) {
        v0((u16)arg0, _0221A680->newClientCallbackArg);
    }
}
