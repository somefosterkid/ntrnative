#include "global.h"
struct HMCutIn;

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

void ov02_0224A674(struct HMCutIn *arg0);

void ov02_0224A674(HMCutIn *arg0) {
    arg0->window = 0;
}

void ov02_0224A67C(HMCutIn *cutIn);

void ov02_0224A67C(HMCutIn *cutIn) {
    cutIn->planeMaskInside = (GX_WND_PLANEMASK_BG3) | GX_WND_PLANEMASK_OBJ;
    cutIn->effectInside = 0;
    cutIn->planeMaskOutside = (GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3 | GX_WND_PLANEMASK_OBJ) & (~(GX_WND_PLANEMASK_BG3));
    cutIn->effectOutside = 1;
}

void ov02_0224A690(struct HMCutIn *arg0);

void ov02_0224A690(HMCutIn *arg0) {
    arg0->planeMaskOutside = 0x17;
    arg0->effectOutside = 1;
}
