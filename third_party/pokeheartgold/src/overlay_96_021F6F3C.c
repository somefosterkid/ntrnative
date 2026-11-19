#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "bg_window.h"
#include "player_data.h"
#include "pm_string.h"
#include "sprite.h"
#include "text.h"
#include "overlay_96_unk_struct_ov96_021_f5018.h"

void ov96_021F6F3C(UnkStruct_ov96_021F5018 *arg0, s32 arg1);

void ov96_021F6F3C(UnkStruct_ov96_021F5018 *arg0, s32 arg1) {
    u8 v0;

    v0 = (u8)(arg1 / 30);
    Sprite_SetAnimCtrlSeq(arg0->unk_84[0], (u8)((s32)v0 / 10) + 1);
    Sprite_SetAnimCtrlSeq(arg0->unk_84[1], (u8)((s32)v0 % 10) + 1);
}

void ov96_021F6F80(PokeathlonCourseData *arg0, UnkStruct_ov96_021F5018 *arg1);

void ov96_021F6F80(PokeathlonCourseData *arg0, UnkStruct_ov96_021F5018 *arg1) {
    Window *v0;
    s32 index;
    PokeathlonCourseData *data;
    String *string;
    String *v1;
    u8 v3;
    s32 v5;
    u8 v2;
    u8 v4;
    v2 = 0;
    data = arg0;
    do {
        FillWindowPixelBuffer(&arg1->windows[v2], 0U);
        v2 += 1;
    } while (((u32)v2) < 5U);
    index = (s32)((u8)ov96_021E5F24(data));
    string = PlayerProfile_GetPlayerName_NewString(PokeathlonCourse_GetPlayerProfileFromData(data, index), arg1->heapID);
    AddTextPrinterParameterizedWithColor(&arg1->windows[0], FONT_SYSTEM, string, 0U, 0U, 0xFFU, 0xF0E00U, 0L);
    String_Delete(string);
    v3 = 1;
    v5 = 0;
    v0 = arg1->windows;
    do {
        if (v5 != index) {
            v1 = PlayerProfile_GetPlayerName_NewString(PokeathlonCourse_GetPlayerProfileFromData(data, v5), arg1->heapID);
            AddTextPrinterParameterizedWithColor(v0 + v3, FONT_SYSTEM, v1, 0U, 0U, 0xFFU, 0xF0E00U, 0L);
            String_Delete(v1);
            v0 = arg1->windows;
            v3 += 1;
        }
        v5 = (s32)((u8)(v5 + 1));
    } while (((u32)v5) < 4U);
    v4 = 0;
    do {
        CopyWindowToVram(&arg1->windows[v4]);
        v4 += 1;
    } while (((u32)v4) < 5U);
}
