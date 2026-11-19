#include "global.h"
#include "overlay_91_unk_struct_ov115_02261_c18.h"
struct UnkStruct_ov115_02261C18;

s32 ov91_022601C8();

s32 ov91_022601F4(void *, void *);

s32 ov91_02260218(void *, void *);

s32 ov91_02260254(void *, u32);

s32 ov91_02260298(void *, s32);

s32 ov91_0226031C(void *, s32);

s32 ov91_02260334(void *, u16);

void ov91_0226014C(void *arg0, struct UnkStruct_ov115_02261C18 *arg1, s32 arg2);

void ov91_0226014C(void *arg0, UnkStruct_ov115_02261C18 *arg1, s32 arg2) {
    ov91_022601C8();
    ov91_022601F4(arg0, arg1);
    ov91_02260218(arg0, arg1);
    switch (arg2) {
    case 0:
    case 2:
        ov91_02260298(arg0, 1);
        ov91_02260254(arg0, arg1->unk_04);
        ov91_02260334(arg0, 1);
        return;
    case 1:
        ov91_02260298(arg0, 3);
        ov91_02260334(arg0, 0);
        ov91_0226031C(arg0, 0x1000);
        return;
    }
}
