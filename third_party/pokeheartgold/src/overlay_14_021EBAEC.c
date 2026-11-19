#include "global.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021E8740(void *);
s32 ov14_021F01D8(void *, s32);
s32 ov14_021F3488(void *, s32, s32);
s32 ov14_021F6844(void *, s32, s32);
s32 ov14_021F6AC0(void *, s32, s32);

void ov14_021EBAEC(void *arg0) {
    ov14_021F6AC0(arg0, 9, 0xA);
    ((UnkStruct_ov14_boxapp *)arg0)->sys->unk_43C = (s32)((s32) ((UnkStruct_ov14_boxapp *)arg0)->unk25 % 6);
    ov14_021E8740(arg0);
    ov14_021F6844(arg0, 0, 0x27);
    if (*(s32 *)((u8 *)(*(void **)arg0) + 8) == 3) {
        ov14_021F3488(arg0, 0x81, 1);
    }
    ov14_021F01D8(arg0, 0x3D);
}
