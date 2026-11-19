#include "global.h"

#include "constants/heap.h"

struct SoftwareSprite;
struct SoftwareSpriteChars;
struct SoftwareSpriteManager;
struct SoftwareSpritePalette;
typedef struct SoftwareSpriteChars SoftwareSpriteChars;
typedef struct SoftwareSpritePalette SoftwareSpritePalette;
typedef struct SoftwareSprite SoftwareSprite;
typedef struct SoftwareSpriteManager SoftwareSpriteManager;
void sub_02015EDC(SoftwareSpriteChars *chars);
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

void sub_02015EF4(SoftwareSpriteManager *softSpriteMan);

void sub_02015EF4(SoftwareSpriteManager *softSpriteMan) {
    for (int i = 0; i < softSpriteMan->numChars; i++) {
        if (softSpriteMan->chars[i].vramKey) {
            sub_02015EDC(softSpriteMan->chars + i);
        }
    }
}
