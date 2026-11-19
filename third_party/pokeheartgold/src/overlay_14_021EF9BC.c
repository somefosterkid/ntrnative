#include "global.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"

#include "heap.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021EF9BC(UnkStruct_ov14_boxapp *arg0) {
    Heap_Free(arg0->sys->unk_C);
    return 0x88;
}
