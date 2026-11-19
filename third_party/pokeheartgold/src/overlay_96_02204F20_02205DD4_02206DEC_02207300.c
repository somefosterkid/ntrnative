#include "global.h"

#include "overlay_96_ov96hurdle_work.h"

void ov96_02206F1C(Ov96HurdleWork *, s32, s32, Ov96HurdlePoint **, u8 *);

s32 ov96_02206FA4(s32, Ov96HurdlePoint *, s32 *);

s32 ov96_0220703C(void *, void *, s32, s32 *, s32 *);

s32 ov96_02207300(Ov96HurdleWork *arg0, s32 arg1, s32 arg2, void *arg3, void *arg4, s32 *arg5);

s32 ov96_02207300(Ov96HurdleWork *arg0, s32 arg1, s32 arg2, void *arg3, void *arg4, s32 *arg5) {
    Ov96HurdlePoint *list[20];
    s32 buf[24];
    s32 pos[3];
    u8 count;
    u8 result;
    s32 i;
    s32 r;
    Ov96HurdlePoint **p;
    ov96_02206F1C(arg0, ((s32 *)arg3)[0], ((s32 *)arg3)[1], list, &count);
    i = 0;
    result = 0;
    if ((s32)count > 0) {
        p = list;
        do {
            ov96_02206FA4(arg2, *p, buf);
            r = ov96_0220703C(arg3, arg4, arg2, buf, pos);
            if (r != 0) {
                if (r >= 9) {
                    arg5[0] = pos[0];
                    arg5[1] = pos[1];
                    arg5[2] = 0;
                } else if (r != arg1) {
                    arg5[0] = pos[0];
                    arg5[1] = pos[1];
                    arg5[2] = 0;
                }
                result = (u8)r;
                break;
            }
            i++;
            p++;
        } while (i < (s32)count);
    }
    return result;
}
