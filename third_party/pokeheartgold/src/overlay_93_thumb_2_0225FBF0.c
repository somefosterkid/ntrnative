#include "global.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"

struct BgConfig;

struct NARC;

typedef struct {
    int unk_00;
    int unk_04;
    s32 unk_08;
    s32 unk_0C;
    s32 unk_10;
    s32 unk_14;
    s32 unk_18[4];
    int unk_28;
    u8 unk_2C;
    u8 unk_2D;
    u8 padding_2E[2];
} UnkStruct_ov117_02263DAC;

typedef struct NARC NARC;

NARC *NARC_New(enum NarcId narcID, enum HeapID heapID, int);

void NARC_Delete(NARC *narc);

typedef struct BgConfig BgConfig;

u32 GfGfxLoader_LoadCharDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, BgConfig *bgConfig, u32 bgLayer, u32 offset, u32 size, BOOL compressed, u32 heapID);

void GfGfxLoader_LoadScrnDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, BgConfig *bgConfig, u32 bgLayer, u32 offset, u32 size, BOOL compressed, enum HeapID heapID);

BOOL PlaySE(u16 seqID);

typedef struct {
    u16 unk_00;
    u16 unk_02;
    fx32 unk_04;
} UnkStruct_ov117_02266BEC;

typedef struct {
    s32 unk_00[3];
} UnkStruct_ov117_02266ED4;

void ov93_0225FCA4(BgConfig *param0, UnkStruct_ov117_02263DAC *param1, int param2);

extern const UnkStruct_ov117_02266ED4 ov93_02262FD4[5];

extern const UnkStruct_ov117_02266BEC ov93_02262CEC[3];

void ov93_0225FBF0(BgConfig *param0, int param1, int param2, UnkStruct_ov117_02263DAC *param3);

void ov93_0225FBF0(BgConfig *param0, int param1, int param2, UnkStruct_ov117_02263DAC *param3) {
    NARC *v0 = NARC_New((enum NarcId)201, HEAP_ID_117, param2);

    GfGfxLoader_LoadCharDataFromOpenNarc(v0, ov93_02262CEC[param2].unk_00, param0, 7, 0, 0, 0, HEAP_ID_117);
    GfGfxLoader_LoadScrnDataFromOpenNarc(v0, ov93_02262CEC[param2].unk_02, param0, 7, 0, 0, 0, HEAP_ID_117);
    NARC_Delete(v0);

    MI_CpuClear8(param3, sizeof(UnkStruct_ov117_02263DAC));

    param3->unk_0C = ov93_02262FD4[param1].unk_00[param2];
    param3->unk_04 = param2;
    param3->unk_00 = 1;
    param3->unk_2C = 1;

    ov93_0225FCA4(param0, param3, param1);
    PlaySE(1515);
}
