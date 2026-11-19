#include "global.h"
struct UnkStruct_ov66_022336C4;

#include "heap.h"
#include "overlay_45_unk_struct_ov66_022336_c4.h"

void ov45_0222F878(void);

void ov45_0222F898(struct UnkStruct_ov66_022336C4 *, s32, const void *, u32);
void ov45_0222F88C(struct UnkStruct_ov66_022336C4 *arg0, s32 arg1, const void *arg2);

void ov45_0222F878(void) {
    Heap_Free(((void *)_022577C0->unk_24));
}

void ov45_0222F88C(UnkStruct_ov66_022336C4 *arg0, s32 arg1, const void *arg2) {
    ov45_0222F898(arg0, arg1, arg2, arg0->unk_28);
}
