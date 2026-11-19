#include "global.h"
#include "constants/heap.h"
#include "filesystem.h"
#include "field/overlay_01_021FB878.h"

struct Field3DModelAnimation;

struct Field3dModel;

struct Field3dObject;

struct NARC;

void sub_02018030(Field3dModel *model, NARC *narc, u32 memberIndex, u32 heapID);

void sub_020180BC(Field3DModelAnimation *anim, const Field3dModel *model, NARC *narc, u32 memberIndex, enum HeapID heapID, NNSFndAllocator *allocator);

void sub_02018198(Field3DModelAnimation *anim, fx32 frame);

void sub_020181B0(Field3dObject *obj, Field3dModel *model);

void sub_020181D4(Field3dObject *obj, Field3DModelAnimation *anim);

void sub_020182A0(Field3dObject *obj, BOOL visible);

void Field3dObject_SetPosEx(Field3dObject *obj, fx32 x, fx32 y, fx32 z);

void sub_020182C4(Field3dObject *obj, fx32 x, fx32 y, fx32 z);

typedef struct {
    Field3dObject unk_00[3];
    Field3dModel unk_168[3];
    Field3DModelAnimation unk_198[6];
    u16 unk_210;
    u16 unk_212;
    Field3DModelAnimation unk_214[1];
    fx32 unk_228;
    u16 unk_22C;
    u16 unk_22E;
} UnkStruct_ov115_02263DF8;

void ov91_02260334(UnkStruct_ov115_02263DF8 *param0, u16 param1);

extern const NNSG3dResName ov91_022621AC[6][15];

extern const u8 ov91_0226274C[6];

extern const u8 _02261BE8[1];

void ov91_0225FF00(UnkStruct_ov115_02263DF8 *param0, NARC *param1, u32 heapID, NNSFndAllocator *param3);

void sub_020182A8(void *arg0, s32 arg1, s32 arg2, s32 arg3);

void ov91_0225FF00(UnkStruct_ov115_02263DF8 *param0, NARC *param1, u32 heapID, NNSFndAllocator *param3) {
    extern const u16 ov91_02261C1C[6];

    extern const u16 ov91_02261C04[3];

    int v2, v3, v5;
    const NNSG3dResName *v6;
    const NNSG3dResNodeInfo *v7;

    memset(param0, 0, sizeof(UnkStruct_ov115_02263DF8));

    for (v2 = 0; v2 < 3; v2++) {
        sub_02018030(&param0->unk_168[v2], param1, ov91_02261C04[v2], heapID);
        sub_020181B0(&param0->unk_00[v2], &param0->unk_168[v2]);
        sub_020182A0(&param0->unk_00[v2], 0);
        sub_020182A8(&param0->unk_00[v2], 0, -0x122000, 0);
        sub_020182C4(&param0->unk_00[v2], ((fx32)(((1.50f) > 0) ? ((1.50f) * (1 << 12) + 0.5f) : ((1.50f) * (1 << 12) - 0.5f))), ((fx32)(((1.50f) > 0) ? ((1.50f) * (1 << 12) + 0.5f) : ((1.50f) * (1 << 12) - 0.5f))), ((fx32)(((1.50f) > 0) ? ((1.50f) * (1 << 12) + 0.5f) : ((1.50f) * (1 << 12) - 0.5f))));
    }

    for (v2 = 0; v2 < 6; v2++) {
        sub_020180BC(&param0->unk_198[v2], &param0->unk_168[ov91_0226274C[v2]], param1, ov91_02261C1C[v2], (enum HeapID)(heapID), param3);
        sub_02018198(&param0->unk_198[v2], 0);

        v3 = 0;
        v7 = NNS_G3dGetNodeInfo(param0->unk_168[ov91_0226274C[v2]].model);

        while ((v6 = NNS_G3dGetNodeNameByIdx(v7, v3)) != 0L) {
            NNS_G3dAnmObjDisableID(param0->unk_198[v2].animObj, v3);
            v3++;
        }

        for (v3 = 0; v3 < 15; v3++) {
            v5 = NNS_G3dGetNodeIdxByName(v7, &ov91_022621AC[v2][v3]);

            if (v5 != -1) {
                NNS_G3dAnmObjEnableID(param0->unk_198[v2].animObj, v5);
            }
        }
    }

    for (v2 = 0; v2 < 1; v2++) {
        sub_020180BC(&param0->unk_214[v2], &param0->unk_168[_02261BE8[v2]], param1, 32, (enum HeapID)(heapID), param3);
        sub_020181D4(&param0->unk_00[_02261BE8[v2]], &param0->unk_214[v2]);
    }

    param0->unk_22C = 4;
    param0->unk_22E = 0;

    ov91_02260334(param0, 0);

    sub_020181D4(&param0->unk_00[1], &param0->unk_198[4]);
    sub_02018198(&param0->unk_198[4], 0);
    sub_020182A0(&param0->unk_00[1], 1);

    param0->unk_210 = 4;
}
