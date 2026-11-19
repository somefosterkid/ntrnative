#include "global.h"
#include "error_handling.h"
#include "filesystem.h"
#include "heap.h"
#include "pokemon.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "unk_02013FDC.h"
#include "overlay_97_course_sub_screen.h"

void ov97_0221FB80(SpriteTemplate *arg0, SpriteList *arg1, SpriteResourcesHeader *arg2, NNS_G2D_VRAM_TYPE arg3, enum HeapID arg4);

void ov97_0221FB80(SpriteTemplate *arg0, SpriteList *arg1, SpriteResourcesHeader *arg2, NNS_G2D_VRAM_TYPE arg3, enum HeapID arg4) {
    arg0->spriteList = arg1;
    arg0->header = arg2;
    arg0->position.z = 0;
    arg0->scale.x = 0x1000;
    arg0->scale.y = 0x1000;
    arg0->scale.z = 0x1000;
    arg0->rotation = 0;
    arg0->drawPriority = 0;
    arg0->whichScreen = arg3;
    arg0->heapID = arg4;
}

void ov97_0221FBA0(Ov97CourseSubScreenSprites *arg0);

void ov97_0221FBA0(Ov97CourseSubScreenSprites *arg0) {
    NNSG2dImagePaletteProxy *pImg;
    NNSG2dImageProxy *nNSG2dImageProxy;
    SpriteResource *plttResObj;

    plttResObj = arg0->resObj[2][1];
    nNSG2dImageProxy = SpriteTransfer_GetCharProxy(arg0->resObj[2][0]);
    pImg = SpriteTransfer_GetPaletteProxy(plttResObj, nNSG2dImageProxy);
    arg0->charLocation = NNS_G2dGetImageLocation(nNSG2dImageProxy, NNS_G2D_VRAM_TYPE_2DSUB);
    arg0->plttLocation = NNS_G2dGetImagePaletteLocation(pImg, NNS_G2D_VRAM_TYPE_2DSUB);
}

void ov97_0221FBDC(Ov97CourseSubScreenSprites *arg0, void *arg1);

void ov97_0221FBDC(Ov97CourseSubScreenSprites *arg0, void *arg1) {
    PokepicTemplate pokepicTemplate;
    NNSG2dPaletteData *ppPltData;
    void *v0;

    GetMonSpriteCharAndPlttNarcIdsEx(&pokepicTemplate, *(u16 *)((u8 *)arg1 + 4), *(u8 *)((u8 *)arg1 + 8), 2U, (u8)(s32) * (u8 *)((u8 *)arg1 + 6), (u8)(s32) * (u8 *)((u8 *)arg1 + 7), *(u32 *)arg1);
    sub_02014540((enum NarcId)pokepicTemplate.narcID, (s32)pokepicTemplate.charDataID, arg0->heapID, arg0->charData, *(u32 *)arg1, 0, 2, (s32) * (u16 *)((u8 *)arg1 + 4));
    v0 = AllocAndReadWholeNarcMemberByIdPair((enum NarcId)pokepicTemplate.narcID, (s32)pokepicTemplate.palDataID, arg0->heapID);
    if (NNS_G2dGetUnpackedPaletteData(v0, &ppPltData) != 1) {
        GF_AssertFail();
    }
    MIi_CpuCopy16(ppPltData->pRawData, arg0->plttData, 0x20U);
    Heap_Free(v0);
}

void ov97_0221FC54(Ov97CourseSubScreenSprites *arg0);

void ov97_0221FC54(Ov97CourseSubScreenSprites *arg0) {
    DC_FlushRange(arg0->charData, 0xC80U);
    GXS_LoadOBJ(arg0->charData, arg0->charLocation, 0xC80U);
    DC_FlushRange(arg0->plttData, 0x20U);
    GXS_LoadOBJPltt(arg0->plttData, arg0->plttLocation, 0x20U);
}
