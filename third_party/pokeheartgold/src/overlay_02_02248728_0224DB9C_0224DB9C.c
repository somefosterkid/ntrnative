#include "global.h"

#include "field/field_3d_object_task.h"
#include "field/overlay_01_021FB878.h"

#include "camera.h"
#include "field_system.h"
#include "overlay_02_0224D0C8.h"

void ov02_0224DB9C(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);

void ov02_0224DB9C(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data) {
    Field3dCameraEffectData *effect = data;

    switch (effect->state) {
    case 0:
        if (effect->shiftCount < 2) {
            if (effect->delay <= 0) {
                effect->delay = 1;
                effect->shifted = (effect->shifted + 1) % 2;
                if (effect->shifted != 0) {
                    Camera_OffsetLookAtPosAndTarget(&effect->cameraOffset, fieldSystem->camera);
                } else {
                    Camera_SetLookAtCamTarget(&effect->cameraTarget, fieldSystem->camera);
                    Camera_SetLookAtCamPos(&effect->cameraPos, fieldSystem->camera);
                    effect->shiftCount++;
                }
            } else {
                effect->delay--;
            }
        }
        if (ov02_0224D178(&effect->model) == 1) {
            Field3dObject_SetActiveFlag(&effect->model.object, 0);
            effect->state++;
            return;
        }
        return;
    case 1:
        Camera_SetLookAtCamTarget(&effect->cameraTarget, fieldSystem->camera);
        Camera_SetLookAtCamPos(&effect->cameraPos, fieldSystem->camera);
        break;
    }
}
