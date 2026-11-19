#include "global.h"

#include "constants/heap.h"

#include "struct_defs/resdat_narc_structs.h"

#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_0200B150.h"
#include "unk_02013534.h"
#include "overlay_91_unk_struct_ov115_02261_adc.h"

typedef enum GF_GraphicsResourceType GF_GraphicsResourceType;

extern ObjCharTransferTemplate ov91_02261C64;

void ov91_0225FA9C(u8 *param_1, enum HeapID param_2);

void ov91_0225FA9C(u8 *param_1, enum HeapID param_2) {
    SpriteList *pSVar1;
    GF_2DGfxResMan *pGVar2;
    UnkStruct_02013534 *pUVar3;
    GfGfxResType type;
    u8 *iVar4;
    ObjCharTransferTemplate OStack_24;

    NNS_G2dInitOamManagerModule();
    OamManager_Create(0, 0x7e, 0, 0x1f, 0, 0x7e, 0, 0x1f, param_2);
    OStack_24 = ov91_02261C64;
    OStack_24.heapID = param_2;
    ObjCharTransfer_InitEx(&OStack_24, GX_OBJVRAMMODE_CHAR_1D_128K, GX_OBJVRAMMODE_CHAR_1D_128K);
    ObjPlttTransfer_Init(0x10, param_2);
    ObjCharTransfer_ClearBuffers();
    ObjPlttTransfer_Reset();
    G2dRenderer_SetObjCharTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN, GX_OBJVRAMMODE_CHAR_1D_128K);
    G2dRenderer_SetPlttTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN);
    pSVar1 = G2dRenderer_Init(0x80, (G2dRenderer *)(param_1 + 0x20), param_2);
    ((UnkStruct_ov115_02261ADC *)param_1)->unk_1C = pSVar1;
    G2dRenderer_SetSubSurfaceCoords((G2dRenderer *)(param_1 + 0x20), 0, 0x200000);
    type = GF_GFX_RES_TYPE_CHAR;
    iVar4 = param_1;
    do {
        pGVar2 = Create2DGfxResObjMan(0x10, type, param_2);
        *(GF_2DGfxResMan **)(iVar4 + 0x148) = pGVar2;
        type = (GF_GraphicsResourceType)(type + GF_GFX_RES_TYPE_PLTT);
        iVar4 = iVar4 + 4;
    } while ((s32)type < 4);
    pUVar3 = FontSystem_NewInit(8, param_2);
    *((UnkStruct_02013534 **)&((UnkStruct_ov115_02261ADC *)param_1)->unk_1F0) = pUVar3;
    return;
}
