#include "global.h"
#include "unk_struct_02013034.h"
struct UnkStruct_02013034;

void sub_02013F94(void *arg0);

void sub_02013FA8(void *arg0);

void sub_02013FC0(struct UnkStruct_02013034 *arg0, void *arg1);
s32 sub_02013728();
void sub_02013FD0(void *arg0, s32 arg1);

void sub_02013FA8(void *arg0) {
    void *v0;
    void *v1;

    v1 = *(void **)((u8 *)arg0 + 0xC);
    if (v1 != arg0) {
        do {
            v0 = *(void **)((u8 *)v1 + 0xC);
            sub_02013F94(v1);
            v1 = v0;
        } while (v0 != arg0);
    }
}

void sub_02013FC0(UnkStruct_02013034 *arg0, void *arg1) {
    *((void * *)&arg0->unk_0C) = (void *)*(void **)((u8 *)arg1 + 0xC);
    *((void * *)&arg0->unk_10) = arg1;
    *(void **)((u8 *)(*(void **)((u8 *)arg1 + 0xC)) + 0x10) = arg0;
    *(void **)((u8 *)arg1 + 0xC) = arg0;
}

void sub_02013FD0(void *arg0, s32 arg1) {
    *(s32 *)((u8 *)arg0 + 8) = arg1;
    sub_02013728();
}
