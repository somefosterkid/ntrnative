#include "global.h"

#include "unk_0201956C.h"
#include "unk_struct_ov70_02242e58.h"

typedef s32 (*UnkStruct_ov70_02242E58_Handler)(UnkStruct_ov70_02242E58 *);

extern UnkStruct_ov70_02242E58_Handler ov70_022466F8[];

s32 ov70_02242144(UnkStruct_ov70_02242E58 *param_1);

s32 ov70_02242144(UnkStruct_ov70_02242E58 *param_1) {
    s32 sVar1;
    s32 idx = param_1->unk_4C;
    sVar1 = ov70_022466F8[idx](param_1);
    sub_02019934(param_1->unk_1C);
    return sVar1;
}
