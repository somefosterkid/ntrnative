#include "global.h"

#include "error_handling.h"
#include "heap.h"
#include "pokemon.h"
#include "unk_02013FDC.h"

void *ov02_0224A7B8(Pokemon *mon, PokepicTemplate *spriteTemplate, enum HeapID heapID);

void *ov02_0224A7B8(Pokemon *mon, PokepicTemplate *spriteTemplate, enum HeapID heapID) {
    void *dest;

    dest = Heap_Alloc(HEAP_ID_FIELD1, 0xC80U);
    if (dest == NULL) {
        GF_AssertFail();
    }
    sub_02014540((enum NarcId)spriteTemplate->narcID, spriteTemplate->charDataID, heapID, dest, GetMonData(mon, 0, NULL), 0, 2, spriteTemplate->species);
    return dest;
}
