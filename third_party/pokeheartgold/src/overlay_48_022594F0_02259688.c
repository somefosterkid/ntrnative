#include "global.h"

#include "gf_gfx_planes.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_0200B150.h"
#include "unk_0203A3B0.h"

extern ObjCharTransferTemplate ov48_0225B1A0;

void ov48_02259688(void *arg0, enum HeapID arg1);

void ov48_02259688(void *arg0, enum HeapID heapID) {
    GfGfxResType type;
    void *v0;

    v0 = arg0;
    NNS_G2dInitOamManagerModule();
    OamManager_Create(0, 0x7E, 0, 0x1F, 0, 0x7E, 0, 0x1F, heapID);
    ObjCharTransfer_InitEx(&ov48_0225B1A0, GX_OBJVRAMMODE_CHAR_1D_32K, GX_OBJVRAMMODE_CHAR_1D_32K);
    ObjPlttTransfer_Init(0x20, heapID);
    ObjCharTransfer_ClearBuffers();
    ObjPlttTransfer_Reset();
    G2dRenderer_SetObjCharTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN, GX_OBJVRAMMODE_CHAR_1D_32K);
    G2dRenderer_SetPlttTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN);
    *(SpriteList **)((u8 *)v0 + 4) = G2dRenderer_Init(0x20, v0 + 8, heapID);
    G2dRenderer_SetSubSurfaceCoords(v0 + 8, 0, 0x100000);
    type = GF_GFX_RES_TYPE_CHAR;
    do {
        *(GF_2DGfxResMan **)((u8 *)v0 + 0x130) = Create2DGfxResObjMan(0x20, type, heapID);
        type += 1;
        v0 += 4;
    } while (type < 4);
    sub_0203A880();
    GfGfx_EngineATogglePlanes(0x10U, 1U);
    GfGfx_EngineBTogglePlanes(0x10U, 1U);
}
