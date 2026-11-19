#include "global.h"
#include "constants/heap.h"
#include "filesystem.h"
#include "field/overlay_01_021FB878.h"

struct Field3DModelAnimation;

struct Field3dModel;

struct NARC;

void ov45_0222D740(void *param0);

void sub_020180BC(Field3DModelAnimation *anim, const Field3dModel *model, NARC *narc, u32 memberIndex, enum HeapID heapID, NNSFndAllocator *allocator);

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

typedef struct {
    Field3dModel unk_00[2];
    Field3DModelAnimation unk_20[5];
    BOOL unk_84[5];
} UnkStruct_ov70_022615A4;

UnkStruct_ov70_02261418 *ov70_022613F4(int param0, int param1, enum HeapID heapID);

void ov49_0225D528(Field3dModel *param0, NARC *param1, u32 param2, u32 heapID);

void ov49_0225D5FC(UnkStruct_ov70_022615A4 *param0, NARC *param1, const UnkStruct_ov70_02261418 *param2, u32 heapID, NNSFndAllocator *param4);

void ov49_0225D5FC(UnkStruct_ov70_022615A4 *param0, NARC *param1, const UnkStruct_ov70_02261418 *param2, u32 heapID, NNSFndAllocator *param4) {
    int v0;

    for (v0 = 0; v0 < 2; v0++) {
        ov49_0225D528(&param0->unk_00[v0], param1, param2->unk_180[v0], heapID);
        ov45_0222D740(param0->unk_00[v0].dataRaw);
    }

    for (v0 = 0; v0 < 5; v0++) {
        if (param2->unk_18C[v0] == param2->unk_180[0]) {
            param0->unk_84[v0] = 0;
        } else {
            param0->unk_84[v0] = 1;

            if (v0 != 3) {
                sub_020180BC(&param0->unk_20[v0], &param0->unk_00[0], param1, param2->unk_18C[v0], (enum HeapID)(heapID), param4);
            } else {
                sub_020180BC(&param0->unk_20[v0], &param0->unk_00[1], param1, param2->unk_18C[v0], (enum HeapID)(heapID), param4);
            }
        }
    }
}
