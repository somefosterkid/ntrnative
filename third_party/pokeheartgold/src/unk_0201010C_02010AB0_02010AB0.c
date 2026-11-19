#include "global.h"

#include "screen_fade_fx.h"
#include "screen_fade_shape_globals.h"

s32 sub_02010A6C(int, int, s32);

void sub_02010AB0(UnkStruct_020101B0 *arg0, UnkStruct_020101B0 *arg1, UnkStruct_020101B0 *arg2, const UnkStruct_0200FD8C *arg3, const UnkStruct_0200FD8C *arg4, s32 arg5);

void sub_02010AB0(UnkStruct_020101B0 *arg0, UnkStruct_020101B0 *arg1, UnkStruct_020101B0 *arg2, const UnkStruct_0200FD8C *arg3, const UnkStruct_0200FD8C *arg4, s32 arg5) {
    arg0->unk_00 = (s32)(arg3->unk_00 << 7);
    arg0->unk_04 = (s32)(arg3->unk_01 << 7);
    arg0->unk_08 = (s32)(arg3->unk_02 << 7);
    arg0->unk_0C = (s32)(arg3->unk_03 << 7);
    arg1->unk_00 = (s32)arg4->unk_00;
    arg1->unk_04 = (s32)arg4->unk_01;
    arg1->unk_08 = (s32)arg4->unk_02;
    arg1->unk_0C = (s32)arg4->unk_03;
    arg2->unk_00 = sub_02010A6C(arg3->unk_00, arg4->unk_00, arg5);
    arg2->unk_04 = sub_02010A6C(arg3->unk_01, arg4->unk_01, arg5);
    arg2->unk_08 = sub_02010A6C(arg3->unk_02, arg4->unk_02, arg5);
    arg2->unk_0C = sub_02010A6C(arg3->unk_03, arg4->unk_03, arg5);
}
