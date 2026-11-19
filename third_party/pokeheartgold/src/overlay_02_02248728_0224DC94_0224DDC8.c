#include "global.h"

#include "overlay_02_field3d_effects.h"

void ov02_0224DDC8(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data) {
    Ov02App_0224DCB0 *env = data;
    int i;

    for (i = 0; i < 18; i++) {
        Field3dObject_Draw(&env->slots[i].object);
    }
}
