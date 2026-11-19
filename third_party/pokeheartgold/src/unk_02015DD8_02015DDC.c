#include "global.h"

#include "constants/heap.h"

#include "heap.h"

struct SoftwareSprite;
struct SoftwareSpriteChars;
struct SoftwareSpriteManager;
struct SoftwareSpritePalette;
typedef struct SoftwareSpriteChars SoftwareSpriteChars;
typedef struct SoftwareSpritePalette SoftwareSpritePalette;
typedef struct SoftwareSprite SoftwareSprite;
typedef struct SoftwareSpriteManager SoftwareSpriteManager;
typedef struct SoftwareSpriteManagerTemplate {
    int numSprites;
    int numChars;
    int numPalettes;
    enum HeapID heapID;
} SoftwareSpriteManagerTemplate;
struct SoftwareSpriteChars {
    NNSGfdTexKey vramKey;
    NNSG2dImageProxy proxy;
};
struct SoftwareSpritePalette {
    NNSGfdPlttKey vramKey;
    NNSG2dImagePaletteProxy proxy;
};
struct SoftwareSprite {
    NNSG2dSVec2 position;
    NNSG2dSVec2 center;
    NNSG2dSVec2 dimensions;
    fx32 scaleX;
    fx32 scaleY;
    int priority;
    int rotation;
    BOOL inUse;
    BOOL visible;
    NNSG2dImageAttr *attributes;
    u32 vramAddressChars;
    u32 vramAddressPalette;
    u32 paletteSlot;
    BOOL flipH;
    BOOL flipV;
    GXRgb diffuse;
    u8 alpha;
};
struct SoftwareSpriteManager {
    SoftwareSprite *sprites;
    int numSprites;
    SoftwareSpriteChars *chars;
    int numChars;
    SoftwareSpritePalette *palettes;
    int numPalettes;
};
SoftwareSprite *sub_0201605C(int count, enum HeapID heapID);
SoftwareSpriteChars *sub_0201608C(int count, enum HeapID heapID);
SoftwareSpritePalette *sub_020160BC(int count, enum HeapID heapID);

SoftwareSpriteManager *sub_02015DDC(const SoftwareSpriteManagerTemplate *template);

void sub_020161CC(SoftwareSprite *sprite);
void sub_02015E20(SoftwareSpriteManager *softSpriteMan);
void sub_02015E64(SoftwareSpriteManager *manager);

SoftwareSpriteManager *sub_02015DDC(const SoftwareSpriteManagerTemplate *template) {
    SoftwareSpriteManager *softSpriteMan = Heap_Alloc(template->heapID, sizeof(SoftwareSpriteManager));
    ((softSpriteMan) ? (void)0 : GF_AssertFail());

    softSpriteMan->sprites = sub_0201605C(template->numSprites, template->heapID);
    softSpriteMan->numSprites = template->numSprites;
    softSpriteMan->chars = sub_0201608C(template->numChars, template->heapID);
    softSpriteMan->numChars = template->numChars;
    softSpriteMan->palettes = sub_020160BC(template->numPalettes, template->heapID);
    softSpriteMan->numPalettes = template->numPalettes;

    return softSpriteMan;
}

void sub_02015E20(SoftwareSpriteManager *softSpriteMan) {
    ((softSpriteMan) ? (void)0 : GF_AssertFail());
    ((softSpriteMan->sprites) ? (void)0 : GF_AssertFail());
    ((softSpriteMan->chars) ? (void)0 : GF_AssertFail());
    ((softSpriteMan->palettes) ? (void)0 : GF_AssertFail());

    Heap_Free(softSpriteMan->sprites);
    Heap_Free(softSpriteMan->chars);
    Heap_Free(softSpriteMan->palettes);
    Heap_Free(softSpriteMan);

    softSpriteMan = 0L;
}

void sub_02015E64(SoftwareSpriteManager *manager) {
    G3_PushMtx();

    for (int i = 0; i < manager->numSprites; i++) {
        if (manager->sprites[i].visible) {
            sub_020161CC(manager->sprites + i);
        }
    }

    G3_PopMtx(1);
}
