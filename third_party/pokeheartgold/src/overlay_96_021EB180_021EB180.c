#include "global.h"

#include "constants/heap.h"

#include "struct_defs/resdat_narc_structs.h"

#include "heap.h"
#include "overlay_96_ov96_sprite_mgr.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"

Ov96SpriteMgr *ov96_021EB180(enum HeapID heapID, const Ov96SpriteMgrParam *param) {
    Ov96SpriteMgr *mgr;
    GfGfxResType type;
#if !(defined(SDK_PORT) || defined(NONMATCHING))
    u8 *p;
#endif

    mgr = Heap_Alloc(heapID, sizeof(Ov96SpriteMgr));
    MIi_CpuClearFast(0U, mgr, sizeof(Ov96SpriteMgr));
    mgr->heapID = heapID;
    mgr->param = *param;
    mgr->res = Heap_Alloc(heapID, sizeof(Ov96SpriteMgrRes) * mgr->param.maxRes);
    MIi_CpuClear32(0U, mgr->res, sizeof(Ov96SpriteMgrRes) * mgr->param.maxRes);
    mgr->slots = Heap_Alloc(mgr->heapID, sizeof(struct Ov96Obj) * mgr->param.maxSprites);
    MIi_CpuClear32(0U, mgr->slots, sizeof(struct Ov96Obj) * mgr->param.maxSprites);
    mgr->spriteList = G2dRenderer_Init(mgr->param.maxSprites, &mgr->renderer, mgr->heapID);
#if defined(SDK_PORT) || defined(NONMATCHING)
    for (type = GF_GFX_RES_TYPE_CHAR; type < OV96_SPRITE_RES_TYPES; type++) {
        mgr->resMan[type] = Create2DGfxResObjMan(mgr->param.resCounts[type], type, mgr->heapID);
    }
#else
    // DS form: walks the resMan[] slots by byte offset from the struct base (mwcc keeps 0x140 in a register)
    type = GF_GFX_RES_TYPE_CHAR;
    p = (u8 *)mgr;
    do {
        *(GF_2DGfxResMan **)(p + offsetof(Ov96SpriteMgr, resMan)) = Create2DGfxResObjMan(mgr->param.resCounts[type], type, mgr->heapID);
        type += 1;
        p += 4;
    } while (type < OV96_SPRITE_RES_TYPES);
#endif
    return mgr;
}
