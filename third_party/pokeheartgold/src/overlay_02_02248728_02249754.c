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
#include "pm_string.h"
#include "save.h"
#include "sys_task.h"
#include "unk_0200A090.h"
#include "overlay_02_hmcut_in.h"

BOOL ov02_02249754(HMCutIn *cutIn);

BOOL ov02_02249754(HMCutIn *cutIn) {
    cutIn->subState++;

    if (cutIn->subState >= 15) {
        cutIn->subState = 0;
        cutIn->pokemonSpriteDeltaX = (((fx32)0x0000000000001000L) * -64);
        cutIn->state++;
    }

    return 0;
}
