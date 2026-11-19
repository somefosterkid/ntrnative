#include "global.h"

#include "constants/heap.h"

#include "field/overlay_01_021EAFD4.h"

#include "filesystem.h"
#include "unk_0202068C.h"

int ov01_021EB058(FieldTextureManager *textureManager, NNSG3dResTex *textureResource, int textureIndex, NARC *narc);

int FieldTextureManager_LoadTexture(FieldTextureManager *textureManager, NNSG3dResTex *textureResource) {
    int loadedCount = 0;
    NARC *narc = NARC_New(NARC_a_1_3_9, HEAP_ID_FIELD1);

    textureManager->dataBuffer = NARC_AllocAndReadWholeMember(narc, 0, HEAP_ID_FIELD1);
    textureManager->dataHeader = textureManager->dataBuffer;

    for (int textureIndex = 0; textureIndex < textureManager->dataHeader->textureCount; textureIndex++) {
        if (ov01_021EB058(textureManager, textureResource, textureIndex, narc) >= 0) {
            loadedCount++;
        }
    }

    NARC_Delete(narc);

    return loadedCount;
}

int ov01_021EB058(FieldTextureManager *textureManager, NNSG3dResTex *textureResource, int textureIndex, NARC *narc) {
    int slotIndex;
    char debugString[40];

    if (textureManager == 0L) {
        return -1;
    }

    for (slotIndex = 0; slotIndex < 16; slotIndex++) {
        if (textureManager->textureSlots[slotIndex].narcData == 0L) {
            break;
        }
    }

    if (slotIndex == 16 || textureResource == 0L) {
        return -1;
    }

    textureManager->textureSlots[slotIndex].vramOffset = sub_020208DC(textureResource, textureManager->dataHeader->animationData[textureIndex].name);

    if (textureManager->textureSlots[slotIndex].vramOffset == 0) {
        return -1;
    }

    textureManager->textureSlots[slotIndex].vramSize = sub_020209E0(textureResource, textureManager->dataHeader->animationData[textureIndex].name);
    sprintf(debugString, "data/fld_anime%d.bin", textureIndex);
    textureManager->textureSlots[slotIndex].animationData = &(textureManager->dataHeader->animationData[textureIndex]);
    textureManager->textureSlots[slotIndex].narcData = NARC_AllocAndReadWholeMember(narc, textureIndex + 1, HEAP_ID_FIELD1);
    textureManager->textureSlots[slotIndex].textureResource = NNS_G3dGetTex((NNSG3dResFileHeader *)textureManager->textureSlots[slotIndex].narcData);

    return slotIndex;
}
