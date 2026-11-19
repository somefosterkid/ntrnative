#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "bg_window.h"
#include "msgdata.h"

void ov18_021EF1E4(void *param_1, s32 param_2);

void ov18_021EF220(void *param_1, s32 param_2);

void ov18_021EF25C(void *param_1, s32 param_2);

void ov18_021EF298(void *param_1, s32 param_2);

void ov18_021EF2D4(void *param_1, s32 param_2);

void ov18_021EF310(void *param_1, s32 param_2);

void ov18_021EF34C(void *param_1, s32 param_2);

void ov18_021EF388(u8 *param_1, s32 param_2, u32 param_3);

void ov18_021EF45C(PokedexAppData *pokedexApp) {
    u32 uVar1;
    void *new_var;
    Window *window;
    new_var = (u8 *)pokedexApp;
    ov18_021E613C(pokedexApp, 0);
    FillWindowPixelBuffer(pokedexApp->windows + 0x14, 0);
    FillWindowPixelBuffer(pokedexApp->windows + 0x15, 0);
    FillWindowPixelBuffer(pokedexApp->windows + 0x16, 0);
    FillWindowPixelBuffer(pokedexApp->windows + 0x17, 0);
    FillWindowPixelBuffer(pokedexApp->windows + 0x18, 0);
    FillWindowPixelBuffer(pokedexApp->windows + 0x19, 0);
    FillWindowPixelBuffer(pokedexApp->windows + 0x1a, 0);
    ov18_021EF1E4(new_var, 0x14);
    ov18_021EF220((u8 *)pokedexApp, 0x15);
    ov18_021EF25C((u8 *)pokedexApp, 0x16);
    ov18_021EF298((u8 *)pokedexApp, 0x17);
    ov18_021EF2D4((u8 *)pokedexApp, 0x18);
    ov18_021EF310(new_var, 0x19);
    ov18_021EF34C((u8 *)pokedexApp, 0x1a);
    ov18_021EF388(new_var, 0x11, 0x23);
    ov18_021EF388(new_var, 0x12, 0x24);
    ov18_021EF388(new_var, 0x13, 0x25);
    uVar1 = 0x14;
    window = pokedexApp->windows + 0x14;
    do {
        ScheduleWindowCopyToVram(window);
        uVar1 = uVar1 + 1;
        window = window + 1;
    } while (uVar1 <= (0x1b - 1));
    return;
}

void ov18_021EF528(PokedexAppData *pokedexApp) {
    u32 uVar1;
    Window *window;
    ov18_021EFBE8(pokedexApp, 0x1b);
    ov18_021EFC3C(pokedexApp, 0x1c);
    ov18_021EFC9C(pokedexApp, pokedexApp->dexSearchCriteria[2], 0x1d, 0x1d);
    ov18_021EFC9C(pokedexApp, pokedexApp->dexSearchCriteria[3], 0x1e, 0x1d);
    ov18_021EFD00(pokedexApp, pokedexApp->unk_1850[pokedexApp->dexSearchCriteria[4]].unk_0, 0x1f);
    ov18_021EFD00(pokedexApp, pokedexApp->unk_1850[pokedexApp->dexSearchCriteria[5]].unk_0, 0x20);
    ov18_021EFDB4(pokedexApp, pokedexApp->unk_1850[pokedexApp->dexSearchCriteria[6]].unk_2, 0x21);
    ov18_021EFDB4(pokedexApp, pokedexApp->unk_1850[pokedexApp->dexSearchCriteria[7]].unk_2, 0x22);
    ov18_021EFE70(pokedexApp, 0x23);
    uVar1 = 0x1b;
    window = pokedexApp->windows + 0x1b;
    do {
        ScheduleWindowCopyToVram(window);
        uVar1 = uVar1 + 1;
        window = window + 1;
    } while (uVar1 <= (0x24 - 1));
    return;
}

void ov18_021F9648(Window *, MsgData *, u32, u32, u32, u32, u32, u32);

void ov18_021EF5D8(PokedexAppData *pokedexApp) {
    u32 uVar1;
    Window *window;
    ov18_021E613C(pokedexApp, 0);
    FillWindowPixelBuffer(pokedexApp->windows + 0x24, 0);
    FillWindowPixelBuffer(pokedexApp->windows + 0x26, 0);
    FillWindowPixelBuffer(pokedexApp->windows + 0x27, 0);
    FillWindowPixelBuffer(pokedexApp->windows + 0x28, 0);
    FillWindowPixelBuffer(pokedexApp->windows + 0x29, 0);
    FillWindowPixelBuffer(pokedexApp->windows + 0x2a, 0);
    FillWindowPixelBuffer(pokedexApp->windows + 0x2b, 0);
    ov18_021F9648(pokedexApp->windows + 0x24, pokedexApp->msgData, 0x1a, 0x2c, 0, 0, 0x20100, 2);
    ov18_021F9648(pokedexApp->windows + 0x26, pokedexApp->msgData, 0x29, 0x2c, 0, 4, 0x20100, 2);
    ov18_021F9648(pokedexApp->windows + 0x27, pokedexApp->msgData, 0x2a, 0x2c, 0, 4, 0x20100, 2);
    ov18_021F9648(pokedexApp->windows + 0x28, pokedexApp->msgData, 0x2b, 0x2c, 0, 4, 0x20100, 2);
    ov18_021F9648(pokedexApp->windows + 0x29, pokedexApp->msgData, 0x2c, 0x2c, 0, 4, 0x20100, 2);
    ov18_021F9648(pokedexApp->windows + 0x2a, pokedexApp->msgData, 0x2d, 0x2c, 0, 4, 0x20100, 2);
    ov18_021F9648(pokedexApp->windows + 0x2b, pokedexApp->msgData, 0x2e, 0x2c, 0, 4, 0x20100, 2);
    ov18_021EF388((u8 *)pokedexApp, 0x11, 0x27);
    ov18_021EF388((u8 *)pokedexApp, 0x13, 0x28);
    ov18_021EFBE8(pokedexApp, 0x25);
    uVar1 = 0x24;
    window = pokedexApp->windows + 0x24;
    do {
        ScheduleWindowCopyToVram(window);
        uVar1 = uVar1 + 1;
        window = window + 1;
    } while (uVar1 <= (0x2c - 1));
    return;
}
