#include "global.h"

#include "error_handling.h"
#include "heap.h"
#include "overlay_96_ov96_sprite_mgr.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "unk_0200A090.h"

void ov96_021EB21C(Ov96SpriteMgr *mgr) {
    SpriteResource *charResObj;
    SpriteResource *plttResObj;
    u8 v1;
    u8 v2;

    v1 = 0;
    if (mgr->param.maxRes > 0) {
        do {
            charResObj = mgr->res[v1].res[GF_GFX_RES_TYPE_CHAR];
            if (charResObj != NULL) {
                SpriteTransfer_DeleteCharTransferTask(charResObj);
            }
            plttResObj = mgr->res[v1].res[GF_GFX_RES_TYPE_PLTT];
            if (plttResObj != NULL) {
                SpriteTransfer_DeletePlttTransferTask(plttResObj);
            }
            v1 += 1;
        } while ((s32)v1 < mgr->param.maxRes);
    }
    v2 = 0;
    do {
        Destroy2DGfxResObjMan(mgr->resMan[v2]);
        v2 += 1;
    } while ((u32)v2 < 4U);
    SpriteList_Delete(mgr->spriteList);
    Heap_Free(mgr->slots);
    Heap_Free(mgr->res);
    Heap_Free(mgr);
}

void ov96_021EB29C(Ov96SpriteMgr *mgr, s32 index, s32 resId) {
    if (index >= mgr->param.maxRes) {
        GF_AssertFail();
    }
    mgr->res[index].id = resId;
}

void ov96_021EB2BC(Ov96SpriteMgr *mgr, s32 narcId, s32 fileId, s32 arg3, s32 vram) {
    s32 id;
    SpriteResource **v0;

    id = arg3;
    v0 = ov96_021EB5EC(mgr, arg3, GF_GFX_RES_TYPE_CHAR);
    *v0 = AddCharResObjFromNarc(mgr->resMan[GF_GFX_RES_TYPE_CHAR], (NarcId)narcId, fileId, 0, id, vram, mgr->heapID);
}
