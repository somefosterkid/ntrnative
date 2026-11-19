#include "global.h"
#include "sprite_resources_header.h"
#include "field_system_def.h"

#include "constants/heap.h"

#include "field/dynamic_terrain_height.h"
#include "field/map_prop_animation.h"
#include "field/model_attributes.h"

#include "bag_cursor.h"
#include "bg_window.h"
#include "camera.h"
#include "map_matrix.h"
#include "options.h"
#include "overlay_manager.h"
#include "overworld_anim_manager.h"
#include "pm_string.h"
#include "save.h"
#include "sys_task.h"
#include "unk_0200A090.h"
#include "overlay_02_hmcut_in.h"
#include "sprite.h"

struct SpriteList;



void ov01_021FCD78(SysTask *param0);

int ov02_0224AB8C(HMCutIn *cutIn);

void ov02_0224AB9C(HMCutIn *cutIn);

int ov02_0224AB8C(HMCutIn *cutIn) {
    FlyTaskEnv *taskEnv = sub_02068D74(cutIn->flyBirdAnim);
    return taskEnv->subState;
}

void ov02_0224AB9C(HMCutIn *cutIn) {
    FlyTaskEnv *env = sub_02068D74(cutIn->flyBirdAnim);

    if (env->unk_60) {
        sub_02068B48(env->unk_60);
    }

    if (env->task) {
        ov01_021FCD78(env->task);
    }

    sub_02068B48(cutIn->flyBirdAnim);
}
