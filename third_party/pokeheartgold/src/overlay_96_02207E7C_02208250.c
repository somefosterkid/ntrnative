#include "global.h"

#include "overlay_96_ov96hurdle_hud.h"
#include "pokemon.h"
#include "unk_02013FDC.h"

void ov96_02208250(Ov96HurdleHud *hud, void *arg1);

void ov96_02208250(Ov96HurdleHud *hud, void *arg1) {
    PokepicTemplate sp10;
    s32 spC;
#if defined(SDK_PORT) || defined(NONMATCHING)
    s32 temp_r0;
    void *var_r5;

    var_r5 = arg1;
    spC = 0;
    do {
        GetMonSpriteCharAndPlttNarcIdsEx(&sp10, *(u16 *)var_r5, *(u8 *)((u8 *)var_r5 + 7), 2U, (u8)(s32) * (u8 *)((u8 *)var_r5 + 6), (u8)(u8) * (u16 *)((u8 *)var_r5 + 2), *(u32 *)((u8 *)var_r5 + 0xC));
        hud->picRes[spC].charBuf = sub_0201457C((enum NarcId)sp10.narcID, (s32)sp10.charDataID, hud->heapId, *(u32 *)((u8 *)var_r5 + 0xC), 0, 2, (s32) * (u16 *)var_r5);
        hud->picRes[spC].palBuf = sub_02014450((enum NarcId)sp10.narcID, (s32)sp10.palDataID, hud->heapId);
        var_r5 += 0x10;
        temp_r0 = spC + 1;
        spC = temp_r0;
    } while (temp_r0 < 3);
#else
    enum HeapID *var_r4;
    s32 temp_r0;
    void *var_r5;

    var_r5 = arg1;
    spC = 0;
    var_r4 = (enum HeapID *)hud;
    do {
        GetMonSpriteCharAndPlttNarcIdsEx(&sp10, *(u16 *)var_r5, *(u8 *)((u8 *)var_r5 + 7), 2U, (u8)(s32) * (u8 *)((u8 *)var_r5 + 6), (u8)(u8) * (u16 *)((u8 *)var_r5 + 2), *(u32 *)((u8 *)var_r5 + 0xC));
        *(void **)((u8 *)var_r4 + 0x108) = sub_0201457C((enum NarcId)sp10.narcID, (s32)sp10.charDataID, hud->heapId, *(u32 *)((u8 *)var_r5 + 0xC), 0, 2, (s32) * (u16 *)var_r5);
        *(void **)((u8 *)var_r4 + 0x10C) = sub_02014450((enum NarcId)sp10.narcID, (s32)sp10.palDataID, hud->heapId);
        var_r5 += 0x10;
        temp_r0 = spC + 1;
        var_r4 += 2;
        spC = temp_r0;
    } while (temp_r0 < 3);
#endif
}
