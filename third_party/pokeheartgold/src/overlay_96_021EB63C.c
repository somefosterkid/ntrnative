#include "global.h"

#include "sprite.h"
#include "sys_task.h"
#include "pokeathlon/pokeathlon.h"

Sprite *ov96_021EB5B8(void *);

void ov96_021EB63C(void *arg0, s32 arg1);

BOOL ov96_021ED1D0(PokeathlonCourseData *param0);
BOOL ov96_021EB678(PokeathlonCourseData *param0);
s32 ov96_021ED47C(PokeathlonCourseData *data);
s32 ov96_021EB680(PokeathlonCourseData *data);
s32 ov96_021ED48C(PokeathlonCourseData *data);
s32 ov96_021EB688(PokeathlonCourseData *data);
BOOL ov96_021EB690(PokeathlonCourseData *param0);

s32 ov96_021EB698(PokeathlonCourseData *data);
s32 ov96_021EB6A0(PokeathlonCourseData *data);

void ov96_021EB63C(void *arg0, s32 arg1) {
    u8 *v0;
    s32 v1;
    s32 v2;
    s32 flag;

    v2 = 0;
    if ((s32) * (s32 *)((u8 *)arg0 + 8) > 0) {
        v1 = 0;
        if (arg1 == 0) {
            flag = 1;
        } else {
            flag = 0;
        }
        do {
            v0 = *(u8 **)((u8 *)arg0 + 0x154);
            if (*(u16 *)(v0 + v1) != 0) {
                Sprite_SetAnimActiveFlag(ov96_021EB5B8(v0 + v1), flag);
            }
            v2 += 1;
            v1 += 0xC;
        } while (v2 < (s32) * (s32 *)((u8 *)arg0 + 8));
    }
}

BOOL ov96_021EB678(PokeathlonCourseData *param0) {
    return ov96_021ED1D0(param0);
}

s32 ov96_021EB680(PokeathlonCourseData *data) {
    return ov96_021ED47C(data);
}

s32 ov96_021EB688(PokeathlonCourseData *data) {
    return ov96_021ED48C(data);
}

BOOL ov96_021EB690(PokeathlonCourseData *param0) {
    return ov96_021ED1D0(param0);
}

s32 ov96_021EB698(PokeathlonCourseData *data) {
    return ov96_021ED47C(data);
}

s32 ov96_021EB6A0(PokeathlonCourseData *data) {
    return ov96_021ED48C(data);
}
