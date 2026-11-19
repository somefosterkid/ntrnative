#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "heap.h"
#include "message_format.h"
#include "pokemon.h"
#include "pokemon_types_def.h"
#include "pokewalker.h"
#include "string_util.h"

void ov112_021E7CA4(u8 *, u32, u32);

void ov112_021E9290(BoxPokemon *, u8 *, u8 *, u32);

s32 ov112_021E9610(u8 *, u32);

void ov112_021E98E8(u8 *, u16);

void ov112_021EA010(u8 *, u32, String *, u32, u32);

void ov112_021EA044(u8 *, u32, u32, u32);

void ov112_021EA64C(u8 *);

void ov112_021EA670(u8 *, u32);

void ov112_021EA688(u8 *, u32);

void ov112_021EADD0(u8 *param_1, s32 param_2);

void ov112_021EADD0(u8 *param_1, s32 param_2) {
    s32 num;
    Pokemon *mon;
    BoxPokemon *boxMon;
    u32 uStack_18[2];
    sub_02032674(*((POKEWALKER **)(param_1 + 0x1e440)), &uStack_18[1], &uStack_18[0]);
    BufferIntegerAsString(*((MessageFormat **)(param_1 + 0x1e448)), 7, uStack_18[0], 7, PRINTING_MODE_LEFT_ALIGN, 1);
    BufferIntegerAsString(*((MessageFormat **)(param_1 + 0x1e448)), 6, uStack_18[1], 7, PRINTING_MODE_LEFT_ALIGN, 1);
    num = ov112_021E9610(param_1, uStack_18[0]);
    BufferIntegerAsString(*((MessageFormat **)(param_1 + 0x1e448)), 0xc, num, 7, PRINTING_MODE_LEFT_ALIGN, 1);
    ov112_021E7CA4(param_1, 5, 0x10);
    ov112_021E98E8(param_1, 1);
    *((u16 *)(param_1 + 0x1f2e0)) = 0;
    ov112_021EA010(param_1, 1, *((void **)(param_1 + 0x1e50c)), 0, 0x10200);
    ov112_021EA044(param_1, 2, 0x53, 0x10200);
    ov112_021EA010(param_1, 3, *((void **)(param_1 + 0x1e510)), 0, 0x10200);
    ov112_021EA044(param_1, 4, 0x54, 0x10200);
    if (num < 0) {
        ov112_021EA044(param_1, 0, 0x58, 0x10200);
    } else if (num == 0) {
        ov112_021EA044(param_1, 0, 0x57, 0x10200);
    } else {
        ov112_021EA044(param_1, 0, 0x56, 0x10200);
    }
    if (param_2 != 0) {
        mon = AllocMonZeroed(HEAP_ID_154);
        boxMon = Mon_GetBoxMon(mon);
        Pokewalker_TryGetBoxMon(*((POKEWALKER **)(param_1 + 0x1e440)), boxMon);
        BufferBoxMonNickname(*((MessageFormat **)(param_1 + 0x1e448)), 0xd, boxMon);
        ov112_021EA044(param_1, 5, 0x59, 0x30400);
        ov112_021E9290(boxMon, param_1 + 0x1d7ac, 0x1d79c + param_1, 1);
        ov112_021EA670(param_1, 7);
        Heap_Free(mon);
    } else {
        ov112_021EA688(param_1, 7);
        FillWindowPixelBuffer((void *)(param_1 + 0x1ebf8), 0);
        CopyWindowToVram((void *)(param_1 + 0x1ebf8));
    }
    ov112_021EA64C(param_1);
    return;
}
