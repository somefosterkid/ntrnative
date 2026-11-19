#include "global.h"

#include "constants/heap.h"

#include "heap.h"
#include "pokemon.h"
#include "pokemon_types_def.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

void ov14_021F08BC(u8 *param_1);
void ov14_021F08F0(u8 *param_1);

void ov14_021F08BC(u8 *param_1) {
    u32 *puVar1;
    Pokemon *pPVar2;
    u32 uVar3;
    u32 *puVar4;

    puVar1 = Heap_Alloc(HEAP_ID_10, 0xf0);
    uVar3 = 0;
    puVar4 = puVar1;
    do {
        pPVar2 = AllocMonZeroed(HEAP_ID_10);
        *puVar4 = (u32)(pPVar2);
        uVar3 = uVar3 + 1;
        puVar4 = puVar4 + 8;
    } while (uVar3 < 7);
    puVar1[0x38] = 0;
    ((UnkStruct_ov14_boxapp *)param_1)->sys->unk_C = puVar1;
    return;
}

void ov14_021F08F0(u8 *param_1) {
    u32 uVar1;
    u32 *puVar2;
    u32 *ptr;

    uVar1 = 0;
    ptr = ((UnkStruct_ov14_boxapp *)param_1)->sys->unk_C;
    puVar2 = ptr;
    do {
        Heap_Free((void *)*puVar2);
        uVar1 = uVar1 + 1;
        puVar2 = puVar2 + 8;
    } while (uVar1 < 7);
    Heap_Free(ptr);
    return;
}
