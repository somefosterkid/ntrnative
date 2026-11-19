#include "global.h"

#include "constants/heap.h"

#include "overlay_96_ov96_sprite_mgr.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "unk_0200A090.h"

void ov96_021EB334(Ov96SpriteMgr *mgr, s32 narcId, s32 fileId, s32 arg3) {
    s32 id;
    SpriteResource **v0;

    id = arg3;
    v0 = ov96_021EB5EC(mgr, arg3, GF_GFX_RES_TYPE_CELL);
    *v0 = AddCellOrAnimResObjFromNarc(mgr->resMan[GF_GFX_RES_TYPE_CELL], (NarcId)narcId, fileId, 0, id, GF_GFX_RES_TYPE_CELL, mgr->heapID);
}

void ov96_021EB36C(Ov96SpriteMgr *mgr, s32 narcId, s32 fileId, s32 arg3) {
    s32 id;
    SpriteResource **v0;

    id = arg3;
    v0 = ov96_021EB5EC(mgr, arg3, GF_GFX_RES_TYPE_ANIM);
    *v0 = AddCellOrAnimResObjFromNarc(mgr->resMan[GF_GFX_RES_TYPE_ANIM], (NarcId)narcId, fileId, 0, id, GF_GFX_RES_TYPE_ANIM, mgr->heapID);
}

void ov96_021EB3A4(Ov96SpriteMgr *mgr) {
    SpriteResource *charResObj;
    SpriteResource *obj;
    s32 v0;
    s32 v1;

    v1 = 0;
    if (mgr->param.maxRes > 0) {
        v0 = 0;
        do {
            charResObj = mgr->res[v0].res[GF_GFX_RES_TYPE_CHAR];
            if (charResObj != NULL) {
                SpriteTransfer_CreateCharTransferTask_AllocAtEnd(charResObj);
            }
            obj = mgr->res[v0].res[GF_GFX_RES_TYPE_PLTT];
            if (obj != NULL) {
                SpriteTransfer_CreateExtPlttTransferTask(obj);
            }
            v1 += 1;
            v0 += 1;
        } while (v1 < mgr->param.maxRes);
    }
}

// Creates the sprite and returns its ov96_021EB4F4 handle.
struct Ov96Obj *ov96_021EB3E4(Ov96SpriteMgr *mgr, s32 param_2, s32 vram, u8 resId, u8 group) {
    ov96_021EB408(mgr, param_2, vram, resId, group);
    return ov96_021EB4F4(mgr, resId, (u32)group);
}
