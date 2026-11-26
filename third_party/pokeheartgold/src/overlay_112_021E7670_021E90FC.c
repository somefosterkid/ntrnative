#include "global.h"

#include "font.h"
#include "player_data.h"
#include "pm_string.h"
#include "text.h"
#include "overlay_112_pokewalker_app.h"

void ov112_021E90FC(Window *window, Ov112PokewalkerApp *arg1) {
    String *string;

    string = PlayerProfile_GetPlayerName_NewString(Save_PlayerData_GetProfile(arg1->saveData), HEAP_ID_154);
    FontID_Alloc(FONT_UNK_5, HEAP_ID_154);
    AddTextPrinterParameterizedWithColor(window, FONT_UNK_5, string, 2U, 0U, 0xFFU, 0x30100U, NULL);
    FontID_Release(FONT_UNK_5);
    String_Delete(string);
}
