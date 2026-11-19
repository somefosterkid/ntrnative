#include "global.h"

#include "constants/heap.h"

#include "struct_defs/resdat_narc_structs.h"

#include "error_handling.h"
#include "filesystem.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "sprite_transfer.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_0200B150.h"
#include "vram_transfer_manager.h"
#include "overlay_43_palpad.h"

extern ObjCharTransferTemplate ov43_0222EDD8;

extern NNSG2dViewRect ov43_0222F0FC;

void ov43_0222A690(PalPadGfx *gfx, enum HeapID heapID);

void ov43_0222A690(PalPadGfx *gfx, enum HeapID heapID) {
    SpriteList *spriteList;
    BOOL ok;
    GfGfxResType type;
    ObjCharTransferTemplate template;

    NNS_G2dInitOamManagerModule();
    GF_CreateVramTransferManager(0x10, heapID);
    OamManager_Create(0, 0x7e, 0, 0x1e, 0, 0x7e, 0, 0x1e, heapID);
    template = ov43_0222EDD8;
    template.heapID = heapID;
    ObjCharTransfer_InitEx(&template, GX_OBJVRAMMODE_CHAR_1D_32K, GX_OBJVRAMMODE_CHAR_1D_32K);
    ObjPlttTransfer_Init(0x40, heapID);
    ObjCharTransfer_ClearBuffers();
    ObjPlttTransfer_Reset();
    spriteList = G2dRenderer_Init(0x40, &gfx->renderer, heapID);
    gfx->spriteList = spriteList;
    sub_0200B27C(&gfx->renderSurface, &ov43_0222F0FC, NNS_G2D_VRAM_TYPE_2DMAIN, &gfx->renderer.rendererInstance);
    {
        s32 i = 0;
        type = GF_GFX_RES_TYPE_CHAR;
        do {
            gfx->resourceManagers[i] = Create2DGfxResObjMan(0x40, type, heapID);
            type++;
            i++;
        } while ((s32)type < 4);
    }
    gfx->spriteResources[0] = AddCharResObjFromOpenNarc(gfx->resourceManagers[0], gfx->narc, 1, 1, 100, 1, heapID);
    gfx->spriteResources[1] = AddPlttResObjFromOpenNarc(gfx->resourceManagers[1], gfx->narc, 0, 0, 100, 1, 5, heapID);
    gfx->spriteResources[2] = AddCellOrAnimResObjFromOpenNarc(gfx->resourceManagers[2], gfx->narc, 2, 1, 100, GF_GFX_RES_TYPE_CELL, heapID);
    gfx->spriteResources[3] = AddCellOrAnimResObjFromOpenNarc(gfx->resourceManagers[3], gfx->narc, 3, 1, 100, GF_GFX_RES_TYPE_ANIM, heapID);
    ok = SpriteTransfer_CreateCharTransferTask_AllocAtEnd(gfx->spriteResources[0]);
    if (ok == 0) {
        GF_AssertFail();
    }
    ok = SpriteTransfer_CreatePlttTransferTask(gfx->spriteResources[1]);
    if (ok == 0) {
        GF_AssertFail();
    }
    sub_0200A740(gfx->spriteResources[0]);
    sub_0200A740(gfx->spriteResources[1]);
    CreateSpriteResourcesHeader(&gfx->spriteHeader, 100, 100, 100, 100, -1, -1, 0, 0, gfx->resourceManagers[0], gfx->resourceManagers[1], gfx->resourceManagers[2], gfx->resourceManagers[3], NULL, NULL);
}
