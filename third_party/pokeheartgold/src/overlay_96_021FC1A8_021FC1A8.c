#include "global.h"

#include "heap.h"
#include "overlay_96_ov96_icon_work.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"

void ov96_021FC1A8(Ov96IconWork *work) {
    s32 i;

    for (i = 0; i < 12; i++) {
        Sprite_Delete(work->icons[i].monIcon);
        Sprite_Delete(work->icons[i].teamMark);
        Sprite_Delete(work->icons[i].rank);
    }
    SpriteTransfer_DeleteCharTransferTask(work->charRes);
    SpriteTransfer_DeletePlttTransferTask(work->plttRes);
    for (i = 0; i < 6; i++) {
        Destroy2DGfxResObjMan(work->resMan[i]);
    }
    SpriteList_Delete(work->spriteList);
    Heap_Free(work);
}
