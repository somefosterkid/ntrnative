#include "global.h"

#include "field_system.h"
#include "map_object.h"
#include "overlay_01_021FD41C.h"
#include "sys_task_api.h"

void ov01_021FD458(UnkStruct_ov5_021F1388 *ctx) {
    FieldSystem *fieldSystem = ov01_021F1468(ctx->unk_20);
    int v1 = MapObjectManager_GetPriority(fieldSystem->mapObjectManager) - 1;
    SysTask *v2 = SysTask_CreateOnMainQueue(ov01_021FD4F4, ctx, v1);

    ctx->unk_24 = v2;
}
