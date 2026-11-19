#include "global.h"

#include "constants/heap.h"

#include "field/field_control.h"

#include "field_types_def.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "heap.h"
#include "map_object.h"
#include "overlay_28.h"
#include "sys_task.h"
#include "systask_environment.h"
#include "unk_0203DB6C.h"

s32 ov01_021F6BD0(s32 arg0);

s32 ov01_021F6BB0(u32 param_1);

SysTask *ov28_0225D520(BgConfig *bgConfig, u16 *lastTouchMenuInput, FieldSystem *fieldSystem, void *args) {
    LocalMapObject *sp8;
    void *sp4;
    FieldSystem *sp0;
    NARC *temp_r7;
    SysTask *temp_r0;
    DowsingMchnApp *app;

    sp0 = fieldSystem;
    sp4 = args;
    Heap_Create(HEAP_ID_3, HEAP_ID_8, 0x18000U);
    *(s16 *)REG_DB_BLDCNT_ADDR = 0;
    temp_r0 = CreateSysTaskAndEnvironment(ov28_0225DC2C, sizeof(DowsingMchnApp), 0xAU, HEAP_ID_8);
    app = SysTask_GetData(temp_r0);
    app->bgConfig = bgConfig;
    app->lastTouchMenuInput = lastTouchMenuInput;
    app->fieldSystem = sp0;
    app->args = sp4;
    app->task = temp_r0;
    app->touching = 0;
    app->windowSel = ov01_021E7F54(app->fieldSystem);
    if (app->windowSel == 1) {
        FieldSystem_GetFacingObject(app->fieldSystem, &sp8);
        if (ov01_021F6BD0((s32)MapObject_GetScriptID(sp8)) != 1) {
            if (ov01_021F6BB0(MapObject_GetSpriteID(sp8)) == 1) {
                goto block_3;
            }
        } else {
block_3:
            app->windowSel = 0U;
        }
    }
    temp_r7 = NARC_New(NARC_a_2_5_1, HEAP_ID_8);
    ov28_0225D628();
    ov28_0225D650(bgConfig);
    ov28_0225D6FC(app, temp_r7);
    ov28_0225D764(app);
    ov28_0225D7E0(app);
    ov28_0225D898(app, temp_r7);
    NARC_Delete(temp_r7);
    return temp_r0;
}
