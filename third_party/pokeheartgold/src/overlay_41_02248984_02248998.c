#include "global.h"
#include "overlay_41_unk_struct_ov22_02257_f50.h"

s32 ov41_02248998(void *arg0);

s32 ov41_02248998(void *arg0) {
    if (*(s32 *)((u8 *)arg0 + 0x3C) == 0) {
        return 1;
    }
    return 0;
}

s32 ov41_022489E4(void *, s32);

s32 ov41_02248A28(void *, s32);

void ov41_022489A8(void *arg0, void *arg1);

void ov41_022489A8(void *arg0, void *arg1) {
    ov41_02248A28(arg0 + 0xC, *(s32 *)((u8 *)arg1 + 0x1C));
    *(s32 *)arg0 = 0;
    ov41_022489E4(*((void **)&((UnkStruct_ov22_02257F50 *)arg0)->unk_00.unk_0C[0].unk_00), 1);
    ov41_02248A28(arg0 + 0x18, *(s32 *)((u8 *)arg1 + 0x20));
    ov41_02248A28(arg0 + 0x24, *(s32 *)((u8 *)arg1 + 0x24));
    ov41_02248A28(arg0 + 0x30, 1);
}
