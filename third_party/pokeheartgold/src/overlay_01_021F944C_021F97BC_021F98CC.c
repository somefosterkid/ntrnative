#include "global.h"

#include "error_handling.h"
#include "map_object.h"

u16 *ov01_021F98B4(u32, u32, u16 *);

struct ResourceHeap;

s32 ov01_021FC5B8(struct ResourceHeap *, u32);

void *ov01_021FC5CC(struct ResourceHeap *, u32, NARC *, u32, BOOL);

s32 ov01_021F98CC(UnkStruct_ov5_021ED0A4 *arg0, struct ResourceHeap *arg1, s32 arg2, s32 arg3, u16 *arg4) {
    void *v0;

    if (ov01_021FC5B8(arg1, arg2) == 1) {
        return 0;
    }
    v0 = ov01_021F98B4(arg2, arg3, arg4);
    if (v0 == NULL) {
        GF_AssertFail();
    }
    ov01_021FC5CC(arg1, arg2, MapObjectManager_GetMapModelNarc(arg0->unk_104), *(u16 *)((u8 *)v0 + 2), 0);
    return 1;
}
