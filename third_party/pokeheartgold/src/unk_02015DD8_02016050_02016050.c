#include "global.h"
#include "constants/heap.h"

struct SoftwareSpriteChars;

typedef struct SoftwareSpriteChars SoftwareSpriteChars;

struct SoftwareSpriteChars {
    NNSGfdTexKey vramKey;
    NNSG2dImageProxy proxy;
};

SoftwareSpriteChars *sub_0201608C(int count, enum HeapID heapID);

void sub_02016050(SoftwareSpriteChars *chars);

void sub_02016050(SoftwareSpriteChars *chars) {
    chars->vramKey = 0;
    NNS_G2dInitImagePaletteProxy((struct NNSG2dImagePaletteProxy *)(&chars->proxy));
}
