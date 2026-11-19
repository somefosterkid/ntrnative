#include "global.h"

#include "heap.h"

struct DotArtistGraphics;
typedef struct DotArtistGraphics DotArtistGraphics;

void FieldTextureManager_Destroy(DotArtistGraphics *graphics);

void FieldTextureManager_Destroy(DotArtistGraphics *graphics) {
    if (graphics != 0L) {
        Heap_Free(graphics);
    }
}
