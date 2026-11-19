#include "global.h"

#include "constants/heap.h"

#include "application/pokedex/pokedex_internal.h"

#include "bg_window.h"
#include "font.h"
#include "pm_string.h"

void ov18_021F95FC(void *, String *, s32, u32, u32, u32, u32);

void ov18_021EE984(PokedexAppData *param_1, u16 param_2, s32 param_3, s32 param_4);

void ov18_021EE984(PokedexAppData *param_1, u16 param_2, s32 param_3, s32 param_4) {
    u32 bVar1;
    String *string;
    Window *iVar4;
    u32 new_var;
    if (param_1->unk_1030[param_3].unk_2 == 2) {
        iVar4 = param_1->windows;
        FillWindowPixelBuffer(&iVar4[param_4], 0);
        string = ov18_021E59A8(param_2, (u32)(param_1->unk_185C), 0, HEAP_ID_POKEDEX_APP);
        bVar1 = GetWindowWidth(&iVar4[param_4]);
        new_var = (((u32)bVar1) * 8 - FontID_String_GetWidthMultiline(FONT_SYSTEM, string, 0)) >> 1;
        ov18_021F95FC(&iVar4[param_4], string, new_var, 0, 0, 0x20100, 0);
        String_Delete(string);
        ScheduleWindowCopyToVram(&iVar4[param_4]);
    }
    return;
}
