#include "global.h"

#include "bg_window.h"

s32 ov83_022477E4(u32 *);

void ov83_0224780C(u32 *arg0);

void ov83_0224780C(u32 *arg0) {
    u16 new_var[2];
    new_var[1] = 0;
    do {
        new_var[0] = (((u32)(*arg0)) >> (new_var[1] * 4)) & 0xF;
        if (new_var[0] != 0) {
            ToggleBgLayer(new_var[1], (new_var[0] - 1));
        }
        new_var[1] = (new_var[1] + 1);
    } while (((u32)new_var[1]) <= 7U);
    ov83_022477E4(arg0);
}
