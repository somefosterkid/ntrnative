#include "global.h"

#include "pokeathlon/pokeathlon.h"
#include "overlay_96_unk_struct_ov96_0220e3f0.h"

#include "math_util.h"

typedef struct UnkStruct_ov96_02210AE0 {
    PokeathlonCourseData *course;
    u32 unk4;
    u32 unk8;
    u32 active : 1;
    u32 unkC_1 : 2;
    u32 unkC_3 : 2;
    u32 unkC_5 : 2;
    u32 unkC_7 : 4;
    u32 timer : 8;
    u32 unkC_19 : 13;
    u8 countdown;
} UnkStruct_ov96_02210AE0;

#if defined(SDK_PORT) || defined(NONMATCHING) // as defined in src/overlay_96_0220E70C.c (tools/pokehg_abi.py)
BOOL ov96_0220E70C(UnkStruct_ov96_0220E3F0 *data, u8 a, u32 b);
#else
BOOL ov96_0220E70C(UnkStruct_ov96_0220E3F0 *data, u8 a, u8 b);
#endif

#if defined(SDK_PORT) || defined(NONMATCHING) // as defined in src/overlay_96_0220E70C.c (tools/pokehg_abi.py)
void ov96_0220E770(UnkStruct_ov96_0220E3F0 *data, u32 a, u32 b, u16 *out1, u32 *out2);
#else
void ov96_0220E770(UnkStruct_ov96_0220E3F0 *data, u8 a, u8 b, u16 *out1, u32 *out2);
#endif

#if defined(SDK_PORT) || defined(NONMATCHING) // as defined in src/overlay_96_0220E818.c (tools/pokehg_abi.py)
void ov96_0220E834(UnkStruct_ov96_0220E3F0 *data, s32 a, s32 b);
#else
void ov96_0220E834(UnkStruct_ov96_0220E3F0 *data, u8 a, u8 b);
#endif

void ov96_0221090C(UnkStruct_ov96_02210AE0 *s);

void ov96_022109D8(UnkStruct_ov96_02210AE0 *s);

#if defined(SDK_PORT) || defined(NONMATCHING)
BOOL ov96_02210BD0(UnkStruct_ov96_02210AE0 *s, u16 a, u32 b);
#else
BOOL ov96_02210BD0(UnkStruct_ov96_02210AE0 *s, u32 a, u32 b);
#endif

extern const s32 ov96_0221D1B8[];

void ov96_02210AE0(UnkStruct_ov96_02210AE0 *s, int a, int b);

void ov96_02210AE0(UnkStruct_ov96_02210AE0 *s, int a, int b) {
    UnkStruct_ov96_0220E3F0 *data;
    s32 rand;
    s32 chance;
    u16 sp1c[2];
    u32 sp20;

    data = PokeathlonCourse_GetHeapAllocPtr4(s->course);
    if (ov96_0220E70C(data, s->unkC_3, s->unkC_5)) {
        s->countdown--;
        if (s->countdown == 0) {
            ov96_0221090C(s);
            s->countdown = 90;
        }
        s->timer--;
        if (s->timer == 0) {
            ov96_022109D8(s);
            s->timer = 20;
        }
        if (s->active && ov96_02210BD0(s, s->unkC_1, 16)) {
            rand = MTRandom() % 100;
            ov96_0220E770(data, s->unkC_3, s->unkC_5, sp1c, &sp20);
            if (a + 20 < b) {
                chance = 0;
            } else {
                chance = ov96_0221D1B8[s->unkC_5];
            }
            if (rand < chance) {
                ov96_0220E834(data, s->unkC_3, s->unkC_5);
            }
        }
    }
}
