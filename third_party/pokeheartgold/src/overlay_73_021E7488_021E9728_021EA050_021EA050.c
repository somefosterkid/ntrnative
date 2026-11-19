#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "pm_string.h"

s32 ov73_021EA050(Window *param0, String *param1, s32 param2, s32 param3, u32 param4, enum Font param5);

s32 ov73_021EA050(Window *param0, String *param1, s32 param2, s32 param3, u32 param4, enum Font param5) {
    s32 var_r2;
    u32 temp_r1;

    var_r2 = param2;
    switch (param3) {
    case 1:
        temp_r1 = (param0->width * 8) - FontID_String_GetWidth(param5, param1, 0U);
        var_r2 = (s32)(temp_r1 + (temp_r1 >> 0x1F)) >> 1;
        break;
    case 2:
        var_r2 = (param0->width * 8) - FontID_String_GetWidth(param5, param1, 0U);
        break;
    }
    return var_r2;
}
