#include "global.h"

#include "error_handling.h"

typedef struct {
    u32 a[4];
} _020FDA28_Array;

extern const _020FDA28_Array _020FDA28;

u32 sub_020623D8(s32 param_1);

u32 sub_020623D8(s32 param_1) {
    _020FDA28_Array auStack_18;
    auStack_18 = _020FDA28;
    if (param_1 >= 4) {
        GF_AssertFail();
    }
    return auStack_18.a[param_1];
}
