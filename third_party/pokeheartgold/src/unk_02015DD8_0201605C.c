#include "global.h"

#include "constants/heap.h"

#include "heap.h"
#include "unk_02015DD8.h"

struct SoftwareSprite;
struct SoftwareSpriteChars;
struct SoftwareSpriteManager;
struct SoftwareSpritePalette;
typedef struct SoftwareSprite SoftwareSprite;
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
void sub_02016024(SoftwareSprite *sprite);
typedef struct SoftwareSpriteChars SoftwareSpriteChars;
struct SoftwareSpriteChars {
    NNSGfdTexKey vramKey;
    NNSG2dImageProxy proxy;
};
void sub_02016044(SoftwareSpriteChars *chars);
typedef struct SoftwareSpritePalette SoftwareSpritePalette;
struct SoftwareSpritePalette {
    NNSGfdPlttKey vramKey;
    NNSG2dImagePaletteProxy proxy;
};
void sub_02016050(SoftwareSpritePalette *palette);
typedef struct SoftwareSpriteManager SoftwareSpriteManager;
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
SoftwareSprite *sub_020160EC(SoftwareSpriteManager *softSpriteMan);
SoftwareSpriteChars *sub_02016118(SoftwareSpriteManager *softSpriteMan);
SoftwareSpritePalette *sub_02016144(SoftwareSpriteManager *softSpriteMan);

SoftwareSprite *sub_0201605C(int count, enum HeapID heapID) {
    SoftwareSprite *sprites = Heap_Alloc(heapID, sizeof(SoftwareSprite) * count);
    ((sprites) ? (void)0 : GF_AssertFail());

    for (int i = 0; i < count; i++) {
        sub_02016024(sprites + i);
    }

    return sprites;
}

SoftwareSpriteChars *sub_0201608C(int count, enum HeapID heapID) {
    SoftwareSpriteChars *chars = Heap_Alloc(heapID, sizeof(SoftwareSpriteChars) * count);
    ((chars) ? (void)0 : GF_AssertFail());

    for (int i = 0; i < count; i++) {
        sub_02016044(chars + i);
    }

    return chars;
}

SoftwareSpritePalette *sub_020160BC(int count, enum HeapID heapID) {
    SoftwareSpritePalette *palettes = Heap_Alloc(heapID, sizeof(SoftwareSpritePalette) * count);
    ((palettes) ? (void)0 : GF_AssertFail());

    for (int i = 0; i < count; i++) {
        sub_02016050(palettes + i);
    }

    return palettes;
}

SoftwareSprite *sub_020160EC(SoftwareSpriteManager *softSpriteMan) {
    for (int i = 0; i < softSpriteMan->numSprites; i++) {
        if (softSpriteMan->sprites[i].inUse == 0) {
            return softSpriteMan->sprites + i;
        }
    }

    return 0L;
}

SoftwareSpriteChars *sub_02016118(SoftwareSpriteManager *softSpriteMan) {
    for (int i = 0; i < softSpriteMan->numSprites; i++) {
        if (softSpriteMan->chars[i].vramKey == 0) {
            return softSpriteMan->chars + i;
        }
    }

    return 0L;
}

SoftwareSpritePalette *sub_02016144(SoftwareSpriteManager *softSpriteMan) {
    for (int i = 0; i < softSpriteMan->numSprites; i++) {
        if (softSpriteMan->palettes[i].vramKey == 0) {
            return softSpriteMan->palettes + i;
        }
    }

    return 0L;
}

NNSGfdTexKey sub_02016170(NNSG2dCharacterData *charsData) {
    return NNS_GfdAllocTexVram(charsData->szByte, 0, 0);
}

NNSGfdPlttKey sub_02016184(int paletteSlot) {
    return NNS_GfdAllocPlttVram(((paletteSlot) * ((16) * sizeof(u16))), 0, 0);
}

void sub_02016198(NNSG2dCharacterData *charsData, NNSGfdTexKey vramKey, NNSG2dImageProxy *proxy) {
    NNS_G2dLoadImage2DMapping(charsData, NNS_GfdGetTexKeyAddr(vramKey), NNS_G2D_VRAM_TYPE_3DMAIN, proxy);
}

void sub_020161A8(NNSG2dPaletteData *paletteData, NNSGfdPlttKey vramKey, NNSG2dImagePaletteProxy *proxy) {
    int size = paletteData->szByte;
    paletteData->szByte = NNS_GfdGetPlttKeySize(vramKey);

    NNS_G2dLoadPalette(paletteData, NNS_GfdGetPlttKeyAddr(vramKey), NNS_G2D_VRAM_TYPE_3DMAIN, proxy);
    paletteData->szByte = size;
}
