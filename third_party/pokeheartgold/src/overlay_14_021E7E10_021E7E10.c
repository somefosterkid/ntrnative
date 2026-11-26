#include "global.h"
struct UnkStruct_ov14_boxapp;

#include "unk_0201956C.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
s32 ov14_021E7D9C(void *, u16);
#else
s32 ov14_021E7D9C(void *, s32);
#endif

void ov14_021E7E10(struct UnkStruct_ov14_boxapp *arg0, s32 arg1);

void ov14_021E7E10(UnkStruct_ov14_boxapp *arg0, s32 arg1) {
    u8 *var_r5;
    var_r5 = *((u8 * *)&arg0->sys);
    ((UnkStruct_ov14_boxsys *)var_r5)->unk_40C0 = ((UnkStruct_ov14_boxsys *)var_r5)->unk_40C0 ^ (1 << arg1);
    ov14_021E7D9C(arg0, arg1);
    sub_020196E8(arg0->sys->unk_2F0, 2, 0x15, 6);
}
