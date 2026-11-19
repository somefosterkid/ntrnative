#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "bg_window.h"
#include "msgdata.h"

void ov18_021EF388(u8 *param_1, s32 param_2, u32 param_3);

void ov18_021F9648(Window *, MsgData *, u32, u32, u32, u32, u32, u32);

void ov18_021EF91C(PokedexAppData *pokedexApp) {
    u32 uVar1;
    Window *window;
    ov18_021E613C(pokedexApp, 0);
    FillWindowPixelBuffer(pokedexApp->windows + 0x44, 0);
    ov18_021F9648(pokedexApp->windows + 0x44, pokedexApp->msgData, 0x1d, 0x14, 0, 0, 0x20100, 2);
    ov18_021EF388((u8 *)pokedexApp, 0x11, 0x27);
    ov18_021EF388((u8 *)pokedexApp, 0x13, 0x28);
    ov18_021EFD00(pokedexApp, pokedexApp->unk_1850[pokedexApp->dexSearchCriteria[4]].unk_0, 0x45 & 0xFFFF);
    ov18_021EFD00(pokedexApp, pokedexApp->unk_1850[pokedexApp->dexSearchCriteria[5]].unk_0, 0x46);
    uVar1 = 0x44;
    window = pokedexApp->windows + 0x44;
    do {
        ScheduleWindowCopyToVram(window);
        uVar1 = uVar1 + 1;
        window = window + 1;
    } while (uVar1 <= (0x47 - 1));
    return;
}

void ov18_021EF9B4(PokedexAppData *pokedexApp) {
    u32 uVar1;
    Window *window;
    ov18_021E613C(pokedexApp, 0);
    FillWindowPixelBuffer(pokedexApp->windows + 0x47, 0);
    ov18_021F9648(pokedexApp->windows + 0x47, pokedexApp->msgData, 0x1e, 0x18, 0, 0, 0x20100, 2);
    ov18_021EF388((u8 *)pokedexApp, 0x11, 0x27);
    ov18_021EF388((u8 *)pokedexApp, 0x13, 0x28);
    ov18_021EFDB4(pokedexApp, pokedexApp->unk_1850[pokedexApp->dexSearchCriteria[6]].unk_2, 0x48);
    ov18_021EFDB4(pokedexApp, pokedexApp->unk_1850[pokedexApp->dexSearchCriteria[7]].unk_2, 0x49);
    uVar1 = 0x47;
    window = pokedexApp->windows + 0x47;
    do {
        ScheduleWindowCopyToVram(window);
        uVar1 = uVar1 + 1;
        window = window + 1;
    } while (uVar1 <= (0x4a - 1));
    return;
}

void ov18_021EFA50(PokedexAppData *pokedexApp) {
    u32 uVar1;
    Window *window;
    ov18_021E613C(pokedexApp, 0);
    FillWindowPixelBuffer(pokedexApp->windows + 0x4a, 0);
    FillWindowPixelBuffer(pokedexApp->windows + 0x4c, 0);
    FillWindowPixelBuffer(pokedexApp->windows + 0x4d, 0);
    FillWindowPixelBuffer(pokedexApp->windows + 0x4e, 0);
    FillWindowPixelBuffer(pokedexApp->windows + 0x4f, 0);
    ov18_021F9648(pokedexApp->windows + 0x4a, pokedexApp->msgData, 0x1f, 0x18, 0, 0, 0x20100, 2);
    ov18_021F9648(pokedexApp->windows + 0x4c, pokedexApp->msgData, 0x41, 0x1c, 0, 4, 0x20100, 2);
    if (pokedexApp->unk_1860 == 1) {
        ov18_021F9648(pokedexApp->windows + 0x4d, pokedexApp->msgData, 0x42, 0x1c, 0, 4, 0x20100, 2);
    }
    ov18_021F9648(pokedexApp->windows + 0x4e, pokedexApp->msgData, 0x43, 0x1c, 0, 4, 0x20100, 2);
    ov18_021F9648(pokedexApp->windows + 0x4f, pokedexApp->msgData, 0x44, 0x1c, 0, 4, 0x20100, 2);
    ov18_021EF388((u8 *)pokedexApp, 0x11, 0x27);
    ov18_021EF388((u8 *)pokedexApp, 0x13, 0x28);
    ov18_021EFE70(pokedexApp, 0x4b);
    uVar1 = 0x4a;
    window = pokedexApp->windows + 0x4a;
    do {
        ScheduleWindowCopyToVram(window);
        uVar1 = uVar1 + 1;
        window = window + 1;
    } while (uVar1 <= (0x50 - 1));
    return;
}
