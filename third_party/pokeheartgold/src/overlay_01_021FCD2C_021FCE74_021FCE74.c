#include "global.h"
#include "overlay_01_unk_struct_ov5_021f0fb8.h"
struct UnkStruct_ov5_021F0FB8;

s32 ov01_021FCE74(struct UnkStruct_ov5_021F0FB8 *arg0);

s32 ov01_021FCE74(UnkStruct_ov5_021F0FB8 *arg0) {
    u32 v0;
    u32 v1;

    *((s32 *)&arg0->unk_20) = (s32)(*((s32 *)&arg0->unk_20) + *((s32 *)&arg0->unk_28));
    v1 = arg0->unk_24 + 1;
    arg0->unk_24 = v1;
    v0 = arg0->unk_1C;
    if (v1 >= v0) {
        arg0->unk_24 = v0;
        *((s32 *)&arg0->unk_20) = (s32) *((s32 *)&arg0->unk_18);
        return 1;
    }
    return 0;
}
