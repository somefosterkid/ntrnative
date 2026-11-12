// Black's code at ov021 0x021c6b18-0x021c6c70. The name is the file name its asserts pass (tools/name_files.py).

#include "types.h"
#include "field/field.h"
#include "field/field_camera.h"
#include "gfl/heap.h"
#include "gfl/tcb.h"
#include "nitro/fx/fx.h"
#include "struct_decls.h"
#include "system/fp.h"

// pokebw2's FieldCameraZoomTCB: the zoom task's work, Black's layout the same
typedef struct FieldCameraZoomTCB {
    Field *field;
    u16 frame;
    u16 frameCount;
    fx32 startZoom;
    fx32 endZoom;
} FieldCameraZoomTCB;

void FieldCameraZoomTCB_Callback(TCB *param_1, void *param_2);
void func_ov021_021c6bb4(FieldCameraZoomTCB *param_1);

void FieldCameraZoomTCB_Create(Field *param_1, u16 param_2, s32 param_3) {
    u16 heapID;
    FieldCamera *camera;
    TCBManager *manager;
    FieldCameraZoomTCB *data;
    fx32 result;

    heapID = Field_GetHeapID(param_1);
    camera = Field_GetCameraSystem(param_1);
    manager = Field_GetTCBMgr(param_1);
    data = GFL_HeapAllocate(heapID & 0x7fff | 0x8000, sizeof(FieldCameraZoomTCB), 0, "fieldmap_tcb_camera_zoom.c", 0x5f);
    data->frame = 0;
    data->frameCount = param_2;
    result = FieldCamera_CoordsGetZoom(camera);
    data->startZoom = result;
    result = FieldCamera_CoordsGetZoom(camera);
    data->endZoom = param_3 + result;
    data->field = param_1;
    GFL_TCBMgrAddTask(manager, FieldCameraZoomTCB_Callback, data, 0);
}

void FieldCameraZoomTCB_Callback(TCB *param_1, void *param_2) {
    FieldCameraZoomTCB *zoom = param_2;

    func_ov021_021c6bb4(zoom);
    zoom->frame = zoom->frame + 1;
    if (zoom->frameCount < zoom->frame) {
        GFL_HeapFree(zoom);
        GFL_TCBRemove(param_1);
    }
}

void func_ov021_021c6bb4(FieldCameraZoomTCB *param_1) {
    FieldCamera *camera;
    u16 max;
    u16 cur;
    f32 ratio;
    f32 one = 1.0f;
    f32 t;
    f32 p;
    f32 q;
    f32 mix;
    s32 zoom;

    camera = Field_GetCameraSystem(param_1->field);
    max = param_1->frameCount;
    cur = param_1->frame;
    if (cur <= max) {
        ratio = (f32)cur / (f32)max;
        t = (f32)sqrt((f64)(one * ratio));
        p = (f32)param_1->startZoom / 4096.0f;
        q = (f32)param_1->endZoom / 4096.0f;
        mix = one - t;
        mix *= p;
        mix += t * q;
        if (mix > 0.0f) {
            zoom = (s32)(0.5f + 4096.0f * mix);
        } else {
            zoom = (s32)(4096.0f * mix - 0.5f);
        }
        FieldCamera_CoordsSetZoom(camera, zoom);
    }
}


