#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "font.h"
#include "player_data.h"
#include "pm_string.h"
#include "text.h"
#include "unk_02034354.h"
#include "unk_02035900.h"

typedef enum Font Font;

void ov81_0224093C(void *param_1, Window *param_2, u32 param_3, u32 param_4, u8 param_5);

void ov81_0224093C(void *param_1, Window *param_2, u32 param_3, u32 param_4, u8 param_5) {
    u16 uVar1;
    PlayerProfile *profile;
    String *str;
    u32 uVar2;

    uVar1 = sub_0203769C();
    profile = sub_02034818(1 - uVar1);
    str = String_New(8, HEAP_ID_100);
    FillWindowPixelBuffer(param_2, 0);
    PlayerName_FlatToString(profile, str);
    uVar2 = PlayerProfile_GetTrainerGender(profile);
    if (uVar2 == 0) {
        uVar2 = 0x70800;
    } else {
        uVar2 = 0x30400;
    }
    AddTextPrinterParameterizedWithColor((struct Window *)((param_2)), (Font)((u32)param_5), (struct String *)((str)), param_3, param_4, 0xff, uVar2, (PrinterCallback_t)0x0);
    String_Delete(str);
    ScheduleWindowCopyToVram(param_2);
    return;
}
