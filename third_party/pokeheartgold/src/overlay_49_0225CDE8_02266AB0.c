#include "global.h"
struct UnkStruct_ov70_02269800;

#include "overlay_49.h"
#include "overlay_49_unk_struct_ov70_02269800.h"

s32 ov49_02267D00(void *, void *);

s32 ov49_02267D34(void *, void *);

s32 ov49_02267E18(void *, u32);

s32 ov49_02266AB0(void *arg0, void *arg1) {
    s32 v0;

    v0 = ov49_02267E18(arg1 + 0x954, *(s16 *)((u8 *)arg1 + 2));
    *(s16 *)((u8 *)arg1 + 2) = (s16)(*(s16 *)((u8 *)arg1 + 2) + 1);
    if (v0 == 1) {
        ov49_02267D00(arg0, arg1);
    }
    ov49_02267D34(arg0, arg1);
    if (v0 == 2) {
        return 1;
    }
    return 0;
}

s32 ov49_02258F70(void *);

s32 ov49_02265968(void *, s32);

s32 ov49_022659D0(void *, void *, s32);

s32 ov49_02266AF0(void *arg0, struct UnkStruct_ov70_02269800 *arg1);

s32 ov49_02266AF0(void *arg0, UnkStruct_ov70_02269800 *arg1) {
    s32 v0;

    if (ov49_02258F70(*((void **)&arg1->unk_08)) == 1) {
        return 0;
    }
    v0 = 0;
    do {
        if (ov49_02265968(arg1, v0) == 1) {
            ov49_022659D0(arg0, arg1, v0);
        }
        v0 += 1;
    } while (v0 < 0x12);
    return 1;
}
