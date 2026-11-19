#include "global.h"

#include "pokeathlon/pokeathlon.h"
#include "overlay_96_unk_struct_ov96_0220e3f0.h"

typedef struct UnkStruct_ov96_02210858 {
    u8 unk_0[0x8];
    u8 unk_8;
    u8 unk_9[0xB3];
    u8 unk_BC;
} UnkStruct_ov96_02210858;

// ov96_0220E850 returns an event score's low byte (0..255, ldrb-wide on the DS): the PC must not sign-extend it
#if defined(SDK_PORT) || defined(NONMATCHING)
u8 ov96_0220E850(UnkStruct_ov96_0220E3F0 *, u8);
#else
s8 ov96_0220E850(UnkStruct_ov96_0220E3F0 *, u8);
#endif

s32 ov96_022108EC(void *, u32, s32);

void ov96_02210858(void *arg0);

s32 ov96_02210980(void *, void *, int);
void ov96_022108BC(void *arg0, s32 arg1, void *arg2);

void ov96_02210858(void *arg0) {
    UnkStruct_ov96_02210858 *v0;
    s32 v5;
    PokeathlonCourseData **new_var;
    u32 v2;
    u32 v4;
    UnkStruct_ov96_0220E3F0 *v1;
    void *v3;
    v0 = arg0;
    new_var = (PokeathlonCourseData **)(((u8 *)arg0) + 4);
    v1 = PokeathlonCourse_GetHeapAllocPtr4(*new_var);
    v5 = 0;
    v2 = 0;
    do {
        v4 = ov96_0220E850(v1, v2);
        v2 += 1;
        v5 += v4;
    } while (((s32)v2) < 4);
    v4 = 0;
    if (((s32)(v0->unk_BC)) > 0) {
        v3 = (void *)&v0->unk_8;
        do {
            ov96_022108EC(v3, ((u32)((v5 + (((u32)(v5 >> 1)) >> 0x1E)) << 0xE)) >> 0x10, (u16)ov96_0220E850(v1, v4));
            v4 += 1;
            v3 += 0x3C;
        } while (((s32)v4) < ((s32)(v0->unk_BC)));
    }
}

void ov96_022108BC(void *dest, s32 arg1, void *arg2) {
    s32 v2;
    u32 v0;
    void *v1;

    v1 = dest;
    MI_CpuFill8(dest, 0U, 0x3CU);
    v2 = 0;
    v0 = arg1 * 3;
    do {
        ov96_02210980(v1, arg2, (u8)v0);
        v0 += 1;
        v2 += 1;
        v1 += 0x14;
    } while (v2 < 3);
}
