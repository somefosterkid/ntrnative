#include "global.h"

#include "constants/heap.h"

#include "heap.h"
#include "pokemon_anim.h"
#include "unk_02016EDC.h"


PokepicAnimManager *sub_02016EDC(enum HeapID heapId, int a1, int a2) {
    PokepicAnimManager *pokepicAnimManager;
    int i;
    int size;

    pokepicAnimManager = Heap_Alloc(heapId, sizeof(PokepicAnimManager));
    pokepicAnimManager->flipSprite = a2;
    pokepicAnimManager->animCount = a1;
    size = a1 * sizeof(PokemonAnim);
    pokepicAnimManager->heapID = heapId;
    pokepicAnimManager->anims = Heap_Alloc(heapId, size);
    MI_CpuFill8(pokepicAnimManager->anims, 0, size);
    for (i = 0; i < a1; i++) {
        pokepicAnimManager->anims[i].completed = TRUE;
    }
    return pokepicAnimManager;
}

void sub_02016F2C(PokepicAnimManager *pokepicAnimManager) {
    Heap_Free(pokepicAnimManager->anims);
    Heap_Free(pokepicAnimManager);
    return;
}
