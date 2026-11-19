#include "global.h"

#include "sprite_transfer.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_0200B150.h"
#include "signature_app.h"

void ov52_021E870C(SignatureApp *signatureApp, NARC *narc);

void ov52_021E870C(SignatureApp *signatureApp, NARC *narc) {
    GfGfxResType type;
#if !(defined(SDK_PORT) || defined(NONMATCHING))
    struct GF_2DGfxResMan **v0;
#endif

    NNS_G2dInitOamManagerModule();
    OamManager_Create(0, 0x7E, 0, 0x20, 0, 0x7E, 0, 0x20, HEAP_ID_39);
    signatureApp->spriteList = G2dRenderer_Init(0x32, &signatureApp->g2DRenderer, HEAP_ID_39);
    G2dRenderer_SetSubSurfaceCoords(&signatureApp->g2DRenderer, 0, 0x100000);
    type = GF_GFX_RES_TYPE_CHAR;
#if defined(SDK_PORT) || defined(NONMATCHING)
    do {
        signatureApp->spriteResourceCollection[type] = Create2DGfxResObjMan(2, type, HEAP_ID_39);
        type += 1;
    } while (type < 4);
#else
    v0 = (struct GF_2DGfxResMan **)signatureApp;
    do {
        *(struct GF_2DGfxResMan **)((u8 *)v0 + offsetof(SignatureApp, spriteResourceCollection)) = Create2DGfxResObjMan(2, type, HEAP_ID_39);
        v0 += 1;
        type += 1;
    } while (type < 4);
#endif
    signatureApp->spriteResources[0][0] = AddCharResObjFromOpenNarc(signatureApp->spriteResourceCollection[0], narc, 7, 1, 0, 1, HEAP_ID_39);
    signatureApp->spriteResources[0][1] = AddPlttResObjFromOpenNarc(signatureApp->spriteResourceCollection[1], narc, 1, 0, 0, 1, 3, HEAP_ID_39);
    signatureApp->spriteResources[1][0] = AddCellOrAnimResObjFromOpenNarc(signatureApp->spriteResourceCollection[2], narc, 8, 1, 0, GF_GFX_RES_TYPE_CELL, HEAP_ID_39);
    signatureApp->spriteResources[1][1] = AddCellOrAnimResObjFromOpenNarc(signatureApp->spriteResourceCollection[3], narc, 9, 1, 0, GF_GFX_RES_TYPE_ANIM, HEAP_ID_39);
    signatureApp->spriteResources[2][0] = AddCharResObjFromOpenNarc(signatureApp->spriteResourceCollection[0], narc, 7, 1, 1, 2, HEAP_ID_39);
    signatureApp->spriteResources[2][1] = AddPlttResObjFromOpenNarc(signatureApp->spriteResourceCollection[1], narc, 1, 0, 1, 2, 3, HEAP_ID_39);
    signatureApp->spriteResources[3][0] = AddCellOrAnimResObjFromOpenNarc(signatureApp->spriteResourceCollection[2], narc, 8, 1, 1, GF_GFX_RES_TYPE_CELL, HEAP_ID_39);
    signatureApp->spriteResources[3][1] = AddCellOrAnimResObjFromOpenNarc(signatureApp->spriteResourceCollection[3], narc, 9, 1, 1, GF_GFX_RES_TYPE_ANIM, HEAP_ID_39);
    SpriteTransfer_CreateCharTransferTask(signatureApp->spriteResources[0][0]);
    SpriteTransfer_CreateCharTransferTask(signatureApp->spriteResources[2][0]);
    SpriteTransfer_CreateExtPlttTransferTask(signatureApp->spriteResources[0][1]);
    SpriteTransfer_CreateExtPlttTransferTask(signatureApp->spriteResources[2][1]);
}
