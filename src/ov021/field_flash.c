// Black's code at ov021 0x021c7dac-0x021c7de0. The name is the file name its asserts pass (tools/name_files.py).

#include "types.h"
#include "field/field_flash.h"
#include "field/ov021_021c7de0.h"
#include "gfl/heap.h"

u32 *FieldFlashG3D_Create(HeapID heapId);

FieldFlash *FieldFlash_Create(HeapID heapId) {
    FieldFlash *flash = GFL_HeapAllocate(heapId, sizeof(FieldFlash), 1, "field_flash.c", 0x7a);

    flash->g3d = (FieldFlashG3D *)FieldFlashG3D_Create(heapId);
    flash->state = 0;
    flash->nextState = 4;
    FieldFlashAnimation_Init(&flash->animation);
    return flash;
}
