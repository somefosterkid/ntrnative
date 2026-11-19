#include "global.h"
struct HMCutIn;

#include "constants/heap.h"

#include "filesystem_files_def.h"
#include "pokemon_types_def.h"
#include "unk_02013FDC.h"
#include "unk_0200A090.h"
#include "overlay_02_hmcut_in.h"

void *ov02_0224A800(PokepicTemplate *spriteTemplate, enum HeapID heapID);

void ov02_0224A810(struct HMCutIn *arg0, NARC *arg1);

void *ov02_0224A800(PokepicTemplate *spriteTemplate, enum HeapID heapID) {
    return sub_02014450((enum NarcId)spriteTemplate->narcID, (s32)spriteTemplate->palDataID, heapID);
}

void ov02_0224A810(HMCutIn *arg0, NARC *narc) {
    AddCharResObjFromOpenNarc(arg0->charLocation, narc, 9, 0, 3, 1, HEAP_ID_FIELD1);
}
