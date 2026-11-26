#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "bg_window.h"
#include "wild_encounter.h"
#include "message_format.h"
#include "msgdata.h"
#include "string_util.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
s32 ov18_021EE984(PokedexAppData *, u16, s32, s32);
#else
s32 ov18_021EE984(PokedexAppData *, s32, s32, s32);
#endif

s32 ov18_021EE9FC(PokedexAppData *, s32);

s32 ov18_021EEA40(PokedexAppData *, s32);

s32 ov18_021EEAE4(PokedexAppData *, s32, s32, s32);

s32 ov18_021EEB94(PokedexAppData *, s32, s32, s32);

s32 ov18_021EEBE4(PokedexAppData *, s32);

s32 ov18_021EEC34(PokedexAppData *, s32, s32, s32);

#if defined(SDK_PORT) || defined(NONMATCHING)
s32 ov18_021EECB0(PokedexAppData *, u16, s32);
#else
s32 ov18_021EECB0(PokedexAppData *, s32, s32);
#endif

#if defined(SDK_PORT) || defined(NONMATCHING)
s32 ov18_021EED00(PokedexAppData *, u16, s32, s32);
#else
s32 ov18_021EED00(PokedexAppData *, s32, s32, s32);
#endif

s32 ov18_021EE35C(PokedexAppData *, s32 *, s32);
void ov18_021EEE84(PokedexAppData *pokedexApp);
extern s32 ov18_021F9FDC;
s32 sub_02019A60(UnkStruct_0201956C *, s32, void *);

void ov18_021EE3AC(PokedexAppData *, MsgData *, u32, u32, u32, u32, u32, u32, u32);
void ov18_021F9648(Window *, MsgData *, u32, u32, u32, u32, u32, u32);

void ov18_021EED70(PokedexAppData *pokedexApp, int species, int a2) {
    ClearWindowTilemapAndScheduleTransfer(&pokedexApp->windows[0x56]);
    ClearWindowTilemapAndScheduleTransfer(&pokedexApp->windows[0x58]);
    ClearWindowTilemapAndScheduleTransfer(&pokedexApp->windows[0x55]);
    ClearWindowTilemapAndScheduleTransfer(&pokedexApp->windows[0x57]);
    ClearWindowTilemapAndScheduleTransfer(&pokedexApp->windows[0x59]);
    ClearWindowTilemapAndScheduleTransfer(&pokedexApp->windows[0x5A]);
    ClearWindowTilemapAndScheduleTransfer(&pokedexApp->windows[0x5B]);
    ClearWindowTilemapAndScheduleTransfer(&pokedexApp->windows[0x5C]);
    ClearWindowTilemapAndScheduleTransfer(&pokedexApp->windows[0x5D]);
    if (species != 0) {
        ov18_021EE984(pokedexApp, species, a2, 0x55);
        if (pokedexApp->unk_185C == 2) {
            ov18_021EE9FC(pokedexApp, 0x56);
            ov18_021EEA40(pokedexApp, 0x58);
            ov18_021EEAE4(pokedexApp, species, a2, 0x57);
            ov18_021EEB94(pokedexApp, species, a2, 0x59);
            return;
        }
        ov18_021EEBE4(pokedexApp, 0x5A);
        ov18_021EEC34(pokedexApp, species, 0x5B, pokedexApp->unk_185C);
        ov18_021EECB0(pokedexApp, species, 0x5C);
        ov18_021EED00(pokedexApp, species, a2, 0x5D);
    }
}

void ov18_021EEE58(PokedexAppData *pokedexApp) {
    ov18_021EE35C(pokedexApp, &ov18_021F9FDC, 0x65);
    ov18_021EEED0(pokedexApp, 0);
    ov18_021EF45C(pokedexApp);
    ov18_021EF528(pokedexApp);
    ov18_021EEE84(pokedexApp);
}

