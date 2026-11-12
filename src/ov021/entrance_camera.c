// Black's code at ov021 0x021bdc94-0x021bddc0. The name is the file name its asserts pass (tools/name_files.py).

#include "types.h"
#include "field/event_entrance_effect.h"
#include "field/field.h"
#include "field/field_camera.h"
#include "field/ov021_021bddc0.h"
#include "gfl/heap.h"
#include "gfl/std.h"
#include "struct_decls.h"

EntranceCamera *func_ov021_021bdd58(HeapID heapId);
void func_ov021_021bdd7c(EntranceCamera *camera, Field *field);
void func_ov021_021bdd9c(EntranceCamera *camera, HeapID heapId);
void func_ov021_021bdd74(EntranceCamera *camera);
void func_ov021_021bddac(EntranceCamera *camera);

EntranceCamera *func_ov021_021bdc94(Field *field) {
    HeapID heapID;
    EntranceCamera *camera;

    heapID = Field_GetHeapID(field);
    camera = func_ov021_021bdd58(heapID);
    func_ov021_021bdd7c(camera, field);
    func_ov021_021bdd9c(camera, heapID);
    return camera;
}

void func_ov021_021bdcb8(EntranceCamera *camera) {
    func_ov021_021bddac(camera);
    func_ov021_021bdd74(camera);
}

void func_ov021_021bdcc8(EntranceCamera *camera, const EntranceCameraParam *param) {
    func_ov021_021be184(camera, param);
    FieldCameraAnimationController_SetAnimation(camera->controller, &camera->animation);
    if (func_ov021_021be1a0(camera)) {
        switch (param->isArrival) {
        case 0:
            func_ov021_021bdfb0(camera);
            break;
        case 1:
            func_ov021_021be068(camera);
            break;
        }
    } else {
        switch (param->isArrival) {
        case 0:
            func_ov021_021bddc0(camera);
            break;
        case 1:
            func_ov021_021bdeb4(camera);
            break;
        }
    }
    camera->unk_64 = 1;
}

void func_ov021_021bdd20(EntranceCamera *camera) {
    if (func_ov021_021be1c4(camera) != 0) {
        FieldCameraAnimationController_StartAnimation(camera->controller);
    }
}

void func_ov021_021bdd34(EntranceCamera *camera) {
    FieldCameraAnimationController_FinishCamera(camera->controller);
}

BOOL func_ov021_021bdd40(EntranceCamera *camera) {
    return func_ov021_021be1c4(camera);
}

BOOL func_ov021_021bdd48(EntranceCamera *camera) {
    if (camera->animation.frames == 0) {
        return 1;
    }
    return 0;
}

EntranceCamera *func_ov021_021bdd58(HeapID heapId) {
    return GFL_HeapAllocate(heapId, sizeof(EntranceCamera), 0, "entrance_camera.c", 0x12a);
}

void func_ov021_021bdd74(EntranceCamera *camera) {
    GFL_HeapFree(camera);
}

void func_ov021_021bdd7c(EntranceCamera *camera, Field *field) {
    MI_CpuFill8(camera, 0, sizeof(EntranceCamera));
    camera->field = field;
    camera->camera = Field_GetCameraSystem(field);
    camera->unk_64 = 0;
}

void func_ov021_021bdd9c(EntranceCamera *camera, HeapID heapId) {
    camera->controller = Field_CreateCameraAnimationController(camera->field);
}

void func_ov021_021bddac(EntranceCamera *camera) {
    if (camera->controller != NULL) {
        FieldCameraAnimationController_FreeAnimation(camera->controller);
        camera->controller = NULL;
    }
}
