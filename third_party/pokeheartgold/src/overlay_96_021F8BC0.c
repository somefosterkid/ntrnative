#include "global.h"

#include "constants/heap.h"

#include "pokeathlon/pokeathlon.h"

#include "bg_window.h"
#include "player_data.h"
#include "pm_string.h"
#include "sprite.h"
#include "unk_02013534.h"

void ov96_021F8DF4(enum HeapID *param_1, Window *param_2, String *param_3, u32 param_4, s32 param_5, s32 param_6, Sprite *param_7, u32 *param_8);

void ov96_021F8BC0(enum HeapID *param_1, s32 param_2, u32 *param_3);

void ov96_021F8BC0(enum HeapID *param_1, s32 param_2, u32 *param_3) {
    PlayerProfile *profile;
    String *string;

    profile = PokeathlonCourse_GetPlayerProfileFromData((PokeathlonCourseData *)param_1[1], param_2);
    string = PlayerProfile_GetPlayerName_NewString(profile, *param_1);
    ov96_021F8DF4(param_1, (Window *)(param_1 + 0xbf), string, 0, -0x38, -0x10, (Sprite *)*param_3, param_3 + 4);
    String_Delete(string);
    return;
}

void ov96_021F8C04(u8 *param_1, String *param_2, u32 *param_3);

void ov96_021F8C04(u8 *param_1, String *param_2, u32 *param_3) {
    ov96_021F8DF4((enum HeapID *)param_1, (Window *)(param_1 + 0x314), param_2, 0, -0x38, 0, (Sprite *)*param_3, param_3 + 8);
    return;
}

void ov96_021F8C88(s32 param_1, s32 param_2, u32 *param_3);

void ov96_021F8C2C(u32 *param_1);

void ov96_021F8C2C(u32 *param_1) {
    ov96_021F8C88(param_1[0xe], param_1[0xf], param_1);
    Sprite_SetPaletteOverride((Sprite *)*param_1, param_1[0x10] + 2);
    Sprite_SetAnimCtrlSeq((Sprite *)param_1[1], *(u16 *)(param_1 + 0x11) + 5);
    return;
}

void ov96_021F8C54(s32 param_1, BOOL param_2, u32 *param_3);

void ov96_021F8C54(s32 param_1, BOOL param_2, u32 *param_3) {
    Sprite_SetDrawFlag((Sprite *)*param_3, param_2);
    Sprite_SetDrawFlag((Sprite *)param_3[1], param_2);
    if (param_1 != 0) {
        Sprite_SetDrawFlag((Sprite *)param_3[2], param_2);
    }
    TextOBJ_SetSpritesDrawFlag((TextOBJ *)param_3[4], param_2);
    TextOBJ_SetSpritesDrawFlag((TextOBJ *)param_3[8], param_2);
    return;
}
