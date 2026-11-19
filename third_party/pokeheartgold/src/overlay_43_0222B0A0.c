#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "message_format.h"
#include "msgdata.h"
#include "options.h"
#include "player_data.h"
#include "pm_string.h"
#include "save.h"
#include "text.h"
#include "heap.h"
#include "overlay_43_palpad.h"

void ov43_0222B0A0(PalPadMenu *menu, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapID);
void ov43_0222B1D8(PalPadMenu *menu);

void ov43_0222B0A0(PalPadMenu *menu, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapID) {
    u8 bVar1;
    String *pSVar2;
    Options *options;
    String *dest;
    PlayerProfile *playerProfile;
    u32 uVar3;
    void *pvVar4;
    s32 iVar5;
    u32 y;

    InitWindow(&menu->menuWindow);
    AddWindowParameterized(gfx->bgConfig, &menu->menuWindow, 3, 4, 4, 0x18, 0x14, 0xb, 1);
    InitWindow(&menu->msgWindow);
    AddWindowParameterized(gfx->bgConfig, &menu->msgWindow, 1, 2, 0x13, 0x1b, 4, 0xb, 0xac);
    FillWindowPixelBuffer(&menu->menuWindow, 0);
    FillWindowPixelBuffer(&menu->msgWindow, 0);
    pSVar2 = String_New(0x80, heapID);
    menu->msgText = pSVar2;
    options = Save_PlayerData_GetOptionsAddr(shared->saveData);
    bVar1 = Options_GetTextFrameDelay(options);
    menu->textSpeed = (u32)bVar1;
    pSVar2 = String_New(0x80, heapID);
    dest = String_New(0x80, heapID);
    playerProfile = Save_PlayerData_GetProfile(shared->saveData);
    BufferPlayersName(gfx->msgFormat, 0, playerProfile);
    iVar5 = 0;
    y = 8;
    do {
        ReadMsgDataIntoString(gfx->msgData, iVar5 + 1, dest);
        StringExpandPlaceholders(gfx->msgFormat, pSVar2, dest);
        uVar3 = FontID_String_GetWidth(FONT_UNK_4, pSVar2, 0);
        AddTextPrinterParameterizedWithColor(&menu->menuWindow, FONT_UNK_4, pSVar2, 0xc0 - uVar3 >> 1, y, 0xff, 0x10f00, NULL);
        iVar5 = iVar5 + 1;
        y = y + 0x28;
    } while (iVar5 < 4);
    String_Delete(pSVar2);
    String_Delete(dest);
    pvVar4 = GfGfxLoader_GetScrnDataFromOpenNarc(gfx->narc, 0xb, 1, &menu->screen, heapID);
    menu->screenFile = pvVar4;
    return;
}

void ov43_0222B1D8(PalPadMenu *menu) {
    String_Delete(menu->msgText);
    RemoveWindow(&menu->menuWindow);
    RemoveWindow(&menu->msgWindow);
    Heap_Free(menu->screenFile);
}
