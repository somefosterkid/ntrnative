#include "global.h"

#include "constants/heap.h"

#include "sys_task.h"
#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "palette.h"
#include "sys_task_api.h"
#include "overlay_07_bg_scroll_context.h"
#include "overlay_73_wifi_battle_tower_app_state.h"

void sub_02014AA0(void);

typedef void (*UnkFuncPtr_ov104_0223F110)(void *);

typedef struct {
    SysTask *unk_00;
    SysTask *unk_04;
    BOOL unk_08;
    BOOL unk_0C;
    UnkFuncPtr_ov104_0223F110 unk_10;
    UnkFuncPtr_ov104_0223F110 unk_14;
    void *unk_18;
} UnkStruct_ov104_0223F0D8;

void ov07_02222AF8(SysTask *param0, void *param1);

void ov07_02222B14(SysTask *param0, void *param1);
SysTask *SysTask_ExecuteOnVBlank(SysTaskFunc callback, void *param, u32 priority);
static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *);
static inline void inline_ov61_0222C3B0_sub(SysTask *, void *);
void ov07_02222B14(SysTask *param0, void *param1);
void ov07_02222B30(UnkStruct_ov104_0223F0D8 *param0, void *param1, UnkFuncPtr_ov104_0223F110 param2, UnkFuncPtr_ov104_0223F110 param3);
void ov07_02222B70(UnkStruct_ov104_0223F0D8 *param0);
void ov07_02222B94(VBlankDMAController *dmaController);

void ov07_02222AF8(SysTask *param0, void *param1) {
    UnkStruct_ov104_0223F0D8 *v0 = param1;

    if (v0->unk_08) {
        v0->unk_0C = 1;

        v0->unk_10(v0->unk_18);
    } else {
        sub_02014AA0();
    }
}

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

void ov07_02222B14(SysTask *param0, void *param1) {
    UnkStruct_ov104_0223F0D8 *v0 = param1;

    if (v0->unk_08 && v0->unk_0C) {
        v0->unk_14(v0->unk_18);

        v0->unk_0C = 0;
    }
}

void ov07_02222B30(UnkStruct_ov104_0223F0D8 *param0, void *param1, UnkFuncPtr_ov104_0223F110 param2, UnkFuncPtr_ov104_0223F110 param3) {
    ((param0) ? (void)0 : GF_AssertFail());

    param0->unk_08 = 1;
    param0->unk_0C = 0;
    param0->unk_18 = param1;
    param0->unk_10 = param2;
    param0->unk_14 = param3;
    param0->unk_04 = SysTask_CreateOnVWaitQueue(ov07_02222AF8, param0, 0);
    param0->unk_00 = SysTask_CreateOnVBlankQueue(ov07_02222B14, param0, 0);
}

void ov07_02222B70(UnkStruct_ov104_0223F0D8 *param0) {
    ((param0) ? (void)0 : GF_AssertFail());

    if (param0->unk_04 != 0L) {
        SysTask_Destroy(param0->unk_04);
    }

    if (param0->unk_00 != 0L) {
        SysTask_Destroy(param0->unk_00);
    }

    sub_02014AA0();
}

void ov07_02222B94(VBlankDMAController *dmaController) {
    ((dmaController) ? (void)0 : GF_AssertFail());
    dmaController->doDMA = 0;
}
