#include "global.h"

void ov70_02237F2C(void);

typedef struct GTSNetworkingStateHead {
    s32 status;
    s32 result;
} GTSNetworkingStateHead;

extern GTSNetworkingStateHead _02246800;

void ov70_02237F2C(void) {
    _02246800.status = 0;
}

BOOL ov70_02237F38(void);

BOOL ov70_02237F38(void) {
    switch (_02246800.status) {
    case 1:
        return TRUE;
    case 0x18:
        _02246800.status = 1;
        return TRUE;
    default:
        break;
    }

    return FALSE;
}

s32 ov70_02237F58(void);

s32 ov70_02237F58(void) {
    return _02246800.result;
}
