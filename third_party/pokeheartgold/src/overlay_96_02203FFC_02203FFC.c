#include "global.h"

#include "constants/heap.h"

#include "pokeathlon/pokeathlon.h"

#include "bg_window.h"
#include "overlay_96_ov96_pic_panel.h"
#include "player_data.h"
#include "pm_string.h"
#include "text.h"

void ov96_02203FFC(Ov96PicPanel *panel) {
    u8 uVar1;
    PlayerProfile *profile;
    String *string;
    uVar1 = 0xff;
    uVar1 = ov96_021E5F24(panel->course);
    profile = PokeathlonCourse_GetPlayerProfileFromData(panel->course, uVar1 & uVar1);
    string = PlayerProfile_GetPlayerName_NewString(profile, panel->heapID);
    AddTextPrinterParameterizedWithColor(&panel->window, FONT_SYSTEM, string, 0, 0, 0xff, 0xf0e00, (PrinterCallback_t)0x0);
    String_Delete(string);
    CopyWindowToVram(&panel->window);
    return;
}
