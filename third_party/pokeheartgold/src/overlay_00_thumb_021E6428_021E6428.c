#include "global.h"

#include "overlay_00_nintendo_wfcmanager.h"

extern NintendoWFCManager *_0221A680;

void ov00_021E6428(int param_1, u8 param_2);

void ov00_021E6428(int param_1, u8 param_2) {
    _0221A680->dataSent = 0;
    _0221A680->clientUpdateTimers[param_2] = 0;
    return;
}
