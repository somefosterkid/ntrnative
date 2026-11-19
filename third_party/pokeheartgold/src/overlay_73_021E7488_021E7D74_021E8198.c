#include "global.h"
struct WifiBattleTowerAppState;

#include "filesystem.h"
#include "sprite_transfer.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_0200B150.h"
#include "overlay_73_wifi_battle_tower_app_state.h"
#include "sprite.h"

void ov73_021E8198(WifiBattleTowerAppState *arg0);

void ov73_021E8198(WifiBattleTowerAppState *arg0) {
    NARC *narc;
    int i;

    narc = NARC_New(NARC_a_1_0_0, HEAP_ID_150);
    NNS_G2dInitOamManagerModule();
    OamManager_Create(0, 0x7E, 0, 0x20, 0, 0x7E, 0, 0x20, HEAP_ID_150);
    arg0->spriteList = G2dRenderer_Init(0xA, &arg0->renderer, HEAP_ID_150);
    G2dRenderer_SetSubSurfaceCoords(&arg0->renderer, 0, 0x100000);
    for (i = 0; i < 4; i++) {
        arg0->resourceCollection[i] = Create2DGfxResObjMan(2, (GfGfxResType)i, HEAP_ID_150);
    }
    arg0->spriteResource[0][0] = AddCharResObjFromOpenNarc(arg0->resourceCollection[0], narc, 0x2E, 1, 0, 1, HEAP_ID_150);
    arg0->spriteResource[0][1] = AddPlttResObjFromOpenNarc(arg0->resourceCollection[1], narc, 0xA, 0, 0, 1, 3, HEAP_ID_150);
    arg0->spriteResource[0][2] = AddCellOrAnimResObjFromOpenNarc(arg0->resourceCollection[2], narc, 0x2F, 1, 0, GF_GFX_RES_TYPE_CELL, HEAP_ID_150);
    arg0->spriteResource[1][0] = AddCellOrAnimResObjFromOpenNarc(arg0->resourceCollection[3], narc, 0x30, 1, 0, GF_GFX_RES_TYPE_ANIM, HEAP_ID_150);
    SpriteTransfer_CreateCharTransferTask(arg0->spriteResource[0][0]);
    SpriteTransfer_CreateExtPlttTransferTask(arg0->spriteResource[0][1]);
    NARC_Delete(narc);
}

void ov73_021E82A8(SpriteTemplate *arg0, WifiBattleTowerAppState *arg1, SpriteResourcesHeader *arg2, NNS_G2D_VRAM_TYPE arg3);

void ov73_021E82A8(SpriteTemplate *arg0, WifiBattleTowerAppState *arg1, SpriteResourcesHeader *arg2, NNS_G2D_VRAM_TYPE arg3) {
    arg0->spriteList = arg1->spriteList;
    arg0->header = arg2;
    arg0->position.z = 0;
    arg0->scale.x = 0x1000;
    arg0->scale.y = 0x1000;
    arg0->scale.z = 0x1000;
    arg0->rotation = 0;
    arg0->drawPriority = 1;
    arg0->whichScreen = arg3;
    arg0->heapID = HEAP_ID_150;
}
