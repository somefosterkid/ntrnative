#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "pm_string.h"

s32 ov75_0224964C(Window *param0, String *param1, s32 param2, s32 param3, u32 param4, enum Font param5);

s32 ov75_0224964C(Window *param0, String *param1, s32 param2, s32 param3, u32 param4, enum Font param5) {
    s32 v1;
    u32 v0;

    v1 = param2;
    switch (param3) {
    case 1:
        v0 = (param0->width * 8) - FontID_String_GetWidth(param5, param1, 0U);
        v1 = (s32)(v0 + (v0 >> 0x1F)) >> 1;
        break;
    case 2:
        v1 = (param0->width * 8) - FontID_String_GetWidth(param5, param1, 0U);
        break;
    }
    return v1;
}