void ov18_021EEE84(PokedexAppData *pokedexApp) {
    sub_02019A60(pokedexApp->unk_0008, 6, &pokedexApp->windows[17]);
    sub_02019A60(pokedexApp->unk_0008, 6, &pokedexApp->windows[18]);
    sub_02019A60(pokedexApp->unk_0008, 6, &pokedexApp->windows[19]);
    sub_02019A60(pokedexApp->unk_0008, 7, &pokedexApp->windows[17]);
    sub_02019A60(pokedexApp->unk_0008, 7, &pokedexApp->windows[19]);
}

void ov18_021EEED0(PokedexAppData *pokedexApp, int a1) {
    Window *window;

    window = pokedexApp->windows;
    FillWindowPixelBuffer(window, 0);
    switch (a1) {
    case 0:
        ov18_021F9648(window, pokedexApp->msgData, 0xc, 0x70, 6, 0, 0x20100, 2);
        ov18_021F9648(window, pokedexApp->msgData, 0xd, 0x70, 0x16, 0, 0x20100, 2);
        break;
    case 1:
        ov18_021F9648(window, pokedexApp->msgData, 0xe, 0x70, 6, 0, 0x20100, 2);
        BufferIntegerAsString(pokedexApp->msgFormat, 0, (u32)(pokedexApp->unk_0878).unk_7B4, 3, PRINTING_MODE_LEADING_ZEROS, 1);
        ov18_021EE3AC(pokedexApp, pokedexApp->msgData, 0, 0xf, 0x70, 0x16, 0, 0x20100, 2);
        break;
    case 2:
        ov18_021F9648(window, pokedexApp->msgData, 0xc, 0x70, 6, 0, 0x20100, 2);
        ov18_021F9648(window, pokedexApp->msgData, 0x10, 0x70, 0x16, 0, 0x20100, 2);
        break;
    case 3:
        ov18_021F9648(window, pokedexApp->msgData, 0xc, 0x70, 6, 0, 0x20100, 2);
        ov18_021F9648(window, pokedexApp->msgData, 0x11, 0x70, 0x16, 0, 0x20100, 2);
        break;
    case 4:
        ov18_021F9648(window, pokedexApp->msgData, 0xc, 0x70, 6, 0, 0x20100, 2);
        ov18_021F9648(window, pokedexApp->msgData, 0x12, 0x70, 0x16, 0, 0x20100, 2);
        break;
    case 5:
        ov18_021F9648(window, pokedexApp->msgData, 0xc, 0x70, 6, 0, 0x20100, 2);
        ov18_021F9648(window, pokedexApp->msgData, 0x14, 0x70, 0x16, 0, 0x20100, 2);
        break;
    case 6:
        ov18_021F9648(window, pokedexApp->msgData, 0xc, 0x70, 6, 0, 0x20100, 2);
        ov18_021F9648(window, pokedexApp->msgData, 0x13, 0x70, 0x16, 0, 0x20100, 2);
        break;
    case 7:
        ov18_021F9648(window, pokedexApp->msgData, 0xc, 0x70, 6, 0, 0x20100, 2);
        ov18_021F9648(window, pokedexApp->msgData, 0x16, 0x70, 0x16, 0, 0x20100, 2);
        break;
    case 8:
        ov18_021F9648(window, pokedexApp->msgData, 0xc, 0x70, 6, 0, 0x20100, 2);
        ov18_021F9648(window, pokedexApp->msgData, 0x15, 0x70, 0x16, 0, 0x20100, 2);
        break;
    case 9:
        ov18_021F9648(window, pokedexApp->msgData, 0xc, 0x70, 6, 0, 0x20100, 2);
        ov18_021F9648(window, pokedexApp->msgData, 0x19, 0x70, 0x16, 0, 0x20100, 2);
        break;
    case 10:
        ov18_021F9648(window, pokedexApp->msgData, 0x17, 0x70, 6, 0, 0x20100, 2);
        ov18_021F9648(window, pokedexApp->msgData, 0x18, 0x70, 0x16, 0, 0x20100, 2);
    }
    ScheduleWindowCopyToVram(window);
    return;
}
