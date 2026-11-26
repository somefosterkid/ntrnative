#include "global.h"
#include "unk_struct_02013034.h"
#include "unk_02013534.h"
struct UnkStruct_02013034;

void sub_02013F94(void *arg0);

void sub_02013FA8(void *arg0);

void sub_02013FC0(struct UnkStruct_02013034 *arg0, void *arg1);
void sub_02013728(TextOBJ *param0);
void sub_02013FD0(void *arg0, s32 arg1);

// the list links are UnkStruct_02013034's unk_0C / unk_10 (DS offsets 0xC / 0x10; 0x10 / 0x18 on the host)
void sub_02013FA8(void *arg0) {
    void *v0;
    void *v1;

    v1 = ((UnkStruct_02013034 *)arg0)->unk_0C;
    if (v1 != arg0) {
        do {
            v0 = ((UnkStruct_02013034 *)v1)->unk_0C;
            sub_02013F94(v1);
            v1 = v0;
        } while (v0 != arg0);
    }
}

void sub_02013FC0(UnkStruct_02013034 *arg0, void *arg1) {
    *((void * *)&arg0->unk_0C) = (void *)((UnkStruct_02013034 *)arg1)->unk_0C;
    *((void * *)&arg0->unk_10) = arg1;
    *((void * *)&((UnkStruct_02013034 *)((UnkStruct_02013034 *)arg1)->unk_0C)->unk_10) = arg0;
    *((void * *)&((UnkStruct_02013034 *)arg1)->unk_0C) = arg0;
}

void sub_02013FD0(void *arg0, s32 arg1) {
    *(s32 *)((u8 *)arg0 + 8) = arg1;
    sub_02013728(arg0);
}
