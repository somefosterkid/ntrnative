#include "global.h"

#include "constants/heap.h"

#include "frontier/overlay_80_02229EE0.h"

#include "heap.h"
#include "overlay_80_02236B78.h"
#include "party.h"
#include "pokemon.h"
#include "pokemon_types_def.h"

void ov80_0222A140(u8 *, Pokemon *, u32);

void ov80_0222A3BC(void *, Party *, Pokemon *);

u32 ov80_02237120(s32);

void ov80_022371B0(s32 param_1) {
    s32 iVar1;
    Pokemon *ptr;
    u32 uVar2;
    u8 *puVar3;
    s32 iStack_88;
    u8 auStack_84[112];

    for (iVar1 = Party_GetCount(*(Party **)(param_1 + 0x4d4)); 2 < iVar1; iVar1 = iVar1 + -1) {
        Party_RemoveMon(*(Party **)(param_1 + 0x4d4), iVar1 + -1);
    }
    ov80_0222A52C(auStack_84, (u16 *)(param_1 + 0x584), (u8 *)(param_1 + 0x590), (u32 *)(param_1 + 0x598), (void *)0x0, 2, 0xb, 0xcd);
    ptr = AllocMonZeroed(HEAP_ID_FIELD2);
    iStack_88 = 0;
    puVar3 = auStack_84;
    iVar1 = param_1;
    do {
        uVar2 = ov80_02237120(param_1);
        ov80_0222A140(puVar3, ptr, uVar2);
        ov80_0222A3BC(*(void **)(param_1 + 0x4f8), *(void **)(param_1 + 0x4d4), ptr);
        puVar3 = puVar3 + 0x38;
        *(u16 *)(iVar1 + 0x4ec) = *(u16 *)(iVar1 + 0x584);
        iVar1 = iVar1 + 2;
        iStack_88 = iStack_88 + 1;
    } while (iStack_88 < 2);
    Heap_Free(ptr);
    return;
}
