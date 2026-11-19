#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "font.h"
#include "overlay_92.h"
#include "pm_string.h"

int ov92_0225E188(Window *param0, String *param1) {
    int v0 = FontID_String_GetWidth(FONT_SYSTEM, param1, 0);
    int v1 = (GetWindowWidth(param0) * 8 - v0) / 2;

    return v1;
}
