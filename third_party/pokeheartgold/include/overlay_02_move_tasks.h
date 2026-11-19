#ifndef POKEHEARTGOLD_OVERLAY_02_MOVE_TASKS_H
#define POKEHEARTGOLD_OVERLAY_02_MOVE_TASKS_H

#include "global.h"

#include "unk_02062108.h"

// Environment of ov02_022507E8 (created by ov02_022507B4): runs the follow mon movement script (one per facing) repeat times. DS 0x8.
typedef struct FollowMonMoveTask {
    u16 repeat;                        // 0x00
    u16 count;                         // 0x02
    EventObjectMovementMan *movement;  // 0x04, the DS pointer fills the second half of the block
} FollowMonMoveTask;

// Environment of ov02_022508D8 (created by ov02_022508B4): scales the map object manager's vector up, holds, then back down. DS 0x4.
typedef struct ScaleBounceTask {
    u16 hold;   // 0x00
    u16 frame;  // 0x02
} ScaleBounceTask;

#endif // POKEHEARTGOLD_OVERLAY_02_MOVE_TASKS_H
