#include "global.h"

#include "constants/heap.h"

#include "application/pokedex/pokedex_internal.h"

#include "bg_window.h"
#include "msgdata.h"
#include "player_data.h"
#include "pm_string.h"
#include "pokedex.h"

void ov18_021EE35C(PokedexAppData *, u8 *, u32);

void ov18_021EEA84(PokedexAppData *, s32, u32, u32, u32, u32, u32, u32);

void ov18_021EEB34(PokedexAppData *, s32, u32, u32, u32, u32, u32, u32);

void ov18_021F95FC(Window *, String *, u32, u32, u32, u32, u32);

void ov18_021F9648(Window *, MsgData *, u32, u32, u32, u32, u32, u32);

extern void *ov18_021F9DE4;

void ov18_021F05E8(PokedexAppData *pokedexApp) {
    BOOL BVar1;
    String *pSVar2;
    u32 uVar3;
    Window *pWVar4;
    u32 uVar5;
    u32 uVar6;

    ov18_021EE35C((struct PokedexAppData *)((unsigned char *)(pokedexApp)), (unsigned char *)(&ov18_021F9DE4), 0xd);
    uVar6 = 0;
    pWVar4 = pokedexApp->windows;
    do {
        FillWindowPixelBuffer(pWVar4, 0);
        uVar6 = uVar6 + 1;
        pWVar4 = pWVar4 + 1;
    } while (uVar6 < 0xd);
    BVar1 = Pokedex_CheckMonCaughtFlag(pokedexApp->args->pokedex, pokedexApp->curSpecies);
    if (!BVar1) {
        uVar5 = 1;
    } else {
        uVar5 = 2;
    }
    ov18_021F9648(pokedexApp->windows, pokedexApp->msgData, 0x8f, 0, 0, 4, 0x20100, 0);
    ov18_021F9648(pokedexApp->windows + 3, pokedexApp->msgData, 0x88, 0x30, 0, 0, 0x20100, 2);
    ov18_021F9648(pokedexApp->windows + 4, pokedexApp->msgData, 10, 0x10, 0, 0, 0x20100, 2);
    ov18_021F9648(pokedexApp->windows + 6, pokedexApp->msgData, 10, 0x10, 0, 0, 0x20100, 2);
    pSVar2 = ov18_021E590C(pokedexApp->curSpecies, 2, HEAP_ID_POKEDEX_APP);
    ov18_021F95FC(pokedexApp->windows + 1, pSVar2, 0x20, 0, 0, 0x20100, 2);
    String_Delete(pSVar2);
    pSVar2 = PlayerProfile_GetPlayerName_NewString(pokedexApp->args->playerProfile, HEAP_ID_POKEDEX_APP);
    ov18_021F95FC(pokedexApp->windows + 2, pSVar2, 0x20, 0, 0, 0x20100, 2);
    String_Delete(pSVar2);
    ov18_021EEA84(pokedexApp, pokedexApp->curSpecies, uVar5, 5, 0x20, 0, 0x20100, 2);
    ov18_021F9648(pokedexApp->windows + 8, pokedexApp->msgData, 0x89, 0x30, 0, 0, 0x50900, 2);
    ov18_021F9648(pokedexApp->windows + 9, pokedexApp->msgData, 0xb, 0x10, 0, 0, 0xf0500, 2);
    ov18_021F9648(pokedexApp->windows + 0xb, pokedexApp->msgData, 0xb, 0x10, 0, 0, 0xf0500, 2);
    ov18_021EEB34(pokedexApp, pokedexApp->curSpecies, uVar5, 10, 0x20, 0, 0xf0500, 2);
    uVar3 = PlayerProfile_GetTrainerGender(pokedexApp->args->playerProfile);
    if (uVar3 == 0) {
        ov18_021F9648(pokedexApp->windows + 7, pokedexApp->msgData, 0x8a, 0x20, 0, 0, 0x20100, 2);
        ov18_021F9648(pokedexApp->windows + 0xc, pokedexApp->msgData, 0x8c, 0x20, 0, 0, 0xf0500, 2);
    } else {
        ov18_021F9648(pokedexApp->windows + 7, pokedexApp->msgData, 0x8b, 0x20, 0, 0, 0x20100, 2);
        ov18_021F9648(pokedexApp->windows + 0xc, pokedexApp->msgData, 0x8d, 0x20, 0, 0, 0xf0500, 2);
    }
    uVar6 = 0;
    pWVar4 = pokedexApp->windows;
    do {
        ScheduleWindowCopyToVram(pWVar4);
        uVar6 = uVar6 + 1;
        pWVar4 = pWVar4 + 1;
    } while (uVar6 < 0xd);
    return;
}

void ov18_021F0838(PokedexAppData *pokedexApp) {
    u32 uVar1;
    Window *window;

    uVar1 = 0;
    window = pokedexApp->windows;
    do {
        ClearWindowTilemapAndScheduleTransfer(window);
        uVar1 = uVar1 + 1;
        window = window + 1;
    } while (uVar1 < 0xd);
    ov18_021EE388(pokedexApp);
    return;
}
