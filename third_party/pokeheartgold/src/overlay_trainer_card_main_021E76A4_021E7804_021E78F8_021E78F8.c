#include "global.h"

#include "error_handling.h"
#include "filesystem.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_0200B150.h"
#include "trainer_card_main.h"

s32 ov51_021E7D68(void);

extern u8 ov51_021E7FB8;

extern s32 ov51_021E7FBC;

extern u8 ov51_021E7FC4;

extern s32 ov51_021E7FDC;

typedef struct Words16 {
    u32 w[16];
} Words16;

typedef struct {
    s32 w[2];
} Words2x;

typedef struct Bytes8 {
    u8 b[2][4];
} Bytes8x;

void ov51_021E78F8(TrainerCardMainSprites *gfx);

void ov51_021E78F8(TrainerCardMainSprites *gfx) {
    Words2x *new_var;
    u32 sp2C[16];
    s32 sp24[2];
    u8 sp1C[2][4];
    NARC *narc;
    int i;
    u8 j;
    reg_GX_DISPCNT = 0x200010 | (reg_GX_DISPCNT & 0xFFCFFFEF);
    reg_GXS_DB_DISPCNT = 0x10 | (0xFFCFFFEF & reg_GXS_DB_DISPCNT);
    ov51_021E7D68();
    NNS_G2dInitOamManagerModule();
    OamManager_Create(0, 0x80, 0, 0x20, 0, 0x80, 0, 0x20, HEAP_ID_25);
    gfx->spriteList = G2dRenderer_Init(0x21, &gfx->renderer, HEAP_ID_25);
    G2dRenderer_SetSubSurfaceCoords(&gfx->renderer, 0, (fx32)((224 > 0) ? ((224 * (1 << 12)) + 0.5f) : ((224 * (1 << 12)) - 0.5f)));
    new_var = &(*((Words2x *)(&ov51_021E7FBC)));
    for (i = 0; i < 4; i++) {
        gfx->resourceManagers[0][i] = Create2DGfxResObjMan((&ov51_021E7FB8)[i], (GfGfxResType)i, HEAP_ID_25);
        gfx->resourceManagers[1][i] = Create2DGfxResObjMan((&ov51_021E7FB8)[i], (GfGfxResType)i, HEAP_ID_25);
    }

    i = 0;
    *((Words2x *)sp24) = *new_var;
    *((Bytes8x *)sp1C) = *((Bytes8x *)(&ov51_021E7FC4));
    for (; i < 2; i++) {
        gfx->resourceObjects[i][0] = AddCharResObjFromNarc(gfx->resourceManagers[i][0], NARC_a_0_4_9, sp1C[i][0], 0, i, sp24[i], HEAP_ID_25);
        gfx->resourceObjects[i][1] = AddPlttResObjFromNarc(gfx->resourceManagers[i][1], NARC_a_0_4_9, sp1C[i][1], 0, i, sp24[i], 0x10, HEAP_ID_25);
        gfx->resourceObjects[i][2] = AddCellOrAnimResObjFromNarc(gfx->resourceManagers[i][2], NARC_a_0_4_9, sp1C[i][2], 0, i, GF_GFX_RES_TYPE_CELL, HEAP_ID_25);
        gfx->resourceObjects[i][3] = AddCellOrAnimResObjFromNarc(gfx->resourceManagers[i][3], NARC_a_0_4_9, sp1C[i][3], 0, i, GF_GFX_RES_TYPE_ANIM, HEAP_ID_25);
        SpriteTransfer_CreateCharTransferTask(gfx->resourceObjects[i][0]);
        SpriteTransfer_CreateExtPlttTransferTask(gfx->resourceObjects[i][1]);
    }

    *((Words16 *)sp2C) = *((Words16 *)(&ov51_021E7FDC));
    narc = NARC_New(NARC_a_0_4_9, HEAP_ID_25);
    for (j = 0; j < 16; j++) {
        gfx->paletteFiles[j] = NARC_AllocAndReadWholeMember(narc, sp2C[j], HEAP_ID_25);
        if (gfx->paletteFiles[j] != 0L) {
            if (NNS_G2dGetUnpackedPaletteData(gfx->paletteFiles[j], &gfx->palettes[j]) == 0) {
                Heap_Free(gfx->palettes[j]);
                GF_AssertFail();
            }
        } else {
            GF_AssertFail();
        }
    }

    NARC_Delete(narc);
    GfGfx_EngineATogglePlanes(0x10U, 1U);
    GfGfx_EngineBTogglePlanes(0x10U, 1U);
}
