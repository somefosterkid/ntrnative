#include "global.h"
#include "overlay_96_ov96_sprite_mgr.h"

extern u32 ov96_0221BFF4[];

void ov96_021F3E60(void *param_1);

void ov96_021F3E60(void *param_1) {
    u32 *p;
    s32 i;
    u32 arr[2];

    arr[0] = ov96_0221BFF4[2];
    arr[1] = ov96_0221BFF4[3];
    p = arr;
    for (i = 0; i < 2; i++) {
        ov96_021EB2BC(param_1, 0x5d, 9, *p, 2);
        ov96_021EB2F4(param_1, 0x5d, 6, *p, 2, 1);
        ov96_021EB334(param_1, 0x5d, 10, *p);
        ov96_021EB36C(param_1, 0x5d, 10, *p);
        p++;
    }
}
