#include "global.h"

#include "heap.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021E88F8(void *);
s32 ov14_021EF6E4(void *arg0);

s32 ov14_021EF6D4(UnkStruct_ov14_boxapp *arg0) {
    Heap_Free(arg0->sys->unk_C);
    return 0x82;
}

s32 ov14_021F40DC();

s32 ov14_021EF6E4(void *arg0) {
    ov14_021F40DC();
    ov14_021E88F8(((u8 *)((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0));
    return 0x85;
}
