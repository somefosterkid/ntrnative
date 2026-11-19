#include "global.h"

#include "heap.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "trainer_card_main.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_0200B150.h"

void ov51_021E7CA4(TrainerCardMainSprites *sprites) {
    u8 v1;
    u8 v2;

    v1 = 0;
    do {
        Heap_Free(sprites->paletteFiles[v1]);
        v1 += 1;
    } while ((u32)v1 < 0x10U);
    SpriteTransfer_DeleteCharTransferTask(sprites->resourceObjects[0][0]);
    SpriteTransfer_DeleteCharTransferTask(sprites->resourceObjects[1][0]);
    SpriteTransfer_DeletePlttTransferTask(sprites->resourceObjects[0][1]);
    SpriteTransfer_DeletePlttTransferTask(sprites->resourceObjects[1][1]);
    v2 = 0;
    do {
        Destroy2DGfxResObjMan(sprites->resourceManagers[0][v2]);
        Destroy2DGfxResObjMan(sprites->resourceManagers[1][v2]);
        v2 += 1;
    } while ((u32)v2 < 4U);
    G2dRenderer_SetSubSurfaceCoords(&sprites->renderer, 0, 0xC0000);
    SpriteList_Delete(sprites->spriteList);
    OamManager_Free();
    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
    reg_GX_DISPCNT = 0x10 | (reg_GX_DISPCNT & 0xFFCFFFEF);
}

void ov51_021E7D44(TrainerCardMainSprites *sprites, int buttonIdx, int animSeq, BOOL draw) {
    Sprite_SetAnimCtrlSeq(sprites->buttonSprites[buttonIdx], animSeq);
    Sprite_ResetAnimCtrlState(sprites->buttonSprites[buttonIdx]);
    Sprite_SetDrawFlag(sprites->buttonSprites[buttonIdx], draw);
}
