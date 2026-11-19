#include "global.h"

#include "gf_gfx_loader.h"
#include "overlay_00_thumb.h"
#include "overlay_manager.h"
#include "unk_02037C94.h"
#include "overlay_75_unk_struct_ov98_02246_e88.h"
#include "overlay_70_gtsapplication_state.h"
#include "overlay_75_unk_struct_ov98_02247704.h"
#include "msgdata.h"
#include "overlay_73_wifi_battle_tower_app_state.h"

struct OverlayManager;

struct ColoredArrow;

struct Menu;


struct ListMenuItem;

struct MessageFormat;

typedef struct ColoredArrow ColoredArrow;

typedef struct Menu Menu;

void ov70_02237F2C(void);

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

void sub_0203A914(void);

void ov75_022494CC(UnkStruct_ov98_02247704 *param0, MsgData *param1, int param2, int param3, u16 param4);

int ov75_02249534(int param0);

int ov75_022490D8(UnkStruct_ov98_02247704 *param0);

int ov75_022490D8(UnkStruct_ov98_02247704 *param0) {
    if (ov75_02249534(param0->unk_44) == 1) {
        return 0;
    }

    switch (param0->unk_94) {
    case 0:
        ov75_022494CC(param0, param0->unk_34, 15, 1, 0xf0f);
        param0->unk_94++;
        break;
    case 1:

        DWC_CleanupInet();

        sub_0203A914();
        sub_0203957C();
        Sys_ClearSleepDisableFlag(4);

        if (param0->unk_00->unk_11C == 1) {
            ov70_02237F2C();
            param0->unk_00->unk_11C = 0;
        }

        param0->unk_94++;
        break;
    case 2:
        if (ov75_02249534(param0->unk_44) == 0) {
            ov75_022494CC(param0, param0->unk_34, 16, 1, 0xf0f);
            param0->unk_94++;
        }
        break;
    case 3:
        if (ov75_02249534(param0->unk_44) == 0) {
            param0->unk_94++;
        }
        break;
    case 4:
        param0->unk_90++;

        if (param0->unk_90 > 30) {
            param0->unk_08 = param0->unk_0C;
        }
        break;
    }

    return 0;
}

void ov75_0224971C(UnkStruct_ov98_02247704 *param0, int param1, int param2);

int ov75_022491CC(UnkStruct_ov98_02247704 *param0);

int ov75_022491CC(UnkStruct_ov98_02247704 *param0) {
    int v0 = ov00_021E6A70(-param0->unk_18, param0->unk_1C);

    ov75_0224971C(param0, v0, -param0->unk_18);

    param0->unk_08 = 28;

    return 0;
}
