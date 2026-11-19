#include "global.h"
struct Ov112PokewalkerApp;

s32 ov112_021EA670(void *, s32);

s32 ov112_021EA688(void *, s32);

s32 ov112_021EC7B8(struct Ov112PokewalkerApp *arg0);

void ov112_021ECA18(void *arg0, s32 arg1);

void ov112_021ECA18(void *arg0, s32 arg1) {
    s32 v0;

    v0 = ov112_021EC7B8(arg0);
    if ((u32) * (u16 *)((u8 *)arg0 + 0x1EC76) > 5U) {
        if (arg1 == 0) {
            ov112_021EA688(arg0, 3);
            ov112_021EA670(arg0, 4);
            return;
        }
        if ((arg1 > 0) && (arg1 < (s32)(v0 - 1))) {
            ov112_021EA670(arg0, 3);
            ov112_021EA670(arg0, 4);
            return;
        }
        if (arg1 == (v0 - 1)) {
            ov112_021EA670(arg0, 3);
            ov112_021EA688(arg0, 4);
        }
    } else {
        ov112_021EA688(arg0, 3);
        ov112_021EA688(arg0, 4);
    }
}

s8 ov112_021ECA88(void *arg0);

s8 ov112_021ECA88(void *arg0) {
    return *(s8 *)((u8 *)(arg0 + (*(s32 *)((u8 *)arg0 + 0x1EC50) + (*(s32 *)((u8 *)arg0 + 0x1EC54) * 5))) + 0x1EC58);
}
