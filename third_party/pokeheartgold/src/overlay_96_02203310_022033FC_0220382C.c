#include "global.h"

#include "constants/heap.h"

#include "pokeathlon/pokeathlon.h"

#include "bg_window.h"
#include "font.h"
#include "player_data.h"
#include "pm_string.h"
#include "text.h"
#include "overlay_96_ov96_02203468work.h"

void ov96_0220382C(Ov96_02203468Work *arg0, PokeathlonCourseData *arg1);

void ov96_0220382C(Ov96_02203468Work *arg0, PokeathlonCourseData *arg1) {
    PokeathlonCourseData *sp14;
    Ov96_02203468Work *sp10;
    String *temp_r0;
    s32 var_r7;
    s32 var_r4;
    Window *var_r5;
    BOOL new_var;
    sp10 = arg0;
    new_var = ov96_021E5F24(arg1);
    sp14 = arg1;
    var_r7 = 0;
    var_r4 = 1 + ((u8)new_var);
    var_r5 = &sp10->windows[1];
    do {
        temp_r0 = PlayerProfile_GetPlayerName_NewString(PokeathlonCourse_GetPlayerProfileFromData(sp14, (u8)(var_r4 % 4)), sp10->heapID);
        AddTextPrinterParameterizedWithColor(var_r5, FONT_SYSTEM, temp_r0, 0U, 0U, 0xFFU, 0xF0E00U, 0L);
        String_Delete(temp_r0);
        CopyWindowToVram(var_r5);
        var_r4 += 1;
        var_r7 += 1;
        var_r5 += 1;
    } while (var_r7 < 3);
}
