#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "bg_window.h"
#include "msgdata.h"
#include "unk_0201956C.h"

void ov18_021EE3AC(PokedexAppData *, MsgData *, u32, s32, u32, u32, u32, u32, u32);

s32 ov18_021F09D8(PokedexAppData *, s32);

void sub_02019B1C(UnkStruct_0201956C *, u32, u8 *, s8 *);

void ov18_021F0B70(PokedexAppData *pokedexApp, int a1) {
    s32 iVar1;
    u32 uVar2;
    s8 cStack_18;
    u8 auStack_17[3];

    uVar2 = 0;
    do {
        sub_02019B1C(pokedexApp->unk_0008, uVar2 + 0x11, auStack_17, &cStack_18);
        if ((cStack_18 == -2) || (cStack_18 == 0x10)) {
            break;
        }
        uVar2 = uVar2 + 1;
    } while (uVar2 < 6);
    FillWindowPixelBuffer(pokedexApp->windows + (uVar2 + 10), 0);
    if (a1 < 0) {
        sub_020196E8(pokedexApp->unk_0008, uVar2 + 0x11, 8, 0x10);
        iVar1 = pokedexApp->unk_18C5 + 2;
        if (iVar1 < pokedexApp->numSeenForms) {
            iVar1 = ov18_021F09D8(pokedexApp, iVar1);
            ov18_021EE3AC(pokedexApp, pokedexApp->msgData, uVar2 + 10, iVar1, 0, 0, 4, 0xf0c00, 0);
        }
    } else {
        sub_020196E8(pokedexApp->unk_0008, uVar2 + 0x11, 8, -2);
        iVar1 = pokedexApp->unk_18C5 + -2;
        if (iVar1 >= 0) {
            iVar1 = ov18_021F09D8(pokedexApp, iVar1);
            ov18_021EE3AC(pokedexApp, pokedexApp->msgData, uVar2 + 10, iVar1, 0, 0, 4, 0xf0c00, 0);
        }
    }
    CopyWindowPixelsToVram_TextMode(pokedexApp->windows + (uVar2 + 10));
    return;
}
