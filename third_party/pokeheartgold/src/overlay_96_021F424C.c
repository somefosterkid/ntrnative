#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "bg_window.h"
#include "player_data.h"
#include "pm_string.h"
#include "sprite.h"
#include "text.h"
#include "overlay_96_app_021f424c.h"

Sprite *ov96_021EB5B8(s32);

s32 ov96_021F459C(void *, s32);

void ov96_021F424C(void *arg0);

void ov96_021F4364(void *arg0, s32 arg1);

void ov96_021F424C(void *arg0) {
    void *v0;
    s32 index;
    String *string;
    String *v2;
    s32 v4;
    u8 v1;
    u8 v3;
    u8 v5;
    u8 v6;

    v3 = 0;
    do {
        FillWindowPixelBuffer(arg0 + 0xC + (v3 * 0x10), 0U);
        v3 += 1;
    } while ((u32)v3 < 5U);
    v1 = (u8)ov96_021E5F24(((Ov96App_021F424C *)arg0)->unk_4);
    index = (s32)v1;
    string = PlayerProfile_GetPlayerName_NewString(PokeathlonCourse_GetPlayerProfileFromData(((Ov96App_021F424C *)arg0)->unk_4, index), *(enum HeapID *)arg0);
    AddTextPrinterParameterizedWithColor(arg0 + 0xC, FONT_SYSTEM, string, 0U, 0U, 0xFFU, 0xF0E00U, NULL);
    String_Delete(string);
    Sprite_SetAnimCtrlSeq(ov96_021EB5B8(((Ov96App_021F424C *)arg0)->unk_130), index + 0xE);
    v4 = 0;
    *(s8 *)((u8 *)(arg0 + v1) + 0x170) = 0;
    v0 = arg0;
    v6 = 1;
    v0 = arg0 + 0xC;
    do {
        if (v4 != index) {
            v2 = PlayerProfile_GetPlayerName_NewString(PokeathlonCourse_GetPlayerProfileFromData(((Ov96App_021F424C *)arg0)->unk_4, v4), *(enum HeapID *)arg0);
            AddTextPrinterParameterizedWithColor(v0 + (v6 * 0x10), FONT_SYSTEM, v2, 0U, 0U, 0xFFU, 0xF0E00U, NULL);
            String_Delete(v2);
            Sprite_SetAnimCtrlSeq(ov96_021EB5B8(*(s32 *)((u8 *)(arg0 + (v6 * 4)) + 0x130)), v4 + 0xE);
            *(u8 *)((u8 *)(arg0 + v4) + 0x170) = v6;
            v6 += 1;
        }
        v4 = (s32)(u8)(v4 + 1);
    } while ((u32)v4 < 4U);
    ov96_021F459C(arg0, 1);
    v5 = 0;
    do {
        CopyWindowToVram(arg0 + 0xC + (v5 * 0x10));
        v5 += 1;
    } while ((u32)v5 < 5U);
}

void ov96_021F4364(void *arg0, s32 seq) {
    u8 *new_var;
    if ((((Ov96App_021F424C *)arg0)->unk_64) != seq) {
        ((Ov96App_021F424C *)arg0)->unk_64 = seq;
        ov96_021F459C(arg0, seq);
        Sprite_SetAnimCtrlSeq(ov96_021EB5B8(*((s32 *)((new_var = (u8 *)arg0) + 0x120))), seq);
        CopyWindowToVram(arg0 - (-0x4C));
    }
}
