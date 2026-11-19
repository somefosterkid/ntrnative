#include "global.h"

#include "bg_window.h"
#include "heap.h"
#include "gf_gfx_planes.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_0200B150.h"
#include "unk_0203A3B0.h"

extern u32 ov47_02259E68;

void ov47_02258B8C(void **param_1);

extern ObjCharTransferTemplate ov47_02259E58;
void ov47_02258BB4(void *arg0, enum HeapID arg1);

void ov47_02258B8C(void **param_1) {
    s32 iVar1;
    u32 *puVar2;

    puVar2 = (u32 *)&ov47_02259E68;
    iVar1 = 0;
    do {
        FreeBgTilemapBuffer((BgConfig *)*param_1, (u8)*puVar2);
        iVar1 = iVar1 + 1;
        puVar2 = puVar2 + 1;
    } while (iVar1 < 4);
    Heap_Free((void *)*param_1);
    return;
}

void ov47_02258BB4(void *arg0, enum HeapID heapID) {
    GfGfxResType type;
    void *v0;

    v0 = arg0;
    NNS_G2dInitOamManagerModule();
    OamManager_Create(0, 0x7E, 0, 0x1F, 0, 0x7E, 0, 0x1F, heapID);
    ObjCharTransfer_InitEx(&ov47_02259E58, GX_OBJVRAMMODE_CHAR_1D_32K, GX_OBJVRAMMODE_CHAR_1D_32K);
    ObjPlttTransfer_Init(0x20, heapID);
    ObjCharTransfer_ClearBuffers();
    ObjPlttTransfer_Reset();
    G2dRenderer_SetObjCharTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN, GX_OBJVRAMMODE_CHAR_1D_32K);
    G2dRenderer_SetPlttTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN);
    *(SpriteList **)((u8 *)v0 + 4) = G2dRenderer_Init(0x20, v0 + 8, heapID);
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
