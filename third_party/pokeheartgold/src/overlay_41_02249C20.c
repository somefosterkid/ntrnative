#include "global.h"
#include "filesystem_files_def.h"
#include "constants/heap.h"
#include "overlay_41_unk_struct_ov22_02259560.h"
#include "bg_window.h"

struct BgConfig;
void ov41_02249B44(UnkStruct_ov22_02259560 *param0, int *param1, int *param2);
void ov41_02249B94(UnkStruct_ov22_02259560 *param0, int *param1, int *param2);
void ov41_02249BAC(UnkStruct_ov22_02259560 *param0, int *param1, int *param2, int *param3, int *param4);
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
void ov41_02249E60(UnkStruct_ov22_022599A0 *param0, int *param1, int *param2);
void BgClearTilemapBufferAndCommit(BgConfig *bgConfig, u8 bgLayer);
typedef struct {
    const UnkStruct_ov22_022597BC *unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
} UnkStruct_ov22_02259820;
typedef struct {
    const UnkStruct_ov22_022597BC *unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
} UnkStruct_ov22_0225AF44;

void ov41_02249C20(UnkStruct_ov22_02259560 *param0, int *param1, int *param2, int *param3, int *param4);
void ov41_02249C7C(UnkStruct_ov22_022597BC *param0, UnkStruct_ov22_022599A0 *param1);
void ov41_02249CC4(UnkStruct_ov22_022597BC *param0);
void ov41_02249CE0(UnkStruct_ov22_02259820 *param0, UnkStruct_ov22_0225AF44 *param1);

void ov41_02249C20(UnkStruct_ov22_02259560 *param0, int *param1, int *param2, int *param3, int *param4) {
    int v0, v1;
    int v2, v3;
    int v4, v5;
    int v6, v7;

    ov41_02249B94(param0, &v0, &v1);
    ov41_02249B44(param0, &v2, &v3);
    ov41_02249BAC(param0, &v4, &v6, &v5, &v7);

    *param1 = v3 + v5;
    *param2 = v3 + v1 - v7;
    *param3 = v2 + v4;
    *param4 = v2 + v0 - v6;
}

void ov41_02249C7C(UnkStruct_ov22_022597BC *param0, UnkStruct_ov22_022599A0 *param1) {
    param0->unk_00 = param1->unk_00;
    param0->unk_0C = param1->unk_14 / 8;
    param0->unk_10 = param1->unk_18 / 8;
    param0->unk_1C = param1->unk_1C;
    param0->unk_20 = param1->unk_24;
    param0->unk_24 = param1->unk_20;
    param0->unk_28 = param1->unk_28;
    param0->narcID = param1->narcID;
    param0->unk_08 = param1->unk_10;

    ov41_02249E60(param1, &param0->unk_14, &param0->unk_18);
}

void ov41_02249CC4(UnkStruct_ov22_022597BC *param0) {
    BgClearTilemapBufferAndCommit(param0->unk_00, param0->unk_1C);
    memset(param0, 0, sizeof(UnkStruct_ov22_022597BC));
}

void ov41_02249CE0(UnkStruct_ov22_02259820 *param0, UnkStruct_ov22_0225AF44 *param1) {
    param0->unk_00 = param1->unk_00;
    param0->unk_04 = param1->unk_04;
    param0->unk_08 = param1->unk_08;
    param0->unk_0C = 0;
    param0->unk_10 = 0;
    param0->unk_14 = param1->unk_0C;
}
