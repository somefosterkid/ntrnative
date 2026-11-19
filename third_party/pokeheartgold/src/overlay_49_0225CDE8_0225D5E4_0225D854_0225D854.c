#include "global.h"
#include "constants/heap.h"
#include "filesystem.h"
#include "field/overlay_01_021FB878.h"

struct Field3DModelAnimation;

struct Field3dModel;

struct NARC;

void sub_020180BC(Field3DModelAnimation *anim, const Field3dModel *model, NARC *narc, u32 memberIndex, enum HeapID heapID, NNSFndAllocator *allocator);

void ov49_02258830(void **param0, NARC *param1, u32 param2, u32 heapID);

typedef struct {
    u32 unk_00[18];
    u32 unk_48[18][3];
    u32 unk_120[2];
    u32 unk_128[2][3];
    u32 unk_140[2][4];
    u32 unk_160[2];
    u32 unk_168[2][3];
    u32 unk_180[2];
    u32 unk_188;
    u32 unk_18C[5];
    u32 unk_1A0[5];
} UnkStruct_ov70_02261418;

void *GfGfxLoader_LoadFromOpenNarc(NARC *narc, u32 narcMemberIdx, BOOL compressed, enum HeapID heapID, BOOL allocAtEnd);

typedef struct {
    Field3dModel unk_00[2];
    void *unk_20[2][3];
    Field3DModelAnimation unk_38[2][4];
} UnkStruct_ov70_022618C8;

UnkStruct_ov70_02261418 *ov70_022613F4(int param0, int param1, enum HeapID heapID);

void ov49_0225D854(UnkStruct_ov70_022618C8 *param0, NARC *param1, NNSFndAllocator *param2, const UnkStruct_ov70_02261418 *param3, u32 heapID);

void ov49_0225D854(UnkStruct_ov70_022618C8 *param0, NARC *param1, NNSFndAllocator *param2, const UnkStruct_ov70_02261418 *param3, u32 heapID) {
    int v0, v1, v2;

    {
        for (v0 = 0; v0 < 2; v0++) {
            for (v1 = 0; v1 < 3; v1++) {
                ov49_02258830(&param0->unk_20[v0][v1], param1, param3->unk_128[v0][v1], heapID);
            }
        }
    }

    {
        for (v0 = 0; v0 < 2; v0++) {
            param0->unk_00[v0].dataRaw = GfGfxLoader_LoadFromOpenNarc(param1, param3->unk_120[v0], 0, (enum HeapID)(heapID), 0);
            param0->unk_00[v0].modelSet = NNS_G3dGetMdlSet(param0->unk_00[v0].dataRaw);
            param0->unk_00[v0].model = NNS_G3dGetMdlByIdx(param0->unk_00[v0].modelSet, 0);
            param0->unk_00[v0].texture = NNS_G3dGetTex(param0->unk_20[v0][0]);
        }
    }

    {
        for (v0 = 0; v0 < 2; v0++) {
            for (v1 = 0; v1 < 4; v1++) {
                if (param3->unk_120[0] != param3->unk_140[v0][v1]) {
                    sub_020180BC((struct Field3DModelAnimation *)((const struct Field3dModel *)(&param0->unk_38[v0][v1])), (const struct Field3dModel *)((struct NARC *)(&param0->unk_00[v0])), (struct NARC *)((enum HeapID)(param1)), param3->unk_140[v0][v1], (enum HeapID)(heapID), param2);

                    if (v1 >= 1) {
                        for (v2 = 0; v2 < param3->unk_160[v0]; v2++) {
                            if (param3->unk_168[v0][v1 - 1] != v2) {
                                NNS_G3dAnmObjDisableID(param0->unk_38[v0][v1].animObj, v2);
                            }
                        }
                    }
                }
            }
        }
    }
}
