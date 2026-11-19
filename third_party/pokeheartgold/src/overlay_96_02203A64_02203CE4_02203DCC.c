#include "global.h"

#include "pokemon.h"
#include "unk_02013FDC.h"

void ov96_02203DCC(enum HeapID *arg0, void *arg1);

void ov96_02203DCC(enum HeapID *arg0, void *arg1) {
    PokepicTemplate sp10;
    s32 spC;
    enum HeapID *var_r4;
    s32 temp_r0;
    void *var_r5;

    var_r5 = arg1;
    spC = 0;
    var_r4 = arg0;
    do {
        GetMonSpriteCharAndPlttNarcIdsEx(&sp10, *(u16 *)var_r5, *(u8 *)((u8 *)var_r5 + 7), 0U, (u8)(s32) * (u8 *)((u8 *)var_r5 + 6), (u8)(u8) * (u16 *)((u8 *)var_r5 + 2), *(u32 *)((u8 *)var_r5 + 0xC));
        *(void **)((u8 *)var_r4 + 0x24) = sub_0201457C((enum NarcId)sp10.narcID, (s32)sp10.charDataID, *arg0, *(u32 *)((u8 *)var_r5 + 0xC), 0, 0, (s32) * (u16 *)var_r5);
        *(void **)((u8 *)var_r4 + 0x28) = sub_02014450((enum NarcId)sp10.narcID, (s32)sp10.palDataID, *arg0);
        var_r5 += 0x10;
        temp_r0 = spC + 1;
        var_r4 += 2;
        spC = temp_r0;
    } while (temp_r0 < 3);
}
