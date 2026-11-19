#include "global.h"
#include "overlay_96_unk_struct_ov96_021_f5018.h"

#include "pokeathlon/pokeathlon.h"

s32 ov96_021EB564(void *, s32);

s32 ov96_021EB57C(void *);

void ov96_021F5EC4(PokeathlonCourseData *arg0);

void ov96_021F5EC4(PokeathlonCourseData *data) {
    UnkStruct_ov96_021F5018 *v0;
    s32 v3;
    u8 *v2;
    UnkStruct_ov96_021F5018 *v1;

    v2 = ov96_021E8A20(PokeathlonCourse_GetDataCopyArea(data) + 0xF0);
    v1 = PokeathlonCourse_GetHeapAllocPtr4(data);
    v0 = v1;
    v3 = 0;
    do {
        if (v3 == ov96_021E5F24(data)) {
            if ((v0->unk_142 != 0) && (ov96_021EB57C(v1->unk_68[v3]) == 0)) {
                ov96_021EB564(v1->unk_68[v3], v3 + 0xD);
            }
        } else if ((v2[v3] != 0) && (ov96_021EB57C(v1->unk_68[v3]) == 0)) {
            ov96_021EB564(v1->unk_68[v3], v3 + 0xD);
        }
        v3 += 1;
    } while (v3 < 4);
}

void ov96_021F5F34(s32 param_1, s32 param_2, VecFx32 *param_3, VecFx32 *param_4);

void ov96_021F5F34(s32 param_1, s32 param_2, VecFx32 *param_3, VecFx32 *param_4) {
    VecFx32 VStack_18;

    VStack_18.x = param_1 << 0xc;
    VStack_18.y = param_2 * 0x1000 + 0xc0000;
    VStack_18.z = 0;
    VEC_Subtract(&VStack_18, param_3, param_4);
    if (0x70000 < param_4->y) {
        param_4->y = 0x70000;
    }
    return;
}
