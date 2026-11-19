#include "global.h"

#include "error_handling.h"
#include "unk_02013534.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "unk_02013534_font_system.h"

struct TextOBJ;

struct Sprite;

struct UnkStruct_02012744_t;

struct UnkStruct_02012B20_t;

struct UnkStruct_02013034_t;

struct Window;

typedef struct UnkStruct_02012B20_t UnkStruct_02012B20;

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

struct UnkStruct_02012B20_t {
    UnkStruct_02013034 unk_00;
    int unk_14;
};

int sub_02013AF8(int param0, int param1);

UnkStruct_02013034 *sub_02013F78(enum HeapID heapID);

extern const u8 _020F5F2C[12][2];





int sub_02013BD4(int param0, int param1, enum HeapID heapID, UnkStruct_02013034 *param3);

void sub_02013C5C(const Window *param0, const UnkStruct_02013034 *param1, NNSG2dImageProxy *param2, int param3, int param4, enum HeapID heapID);

void sub_02013FC0(void *arg0, void *arg1);

BOOL sub_02013B24(UnkStruct_02012D34 *param0, UnkStruct_02013034 *param1, enum HeapID heapID);

BOOL sub_02013B24(UnkStruct_02012D34 *param0, UnkStruct_02013034 *param1, enum HeapID heapID) {
    UnkStruct_02013034 *v0;
    int v1;
    int v2;

    v0 = sub_02013F78(heapID);
    sub_02013FC0(v0, param1->unk_10);

    v0->unk_08 = sub_02013AF8(param0->unk_00.unk_08, param0->unk_00.unk_0C);
    v0->unk_00 = param0->unk_00.unk_04;
    v0->unk_04 = param0->unk_00.unk_00;

    v2 = param0->unk_00.unk_08 - _020F5F2C[v0->unk_08][0];
    v1 = param0->unk_00.unk_0C - _020F5F2C[v0->unk_08][1];

    if (v2 > 0) {
        param0->unk_10.unk_0C = param0->unk_00.unk_0C;
        param0->unk_10.unk_08 = v2;
        param0->unk_10.unk_00 = param0->unk_00.unk_00;
        param0->unk_10.unk_04 = param0->unk_00.unk_04 + _020F5F2C[v0->unk_08][0];

        GF_ASSERT(param0->unk_20 != 1);
        param0->unk_20 = 1;
    }

    if (v1 > 0) {
        param0->unk_00.unk_00 = param0->unk_00.unk_00 + _020F5F2C[v0->unk_08][1];
        param0->unk_00.unk_0C = v1;
    } else {
        if (param0->unk_20 == 1) {
            param0->unk_00 = param0->unk_10;
            param0->unk_20 = 0;
        } else {
            return 1;
        }
    }

    return 0;
}
