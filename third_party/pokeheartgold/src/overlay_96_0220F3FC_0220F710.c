#include "global.h"

#include "overlay_96_unk_struct_ov96_0220e3f0.h"

#include "pokeathlon/pokeathlon.h"

fx32 _s32_div_f(fx32, u32);

u32 ov96_0220D5D0(UnkStruct_ov96_0220E3F0_Unit *param_1, UnkStruct_ov96_0220E3F0_Unit *param_2);

u32 ov96_0220D694(UnkStruct_ov96_0220E3F0_Unit *param_1);

u32 ov96_0220D6CC(VecFx32 *param_1);

void ov96_0220E974(VecFx32 *param_1, s32 param_2);

void ov96_0220F710(PokeathlonCourseData *param_1);

void ov96_0220F710(PokeathlonCourseData *param_1) {
    s32 i;
    UnkStruct_ov96_0220E3F0 *base;
    u8 (*flags)[12];
    u8 (*flagRow)[12];
    s32 j;
    UnkStruct_ov96_0220E3F0_Unit *a;
    UnkStruct_ov96_0220E3F0_Unit *b;
    u8 *p;
    s32 spd;

    base = PokeathlonCourse_GetHeapAllocPtr4(param_1);
    flags = base->hitFlags;

    i = 0;
    flagRow = flags;
    for (; i < 12; i++) {
        a = &base->entries[i / 3].units[i % 3];
        for (j = i + 1; j < 12; j++) {
#if defined(SDK_PORT) || defined(NONMATCHING)
            p = &flags[j][i];
#else
            p = &(*flags)[i] + j * 12; // DS: (row 0 + i) + j rows
#endif
            b = &base->entries[j / 3].units[j % 3];
            if (ov96_0220D694(a) != 0 && ov96_0220D694(b) != 0 && ov96_0220D5D0(a, b) != 0 && (*flagRow)[j] == 0) {
                VecFx32 vec = { 0, 0, 0 };
                *p = 1;
                (*flagRow)[j] = *p;
                VEC_Subtract(&a->unk_1C, &b->unk_1C, &vec);
                VEC_Normalize(&vec, &vec);
                spd = b->cell->unk_4;
                if (((a->unk_40.raw << 6) >> 30) == 0) {
                    spd = spd * 6 / 5;
                }
                VEC_MultAdd(spd, &vec, &a->unk_28, &a->unk_34);
                spd = a->cell->unk_4;
                if (((b->unk_40.raw << 6) >> 30) == 0) {
                    spd = spd * 6 / 5;
                }
                VEC_MultAdd(spd, &vec, &b->unk_28, &b->unk_34);
                b->unk_34.x *= -1;
                b->unk_34.y *= -1;
                ov96_0220E974(&a->unk_34, 0x8000);
                ov96_0220E974(&b->unk_34, 0x8000);
                b->unk_40.raw = (b->unk_40.raw & 0xff0fffff) | (((u32)ov96_0220D6CC(&vec) << 28) >> 8) | 0x4000000;
                a->unk_40.raw |= 0x4000000;
            } else {
                *p = 0;
                (*flagRow)[j] = *p;
            }
        }
        flagRow++;
    }
}
