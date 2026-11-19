#include "global.h"

#include "heap.h"
#include "overlay_01_021FB878.h"

void ov01_021FB878(void *resourceFile, NNSG3dResTex *texture) {
    u8 *textureData = (u8 *)texture + texture->texInfo.ofsTex;
    u32 strippedTextureDataSize = (u32)(textureData - (u8 *)resourceFile);

    Heap_Realloc(resourceFile, strippedTextureDataSize);
}
