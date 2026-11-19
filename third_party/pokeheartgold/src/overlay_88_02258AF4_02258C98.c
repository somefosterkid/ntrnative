#include "global.h"

#include "gf_gfx_planes.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "overlay_88_wifi_plaza_news.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_0200B150.h"
#include "unk_0203A3B0.h"

extern ObjCharTransferTemplate ov88_02259914;

void ov88_02258C98(WiFiPlazaNewsGraphics *graphics, enum HeapID heapID);

void ov88_02258C98(WiFiPlazaNewsGraphics *graphics, enum HeapID heapID) {
    GfGfxResType type;
    void *v0;

    v0 = graphics;
    NNS_G2dInitOamManagerModule();
    OamManager_Create(0, 0x7E, 0, 0x1F, 0, 0x7E, 0, 0x1F, heapID);
    ObjCharTransfer_InitEx(&ov88_02259914, GX_OBJVRAMMODE_CHAR_1D_128K, GX_OBJVRAMMODE_CHAR_1D_32K);
    ObjPlttTransfer_Init(0x20, heapID);
    ObjCharTransfer_ClearBuffers();
    ObjPlttTransfer_Reset();
    G2dRenderer_SetObjCharTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN, GX_OBJVRAMMODE_CHAR_1D_128K);
    G2dRenderer_SetPlttTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN);
    graphics->spriteList = G2dRenderer_Init(0x20, &graphics->renderer, heapID);
    G2dRenderer_SetSubSurfaceCoords(&graphics->renderer, 0, 0x100000);
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
