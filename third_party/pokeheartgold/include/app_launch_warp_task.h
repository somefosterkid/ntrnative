#ifndef POKEHEARTGOLD_APP_LAUNCH_WARP_TASK_H
#define POKEHEARTGOLD_APP_LAUNCH_WARP_TASK_H

#include "global.h"

#include "field/model_attributes.h"

// Environment of the tasks that fade out, launch an app (ov104 / ov105 via sub_02097810 / sub_020979EC) and warp back:
// sub_020978D0 (created by sub_020977CC) and sub_02097AA0 (created by sub_020979A8). DS 0x58.
typedef struct AppLaunchWarpTask {
    u8 state;                // 0x00
    u8 mode;                 // 0x01
    u8 direction;            // 0x02
    u8 padding_3;            // 0x03
    u32 mapId;               // 0x04
    s32 x;                   // 0x08
    s32 y;                   // 0x0C
    ModelAttributes lights;  // 0x10
} AppLaunchWarpTask;

#endif // POKEHEARTGOLD_APP_LAUNCH_WARP_TASK_H
