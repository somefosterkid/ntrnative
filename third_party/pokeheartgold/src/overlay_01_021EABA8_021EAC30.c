#include "global.h"
#include "camera.h"
#include "field_system.h"
#include "field_types_def.h"
#include "field/overlay_01_021EABA8.h"
#include "heap.h"
#include "overlay_01_021EABA8.h"

SysTask *ov01_021EAC64(void *);

void FieldCamera_Delete(FieldSystem *fieldSystem) {
    ov01_021EAC64(fieldSystem->unk28);
    Camera_UnsetStaticPtr();
    Camera_History_Delete(fieldSystem->camera);
    Camera_Delete(fieldSystem->camera);
}

FieldCameraState *ov01_021EAC4C(enum HeapID heapID) {
    FieldCameraState *dest;

    dest = Heap_Alloc(heapID, sizeof(FieldCameraState));
    MI_CpuFill8(dest, 0U, sizeof(FieldCameraState));
    return dest;
}
