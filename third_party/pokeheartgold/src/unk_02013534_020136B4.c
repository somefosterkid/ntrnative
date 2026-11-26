#include "global.h"
#include "constants/heap.h"

struct BgConfig;
struct TextOBJ;
struct Sprite;
struct UnkStruct_02012B20_t;
struct Window;
typedef struct TextOBJ TextOBJ;
typedef struct UnkStruct_02012B20_t UnkStruct_02012B20;
typedef struct Window Window;
typedef struct Sprite Sprite;
void Sprite_SetMatrix(Sprite *sprite, const VecFx32 *position);
const VecFx32 *Sprite_GetMatrixPtr(const Sprite *sprite);
typedef struct {
    Sprite *unk_00;
    int unk_04;
    int unk_08;
} UnkStruct_02012CE0_sub1;
struct TextOBJ {
    UnkStruct_02012CE0_sub1 *unk_00;
    int unk_04;
    const Sprite *unk_08;
    int x;
    int y;
};
typedef struct UnkStruct_02013034_t {
    int unk_00;
    int unk_04;
    int unk_08;
    struct UnkStruct_02013034_t *unk_0C;
    struct UnkStruct_02013034_t *unk_10;
} UnkStruct_02013034;
struct UnkStruct_02012B20_t {
    UnkStruct_02013034 unk_00;
    int unk_14;
};
void Sprite_SetDrawFlag(Sprite *sprite, BOOL draw);
#if defined(SDK_PORT) || defined(NONMATCHING)
void Sprite_SetPriority(Sprite *sprite, u8 draw);
#else
void Sprite_SetPriority(Sprite *sprite, BOOL draw);
#endif
void Sprite_SetDrawPriority(Sprite *sprite, BOOL draw);
void Sprite_SetPaletteOverride(Sprite *sprite, BOOL draw);
void Sprite_SetPalIndexRespectVramOffset(Sprite *sprite, BOOL draw);
void Sprite_SetPalOffset(Sprite *sprite, BOOL draw);
#if defined(SDK_PORT) || defined(NONMATCHING)
void Sprite_SetPalOffsetRespectVramOffset(Sprite *sprite, u8 draw);
#else
void Sprite_SetPalOffsetRespectVramOffset(Sprite *sprite, BOOL draw);
#endif
typedef struct BgConfig BgConfig;
struct Window {
    BgConfig *bgConfig;
    u8 bgLayer;
    u8 tilemapLeft;
    u8 tilemapTop;
    u8 width;
    u8 height;
    u8 palette;
    u16 baseTile : 15;
    u16 colorMode : 1;
    void *pixels;
};
void *Heap_Alloc(u32 heapID, u32 size);
int sub_02013BD4(int param0, int param1, enum HeapID heapID, UnkStruct_02013034 *param3);

void sub_020136B4(TextOBJ *fontOAM, int x, int y);
void sub_02013728(TextOBJ *param0);
void sub_02013794(const TextOBJ *fontOAM, int *x, int *y);
void TextOBJ_SetSpritesDrawFlag(TextOBJ *param0, BOOL draw);
void sub_020137F0(TextOBJ *param0, BOOL draw);
void sub_02013820(TextOBJ *param0, BOOL draw);
void TextOBJ_SetPaletteNum(TextOBJ *param0, BOOL draw);
void sub_02013880(TextOBJ *param0, BOOL draw);
void sub_020138B0(TextOBJ *param0, BOOL draw);
void sub_020138E0(TextOBJ *param0, BOOL draw);
UnkStruct_02012B20 *sub_02013910(const Window *param0, enum HeapID heapID);

void sub_020136B4(TextOBJ *fontOAM, int x, int y) {
    ((fontOAM) ? (void)0 : GF_AssertFail());

    fontOAM->x = x;
    fontOAM->y = y;

    x *= ((fx32)0x0000000000001000L);
    y *= ((fx32)0x0000000000001000L);

    if (fontOAM->unk_08) {
        const VecFx32 *fontPos = Sprite_GetMatrixPtr(fontOAM->unk_08);

        x += fontPos->x;
        y += fontPos->y;
    }

    VecFx32 spritePos;
    spritePos.z = 0;

    for (int v0 = 0; v0 < fontOAM->unk_04; v0++) {
        spritePos.x = x + (fontOAM->unk_00[v0].unk_04 << 12);
        spritePos.y = y + (fontOAM->unk_00[v0].unk_08 << 12);

        Sprite_SetMatrix(fontOAM->unk_00[v0].unk_00, &spritePos);
    }
}

