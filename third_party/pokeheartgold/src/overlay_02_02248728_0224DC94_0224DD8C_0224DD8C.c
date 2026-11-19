#include "global.h"

#include "overlay_02_field3d_effects.h"

void ov02_0224DD8C(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data) {
    Ov02App_0224DCB0 *env = data;

    if (env->suspended == 0) {
        env->spawnTimer--;
        if (env->spawnTimer < 0) {
            env->spawnTimer = 4;
            ov02_0224DF1C(env);
        }
        ov02_0224E008(env);
    }
}
