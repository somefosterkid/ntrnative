#include "global.h"

#include "constants/heap.h"

#include "field/overlay_01_021EAFD4.h"

#include "error_handling.h"
#include "field_types_def.h"
#include "heap.h"

FieldTextureManager *FieldTextureManager_Init(void) {
    FieldTextureManager *pFVar1;
    s32 iVar2;

    pFVar1 = Heap_Alloc(HEAP_ID_FIELD1, sizeof(FieldTextureManager));
    if (pFVar1 == NULL) {
        GF_AssertFail();
    }
    iVar2 = 0;
    do {
        pFVar1->textureSlots[iVar2].currentFrame = 0;
        pFVar1->textureSlots[iVar2].frameCounter = 0;
        pFVar1->textureSlots[iVar2].vramOffset = 0;
        pFVar1->textureSlots[iVar2].textureResource = NULL;
        pFVar1->textureSlots[iVar2].animationData = NULL;
        pFVar1->textureSlots[iVar2].narcData = NULL;
        iVar2 = iVar2 + 1;
    } while (iVar2 < 0x10);
    pFVar1->keepTextures = 0;
    return pFVar1;
}
