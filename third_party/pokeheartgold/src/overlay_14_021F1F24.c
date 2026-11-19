#include "global.h"

#include "heap.h"
#include "field_system.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

void ov14_021F1F24(FieldSystem *arg0);
void ov14_021F1F38(u8 *param_1);

void ov14_021F1F24(FieldSystem *arg0) {
    *(void **)((u8 *)(*((void * *)&arg0->areaDataManager)) + 0xC) = Heap_Alloc(HEAP_ID_10, 0x1CU);
}

void ov14_021F1F38(u8 *param_1) {
    Heap_Free(((UnkStruct_ov14_boxapp *)param_1)->sys->unk_C);
    return;
}
