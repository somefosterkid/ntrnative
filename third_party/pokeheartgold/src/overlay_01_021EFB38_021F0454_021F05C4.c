#include "global.h"

#include "constants/heap.h"

#include "struct_defs/resdat_narc_structs.h"

#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_01_unk_struct_ov5_021de47c.h"

typedef enum GF_GraphicsResourceType GF_GraphicsResourceType;

void ov01_021F05C4(UnkStruct_ov5_021DE47C *param0, s32 param1, s32 param2);

void ov01_021F05C4(UnkStruct_ov5_021DE47C *param0, s32 param1, s32 param2) {
    SpriteList *pSVar1;
    SpriteResourceCollection *pSVar2;
    GfGfxResType type;

    pSVar1 = G2dRenderer_Init(param1, &param0->unk_04, HEAP_ID_FIELD1);
    param0->unk_00 = pSVar1;
    type = GF_GFX_RES_TYPE_CHAR;
    do {
        pSVar2 = (SpriteResourceCollection *)Create2DGfxResObjMan(param2, type, HEAP_ID_FIELD1);
        param0->unk_190[0] = pSVar2;
        type = (GF_GraphicsResourceType)(type + GF_GFX_RES_TYPE_PLTT);
        param0 = (UnkStruct_ov5_021DE47C *)&param0->unk_04;
    } while ((s32)type < 4);
    return;
}
