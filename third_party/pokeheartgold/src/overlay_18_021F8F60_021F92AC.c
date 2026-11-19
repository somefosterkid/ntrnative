#include "global.h"

#include "sprite_transfer.h"
#include "unk_0200A090.h"

SpriteResource *ov18_021F92AC(GF_2DGfxResMan *arg0, enum HeapID arg1, NARC *arg2, s32 arg3, s32 arg4, s32 arg5);

SpriteResource *ov18_021F92AC(GF_2DGfxResMan *mgr, enum HeapID heapID, NARC *narc, s32 fileId, s32 pltt_num, s32 id) {
    SpriteResource *spriteResource;

    spriteResource = AddPlttResObjFromOpenNarc(mgr, narc, fileId, 0, id, 1, pltt_num, heapID);
    SpriteTransfer_CreatePlttTransferTask(spriteResource);
    sub_0200A740(spriteResource);
    return spriteResource;
}
