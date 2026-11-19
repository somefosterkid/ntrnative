#include "global.h"

#include "bg_window.h"
#include "field_system_def.h"
#include "gf_gfx_planes.h"
#include "overlay_02_02245B80.h"

void ov02_02246964(AlphUnownTextTask *env) {
    GfGfx_EngineATogglePlanes(2U, 0U);
    SetBgPriority(1U, (u16)env->bgPriority);
    ClearWindowTilemapAndScheduleTransfer(&env->window);
    RemoveWindow(&env->window);
    BgClearTilemapBufferAndCommit(env->fieldSystem->bgConfig, 1U);
    BG_ClearCharDataRange(1U, 0x1000U, 0U, env->heapID);
}
