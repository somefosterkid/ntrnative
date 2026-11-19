#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"
#include "overlay_96_unk_struct_ov96_0220e3f0.h"

u8 ov96_0220E7BC(PokeathlonCourseData *arg0, s32 arg1);

u8 ov96_0220E7BC(PokeathlonCourseData *arg0, s32 arg1) {
    s32 i;
    UnkStruct_ov96_0220E3F0 *work;
    u16 temp_r1;
    u16 temp_r3;
    u8 var_r4;
#if !defined(SDK_PORT) && !defined(NONMATCHING)
    u32 *bits; // DS: the loop walks a pointer to entries[i].unk_E0
#endif

    var_r4 = 0;
    if (arg0 == NULL) {
        GF_AssertFail();
    }
    work = PokeathlonCourse_GetHeapAllocPtr4(arg0);
    if (work == NULL) {
        GF_AssertFail();
    }
    i = 0;
    temp_r1 = ((u32)(work->entries[arg1].unk_E0 << 0xE) >> 0x10);
#if !defined(SDK_PORT) && !defined(NONMATCHING)
    bits = &work->entries[0].unk_E0;
#endif
    do {
#if defined(SDK_PORT) || defined(NONMATCHING)
        temp_r3 = (u16)((u32)(work->entries[i].unk_E0 << 0xE) >> 0x10);
#else
        temp_r3 = (u16)((u32)(*bits << 0xE) >> 0x10);
#endif
        if ((i != arg1) && (temp_r1 < temp_r3)) {
            var_r4 += 1;
        }
        i += 1;
#if !defined(SDK_PORT) && !defined(NONMATCHING)
        bits = (u32 *)((u8 *)bits + sizeof(UnkStruct_ov96_0220E3F0_Entry));
#endif
    } while (i < 4);
    return var_r4;
}
