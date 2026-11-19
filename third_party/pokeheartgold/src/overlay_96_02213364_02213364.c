#include "global.h"

#include "error_handling.h"

#include "overlay_96_app_02210C98.h"

s32 ov96_021E6104(void *);

s32 ov96_021E872C(s32, s32, s32, s32, s32, s32 *);

u8 ov96_02213364(Ov96App_02210C98 *arg0, s16 arg1, s16 arg2, s16 arg3);

u8 ov96_02213364(Ov96App_02210C98 *arg0, s16 arg1, s16 arg2, s16 arg3) {
    s32 dist[3];
    u8 valid[4];
    s32 out;
    s32 r;
    s32 min;
    s32 d;
    Ov96App_02210C98_Unit *base;
    Ov96App_02210C98_Unit *p;
    u8 i;
    u8 cnt;
    u8 ret;

    r = ov96_021E6104(arg0);
    cnt = 0;
    min = r << 0xC;
    i = 0;
    base = arg0->unk_5C[arg1];
    do {
        p = &base[i];
        if (ov96_021E872C(p->unk_30.x / 0x1000, p->unk_30.y / 0x1000, arg2, arg3, r, &out) != 0) {
            dist[i] = out;
            valid[i] = 1;
            cnt++;
        } else {
            dist[i] = 0;
            valid[i] = 0;
        }
        i++;
    } while (i < 3);
    if (cnt == 0) {
        return 0xC;
    }
    ret = 3;
    i = 0;
    do {
        if (valid[i] != 0) {
            d = dist[i];
            if (d < min) {
                ret = i;
                min = d;
            }
        }
        i++;
    } while (i < 3);
    if (ret == 3) {
        GF_AssertFail();
        ret = 0xC;
    }
    return ret;
}
