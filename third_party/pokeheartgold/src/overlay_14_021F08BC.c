#include "global.h"

#include "constants/heap.h"

#include "heap.h"
#include "pokemon.h"
#include "pokemon_types_def.h"
#include "overlay_14_move_owner.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

void ov14_021F08BC(UnkStruct_ov14_boxapp *param_1);
void ov14_021F08F0(UnkStruct_ov14_boxapp *param_1);

void ov14_021F08BC(UnkStruct_ov14_boxapp *param_1) {
    MoveSet *set;
    Pokemon *pPVar2;
    u32 uVar3;
    MoveEntry *entry;

    set = Heap_Alloc(HEAP_ID_10, sizeof(MoveSet));
    uVar3 = 0;
    entry = set->e;
    do {
        pPVar2 = AllocMonZeroed(HEAP_ID_10);
        entry->mon = pPVar2;
        uVar3 = uVar3 + 1;
        entry++;
    } while (uVar3 < 7);
    set->frame = 0;
    param_1->sys->unk_C = set;
    return;
}

void ov14_021F08F0(UnkStruct_ov14_boxapp *param_1) {
    u32 uVar1;
    MoveEntry *entry;
    MoveSet *ptr;

    uVar1 = 0;
    ptr = param_1->sys->unk_C;
    entry = ptr->e;
    do {
        Heap_Free(entry->mon);
        uVar1 = uVar1 + 1;
        entry++;
    } while (uVar1 < 7);
    Heap_Free(ptr);
    return;
}
