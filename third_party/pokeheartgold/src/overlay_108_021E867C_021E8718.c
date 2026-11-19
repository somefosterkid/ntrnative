#include "global.h"

#include "sprite.h"
#include "sprite_transfer.h"
#include "unk_0200A090.h"

void ov108_021E8718(void *arg0);

void ov108_021E8718(void *arg0) {
    u32 v0;
    void *v1;

    v1 = arg0;
    SpriteList_Delete(*(SpriteList **)((u8 *)v1 + 0x10));
    SpriteTransfer_DeleteAllCharTransferTasks(*(GF_2DGfxResObjList **)((u8 *)v1 + 0x154));
    SpriteTransfer_DeleteAllPlttTransferTasks(*(GF_2DGfxResObjList **)((u8 *)v1 + 0x158));
    v0 = 0;
    do {
        Delete2DGfxResObjList(*(GF_2DGfxResObjList **)((u8 *)v1 + 0x154));
        Destroy2DGfxResObjMan(*(GF_2DGfxResMan **)((u8 *)v1 + 0x144));
        v0 += 1;
        v1 += 4;
    } while (v0 < 4U);
}
