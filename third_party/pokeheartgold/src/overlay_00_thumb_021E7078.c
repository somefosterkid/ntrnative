#include "global.h"

#include "sys_task.h"
#include "overlay_00_nintendo_wfcmanager.h"

BOOL ov00_021E7AA4(void);

BOOL ov00_021E7078(void);

extern NintendoWFCManager *_0221A680;
s32 ov00_021E7080(void);
s32 ov00_021E709C(void);
void ov00_021E70B8(u8 arg0);
void ov00_021E70CC(s16 arg0);
void ov00_021E70E0(void *arg0);

BOOL ov00_021E7078(void) {
    return ov00_021E7AA4();
}

s32 ov00_021E7080(void) {
    if (_0221A680 != NULL) {
        return *((s32 *)&_0221A680->voiceChatEnabled);
    }
    return 0;
}

s32 ov00_021E709C(void) {
    if (_0221A680 != NULL) {
        return *((s32 *)&_0221A680->latestNewClientFriendIdx);
    }
    return 0;
}

void ov00_021E70B8(u8 arg0) {
    *((s32 *)&_0221A680->voiceChatEnabled) = (s32)arg0;
}

void ov00_021E70CC(s16 arg0) {
    *((s16 *)&_0221A680->voiceChatEnabled_Battles) = arg0;
}

void ov00_021E70E0(void *arg0) {
    *((s8 *)&_0221A680->matchmakingCancelState) = 2;
}
