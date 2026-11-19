#include "global.h"

#include "error_handling.h"
#include "unk_02015DD8.h"
#include "constants/heap.h"
#include "field/overlay_01_02204004.h"

void *sub_020160EC(s32);

s32 sub_0201630C(void *, s32 *);

struct SoftwareSprite;
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
void sub_02015FB0(SoftwareSprite *sprite, BOOL visible);

void sub_02015FC4(void *arg0, s16 arg1, s16 arg2);

void *sub_02015F8C(s32 *arg0) {
    void *v0;

    v0 = sub_020160EC(*arg0);
    if (v0 == NULL) {
        GF_AssertFail();
    }
    sub_0201630C(v0, arg0);
    *((s32 *)&((MapPropAnimation *)v0)->looping) = 1;
    *(s32 *)((u8 *)v0 + 0x20) = 1;
    return v0;
}

void sub_02015FB0(SoftwareSprite *sprite, BOOL visible) {
    ((sprite) ? (void)0 : GF_AssertFail());
    sprite->visible = visible;
}

void sub_02015FC4(void *arg0, s16 arg1, s16 arg2) {
    *(s16 *)arg0 = arg1;
    *(s16 *)((u8 *)arg0 + 2) = arg2;
}
