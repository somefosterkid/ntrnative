#include "global.h"

#include "heap.h"
#include "overlay_96_sprite_res_man.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"

void ov96_021E8AAC(Ov96SpriteResMan *man) {
    u8 i;

    i = 0;
    if ((s32)man->maxEntries > 0) {
        do {
            SpriteTransfer_DeleteCharTransferTask(man->entries[i].charRes);
            SpriteTransfer_DeletePlttTransferTask(man->entries[i].plttRes);
            i++;
        } while (i < man->maxEntries);
    }
    i = 0;
    do {
        Destroy2DGfxResObjMan(man->resMan[i]);
        i++;
    } while (i < 4);
    Heap_Free(man->entries);
    if (man->ownsSpriteList != FALSE) {
        SpriteList_Delete(man->ownedSpriteList);
    }
    Heap_Free(man);
}
