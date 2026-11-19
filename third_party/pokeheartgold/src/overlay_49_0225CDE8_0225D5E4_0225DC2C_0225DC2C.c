#include "global.h"
#include "constants/heap.h"
#include "filesystem.h"
#include "field/overlay_01_021FB878.h"

struct Field3dModel;

struct NARC;

void ov45_0222D740(void *param0);

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
    Field3dModel unk_00[18];
    void *unk_120[18][3];
} UnkStruct_ov70_02261BB4;

UnkStruct_ov70_02261418 *ov70_022613F4(int param0, int param1, enum HeapID heapID);

void ov49_0225D528(Field3dModel *param0, NARC *param1, u32 param2, u32 heapID);

void ov49_0225DC2C(UnkStruct_ov70_02261BB4 *param0, NARC *param1, NNSFndAllocator *param2, const UnkStruct_ov70_02261418 *param3, u32 heapID);

void ov49_0225DC2C(UnkStruct_ov70_02261BB4 *param0, NARC *param1, NNSFndAllocator *param2, const UnkStruct_ov70_02261418 *param3, u32 heapID) {
    int v0, v1;

    for (v0 = 0; v0 < 18; v0++) {
        ov49_0225D528(&param0->unk_00[v0], param1, param3->unk_00[v0], heapID);

        if ((v0 != 11) && (v0 != 12)) {
            ov45_0222D740(param0->unk_00[v0].dataRaw);
        }

        for (v1 = 0; v1 < 3; v1++) {
            if (param3->unk_48[v0][v1] != param3->unk_00[v0]) {
                param0->unk_120[v0][v1] = GfGfxLoader_LoadFromOpenNarc((struct NARC *)((enum HeapID)(param1)), param3->unk_48[v0][v1], 0, (enum HeapID)(heapID), 0);
            } else {
                param0->unk_120[v0][v1] = 0L;
            }
        }
    }
}
