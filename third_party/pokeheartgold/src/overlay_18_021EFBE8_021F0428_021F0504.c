#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "bg_window.h"
#include "msgdata.h"
#include "unk_0201956C.h"

void ov18_021EE3AC(PokedexAppData *, MsgData *, u32, u32, u32, u32, u32, u32, u32);

void sub_02019B1C(UnkStruct_0201956C *, u32, u8 *, s8 *);

void ov18_021F0504(PokedexAppData *pokedexApp, int a1) {
    u32 uVar1;
    u32 uVar2;
    u32 uVar3;
    s8 cStack_18;
    u8 auStack_17[3];
    uVar3 = 0;
    do {
        sub_02019B1C(pokedexApp->unk_0008, uVar3 + 0x11, auStack_17, &cStack_18);
        if ((cStack_18 == 2) || (cStack_18 == 0x14)) {
            break;
        }
        uVar3 = uVar3 + 1;
    } while (uVar3 < 6);
    FillWindowPixelBuffer(pokedexApp->windows + (uVar3 + 8), 0);
    if (a1 < 0) {
        sub_020196E8(pokedexApp->unk_0008, uVar3 + 0x11, 10, 0x14);
        uVar2 = ((s32)pokedexApp->unk_18CA) + 2;
        if (((s32)uVar2) < pokedexApp->unk_18DC.unk_20.nMaps) {
            uVar1 = ov18_021F04C0(pokedexApp, uVar2);
            ov18_021EE3AC(pokedexApp, pokedexApp->msgData, uVar3 + 8, uVar1, 0x48, 0, 4, 0xf0c00, 2);
        }
    } else {
        sub_020196E8(pokedexApp->unk_0008, uVar3 + 0x11, 10, 2);
        uVar2 = ((s32)pokedexApp->unk_18CA) - 2;
        if (((-1) + 1) <= ((s32)uVar2)) {
            uVar1 = ov18_021F04C0(pokedexApp, uVar2);
            ov18_021EE3AC(pokedexApp, pokedexApp->msgData, uVar3 + 8, uVar1, 0x48, 0, 4, 0xf0c00, 2);
        }
    }
    CopyWindowPixelsToVram_TextMode(pokedexApp->windows + (uVar3 + 8));
    return;
}