void sub_02013728(TextOBJ *param0) {
    int v0;
    VecFx32 v1;
    const VecFx32 *v2;
    fx32 v3, v4;

    ((param0) ? (void)0 : GF_AssertFail());

    if (param0->unk_08) {
        v3 = param0->x << 12;
        v4 = param0->y << 12;
        v2 = Sprite_GetMatrixPtr(param0->unk_08);

        v3 += v2->x;
        v4 += v2->y;

        v1.z = 0;

        for (v0 = 0; v0 < param0->unk_04; v0++) {
            v1.x = v3 + (param0->unk_00[v0].unk_04 << 12);
            v1.y = v4 + (param0->unk_00[v0].unk_08 << 12);

            Sprite_SetMatrix(param0->unk_00[v0].unk_00, &v1);
        }
    }
}

void sub_02013794(const TextOBJ *fontOAM, int *x, int *y) {
    ((fontOAM) ? (void)0 : GF_AssertFail());
    ((x) ? (void)0 : GF_AssertFail());
    ((y) ? (void)0 : GF_AssertFail());

    *x = fontOAM->x;
    *y = fontOAM->y;
}

void TextOBJ_SetSpritesDrawFlag(TextOBJ *param0, BOOL draw) {
    int v0;

    ((param0) ? (void)0 : GF_AssertFail());

    for (v0 = 0; v0 < param0->unk_04; v0++) {
        Sprite_SetDrawFlag(param0->unk_00[v0].unk_00, draw);
    }
}

void sub_020137F0(TextOBJ *param0, BOOL draw) {
    int v0;

    ((param0) ? (void)0 : GF_AssertFail());

    for (v0 = 0; v0 < param0->unk_04; v0++) {
        Sprite_SetPriority(param0->unk_00[v0].unk_00, draw);
    }
}

void sub_02013820(TextOBJ *param0, BOOL draw) {
    int v0;

    ((param0) ? (void)0 : GF_AssertFail());

    for (v0 = 0; v0 < param0->unk_04; v0++) {
        Sprite_SetDrawPriority(param0->unk_00[v0].unk_00, draw);
    }
}

void TextOBJ_SetPaletteNum(TextOBJ *param0, BOOL draw) {
    int v0;

    ((param0) ? (void)0 : GF_AssertFail());

    for (v0 = 0; v0 < param0->unk_04; v0++) {
        Sprite_SetPaletteOverride(param0->unk_00[v0].unk_00, draw);
    }
}

void sub_02013880(TextOBJ *param0, BOOL draw) {
    int v0;

    ((param0) ? (void)0 : GF_AssertFail());

    for (v0 = 0; v0 < param0->unk_04; v0++) {
        Sprite_SetPalIndexRespectVramOffset(param0->unk_00[v0].unk_00, draw);
    }
}

void sub_020138B0(TextOBJ *param0, BOOL draw) {
    int v0;

    ((param0) ? (void)0 : GF_AssertFail());

    for (v0 = 0; v0 < param0->unk_04; v0++) {
        Sprite_SetPalOffset(param0->unk_00[v0].unk_00, draw);
    }
}

void sub_020138E0(TextOBJ *param0, BOOL draw) {
    int v0;

    ((param0) ? (void)0 : GF_AssertFail());

    for (v0 = 0; v0 < param0->unk_04; v0++) {
        Sprite_SetPalOffsetRespectVramOffset(param0->unk_00[v0].unk_00, draw);
    }
}

UnkStruct_02012B20 *sub_02013910(const Window *param0, enum HeapID heapID) {
    UnkStruct_02012B20 *v0 = Heap_Alloc(heapID, sizeof(UnkStruct_02012B20));

    v0->unk_00.unk_0C = &v0->unk_00;
    v0->unk_00.unk_10 = &v0->unk_00;
    v0->unk_14 = sub_02013BD4(param0->width, param0->height, heapID, &v0->unk_00);

    return v0;
}
