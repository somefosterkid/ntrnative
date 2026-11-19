#include "global.h"

#include "bg_window.h"
#include "render_window.h"
#include "overlay_75_unk_struct_ov98_02246_e88.h"
#include "overlay_manager.h"
#include "options.h"
#include "pm_string.h"
#include "msgdata.h"
#include "overlay_73_wifi_battle_tower_app_state.h"
#include "list_menu_items.h"
#include "list_menu.h"
#include "overlay_75_unk_struct_ov98_02247704.h"

struct OverlayManager;

struct OverlayManagerTemplate;

struct BgConfig;

struct ColoredArrow;

struct DWCstInetControl;

struct ListMenu;

struct ListMenuTemplate;

struct Menu;

struct MsgData;

struct NARC;

struct Options;

struct SaveData;

struct String;

struct ListMenuItem;

struct MessageFormat;

struct SysTask;

struct SysTaskQueue;

struct Window;

struct WorldExchangeTrainer;

struct WorldExchangeTrainerError;

struct tagDWCWDSData;

typedef BOOL (*OverlayFunc)(OverlayManager *appMan, int *state);

void BlendPalette(const u16 *src, u16 *dest, u16 size, u8 fraction, u16 target);

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

void *GfGfxLoader_GetPlttDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, NNSG2dPaletteData **outPaletteData, enum HeapID heapID);

typedef u32 TextColor;

typedef struct ColoredArrow ColoredArrow;

typedef void (*CursorCallback)(ListMenu *menu, u32 index, u8 onInit);

typedef void (*PrintCallback)(ListMenu *menu, u32 index, u8 yOffset);

typedef struct Menu Menu;

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

void ov75_02249684(Window *param0, String *param1, int param2, int param3, int param4, TextColor param5);

void ov75_02247790(UnkStruct_ov98_02247704 *param0);

void ov75_02247790(UnkStruct_ov98_02247704 *param0) {
    AddWindowParameterized(param0->unk_04, &param0->unk_68, 0, 4, 4, 23, 16, 13, ((1 + (18 + 12)) + 9) + 27 * 4);
    FillWindowPixelBuffer(&param0->unk_68, 0x0);
    AddWindowParameterized(param0->unk_04, &param0->unk_58, 0, 4, 1, 24, 2, 13, (((1 + (18 + 12)) + 9) + 27 * 4) + 23 * 16);
    FillWindowPixelBuffer(&param0->unk_58, 0x0);

    ov75_02249684(&param0->unk_58, param0->unk_3C, 0, 1, 1, ((TextColor)(((15 & 0xFF) << 16) | ((14 & 0xFF) << 8) | ((0 & 0xFF) << 0))));

    AddWindowParameterized(param0->unk_04, &param0->unk_48, 0, 2, 19, 27, 4, 13, (1 + (18 + 12)) + 9);
    FillWindowPixelBuffer(&param0->unk_48, 0x0);
}

void ov75_02247838(UnkStruct_ov98_02247704 *param0);

void ov75_02247838(UnkStruct_ov98_02247704 *param0) {
    RemoveWindow(&param0->unk_48);
    RemoveWindow(&param0->unk_58);
    RemoveWindow(&param0->unk_68);
}

void ov75_02247854(void *arg0, s32 arg1, s32 arg2);

void ov75_02247854(void *arg0, s32 arg1, s32 arg2) {
    *((s32 *)&((UnkStruct_ov98_02247704 *)arg0)->unk_08) = arg1;
    *((s32 *)&((UnkStruct_ov98_02247704 *)arg0)->unk_0C) = arg2;
}

void ov75_0224785C(UnkStruct_ov98_02247704 *param0);

void ov75_0224785C(UnkStruct_ov98_02247704 *param0) {
    if (param0->unk_8C == NULL) {
        param0->unk_8C = WaitingIcon_New(&param0->unk_48, 1);
    }
}

void ov75_02247878(UnkStruct_ov98_02247704 *param0);

void ov75_02247878(UnkStruct_ov98_02247704 *param0) {
    if (param0->unk_8C != NULL) {
        sub_0200F450(param0->unk_8C);
        param0->unk_8C = NULL;
    }
}
