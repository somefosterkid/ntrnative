#include "global.h"

#include "constants/heap.h"

#include "filesystem_files_def.h"
#include "pokemon.h"
#include "pokemon_types_def.h"
#include "unk_02013FDC.h"

s32 ov96_021E6168(void *, s32, s32, s32 *);

void ov96_021F4484(void *arg0);

void ov96_021F4484(void *arg0) {
    PokepicTemplate sp20;
    s32 sp10[4];
    s32 spC;
    s32 var_r4;
    void **temp_r1;
    void *var_r5;
    var_r4 = 0;
    var_r5 = arg0;
    do {
        spC = var_r4 / 3;
        ov96_021E6168(*((void **)(((u8 *)arg0) + 4)), spC, var_r4 % 3, sp10);
        GetMonSpriteCharAndPlttNarcIdsEx(&sp20, *((u16 *)(sp10)), *((u8 *)(((u8 *)(sp10)) + 7)), 2U, (u8)((s32)(*((u8 *)(((u8 *)(sp10)) + 6)))), (u8)((u8)(*((u16 *)(((u8 *)(sp10)) + 2)))), sp10[3]);
        *((void **)(((u8 *)var_r5) + 0x98)) = sub_0201457C((enum NarcId)(*((u16 *)(((u8 *)(sp10)) + 0x10))), (s32)(*((u16 *)(((u8 *)(sp10)) + 0x12))), *((enum HeapID *)arg0), sp10[3], 0, 2, (s32)(*((u16 *)(sp10))));
        temp_r1 = var_r5 + 0x9C;
        *temp_r1 = sub_02014450((enum NarcId)(*((u16 *)(((u8 *)(sp10)) + 0x10))), (s32)(*((u16 *)(((u8 *)(sp10)) + 0x14))), *((enum HeapID *)arg0));
        var_r4 += 1;
        var_r5 += 8;
    } while (var_r4 < 0xC);
}
