#include "global.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"
struct UnkStruct_ov14_boxapp;

u8 sub_02019B10(void *, s32);

s32 ov14_021F23F0(UnkStruct_ov14_boxapp *arg0, s32 arg1, s32 arg2) {
    void *temp_r0;
    void *temp_r0_2;
    void *temp_r2;
    s32 temp_ret;
    temp_r2 = *((void * *)&arg0->sys);
    *((u8 *)&((UnkStruct_ov14_boxsys *)temp_r2)->unk_88D4[0]) = ((*((u8 *)&((UnkStruct_ov14_boxsys *)temp_r2)->unk_88D4[0])) & (~1)) | 1;
    temp_ret = sub_02019B10(((u8 *)arg0->sys->unk_2F0), 2);
    *((u8 *)&arg0->sys->unk_88D4[0]) = (u8)(((*((u8 *)&arg0->sys->unk_88D4[0])) & (~0xFE)) | ((((u32)temp_ret) << 25) >> 24));
    temp_r0 = *((void * *)&arg0->sys);
    *((u8 *)&((UnkStruct_ov14_boxsys *)temp_r0)->unk_88D4[1]) = (u8)(((*((u8 *)&((UnkStruct_ov14_boxsys *)temp_r0)->unk_88D4[1])) & (~0xF)) | 3);
    temp_r0_2 = *((void * *)&arg0->sys);
    *((u8 *)&((UnkStruct_ov14_boxsys *)temp_r0_2)->unk_88D4[1]) = (u8)(((*((u8 *)&((UnkStruct_ov14_boxsys *)temp_r0_2)->unk_88D4[1])) & (~0xF0)) | 0x20);
    *((s8 *)&arg0->sys->unk_88D4[2]) = 0;
    *((s8 *)&arg0->sys->unk_88D4[3]) = 0;
    if (arg1 == 0) {
        *((u8 *)&arg0->sys->unk_88D4[5]) = 0x10;
    } else {
        *((u8 *)&arg0->sys->unk_88D4[5]) = 0x14;
    }
    *((s8 *)&arg0->sys->unk_88D4[4]) = 0x16;
    *((s8 *)&arg0->sys->unk_88D4[6]) = 9;
    *((s8 *)&arg0->sys->unk_88D4[7]) = 4;
    *((s32 *)&arg0->unk30) = arg2;
    return 8;
}
