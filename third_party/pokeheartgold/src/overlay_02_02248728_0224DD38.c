#include "global.h"

#include "overlay_02_field3d_effects.h"

void ov02_0224DD38(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data) {
    Ov02App_0224DCB0 *env = data;

    ov02_0224DCB0(task, fieldSystem, data);
    env->followMonEnabled = 1;
}

void ov02_0224DD4C(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data) {
    Ov02App_0224DCB0 *env = data;
    int i;

    for (i = 0; i < 18; i++) {
        ov02_0224DEF4(&env->slots[i], &env->allocator);
    }
    Field3dModel_Unload(&env->model);
    for (i = 0; i < OV02_FIELD3D_EFFECT_ANIM_COUNT; i++) {
        Heap_Free(env->animFiles[i]);
    }
}
