#include "global.h"

#include "field_system.h"
#include "field_types_def.h"
#include "overlay_01.h"
#include "overlay_27.h"
#include "sprite.h"
#include "task.h"
#include "unk_02009D48.h"

extern s32 (*const ov27_0225D4D4[])(Ov27TaskData *data);

void ov27_0225C434(SysTask *task, void *taskData) {
    Ov27TaskData *data = taskData;
    FieldSystem *fieldSystem;

    if (FieldSystem_TaskIsRunning(data->fieldSystem) == FALSE) {
        fieldSystem = data->fieldSystem;
        if (fieldSystem->unkD2_0 == 2) {
            fieldSystem->unkD2_0 = 3;
        }
        reg_G2S_DB_BLDCNT = 0;
    } else {
        if (ov27_0225D4D4[data->state](data) == 1) {
            data->fieldSystem->unkD2_0 = 0;
            ov01_021F6A9C(data->fieldSystem, 0, NULL);
        }
    }
    SpriteList_RenderAndAnimateSprites(data->spriteList);
}
