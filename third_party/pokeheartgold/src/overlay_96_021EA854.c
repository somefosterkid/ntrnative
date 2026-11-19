#include "global.h"

#include "heap.h"
#include "overlay_96_sprite_res_man.h"

Ov96SpriteSet *ov96_021EA854(enum HeapID heapID, s32 count, s32 drawPriority, UnkStruct_ov96_021E9A78 *resSrc, SpriteList *spriteList) {
    Ov96SpriteSet *set;

    set = Heap_Alloc(heapID, sizeof(Ov96SpriteSet));
    MI_CpuFill8(set, 0, sizeof(Ov96SpriteSet));
    set->heapID = heapID;
    set->count = count;
    set->resSrc = resSrc;
    set->spriteList = spriteList;
    set->drawPriority = drawPriority;
    set->man = ov96_021E8A30(heapID);
    ov96_021E8A44(set->man, count, spriteList);
    return set;
}

void ov96_021EA894(Ov96SpriteSet *set) {
    ov96_021E8AAC(set->man);
    Heap_Free(set);
}
