#include "global.h"
#include "constants/heap.h"
#include "filesystem.h"
#include "field/overlay_01_021FB878.h"
#include "overlay_73_wifi_battle_tower_app_state.h"

struct Field3DModelAnimation;

struct Field3dModel;

struct Field3dObject;

struct NARC;

struct SysTask;

struct SysTaskQueue;

void BlendPalette(const u16 *src, u16 *dest, u16 size, u8 fraction, u16 target);

typedef union {
    struct {
        f32 val1_00;
        f32 val1_04;
        f32 val1_08;
        f32 val1_0C;
    };
    f32 val2[4];
} UnkUnion_ov116_02266FEC;

typedef union {
    struct {
        f32 val1_00;
        f32 val1_04;
        f32 val1_08;
        f32 val1_0C;
        f32 val1_10;
        f32 val1_14;
        f32 val1_18;
        f32 val1_1C;
        f32 val1_20;
        f32 val1_24;
        f32 val1_28;
        f32 val1_2C;
        f32 val1_30;
        f32 val1_34;
        f32 val1_38;
        f32 val1_3C;
    };
    f32 val2[4][4];
    f32 val3[16];
} UnkUnion_ov116_022671EC;

typedef struct {
    Field3dObject unk_00;
    Field3dModel unk_78;
    Field3DModelAnimation unk_88[10];
    UnkUnion_ov116_022671EC unk_150;
    UnkUnion_ov116_02266FEC unk_190;
    UnkUnion_ov116_02266FEC unk_1A0;
    MtxFx43 unk_1B0;
    VecFx32 unk_1E0;
    BOOL unk_1EC;
    BOOL unk_1F0;
    int unk_1F4;
    int unk_1F8;
    f32 unk_1FC;
    f32 unk_200;
    f32 unk_204;
    f32 unk_208;
} UnkStruct_ov116_0226501C;

void ov92_022630F8(UnkUnion_ov116_02266FEC *param0, UnkUnion_ov116_02266FEC *param1);

void ov92_02263108(UnkUnion_ov116_02266FEC *param0, const UnkUnion_ov116_02266FEC *param1, const UnkUnion_ov116_02266FEC *param2);

f32 ov92_0226325C(const UnkUnion_ov116_02266FEC *param0);

void ov92_022632B4(UnkUnion_ov116_02266FEC *param0, const UnkUnion_ov116_02266FEC *param1, f32 param2);

void ov92_022632E8(UnkUnion_ov116_022671EC *param0, const UnkUnion_ov116_02266FEC *param1);

void ov92_02263824(const UnkUnion_ov116_022671EC *param0, MtxFx43 *param1);

void Heap_Free(void *ptr);

void *GfGfxLoader_GetPlttDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, NNSG2dPaletteData **outPaletteData, enum HeapID heapID);

SysTask *SysTask_ExecuteOnVBlank(SysTaskFunc callback, void *param, u32 priority);

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0);

static inline void inline_ov61_0222C3B0_sub(SysTask *param0, void *param1);

static inline void inline_ov61_0222C3B0(UnkStruct_ov61_0222C3B0 *param0, NARC *param1, int param2, enum HeapID heapID) {
    NNSG2dPaletteData *v0;
    void *v1;

    MI_CpuClear8(param0, sizeof(UnkStruct_ov61_0222C3B0));

    v1 = GfGfxLoader_GetPlttDataFromOpenNarc(param1, param2, &v0, heapID);

    MI_CpuCopy16(&((u16 *)(v0->pRawData))[0 * 16], param0->unk_08, 4 * 0x20);
    MI_CpuCopy16(&((u16 *)(v0->pRawData))[0 * 16], param0->unk_88, 4 * 0x20);
    Heap_Free(v1);
    inline_ov61_0222C3B0_sub_1(param0);

    param0->unk_04 = 1;
    param0->unk_00 = SysTask_ExecuteOnVBlank(inline_ov61_0222C3B0_sub, param0, 20);
}

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0) {
    int v0, v1, v2, v3, v4, v5;
    v1 = 0;

    for (v0 = 0; v0 < 4 - 1; v0++) {
        v2 = v0 + 1;
        v3 = 0;
        v5 = 0;
        do {
            ((v1 < (((16 << 8) / 0x300 + 2) * (4 - 1))) ? (void)0 : GF_AssertFail());
            for (v4 = 1; v4 < 1 + 15; v4++) {
                BlendPalette(&param0->unk_08[v0][v4], &param0->unk_88[v1][v4], 1, v3 >> 8, param0->unk_08[v2][v4]);
            }
            v1++;
            if (v5 == 1) {
                break;
            }
            v3 += 0x300;
            if (v3 >= (16 << 8)) {
                v3 = (16 << 8);
                v5 = 1;
            }
        } while (1);
    }
    DC_FlushRange(param0->unk_88, (((16 << 8) / 0x300 + 2) * (4 - 1)) * 16 * sizeof(u16));
}

