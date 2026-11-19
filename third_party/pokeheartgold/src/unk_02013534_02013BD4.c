#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "error_handling.h"
#include "sprite.h"
#include "obj_char_transfer.h"

typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
} UnkStruct_02012DE4;

typedef struct {
    UnkStruct_02012DE4 unk_00;
    UnkStruct_02012DE4 unk_10;
    u8 unk_20;
} UnkStruct_02012D34;

typedef struct UnkStruct_02013034_t {
    int unk_00;
    int unk_04;
    int unk_08;
    struct UnkStruct_02013034_t *unk_0C;
    struct UnkStruct_02013034_t *unk_10;
} UnkStruct_02013034;

int sub_02013AF8(int param0, int param1);

extern const u8 _020F5F2C[12][2];

BOOL sub_02013B24(UnkStruct_02012D34 *param0, UnkStruct_02013034 *param1, enum HeapID heapID);

int sub_02013BD4(int param0, int param1, enum HeapID heapID, UnkStruct_02013034 *param3);

int sub_02013CD0(const Window *param0, const UnkStruct_02013034 *param1, NNSG2dImageProxy *param2, int param3, int param4, int param5, int param6, enum HeapID heapID);
void sub_02013C5C(const Window *param0, const UnkStruct_02013034 *param1, NNSG2dImageProxy *param2, int param3, int param4, enum HeapID heapID);

int sub_02013BD4(int param0, int param1, enum HeapID heapID, UnkStruct_02013034 *param3) {
    UnkStruct_02012DE4 v0;
    UnkStruct_02012D34 v1;
    int v2;
    int v3;

    GF_ASSERT(param0);
    GF_ASSERT(param1);

    v3 = 0;

    v1.unk_00.unk_00 = 0;
    v1.unk_00.unk_04 = 0;
    v1.unk_00.unk_08 = param0;
    v1.unk_00.unk_0C = param1;
    v1.unk_20 = 0;

    v0.unk_04 = 0;
    v0.unk_08 = param0;

    while (v1.unk_00.unk_0C != 0) {
        v2 = sub_02013AF8(v1.unk_00.unk_08, v1.unk_00.unk_0C);

        v0.unk_00 = v1.unk_00.unk_00 + _020F5F2C[v2][1];
        v0.unk_0C = v1.unk_00.unk_0C - _020F5F2C[v2][1];
        v1.unk_00.unk_0C = _020F5F2C[v2][1];

        do {
            v3++;
        } while (sub_02013B24(&v1, param3, heapID) == 0);

        v1.unk_00 = v0;
    }

    return v3;
}

void sub_02013C5C(const Window *param0, const UnkStruct_02013034 *param1, NNSG2dImageProxy *param2, int param3, int param4, enum HeapID heapID) {
    UnkStruct_02013034 *v0;
    int v1;
    int v2;
    GXOBJVRamModeChar v3;

    if (param4 == NNS_G2D_VRAM_TYPE_2DMAIN) {
        v3 = GX_GetOBJVRamModeChar();
    } else {
        v3 = GXS_GetOBJVRamModeChar();
    }

    v2 = ObjCharTransfer_GetBlockSizeFromMode(v3);
    v1 = 0;
    v0 = param1->unk_0C;

    while (v0 != param1) {
        NNS_G2dInitImageProxy(param2 + v1);
        param3 = sub_02013CD0(param0, v0, param2 + v1, v2, v3, param3, param4, heapID);
        v0 = v0->unk_0C;
        v1++;
    }
}
