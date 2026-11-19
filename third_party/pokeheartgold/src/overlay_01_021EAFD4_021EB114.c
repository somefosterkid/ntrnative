#include "global.h"

#include "field/overlay_01_021EAFD4.h"

#include "filesystem.h"
#include "heap.h"
#include "vram_transfer_manager.h"

void *sub_02020838(NNSG3dResTex *texture, const u8 index);

void FieldTextureManager_Free(FieldTextureManager *textureManager) {
    s32 i;
    TextureSlot *slot;
    if (textureManager == ((void *)0) || textureManager->keepTextures != 0) {
        return;
    }
    i = 0;
    slot = textureManager->textureSlots;
    while (i < 16) {
        if (slot->narcData != ((void *)0) && slot->animationData != ((void *)0)) {
            if (slot->animationData->frames[slot->currentFrame][1] <= slot->frameCounter) {
                slot->frameCounter = 1;
                slot->currentFrame++;
                if (slot->animationData->frames[slot->currentFrame][0] == 0xFF) {
                    slot->currentFrame = 0;
                }
                GF_CreateNewVramTransferTask(NNS_GFD_DST_3D_TEX_VRAM, slot->vramOffset, sub_02020838(slot->textureResource, slot->animationData->frames[slot->currentFrame][0]), slot->vramSize);
            } else {
                slot->frameCounter++;
            }
        }
        i++;
        slot++;
    }
}
