#include "global.h"
#include "constants/heap.h"

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

SoftwareSprite *sub_0201605C(int count, enum HeapID heapID);

void sub_020161CC(SoftwareSprite *sprite);

void sub_020161CC(SoftwareSprite *sprite) {
    G3_PushMtx();
    G3_MaterialColorDiffAmb(sprite->diffuse, (((31) << 10) | ((31) << 5) | (31)), 1);
    G3_MaterialColorSpecEmi((((16) << 10) | ((16) << 5) | (16)), (((0) << 10) | ((0) << 5) | (0)), 0);
    G3_TexImageParam(
        sprite->attributes->fmt,
        GX_TEXGEN_TEXCOORD,
        sprite->attributes->sizeS,
        sprite->attributes->sizeT,
        GX_TEXREPEAT_NONE,
        GX_TEXFLIP_NONE,
        sprite->attributes->plttUse,
        sprite->vramAddressChars);
    G3_TexPlttBase(sprite->vramAddressPalette + ((sprite->paletteSlot) * ((16) * sizeof(u16))), sprite->attributes->fmt);
    G3_PolygonAttr(GX_LIGHTMASK_NONE, GX_POLYGONMODE_MODULATE, GX_CULL_NONE, 0, sprite->alpha, 0);

    NNSG2dSVec2 uv0, uv1;
    if (sprite->flipH) {
        uv0.x = sprite->dimensions.x;
        uv1.x = 0;
    } else {
        uv1.x = sprite->dimensions.x;
        uv0.x = 0;
    }

    if (sprite->flipV) {
        uv0.y = sprite->dimensions.y;
        uv1.y = 0;
    } else {
        uv1.y = sprite->dimensions.y;
        uv0.y = 0;
    }

    G3_Translate(
        (sprite->position.x + sprite->center.x) * ((fx32)0x0000000000001000L),
        (sprite->position.y + sprite->center.y) * ((fx32)0x0000000000001000L),
        sprite->priority * ((fx32)0x0000000000001000L));
    G3_RotZ(FX_SinIdx(sprite->rotation), FX_CosIdx(sprite->rotation));
    G3_Scale(sprite->scaleX, sprite->scaleY, ((fx32)0x0000000000001000L));
    G3_Translate(-sprite->center.x * ((fx32)0x0000000000001000L), -sprite->center.y * ((fx32)0x0000000000001000L), 0);
    NNS_G2dDrawSpriteFast(0, 0, 0, sprite->dimensions.x, sprite->dimensions.y, uv0.x, uv0.y, uv1.x, uv1.y);
    G3_PopMtx(1);
}
