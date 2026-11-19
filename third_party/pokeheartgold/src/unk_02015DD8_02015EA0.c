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

typedef struct SoftwareSpriteCharsTemplate {
    SoftwareSpriteManager *softSpriteMan;
    NNSG2dCharacterData *charsData;
} SoftwareSpriteCharsTemplate;

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

SoftwareSpriteChars *sub_02016118(SoftwareSpriteManager *softSpriteMan);

NNSGfdTexKey sub_02016170(NNSG2dCharacterData *charsData);

void sub_02016198(NNSG2dCharacterData *charsData, NNSGfdTexKey vramKey, NNSG2dImageProxy *proxy);

SoftwareSpriteChars *sub_02015EA0(const SoftwareSpriteCharsTemplate *template);

s32 sub_02016044(u32 *);
void sub_02015EDC(u32 *arg0);

SoftwareSpriteChars *sub_02015EA0(const SoftwareSpriteCharsTemplate *template) {
    SoftwareSpriteChars *chars = sub_02016118(template->softSpriteMan);
    GF_ASSERT(chars);
    GF_ASSERT(template->charsData->mappingType == GX_OBJVRAMMODE_CHAR_2D);

    chars->vramKey = sub_02016170(template->charsData);
    GF_ASSERT(chars->vramKey);

    sub_02016198(template->charsData, chars->vramKey, &chars->proxy);
    return chars;
}

void sub_02015EDC(u32 *arg0) {
    NNS_GfdDefaultFuncFreeTexVram(*arg0);
    sub_02016044(arg0);
}
