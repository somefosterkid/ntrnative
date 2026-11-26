#include "global.h"

#include "constants/heap.h"

#include "overlay_00_thumb.h"
#include "save.h"
#include "overlay_00_nintendo_wfcmanager.h"

extern NintendoWFCManager *_0221A680;

void ov00_021E700C(void);

void ov00_021E700C(void) {
    if (!DWC_CheckHasProfile(_0221A680->userData)) {
        DWCFriendData friendData;
        DWC_CreateExchangeToken(_0221A680->userData, &friendData);
    } else {
        DWCFriendData friendData;
        DWC_CreateExchangeToken(_0221A680->userData, &friendData);
    }

    for (int i = 0; i < 32; i++) {
        DWC_GetFriendDataType(&_0221A680->friends[i]);
        DWC_IsBuddyFriendData(&_0221A680->friends[i]);
    }
}
