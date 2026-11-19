#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "heap.h"
#include "unk_02013534.h"

void *ov96_021F86E8(enum HeapID arg0, BgConfig *arg1, PokeathlonCourseData *arg2);

void *ov96_021F86E8(enum HeapID arg0, BgConfig *arg1, PokeathlonCourseData *arg2) {
    s32 v0;
    void *dest;
    void *v1;

    dest = Heap_Alloc(arg0, 0x334U);
    MI_CpuFill8(dest, 0U, 0x334U);
    *(enum HeapID *)dest = arg0;
    *(UnkStruct_02013534 **)((u8 *)dest + 0x10) = FontSystem_NewInit(0xA, arg0);
    *(BgConfig **)((u8 *)dest + 8) = arg1;
    *(PokeathlonCourseData **)((u8 *)dest + 4) = arg2;
    v0 = 0;
    v1 = dest;
    do {
        *(s32 *)((u8 *)v1 + 0x48) = v0;
        v0 += 1;
        v1 += 0x48;
    } while (v0 < 5);
    return dest;
}
