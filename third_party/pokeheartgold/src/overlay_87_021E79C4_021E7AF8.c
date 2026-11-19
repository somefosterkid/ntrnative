#include "global.h"

#include "overlay_87_scratch_off_card_app.h"

#include "constants/heap.h"

#include "struct_defs/resdat_narc_structs.h"

#include "gf_gfx_planes.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_0200B150.h"
#include "vram_transfer_manager.h"

void ov87_021E7BC4(ScratchOffCardsAppSpriteManager *spriteMan, int unused);

void ov87_021E7C50(ScratchOffCardsAppSpriteManager *spriteMan, s32 resourceID, s32 resourceID2, NNS_G2D_VRAM_TYPE vRamType);

void ov87_021E7CE0(ScratchOffCardsAppSpriteManager *spriteMan, int unused);

void ov87_021E7D6C(ScratchOffCardsAppSpriteManager *spriteMan, int unused);

void ov87_021E7EF0(void);

extern const u8 ov87_021E8394[4];

void ov87_021E7AF8(ScratchOffCardsAppSpriteManager *spriteMan);

void ov87_021E7AF8(ScratchOffCardsAppSpriteManager *spriteMan) {
    int i;
    GF_CreateVramTransferManager(0x20, HEAP_ID_122);
    ov87_021E7EF0();
    NNS_G2dInitOamManagerModule();
    OamManager_Create(0, 0x80, 0, 0x20, 0, 0x80, 0, 0x20, HEAP_ID_122);
    spriteMan->spriteList = G2dRenderer_Init(0x28, &spriteMan->g2dRenderer, HEAP_ID_122);
    G2dRenderer_SetSubSurfaceCoords(&spriteMan->g2dRenderer, 0, 0x200000);
    for (i = 0; i < 4; i++) {
        spriteMan->resourceCollections[i] = Create2DGfxResObjMan(ov87_021E8394[i], (GfGfxResType)i, HEAP_ID_122);
    }
    ov87_021E7BC4(spriteMan, 1);
    ov87_021E7C50(spriteMan, 1, 1, NNS_G2D_VRAM_TYPE_2DMAIN);
    ov87_021E7CE0(spriteMan, 1);
    ov87_021E7D6C(spriteMan, 1);
    ov87_021E7C50(spriteMan, 0, 0, NNS_G2D_VRAM_TYPE_2DSUB);
    for (i = 0; i < 5; i++) {
        SpriteTransfer_CreateCharTransferTask(spriteMan->resources[i][GF_GFX_RES_TYPE_CHAR]);
        SpriteTransfer_CreateExtPlttTransferTask(spriteMan->resources[i][GF_GFX_RES_TYPE_PLTT]);
    }
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
}
