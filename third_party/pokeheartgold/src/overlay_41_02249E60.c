#include "global.h"
#include "filesystem_files_def.h"
#include "constants/heap.h"
#include "bg_window.h"

struct BgConfig;
struct SysTask;
struct SysTaskQueue;
typedef struct {
    BgConfig *unk_00;
    enum NarcId narcID;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
    int unk_18;
    int unk_1C;
    int unk_20;
    int unk_24;
    int unk_28;
    enum HeapID heapID;
} UnkStruct_ov22_022599A0;
void *GfGfxLoader_GetScrnData(enum NarcId narcID, u32 narcMemberIdx, BOOL compressed, NNSG2dScreenData **outScreenData, enum HeapID heapID);
void Heap_Free(void *ptr);
void ov41_02249F7C(BgConfig *param0, int param1, const NNSG2dScreenData *param2, int param3, int param4, int param5, int param6, int param7, int param8);
void ov41_02249FFC(enum NarcId narcID, u32 param1, BgConfig *param2, u32 param3, u32 param4, enum HeapID heapID);
void ov41_0224A04C(enum NarcId narcID, u32 param1, int param2, u32 param3, u32 param4, enum HeapID heapID);
typedef struct {
    BgConfig *unk_00;
    enum NarcId narcID;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
    int unk_18;
    int unk_1C;
    int unk_20;
    int unk_24;
    int unk_28;
} UnkStruct_ov22_022597BC;
void ov41_02249C7C(UnkStruct_ov22_022597BC *param0, UnkStruct_ov22_022599A0 *param1);
void DestroySysTaskAndEnvironment(SysTask *task);
typedef struct {
    UnkStruct_ov22_022597BC *unk_00;
    UnkStruct_ov22_022599A0 unk_04;
    BOOL *unk_34;
    int unk_38;
    int unk_3C;
    int unk_40;
    int unk_44;
    int unk_48;
} UnkStruct_ov22_022598F4;
void CopyToBgTilemapRect(BgConfig *bgConfig, u8 bgLayer, u8 destX, u8 destY, u8 destWidth, u8 destHeight, const void *src, u8 srcX, u8 srcY, u8 srcWidth, u8 srcHeight);
void BgTilemapRectChangePalette(BgConfig *bgConfig, u8 bgLayer, u8 x, u8 y, u8 width, u8 height, u8 palette);
void ScheduleBgTilemapBufferTransfer(BgConfig *bgConfig, u8 bgLayer);
void ov41_02249E40(const NNSG2dScreenData *param0, int param1);

void ov41_02249E60(UnkStruct_ov22_022599A0 *param0, int *param1, int *param2);
void ov41_02249F0C(SysTask *param0, void *param1);
void ov41_02249F7C(BgConfig *param0, int param1, const NNSG2dScreenData *param2, int param3, int param4, int param5, int param6, int param7, int param8);

void ov41_02249E60(UnkStruct_ov22_022599A0 *param0, int *param1, int *param2) {
    int v0;
    NNSG2dScreenData *v1;
    void *v2;
    int v3, v4;
    int v5 = param0->unk_14 / 8;
    int v6 = param0->unk_18 / 8;

    ov41_02249FFC(param0->narcID, param0->unk_08, param0->unk_00, param0->unk_1C, param0->unk_28, param0->heapID);

    if (param0->unk_1C < 4) {
        v0 = 0;
    } else {
        v0 = 4;
    }

    ov41_0224A04C(param0->narcID, param0->unk_0C, v0, param0->unk_24 * 32, param0->unk_20 * 32, param0->heapID);

    v2 = GfGfxLoader_GetScrnData(param0->narcID, param0->unk_10, 0, &v1, param0->heapID);
    v4 = v1->screenWidth / 8;
    v3 = v1->screenHeight / 8;

    ov41_02249F7C(param0->unk_00, param0->unk_1C, v1, v4, v3, v5, v6, param0->unk_28, param0->unk_24);
    Heap_Free(v2);

    if (param1) {
        *param1 = v4;
    }

    if (param2) {
        *param2 = v3;
    }
}

void ov41_02249F0C(SysTask *param0, void *param1) {
    UnkStruct_ov22_022598F4 *v0 = param1;

    v0->unk_38--;

    if (v0->unk_38 >= 0) {
        ScheduleSetBgPosText(v0->unk_00->unk_00, v0->unk_00->unk_1C, BG_POS_OP_SUB_X, v0->unk_3C);
        ScheduleSetBgPosText(v0->unk_00->unk_00, v0->unk_00->unk_1C, BG_POS_OP_SUB_Y, v0->unk_40);
    } else {
        ScheduleSetBgPosText(v0->unk_00->unk_00, v0->unk_00->unk_1C, BG_POS_OP_SET_X, v0->unk_44);
        ScheduleSetBgPosText(v0->unk_00->unk_00, v0->unk_00->unk_1C, BG_POS_OP_SET_Y, v0->unk_48);
        ov41_02249C7C(v0->unk_00, &v0->unk_04);

        if (v0->unk_34) {
            *v0->unk_34 = 1;
        }

        DestroySysTaskAndEnvironment(param0);
    }
}

void ov41_02249F7C(BgConfig *param0, int param1, const NNSG2dScreenData *param2, int param3, int param4, int param5, int param6, int param7, int param8) {
    ov41_02249E40(param2, param7);
    CopyToBgTilemapRect(param0, param1, param5, param6, param3, param4, param2->rawData, 0, 0, param3, param4);
    BgTilemapRectChangePalette(param0, param1, param5, param6, param3, param4, param8);
    ScheduleBgTilemapBufferTransfer(param0, param1);
}
