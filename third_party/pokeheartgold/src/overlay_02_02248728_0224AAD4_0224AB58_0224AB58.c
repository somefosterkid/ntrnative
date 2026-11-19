#include "global.h"

#include "constants/heap.h"

#include "field/dynamic_terrain_height.h"
#include "field/map_prop_animation.h"
#include "field/model_attributes.h"

#include "bag_cursor.h"
#include "bg_window.h"
#include "camera.h"
#include "field_types_def.h"
#include "map_matrix.h"
#include "options.h"
#include "overlay_manager.h"
#include "overworld_anim_manager.h"
#include "pm_string.h"
#include "save.h"
#include "sys_task.h"
#include "unk_0200A090.h"
#include "overlay_02_hmcut_in.h"

extern const OverworldAnimManagerFuncs ov02_0225347C;

void ov02_0224AB58(HMCutIn *cutIn);

void ov02_0224AB58(HMCutIn *cutIn) {
    VecFx32 position = { 0, 0, 0 };
    HMCutIn *CutIn = cutIn;

    cutIn->flyBirdAnim = sub_02068B0C(cutIn->animManagerList, &ov02_0225347C, &position, 0, &CutIn, 130);
}
