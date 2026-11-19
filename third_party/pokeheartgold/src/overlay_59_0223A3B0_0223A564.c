#include "global.h"
#include "overlay_59_apricorn_app.h"

s32 ov59_0223A564(s32 arg0, s32 arg1, s32 arg2);

s32 ov59_0223A564(s32 arg0, s32 arg1, s32 arg2) {
    s32 var_r0;
    if (arg2 > 0) {
        var_r0 = arg0;
        if (var_r0 == arg1) {
            return 1;
        }
        var_r0 = var_r0 + arg2;
        if (var_r0 > arg1) {
            return arg1;
        }
    } else {
        if (arg0 == 1) {
            return arg1;
        }
        var_r0 = arg0 + arg2;
        if (var_r0 <= 0) {
            var_r0 = 1;
        }
    }
    return var_r0;
}
