#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "heap.h"
#include "sprite.h"

struct FontOAM;

struct UnkStruct_02012744_t;

typedef struct FontOAM FontOAM;

typedef struct UnkStruct_02012744_t UnkStruct_02012744;

typedef struct {
    const UnkStruct_02012744 *unk_00;
    const Window *unk_04;
    SpriteList *unk_08;
    const NNSG2dImagePaletteProxy *unk_0C;
    const Sprite *unk_10;
    int unk_14;
    int unk_18;
    int unk_1C;
    int unk_20;
    int unk_24;
    int unk_28;
    enum HeapID heapID;
} UnkStruct_020127E8;

typedef struct {
    Sprite *unk_00;
    int unk_04;
    int unk_08;
} UnkStruct_02012CE0_sub1;

struct FontOAM {
    UnkStruct_02012CE0_sub1 *unk_00;
    int unk_04;
    const Sprite *unk_08;
    int x;
    int y;
};

struct UnkStruct_02012744_t {
    void *unk_00[12];
    NNSG2dCellDataBank *unk_30[12];
    FontOAM *unk_60;
    int unk_64;
};

typedef struct UnkStruct_02013034_t {
    int unk_00;
    int unk_04;
    int unk_08;
    struct UnkStruct_02013034_t *unk_0C;
    struct UnkStruct_02013034_t *unk_10;
} UnkStruct_02013034;

FontOAM *sub_02013AD0(const UnkStruct_02012744 *param0);

int sub_02013BD4(int param0, int param1, enum HeapID heapID, UnkStruct_02013034 *param3);

void sub_02013C5C(const Window *param0, const UnkStruct_02013034 *param1, NNSG2dImageProxy *param2, int param3, int param4, enum HeapID heapID);

void sub_02013E78(const UnkStruct_020127E8 *param0, const UnkStruct_02013034 *param1, const NNSG2dImageProxy *param2, FontOAM *param3);

void sub_02013FA8(UnkStruct_02013034 *param0);

int sub_02013688(const Window *param0, int param1, enum HeapID heapID);

FontOAM *sub_020135D8(const UnkStruct_020127E8 *param0);

FontOAM *sub_020135D8(const UnkStruct_020127E8 *param0) {
    FontOAM *v0;
    UnkStruct_02013034 v1;
    int v2;
    NNSG2dImageProxy *v3;

    ((param0) ? (void)0 : GF_AssertFail());

    v0 = sub_02013AD0(param0->unk_00);
    ((v0) ? (void)0 : GF_AssertFail());

    v0->unk_08 = param0->unk_10;
    v0->x = param0->unk_18;
    v0->y = param0->unk_1C;

    v1.unk_0C = &v1;
    v1.unk_10 = &v1;

    v2 = sub_02013BD4(param0->unk_04->width, param0->unk_04->height, param0->heapID, &v1);
    v3 = Heap_AllocAtEnd(param0->heapID, sizeof(NNSG2dImageProxy) * v2);

    v0->unk_00 = Heap_Alloc(param0->heapID, sizeof(UnkStruct_02012CE0_sub1) * v2);
    v0->unk_04 = v2;

    sub_02013C5C(param0->unk_04, &v1, v3, param0->unk_14, param0->unk_28, param0->heapID);
    sub_02013E78(param0, &v1, v3, v0);
    Heap_Free(v3);
    sub_02013FA8(&v1);

    return v0;
}

void sub_02013AC0(FontOAM *param0);

void sub_02013ECC(FontOAM *param0);

void FontOAM_Delete(FontOAM *param0);

void FontOAM_Delete(FontOAM *param0) {
    ((param0) ? (void)0 : GF_AssertFail());
    ((param0->unk_00) ? (void)0 : GF_AssertFail());

    sub_02013ECC(param0);
    Heap_Free(param0->unk_00);
    sub_02013AC0(param0);
}

int sub_02013E24(const UnkStruct_02013034 *param0, int param1);

int sub_02013688(const Window *param0, int param1, enum HeapID heapID) {
    UnkStruct_02013034 v0;
    int v1;

    v0.unk_0C = &v0;
    v0.unk_10 = &v0;

    sub_02013BD4(param0->width, param0->height, heapID, &v0);
    v1 = sub_02013E24(&v0, param1);
    sub_02013FA8(&v0);

    return v1;
}
