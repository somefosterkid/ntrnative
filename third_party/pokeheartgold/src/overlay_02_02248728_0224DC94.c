#include "global.h"

#include "field/field_3d_object_task.h"
#include "constants/heap.h"
#include "field/overlay_01_021FB878.h"
#include "filesystem_files_def.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "overlay_02_0224D0C8.h"
#include "overlay_02_field3d_effects.h"

s32 ov02_0224DC94(Field3dObjectTask *arg0);

s32 ov02_0224DC94(Field3dObjectTask *task) {
    Field3dCameraEffectData *data = Field3dObjectTask_GetData(task);
    if (data->state == 1) {
        return 1;
    }
    return 0;
}

void ov02_0224DCB0(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data) {
    Ov02App_0224DCB0 *env = data;
    int i;

    memset(env, 0, sizeof(Ov02App_0224DCB0));
    HeapExp_FndInitAllocator(&env->allocator, HEAP_ID_FIELD1, 0x20);
    Field3dModel_LoadFromFilesystem(&env->model, NARC_a_1_3_4, 8, HEAP_ID_FIELD1);
    for (i = 0; i < OV02_FIELD3D_EFFECT_ANIM_COUNT; i++) {
        env->animFiles[i] = GfGfxLoader_LoadFromNarc(NARC_a_1_3_4, i + 4, 0, HEAP_ID_FIELD1, 0);
    }
    for (i = 0; i < 18; i++) {
        ov02_0224DEA8(&env->slots[i], &env->model, &env->allocator, env->animFiles);
    }
    env->fieldSystem = fieldSystem;
    env->suspended = 0;
}
