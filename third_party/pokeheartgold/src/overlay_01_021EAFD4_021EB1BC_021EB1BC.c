#include "global.h"

#include "field/overlay_01_021EAFD4.h"

#include "field_types_def.h"
#include "heap.h"

void FieldTextureManager_FreeAllSlots(FieldTextureManager *textureManager) {
    s32 v0;

    if (textureManager != NULL) {
        v0 = 0;
        do {
            ov01_021EB18C(textureManager, v0);
            v0 += 1;
        } while (v0 < 0x10);
        Heap_Free(textureManager->dataBuffer);
    }
}
