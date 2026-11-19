#include "global.h"

#include "field/overlay_01_021EAFD4.h"

#include "heap.h"

void ov01_021EB18C(FieldTextureManager *textureManager, int slotIndex) {
    if (textureManager == 0L) {
        return;
    }

    if (textureManager->textureSlots[slotIndex].narcData) {
        Heap_Free(textureManager->textureSlots[slotIndex].narcData);
    }

    if (textureManager->textureSlots[slotIndex].animationData != 0L) {
        textureManager->textureSlots[slotIndex].animationData = 0L;
    }

    textureManager->textureSlots[slotIndex].currentFrame = 0;
    textureManager->textureSlots[slotIndex].frameCounter = 0;
}
