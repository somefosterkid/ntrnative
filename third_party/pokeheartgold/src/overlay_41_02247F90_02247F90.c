#include "global.h"
#include "overlay_41_unk_struct_ov22_02257964.h"

s32 ov41_02248038(void *model);

s32 ov41_022480E0(void *);

void ov41_02247F90(void *arg0);

void ov41_02247F90(void *arg0) {
    ov41_02248038(arg0);
    ov41_022480E0(arg0);
    memset(arg0, 0, 0x88U);
}

s32 ov41_022495F0(void *, void *, void *, void *, s32);

s32 ov41_02249700(void *, s32);

s32 ov41_0224971C(void *, s32, s32);

void ov41_02247FAC(UnkStruct_ov22_02257964 *arg0, void *arg1, void *arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);

void ov41_02247FAC(UnkStruct_ov22_02257964 *arg0, void *arg1, void *arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    ov41_022495F0(&arg0->unk_2C.unk_4C, *((void **)&arg0->unk_2C.unk_10), arg1, arg2, arg6);
    ov41_02249700(&arg0->unk_2C.unk_4C, arg5);
    ov41_0224971C(&arg0->unk_2C.unk_4C, arg3, arg4);
}
