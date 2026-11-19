#include "global.h"

#include "overlay_96_ov96rival.h"
#include "sys_task.h"

void Heap_Free(void *ptr);

void ov96_0220C998(void *ptr);

void ov96_0220C998(void *ptr) {
    Heap_Free(ptr);
}

s32 ov96_0220CA28(Ov96Rival *);

void ov96_0220CF50(Ov96Rival *, s32, PokeathlonCourseData *);

void ov96_0220C9A0(Ov96RivalSet *arg0);

void ov96_0220C9A0(Ov96RivalSet *arg0) {
    unsigned int new_var2;
    u32 v1;
    Ov96Rival *v0;
    v1 = 0;
    new_var2 = 0U;
    if (arg0->count > new_var2) {
        v0 = &arg0->rivals[0];
        do {
            ov96_0220CF50(v0, ov96_0220CA28(v0), arg0->course);
            v1 += 1;
            v0++;
        } while (v1 < arg0->count);
    }
}
