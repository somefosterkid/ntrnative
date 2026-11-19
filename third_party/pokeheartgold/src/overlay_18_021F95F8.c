#include "global.h"

#include "font.h"
#include "text.h"

#include "overlay_18.h"
#include "overlay_18_pokedex_entry_popup.h"

void ov18_021F95FC(Window *arg0, String *arg1, u32 arg2, u32 arg3, enum Font arg4, u32 arg5, s32 arg6);

Pokepic *ov18_021F95F8(PokedexEntryPopup *popup) {
    return popup->pokepic;
}

void ov18_021F95FC(Window *window, String *string, u32 arg2, u32 y, enum Font fontId, u32 color, s32 arg6) {
    u32 x;

    x = arg2;
    if (arg6 == 1) {
        x -= FontID_String_GetWidth(fontId, string, 0U);
    } else if (arg6 == 2) {
        x -= FontID_String_GetWidth(fontId, string, 0U) >> 1;
    }
    AddTextPrinterParameterizedWithColor(window, fontId, string, x, y, 0xFFU, color, NULL);
}
