#include "global.h"
struct Window;

#include "font.h"

s32 ov39_02228964(struct Window *arg0, String *arg1, s32 arg2, s32 arg3, s32 arg4, enum Font arg5);

s32 ov39_02228964(Window *arg0, String *arg1, s32 arg2, s32 arg3, s32 arg4, enum Font arg5) {
    s32 var_r2;
    u32 temp_r1;

    var_r2 = arg2;
    switch (arg3) {
    case 1:
        temp_r1 = (arg0->width * 8) - FontID_String_GetWidth(arg5, arg1, 0U);
        var_r2 = (s32)(temp_r1 + (temp_r1 >> 0x1F)) >> 1;
        break;
    case 2:
        var_r2 = (arg0->width * 8) - FontID_String_GetWidth(arg5, arg1, 0U);
        break;
    }
    return var_r2;
}
