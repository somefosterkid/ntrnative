#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"

typedef struct UnkStruct_ov96_021F4688 {
    u8 unk_0[0x4];
    PokeathlonCourseData *unk_4;
    u8 unk_8[0x148];
    u8 unk_150;
} UnkStruct_ov96_021F4688;

void ov96_021F4688(UnkStruct_ov96_021F4688 *arg0, u32 arg1, s32 arg2);

void ov96_021F4688(UnkStruct_ov96_021F4688 *arg0, u32 arg1, s32 arg2) {
    u8 v0;

    if (arg2 == ov96_021E5F24(arg0->unk_4)) {
        v0 = arg0->unk_150;
        if (v0 != arg1) {
            if ((u32)v0 >= arg1) {
                GF_AssertFail();
            }
            arg0->unk_150 = (u8)arg1;
        }
    }
}

u8 ov96_021F46B4(void *arg0);

u8 ov96_021F46B4(void *arg0) {
    return *(u8 *)((u8 *)arg0 + 0x150);
}
