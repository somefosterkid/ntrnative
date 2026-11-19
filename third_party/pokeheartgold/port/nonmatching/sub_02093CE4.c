#include "global.h"
s32 sub_02093E7C(void *, u8);
s32 sub_02093F84(void *, u8, u8);
s32 sub_02094F5C(void *, s32);
s32 sub_020956B8(void *);

typedef struct HitRect_02093CE4 {
    u8 left;
    u8 top;
    u8 right;
    u8 bottom;
    s32 flag;
} HitRect_02093CE4;

#define F_02093CE4(p, T, off) (*(T *)((u8 *)(p) + (off)))

s32 sub_02093CE4(void *arg0) {
    HitRect_02093CE4 *rects;
    u32 x, y;
    s32 i, n;

    switch (F_02093CE4(arg0, s32, 0x469C)) {
    case 1:
        n = F_02093CE4(arg0, u8, 0xD);
        rects = F_02093CE4(arg0, HitRect_02093CE4 *, 0x46A4);
        for (i = 0; i < n; i++) {
            x = F_02093CE4(arg0, u32, 0x4698);
            y = F_02093CE4(arg0, u32, 0x4694);
            if (y - rects[i].top < (u32)(rects[i].bottom - rects[i].top) && x - rects[i].left < (u32)(rects[i].right - rects[i].left)) {
                if (rects[i].flag != 0) {
                    sub_02094F5C(arg0, i);
                }
                sub_02093E7C(arg0, (u8)i);
                return 1;
            }
        }
        break;
    case 2:
        x = F_02093CE4(arg0, u32, 0x4698);
        y = F_02093CE4(arg0, u32, 0x4694);
        if (y - F_02093CE4(arg0, u8, 0x46A9) < (u32)(F_02093CE4(arg0, u8, 0x46AB) - F_02093CE4(arg0, u8, 0x46A9))
            && x - F_02093CE4(arg0, u8, 0x46A8) < (u32)(F_02093CE4(arg0, u8, 0x46AA) - F_02093CE4(arg0, u8, 0x46A8))) {
            sub_02094F5C(arg0, F_02093CE4(arg0, s32, 0x4684));
            sub_020956B8(arg0);
            return 2;
        }
        n = F_02093CE4(arg0, u8, 0xD);
        rects = F_02093CE4(arg0, HitRect_02093CE4 *, 0x46A4);
        for (i = 0; i < n; i++) {
            if (rects[i].flag == 0 && y - rects[i].top < (u32)(rects[i].bottom - rects[i].top) && x - rects[i].left < (u32)(rects[i].right - rects[i].left)) {
                sub_02093F84(arg0, (u8)F_02093CE4(arg0, s32, 0x4684), (u8)i);
                return 3;
            }
        }
        break;
    default:
        GF_AssertFail();
        break;
    }
    return 0;
}
