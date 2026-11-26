#include "global.h"

#include "constants/heap.h"

#include "overlay_00_thumb.h"
#include "save.h"
#include "overlay_00_nintendo_wfcmanager.h"

extern NintendoWFCManager *_0221A680;

void ov00_021E6CE8(void);

void ov00_021E6CE8(void) {
    if (_0221A680->friendStatusesBuffer == 0L) {
        return;
    }

    int currentFriendIdx = _0221A680->nextFriendIdx % 32;
    int dummy;

    if (DWC_IsBuddyFriendData(&(_0221A680->friends[currentFriendIdx]))) {
        _0221A680->friendConnStatus[currentFriendIdx] = DWC_GetFriendStatusData(&_0221A680->friends[currentFriendIdx], (void *)(((u8 *)_0221A680->friendStatusesBuffer) + _0221A680->friendStatusSize * currentFriendIdx), &dummy);
    }

    _0221A680->nextFriendIdx = (_0221A680->nextFriendIdx + 1) % 32;
}