static inline void inline_ov61_0222C3B0_sub(SysTask *param0, void *param1) {
    UnkStruct_ov61_0222C3B0 *v0 = param1;

    if (v0->unk_04 == 0) {
        return;
    }

    v0->unk_32B ^= 1;

    if (v0->unk_32B & 1) {
        return;
    }

    GX_LoadBGPltt((const void *)v0->unk_88[v0->unk_328], 0 * 0x20, 1 * 0x20);
    GXS_LoadBGPltt((const void *)v0->unk_88[v0->unk_328], 0 * 0x20, 1 * 0x20);

    if (v0->unk_32A == 0) {
        v0->unk_328++;
        if (v0->unk_328 >= (((16 << 8) / 0x300 + 2) * (4 - 1))) {
            v0->unk_328 = (((16 << 8) / 0x300 + 2) * (4 - 1)) - 2;
            v0->unk_32A ^= 1;
        }
    } else {
        v0->unk_328--;
        if (v0->unk_328 < 0) {
            v0->unk_328 = 1;
            v0->unk_32A ^= 1;
        }
    }
}

BOOL ov92_02260428(UnkStruct_ov116_0226501C *param0, int param1, int param2, int param3, int param4, f32 param5, BOOL param6);

BOOL ov92_02260428(UnkStruct_ov116_0226501C *param0, int param1, int param2, int param3, int param4, f32 param5, BOOL param6) {
    f32 v0;
    fx32 v1;
    f32 v2, v3;

    v2 = (param1 - param3) * ((1.33) / 256) * param5;
    v3 = (param2 - param4) * ((1.00) / 192) * param5;

    if (param6) {
        param0->unk_1FC = v2;
        param0->unk_200 = v3;
        param0->unk_1F8 = 8;
        param0->unk_204 = param0->unk_1FC / param0->unk_1F8;
        param0->unk_208 = param0->unk_200 / param0->unk_1F8;
    }

    {
        f32 v4 = ((v2 * v2) + (v3 * v3));
        fx32 v5 = ((fx32)(((v4) > 0) ? ((v4) * (1 << 12) + 0.5f) : ((v4) * (1 << 12) - 0.5f)));

        v1 = FX_Sqrt(v5);
        v0 = ((f32)((v1) / (f32)(1 << 12)));
    }

    if (v0 != 0.0) {
        fx32 v6 = v1;
        fx64c v7 = FX_SinFx64c(v6);
        fx64c v8 = FX_CosFx64c(v6);
        f32 v9 = ((f32)((v7) / (f32)((fx64c)1 << 32)));
        f32 v10 = ((f32)((v8) / (f32)((fx64c)1 << 32)));
        f32 v11 = v9 / v0;
        UnkUnion_ov116_02266FEC v12;

        v12.val2[0] = v10;
        v12.val2[1] = v3 * v11;
        v12.val2[2] = v2 * v11;
        v12.val2[3] = 0.0;

        ov92_02263108(&param0->unk_190, &v12, &param0->unk_1A0);
        ov92_022632E8(&param0->unk_150, &param0->unk_190);
        ov92_022630F8(&param0->unk_1A0, &param0->unk_190);
        ov92_02263824(&param0->unk_150, &param0->unk_1B0);

        {
            UnkUnion_ov116_02266FEC v13;
            f32 v14 = ov92_0226325C(&param0->unk_190);

            v13 = param0->unk_190;
            ov92_022632B4(&param0->unk_190, &v13, v14);
        }

        return 1;
    }

    return 0;
}

BOOL ov92_02260628(UnkStruct_ov116_0226501C *param0, f32 param1);

BOOL ov92_02260628(UnkStruct_ov116_0226501C *param0, f32 param1) {
    f32 v0;
    fx32 v1;
    f32 v2, v3;

    v2 = param0->unk_1FC;
    v3 = param0->unk_200;

    param0->unk_1FC -= param0->unk_204;
    param0->unk_200 -= param0->unk_208;
    param0->unk_1F8--;

    {
        f32 v4 = ((v2 * v2) + (v3 * v3));
        fx32 v5 = ((fx32)(((v4) > 0) ? ((v4) * (1 << 12) + 0.5f) : ((v4) * (1 << 12) - 0.5f)));

        v1 = FX_Sqrt(v5);
        v0 = ((f32)((v1) / (f32)(1 << 12)));
    }

    if (v0 != 0.0) {
        fx32 v6 = v1;
        fx64c v7 = FX_SinFx64c(v6);
        fx64c v8 = FX_CosFx64c(v6);
        f32 v9 = ((f32)((v7) / (f32)((fx64c)1 << 32)));
        f32 v10 = ((f32)((v8) / (f32)((fx64c)1 << 32)));
        f32 v11 = v9 / v0;
        UnkUnion_ov116_02266FEC v12;

        v12.val2[0] = v10;
        v12.val2[1] = v3 * v11;
        v12.val2[2] = v2 * v11;
        v12.val2[3] = 0.0;

        ov92_02263108(&param0->unk_190, &v12, &param0->unk_1A0);
        ov92_022632E8(&param0->unk_150, &param0->unk_190);
        ov92_022630F8(&param0->unk_1A0, &param0->unk_190);
        ov92_02263824(&param0->unk_150, &param0->unk_1B0);

        return 1;
    }

    return 0;
}
