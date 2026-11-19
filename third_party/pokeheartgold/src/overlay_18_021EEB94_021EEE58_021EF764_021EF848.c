#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "bg_window.h"
#include "msgdata.h"

void ov18_021EF388(u8 *param_1, s32 param_2, u32 param_3);

void ov18_021F9648(Window *, MsgData *, s32, u32, u32, u32, u32, u32);

extern u8 ov18_021F9DE4[];

void ov18_021EF848(PokedexAppData *pokedexApp) {
    Window *pWVar1;
    void *iVar2;
    u32 uVar3;
    ov18_021E613C(pokedexApp, 0);
    FillWindowPixelBuffer(pokedexApp->windows + 0x2c, 0);
    ov18_021F9648(pokedexApp->windows + 0x2c, pokedexApp->msgData, 0x1c, 0x1c, 0, 0, 0x20100, 2);
    iVar2 = ((ov18_021F9DE4 + 0x3a));
    uVar3 = 0x2f;
    pWVar1 = pokedexApp->windows + 0x2f;
    do {
        FillWindowPixelBuffer(pWVar1, 0);
        ov18_021F9648(pWVar1, pokedexApp->msgData, *((u16 *)(iVar2 + (-0x5e))), 0x20, 0, 4, 0x20100, 2);
        pWVar1 = pWVar1 + 1;
        iVar2 = iVar2 + 2;
        uVar3 = uVar3 + 1;
    } while (uVar3 <= (0x41 - 1));
    ov18_021EF388((u8 *)pokedexApp, 0x11, 0x27);
    ov18_021EF388((u8 *)pokedexApp, 0x13, 0x28);
    ov18_021EFC9C(pokedexApp, pokedexApp->dexSearchCriteria[2], 0x2d, 0x1d);
    ov18_021EFC9C(pokedexApp, pokedexApp->dexSearchCriteria[3], 0x2e, 0x23);
    uVar3 = 0x2c;
    pWVar1 = pokedexApp->windows + 0x2c;
    do {
        ScheduleWindowCopyToVram(pWVar1);
        uVar3 = uVar3 + 1;
        pWVar1 = pWVar1 + 1;
    } while (uVar3 <= 0x40);
    return;
}
