#include "global.h"

#include "bg_window.h"
#include "overlay_75_unk_struct_ov98_02246_e88.h"
#include "overlay_manager.h"
#include "overlay_70_gtsapplication_state.h"
#include "save.h"
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

typedef struct ColoredArrow ColoredArrow;

typedef void (*CursorCallback)(ListMenu *menu, u32 index, u8 onInit);

typedef void (*PrintCallback)(ListMenu *menu, u32 index, u8 yOffset);

typedef struct Menu Menu;

SysTask *SysTask_ExecuteOnVBlank(SysTaskFunc callback, void *param, u32 priority);

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0);

static inline void inline_ov61_0222C3B0_sub(SysTask *param0, void *param1);

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

void ov75_02247D0C(UnkStruct_ov98_02247704 *param0);

void ov75_02247D0C(UnkStruct_ov98_02247704 *param0) {
    FillWindowPixelBuffer(&param0->unk_D4, 0xf0f);
    CopyWindowToVram(&param0->unk_D4);
}

void ov75_02247D24(UnkStruct_ov98_02247704 *param0);

void ov75_02247D24(UnkStruct_ov98_02247704 *param0) {
    if ((++param0->unk_BC) == 8) {
        param0->unk_C0 ^= 1;
        param0->unk_BC = 0;

        if (param0->unk_AC + 6 != param0->unk_B4) {
            FillBgTilemapRect(param0->unk_04, 3, 1 + (param0->unk_C0 * 20), 14, 17, 1, 1, 9);
            FillBgTilemapRect(param0->unk_04, 3, 2 + (param0->unk_C0 * 20), 15, 17, 1, 1, 9);
            FillBgTilemapRect(param0->unk_04, 3, 3 + (param0->unk_C0 * 20), 16, 17, 1, 1, 9);
            FillBgTilemapRect(param0->unk_04, 3, 4 + (param0->unk_C0 * 20), 17, 17, 1, 1, 9);
            FillBgTilemapRect(param0->unk_04, 3, 11 + (param0->unk_C0 * 20), 14, 18, 1, 1, 9);
            FillBgTilemapRect(param0->unk_04, 3, 12 + (param0->unk_C0 * 20), 15, 18, 1, 1, 9);
            FillBgTilemapRect(param0->unk_04, 3, 13 + (param0->unk_C0 * 20), 16, 18, 1, 1, 9);
            FillBgTilemapRect(param0->unk_04, 3, 14 + (param0->unk_C0 * 20), 17, 18, 1, 1, 9);
        } else {
            FillBgTilemapRect(param0->unk_04, 3, 0, 14, 17, 4, 2, 9);
        }

        if (param0->unk_AC != 0) {
            FillBgTilemapRect(param0->unk_04, 3, 5 + (param0->unk_C0 * 20), 14, 3, 1, 1, 9);
            FillBgTilemapRect(param0->unk_04, 3, 6 + (param0->unk_C0 * 20), 15, 3, 1, 1, 9);
            FillBgTilemapRect(param0->unk_04, 3, 7 + (param0->unk_C0 * 20), 16, 3, 1, 1, 9);
            FillBgTilemapRect(param0->unk_04, 3, 8 + (param0->unk_C0 * 20), 17, 3, 1, 1, 9);
            FillBgTilemapRect(param0->unk_04, 3, 15 + (param0->unk_C0 * 20), 14, 4, 1, 1, 9);
            FillBgTilemapRect(param0->unk_04, 3, 16 + (param0->unk_C0 * 20), 15, 4, 1, 1, 9);
            FillBgTilemapRect(param0->unk_04, 3, 17 + (param0->unk_C0 * 20), 16, 4, 1, 1, 9);
            FillBgTilemapRect(param0->unk_04, 3, 18 + (param0->unk_C0 * 20), 17, 4, 1, 1, 9);
        } else {
            FillBgTilemapRect(param0->unk_04, 3, 0, 14, 3, 4, 2, 9);
        }

        ScheduleBgTilemapBufferTransfer(param0->unk_04, 3);
    }
}
