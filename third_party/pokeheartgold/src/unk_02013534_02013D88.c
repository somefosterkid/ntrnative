#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "obj_char_transfer.h"
#include "sprite.h"

typedef struct UnkStruct_02013034_t {
    int unk_00;
    int unk_04;
    int unk_08;
    struct UnkStruct_02013034_t *unk_0C;
    struct UnkStruct_02013034_t *unk_10;
} UnkStruct_02013034;

int sub_02013DE0(const Window *param0, const UnkStruct_02013034 *param1, char *param2, int param3, int param4, int param5, int param6);

void sub_02013D88(const Window *param0, char *param1, const UnkStruct_02013034 *param2, int param3, int param4);

void sub_02013A50(const Window *window, int width, int height, int x, int y, char *output);
extern const u8 _020F5F2C[12][2];
int sub_02013E24(const UnkStruct_02013034 *param0, int param1);

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
Sprite *sub_02013EF0(const UnkStruct_020127E8 *param0, const UnkStruct_02013034 *param1, const NNSG2dImageProxy *param2);
void sub_02013E78(const UnkStruct_020127E8 *param0, const UnkStruct_02013034 *param1, const NNSG2dImageProxy *param2, FontOAM *param3);
void sub_02013ECC(FontOAM *param0);

void sub_02013D88(const Window *param0, char *param1, const UnkStruct_02013034 *param2, int param3, int param4) {
    UnkStruct_02013034 *v0;
    int v2;
    int v3;
    GXOBJVRamModeChar v4;

    if (param3 == NNS_G2D_VRAM_TYPE_2DMAIN) {
        v4 = GX_GetOBJVRamModeChar();
    } else {
        v4 = GXS_GetOBJVRamModeChar();
    }

    v2 = ObjCharTransfer_GetBlockSizeFromMode(v4);
    v3 = 0;
    v0 = param2->unk_0C;

    while (v0 != param2) {
        v3 = sub_02013DE0(param0, v0, param1, v3, v2, v4, param4);
        v0 = v0->unk_0C;
    }
}

int sub_02013DE0(const Window *param0, const UnkStruct_02013034 *param1, char *param2, int param3, int param4, int param5, int param6) {
    int v0;
    int v1 = _020F5F2C[param1->unk_08][0];
    int v2 = _020F5F2C[param1->unk_08][1];
    v0 = v1;
    v0 *= v2;

    if (v0 < param4) {
        v0 = param4;
    }

    v0 *= 32;
    sub_02013A50(param0, v1, v2, param1->unk_00, param1->unk_04, &param2[param3]);

    return param3 + v0;
}

int sub_02013E24(const UnkStruct_02013034 *param0, int param1) {
    UnkStruct_02013034 *v0;
    int v1;
    GXOBJVRamModeChar v2;
    int v3;
    int v4;
    int v5, v6;

    if (param1 == NNS_G2D_VRAM_TYPE_2DMAIN) {
        v2 = GX_GetOBJVRamModeChar();
    } else {
        v2 = GXS_GetOBJVRamModeChar();
    }

    v1 = ObjCharTransfer_GetBlockSizeFromMode(v2);
    v3 = 0;
    v0 = param0->unk_0C;

    while (v0 != param0) {
        v5 = _020F5F2C[v0->unk_08][0];
        v6 = _020F5F2C[v0->unk_08][1];
        v4 = v5 * v6;

        if (v4 < v1) {
            v4 = v1;
        }

        v3 += v4 * 32;
        v0 = v0->unk_0C;
    }

    return v3;
}

void sub_02013E78(const UnkStruct_020127E8 *param0, const UnkStruct_02013034 *param1, const NNSG2dImageProxy *param2, FontOAM *param3) {
    UnkStruct_02013034 *v0;
    int v1 = 0;
    v0 = param1->unk_0C;

    while (v0 != param1) {
        param3->unk_00[v1].unk_00 = sub_02013EF0(param0, v0, param2 + v1);
        ((param3->unk_00[v1].unk_00) ? (void)0 : GF_AssertFail());

        param3->unk_00[v1].unk_04 = v0->unk_00 * 8;
        param3->unk_00[v1].unk_08 = v0->unk_04 * 8;

        v0 = v0->unk_0C;
        v1++;
    }
}

void sub_02013ECC(FontOAM *param0) {
    int v0;

    for (v0 = 0; v0 < param0->unk_04; v0++) {
        Sprite_Delete(param0->unk_00[v0].unk_00);
    }
}
