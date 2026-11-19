#include "global.h"

typedef struct UnkStruct_ov41_0224A9F8 {
    u8 unk_0[0x10];
    void *unk_10;
} UnkStruct_ov41_0224A9F8;

s32 ov41_0224A9BC();

s32 sub_02013728(void *);

void ov41_0224A9F8(UnkStruct_ov41_0224A9F8 *arg0);

void ov41_0224A9F8(UnkStruct_ov41_0224A9F8 *arg0) {
    ov41_0224A9BC();
    sub_02013728(arg0->unk_10);
}
