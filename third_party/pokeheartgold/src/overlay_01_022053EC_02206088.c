#include "global.h"

#include "error_handling.h"
#include "follow_mon.h"
#include "map_object.h"
#include "constants/heap.h"
#include "field_types_def.h"
#include "heap.h"
#include "overlay_01.h"
#include "task.h"

s32 GetMoveModelNoBySpriteId(u32 spriteId);

s32 ov01_02206088(u32 spriteId);

s32 ov01_0220610C(TaskManager *arg0);

s32 ov01_02206088(u32 spriteId) {
    s32 v0;

    v0 = GetMoveModelNoBySpriteId(spriteId);
    if (v0 < 0) {
        GF_AssertFail();
        v0 = 0;
    }
    return v0;
}

void ov01_0220609C(FieldSystem *arg0, u32 arg1);

void ov01_0220609C(FieldSystem *fieldSystem, u32 direction) {
    if (FollowMon_IsActive(fieldSystem) != 0) {
        MapObject_SetFacingDirection(fieldSystem->followMon.mapObject, direction);
    }
}

BOOL ov01_022060B8(FieldSystem *fieldSystem, u8 a1, u8 a2) {
    BOOL BVar1;
    FollowMonFadeTask *dest;

    BVar1 = FollowMon_IsActive(fieldSystem);
    if (BVar1 == 0) {
        return 0;
    }
    BVar1 = ov01_022057C4(fieldSystem);
    if (BVar1 != 0) {
        return 0;
    }
    if (a1 == 0) {
        return 0;
    }
    dest = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(FollowMonFadeTask));
    MI_CpuFill8(dest, 0, sizeof(FollowMonFadeTask));
    dest->step = a1;
    dest->delay = a2;
    TaskManager_Call(fieldSystem->taskman, (TaskFunc)(ov01_0220610C), dest);
    return 1;
}
