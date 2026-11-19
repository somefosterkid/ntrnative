#include "global.h"

#include "error_handling.h"
#include "sys_task_api.h"
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
#include "pokemon.h"

void ov02_0224A700(SysTask *task, void *data);

void ov02_0224A6A8(HMCutIn *cutIn);

void ov02_0224A700(SysTask *task, void *hmCutInPtr);

void ov02_0224A7A8(HMCutIn *cutIn, PokepicTemplate *spriteTemplate);

void ov02_0224A6A8(HMCutIn *cutIn) {
    if (cutIn->windowTask != NULL) {
        GF_AssertFail();
    }
    cutIn->windowTask = SysTask_CreateOnVBlankQueue(ov02_0224A700, cutIn, 0x81U);
}

void ov02_0224A6D0(HMCutIn *cutIn);

void ov02_0224A6D0(HMCutIn *cutIn) {
    if (cutIn->windowTask == NULL) {
        GF_AssertFail();
    }
    SysTask_Destroy(cutIn->windowTask);
    reg_GX_DISPCNT &= 0xFFFF1FFF;
}

void ov02_0224A700(SysTask *task, void *hmCutInPtr) {
    HMCutIn *cutIn = hmCutInPtr;

    if (cutIn->updateWindow == 0) {
        return;
    }

    GX_SetVisibleWnd(cutIn->window);
    G2_SetWnd0InsidePlane(cutIn->planeMaskInside, cutIn->effectInside);
    G2_SetWndOutsidePlane(cutIn->planeMaskOutside, cutIn->effectOutside);
    G2_SetWnd0Position(cutIn->windowX1 / ((fx32)0x0000000000001000L), cutIn->windowY1 / ((fx32)0x0000000000001000L), cutIn->windowX2 / ((fx32)0x0000000000001000L), cutIn->windowY2 / ((fx32)0x0000000000001000L));
}

void ov02_0224A7A8(HMCutIn *cutIn, PokepicTemplate *spriteTemplate) {
    GetPokemonSpriteCharAndPlttNarcIds(spriteTemplate, cutIn->mon, 2);
}
