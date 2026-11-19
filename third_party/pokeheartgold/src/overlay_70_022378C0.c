#include "global.h"

void ov70_022378C0(s32 dwcProfileId, u64 friendKey);

typedef struct GTSNetworkingStateHead {
    s32 status;
    s32 result;
    s32 dwcProfileId;
    u64 friendKey;
} GTSNetworkingStateHead;

extern GTSNetworkingStateHead _02246800;

void ov70_022378C0(s32 dwcProfileId, u64 friendKey) {
    _02246800.status = 1;
    _02246800.result = 0;
    _02246800.dwcProfileId = dwcProfileId;
    _02246800.friendKey = friendKey;
}
