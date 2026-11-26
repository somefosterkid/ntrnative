#include "global.h"

#include "overlay_00_nintendo_wfcmanager.h"

#include "overlay_00_thumb.h"

extern NintendoWFCManager *_0221A680;

s32 ov00_021E6D68(s32 arg0) {
    return _0221A680->friendConnStatus[arg0];
}
