#include "global.h"

#include "pokeathlon/pokeathlon.h"

void *ov96_021E9A54(int param_1, void *param_2, int param_3);

void *ov96_021E86FC(PokeathlonCourseData *, u32);
void *ov96_021E9A68(int arg0, void *arg1, int arg2);

void *ov96_021E9A54(int param_1, void *param_2, int param_3) {
    void *pvVar1;

    pvVar1 = PokeathlonCourse_GetFieldBA4(param_2);
    return (void *)((u8 *)pvVar1 + param_1 * param_3);
}

void *ov96_021E9A68(int arg0, void *arg1, int arg2) {
    return ov96_021E86FC(arg1, (u8)arg0);
}
