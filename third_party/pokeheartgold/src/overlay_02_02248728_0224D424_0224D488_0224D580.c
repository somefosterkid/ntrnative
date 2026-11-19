#include "global.h"

#include "overlay_02_0224D0C8.h"

void ov02_0224D580(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);

void ov02_0224D580(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data) {
    Field3dAnimatedModel *models = data;
    s32 i;

    for (i = 0; i < 2; i++) {
        ov02_0224D1DC(&models[i].object);
    }
}
