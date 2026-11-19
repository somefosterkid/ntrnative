#include "global.h"

#include "constants/heap.h"

#include "error_handling.h"

struct SoftwareSprite;

struct SoftwareSpriteChars;

struct SoftwareSpriteManager;

struct SoftwareSpritePalette;

typedef struct SoftwareSpriteChars SoftwareSpriteChars;

typedef struct SoftwareSpritePalette SoftwareSpritePalette;

typedef struct SoftwareSprite SoftwareSprite;

typedef struct SoftwareSpriteManager SoftwareSpriteManager;

typedef struct SoftwareSpritePaletteTemplate {
    SoftwareSpriteManager *softSpriteMan;
    NNSG2dPaletteData *paletteData;
    u32 paletteSlot;
} SoftwareSpritePaletteTemplate;

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

SoftwareSpritePalette *sub_02016144(SoftwareSpriteManager *softSpriteMan);

NNSGfdPlttKey sub_02016184(int paletteSlot);

void sub_020161A8(NNSG2dPaletteData *paletteData, NNSGfdPlttKey vramKey, NNSG2dImagePaletteProxy *proxy);

SoftwareSpritePalette *sub_02015F1C(const SoftwareSpritePaletteTemplate *template);

s32 sub_02016050(u32 *);
void sub_02015F4C(u32 *arg0);

SoftwareSpritePalette *sub_02015F1C(const SoftwareSpritePaletteTemplate *template) {
    SoftwareSpritePalette *palette = sub_02016144(template->softSpriteMan);
    GF_ASSERT(palette);

    palette->vramKey = sub_02016184(template->paletteSlot);
    GF_ASSERT(palette->vramKey);

    sub_020161A8(template->paletteData, palette->vramKey, &palette->proxy);
    return palette;
}

void sub_02015F4C(u32 *arg0) {
    NNS_GfdDefaultFuncFreePlttVram(*arg0);
    sub_02016050(arg0);
}
