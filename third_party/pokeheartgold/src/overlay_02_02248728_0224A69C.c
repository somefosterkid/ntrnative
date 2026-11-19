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

void ov02_0224A69C(HMCutIn *cutIn, fx32 x1, fx32 y1, fx32 x2, fx32 y2);

void ov02_0224A69C(HMCutIn *cutIn, fx32 x1, fx32 y1, fx32 x2, fx32 y2) {
    cutIn->windowX1 = x1;
    cutIn->windowX2 = x2;
    cutIn->windowY1 = y1;
    cutIn->windowY2 = y2;
}
