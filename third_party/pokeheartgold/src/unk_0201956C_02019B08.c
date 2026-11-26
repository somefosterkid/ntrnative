#include "global.h"

#include "unk_0201956C.h"


u8 sub_02019B10(UnkStruct_0201956C *arg0, s32 arg1);

u16 *sub_02019B08(UnkStruct_0201956C *a0, int a1) {
    return a0->entries[a1].screen;
}

u8 sub_02019B10(UnkStruct_0201956C *arg0, s32 arg1) {
    return arg0->entries[arg1].bg;
}

void sub_02019B1C(UnkStruct_0201956C *arg0, s32 arg1, s8 *arg2, s8 *arg3) {
    if (arg2 != NULL) {
        *arg2 = arg0->entries[arg1].x;
    }
    if (arg3 != NULL) {
        *arg3 = arg0->entries[arg1].y;
    }
}

void sub_02019B44(UnkStruct_0201956C *a0, int a1, u16 *a2, u16 *a3) {
    if (a2 != NULL) {
        *a2 = a0->entries[a1].width;
    }
    if (a3 != NULL) {
        *a3 = a0->entries[a1].height;
    }
}
