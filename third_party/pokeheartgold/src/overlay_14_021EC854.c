#include "global.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021E7588(void *, s32);
s32 ov14_021F1580(void *, u8);
s32 ov14_021F6AC0(void *, s32, u32);

s32 ov14_021EC854(void *arg0) {
    s32 v0;

    ((UnkStruct_ov14_boxapp *)arg0)->sys->unk_43C = (s32)(((s32) ((UnkStruct_ov14_boxapp *)arg0)->unk25 % 6) + 0x25);
    if (((UnkStruct_ov14_boxapp *)arg0)->unk2A != 0) {
        ov14_021F6AC0(arg0, 4, ((UnkStruct_ov14_boxapp *)arg0)->unk2B);
        ((UnkStruct_ov14_boxapp *)arg0)->unk21 = (u8) ((UnkStruct_ov14_boxapp *)arg0)->unk2B;
        v0 = ov14_021F1580(arg0, ((UnkStruct_ov14_boxapp *)arg0)->unk2B);
        ((UnkStruct_ov14_boxapp *)arg0)->unk2A = 0U;
        return v0;
    }
    ov14_021F6AC0(arg0, 4, ((UnkStruct_ov14_boxapp *)arg0)->unk2B);
    ov14_021E7588(arg0, ((UnkStruct_ov14_boxapp *)arg0)->unk2B);
    if (*(s32 *)((u8 *)(*(void **)arg0) + 8) == 3) {
        return 0x82;
    }
    return 0x29;
}
