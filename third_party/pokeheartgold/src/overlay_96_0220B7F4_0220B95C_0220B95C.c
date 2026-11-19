#include "global.h"

s32 ov96_0220C578(void *, s32);

typedef struct UnkStruct_ov96_0220B95C {
    u32 a : 16;
    u32 cnt : 8;
    u32 b : 8;
} UnkStruct_ov96_0220B95C;

void ov96_0220B95C(void *arg0);

void ov96_0220B95C(void *arg0) {
    UnkStruct_ov96_0220B95C *p = (UnkStruct_ov96_0220B95C *)((u8 *)arg0 + 0x44);
    p->cnt++;
    if (p->cnt >= 0xA) {
        ov96_0220C578(arg0, 0);
    }
}
