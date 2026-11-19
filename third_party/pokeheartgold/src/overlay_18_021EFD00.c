#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "bg_window.h"
#include "message_format.h"
#include "msgdata.h"
#include "string_util.h"

s32 ov18_021EE3AC(PokedexAppData *, MsgData *, s32, s32, s32, s32, s32, s32, s32);

void ov18_021F9648(Window *, MsgData *, u32, u32, u32, u32, u32, u32);

void ov18_021EFD00(PokedexAppData *pokedexApp, u16 a1, int a2) {
    s32 v0;
    Window *window;
    u32 v1;
    u32 v2;

    if (a1 == 0x3E7) {
        v2 = 0x4A4;
    } else {
        v2 = (u32)(((u32)(0x2710 * a1) / 254U) + 5) / 10U;
    }
    window = pokedexApp->windows;
    v0 = a2 * 0x10;
    FillWindowPixelBuffer(&window[a2], 0U);
    BufferIntegerAsString(pokedexApp->msgFormat, 0U, (s32)(v2 / 12U), 3U, PRINTING_MODE_LEFT_ALIGN, 1);
    BufferIntegerAsString(pokedexApp->msgFormat, 1U, (s32)(v2 % 12U), 2U, PRINTING_MODE_LEADING_ZEROS, 1);
    v1 = GetWindowWidth(&window[a2]) * 8;
    ov18_021EE3AC(pokedexApp, pokedexApp->msgData, a2, 0xAF, (s32)(v1 + (v1 >> 0x1F)) >> 1, 0, 4, 0x20100, 2);
}

void ov18_021EFDB4(PokedexAppData *pokedexApp, u16 a1, int a2) {
    s32 v0;
    Window *window;
    u32 v1;
    u32 v2;

    if (a1 == 0x270F) {
        v2 = 0x18696;
    } else {
        v2 = (u32)((0x35D2E * a1) + 0xC350) / 100000U;
    }
    window = pokedexApp->windows;
    v0 = a2 * 0x10;
    FillWindowPixelBuffer(&window[a2], 0U);
    BufferIntegerAsString(pokedexApp->msgFormat, 0U, (s32)(v2 / 10U), 4U, PRINTING_MODE_LEFT_ALIGN, 1);
    BufferIntegerAsString(pokedexApp->msgFormat, 1U, (s32)(v2 % 10U), 1U, PRINTING_MODE_LEADING_ZEROS, 1);
    v1 = GetWindowWidth(&window[a2]) * 8;
    ov18_021EE3AC(pokedexApp, pokedexApp->msgData, a2, 0x26, (s32)(v1 + (v1 >> 0x1F)) >> 1, 0, 4, 0x20100, 2);
}

void ov18_021EFE70(PokedexAppData *pokedexApp, int a1) {
    u8 bVar1;
    FillWindowPixelBuffer(pokedexApp->windows + a1, 0);
    bVar1 = GetWindowWidth(pokedexApp->windows + a1);
    ov18_021F9648(pokedexApp->windows + a1, pokedexApp->msgData, pokedexApp->dexSearchCriteria[8] + 0x41, (bVar1 * 8) / 2, 0, 4, 0x20100, 2);
    return;
}
