#include "global.h"
struct BattleCastleOpponentApp;

#include "constants/heap.h"

#include "bg_window.h"
#include "font.h"
#include "player_data.h"
#include "pm_string.h"
#include "save.h"
#include "text.h"
#include "overlay_83_battle_castle_opponent_app.h"

typedef enum Font Font;

void ov83_02244AD8(struct BattleCastleOpponentApp *param_1, Window *param_2, u32 param_3, u32 param_4, u8 param_5);

void ov83_02244AD8(BattleCastleOpponentApp *param_1, Window *param_2, u32 param_3, u32 param_4, u8 param_5) {
    PlayerProfile *profile;
    String *string;
    u16 *src;
    u32 uVar1;

    profile = Save_PlayerData_GetProfile(param_1->saveData);
    string = String_New(8, HEAP_ID_107);
    src = (unsigned short *)(PlayerProfile_GetNamePtr(profile));
    CopyU16ArrayToString(string, src);
    uVar1 = PlayerProfile_GetTrainerGender(profile);
    if (uVar1 == 0) {
        uVar1 = 0x70800;
    } else {
        uVar1 = 0x30400;
    }
    AddTextPrinterParameterizedWithColor((struct Window *)((param_2)), (Font)((u32)param_5), string, param_3, param_4, (Font)(0xff), uVar1, (PrinterCallback_t)0x0);
    String_Delete(string);
    return;
}
