#include "global.h"

#include "constants/heap.h"

#include "field/overlay_01_021FB878.h"

#include "field_types_def.h"
#include "follow_mon.h"
#include "heap.h"
#include "map_object.h"
#include "overlay_01_021FCE98.h"

void ov01_021FD064(enum HeapID param_1, FieldSystem *param_2, SweetScentEffect *param_3) {
    LocalMapObject *object;
    VecFx32 VStack_20;

    HeapExp_FndInitAllocator(&param_3->allocator, param_1, 0x20);
    Field3dModel_LoadFromFilesystem(&param_3->model, NARC_a_1_3_4, 0x17, param_1);
    Field3dModelAnimation_LoadFromFilesystem(&param_3->anims[0], &param_3->model, NARC_a_1_3_4, 0x15, param_1, &param_3->allocator);
    Field3dModelAnimation_LoadFromFilesystem(&param_3->anims[1], &param_3->model, NARC_a_1_3_4, 0x16, param_1, &param_3->allocator);
    Field3dModelAnimation_LoadFromFilesystem(&param_3->anims[2], &param_3->model, NARC_a_1_3_4, 0x14, param_1, &param_3->allocator);
    Field3dObject_InitFromModel(&param_3->object, &param_3->model);
    Field3dObject_AddAnimation(&param_3->object, &param_3->anims[0]);
    Field3dObject_AddAnimation(&param_3->object, &param_3->anims[1]);
    Field3dObject_AddAnimation(&param_3->object, &param_3->anims[2]);
    ov01_021FD190(param_3->anims, 3, 0);
    object = FollowMon_GetMapObject(param_2);
    MapObject_CopyPositionVector(object, &VStack_20);
    Field3dObject_SetPosEx(&param_3->object, VStack_20.x, VStack_20.y, VStack_20.z);
    Field3dObject_SetActiveFlag(&param_3->object, 1);
    return;
}

void ov01_021FD128(SweetScentEffect *param_1) {
    Field3dModelAnimation_Unload(&param_1->anims[2], &param_1->allocator);
    Field3dModelAnimation_Unload(&param_1->anims[1], &param_1->allocator);
    Field3dModelAnimation_Unload(&param_1->anims[0], &param_1->allocator);
    Field3dModel_Unload(&param_1->model);
    return;
}
