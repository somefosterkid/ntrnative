#include "global.h"

#include "unk_02058AEC.h"

s32 sub_0205A034(void *, s32 *, s32);

s32 sub_0205ABBC(void *);

extern s32 sub_02059EBC;

void sub_0205A410(int arg0, int arg1, void *arg2, void *arg3) {
    sub_0205A034(*(void **)((u8 *)arg3 + 0x80), &sub_02059EBC, 2);
    sub_0205ABBC(*(void **)((u8 *)arg3 + 0x80));
}
