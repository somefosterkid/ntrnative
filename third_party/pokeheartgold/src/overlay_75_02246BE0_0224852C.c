#include "global.h"

#include "overlay_75.h"
#include "overlay_75_unk_struct_ov98_02247704.h"

s32 ov75_02246BCC(void *, s32, s32);

s32 ov75_02246BE0(void *, s8);

s32 ov75_0224852C(void *arg0) {
    ov75_02246BCC(*(void **)arg0, 1, 0);
    ov75_02246BE0(*(void **)arg0, 6);
    *(s32 *)((u8 *)arg0 + 8) = 0x1D;
    return 0;
}

s32 ov75_02246CF8(void *);

s32 ov75_0224854C(void *arg0);

s32 ov75_0224854C(void *arg0) {
    s32 v0;

    v0 = ov75_02246CF8(*(void **)arg0);
    switch (v0) {
    case 1:
        *((s32 *)&((UnkStruct_ov98_02247704 *)arg0)->unk_08) = 0;
        return 0;
    case 2:
    case 3:
        *((s32 *)&((UnkStruct_ov98_02247704 *)arg0)->unk_08) = 0x25;
        return 0;
    default:
        if (*((s32 *)&((UnkStruct_ov98_02247704 *)arg0)->unk_94) == 0) {
            *((s32 *)&((UnkStruct_ov98_02247704 *)arg0)->unk_08) = 7;
        }
        return 0;
    }
}
