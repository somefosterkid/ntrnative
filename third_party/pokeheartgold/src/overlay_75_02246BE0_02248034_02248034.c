#include "global.h"
#include "heap.h"
#include "overlay_75_unk_struct_ov98_02246_e88.h"
#include "overlay_manager.h"
#include "save.h"
#include "options.h"
#include "pm_string.h"
#include "msgdata.h"
#include "font_types_def.h"
#include "font.h"
#include "overlay_73_wifi_battle_tower_app_state.h"
#include "list_menu_items.h"
#include "list_menu.h"
#include "overlay_75_unk_struct_ov98_02247704.h"
#include "screen_fade_internal.h"

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

struct TextPrinterTemplate;

struct Window;

struct WorldExchangeTrainer;

struct WorldExchangeTrainerError;

struct tagDWCWDSData;

typedef BOOL (*OverlayFunc)(OverlayManager *appMan, int *state);

void Heap_Free(void *ptr);


void String_Delete(String *string);



void BlendPalette(const u16 *src, u16 *dest, u16 size, u8 fraction, u16 target);

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};


void CopyWindowToVram(Window *window);

void FillWindowPixelBuffer(Window *window, u8 val);


void *GfGfxLoader_GetPlttDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, NNSG2dPaletteData **outPaletteData, enum HeapID heapID);

typedef BOOL (*TextPrinterCallback)(TextPrinterTemplate *, u16);

u8 AddTextPrinterParameterized(Window *window, u32 fontID, const String *string, u32 xOffset, u32 yOffset, u32 renderDelay, TextPrinterCallback callback);

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

static void ov75_02248034(UnkStruct_ov98_02247704 *param0);

static void ov75_02248034(UnkStruct_ov98_02247704 *param0) {
    int v0;
    int v1 = 0;
    String *v2;
    String *v3;

    FillWindowPixelBuffer(&param0->unk_C4, 0x0f0f);
    v2 = NewString_ReadMsgData(param0->unk_34, 44);
    v3 = String_New(String_GetLength(v2), HEAP_ID_116);
    for (v0 = param0->unk_AC; v0 < param0->unk_AC + 6; v0++) {
        String_GetLineN(v3, v2, v0);
        AddTextPrinterParameterized(&param0->unk_C4, FONT_SYSTEM, v3, 4, v1 * 16, 0xFF, 0L);
        v1++;
    }
    String_Delete(v2);
    String_Delete(v3);
    CopyWindowToVram(&param0->unk_C4);
}

struct System;


enum BgLayer {
    BG_LAYER_MAIN_0 = 0,
    BG_LAYER_MAIN_1,
    BG_LAYER_MAIN_2,
    BG_LAYER_MAIN_3,
    BG_LAYER_SUB_0,
    BG_LAYER_SUB_1,
    BG_LAYER_SUB_2,
    BG_LAYER_SUB_3,

    BG_LAYER_MAX,
};

void BgClearTilemapBufferAndCommit(BgConfig *bgConfig, u8 bgLayer);

void AddWindowParameterized(BgConfig *bgConfig, Window *window, u8 bgLayer, u8 tilemapLeft, u8 tilemapTop, u8 width, u8 height, u8 palette, u16 baseTile);

void RemoveWindow(Window *window);

void ClearWindowTilemapAndCopyToVram(Window *window);



void GfGfx_EngineATogglePlanes(u8 layerMask, u8 enable);

void DrawFrameAndWindow1(Window *window, u8 skipTransfer, u16 tile, u8 palette);

void ClearWindowPixelBuffer(Window *window, u8 skipTransfer);

void ClearFrameAndWindow2(Window *window, u8 skipTransfer);

typedef void (*UnkCallback)(void);

enum ButtonMode {
    BUTTON_MODE_NORMAL = 0,
    BUTTON_MODE_START_IS_X = 1,
    BUTTON_MODE_SWAP_XY = 2,
    BUTTON_MODE_L_IS_A = 3,
};



BOOL PlaySE(u16 seqID);

void ov75_02247C70(UnkStruct_ov98_02247704 *param0);

void ov75_02247D0C(UnkStruct_ov98_02247704 *param0);

void ov75_02247D24(UnkStruct_ov98_02247704 *param0);

int ov75_022480B8(UnkStruct_ov98_02247704 *param0);

int ov75_022480B8(UnkStruct_ov98_02247704 *param0) {
    switch (param0->unk_A8) {
    case 0:
        int i;

        String *string1 = NewString_ReadMsgData(param0->unk_34, 44);
        u32 lineCount = String_CountLines(string1);
        param0->unk_AC = 0;
        param0->unk_B0 = 0;
        param0->unk_B8 = 0;
        param0->unk_B4 = lineCount;

        AddWindowParameterized(param0->unk_04, &param0->unk_C4, BG_LAYER_MAIN_2, 1, 5, 30, 12, 13, 148);
        AddWindowParameterized(param0->unk_04, &param0->unk_D4, BG_LAYER_MAIN_2, 1, 19, 30, 4, 13, 508);

        FillWindowPixelBuffer(&param0->unk_C4, 15);
        String *string2 = String_New(String_GetLength(string1), HEAP_ID_116);
        for (i = 0; i < 6; i++) {
            String_GetLineN(string2, string1, i);
            AddTextPrinterParameterized(&param0->unk_C4, FONT_SYSTEM, string2, 4, i * 16, 0xFF, 0L);
        }
        String_Delete(string1);
        String_Delete(string2);
        DrawFrameAndWindow1(&param0->unk_C4, 1, 31, 11);
        CopyWindowToVram(&param0->unk_C4);

        FillWindowPixelBuffer(&param0->unk_D4, 15);
        DrawFrameAndWindow1(&param0->unk_D4, 1, 31, 11);
        CopyWindowToVram(&param0->unk_D4);

        param0->unk_E4 = ListMenuCursorNew(HEAP_ID_116);

        FillWindowPixelBuffer(&param0->unk_48, 15);
        ClearFrameAndWindow2(&param0->unk_48, 1);
        ClearWindowTilemapAndCopyToVram(&param0->unk_48);

        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, 1);

        param0->unk_BC = 0;
        param0->unk_C0 = 0;
        param0->unk_A8++;
        break;
    case 1:
        int v1 = param0->unk_AC;

        if (gSystem.newAndRepeatedKeys & (0x0040)) {
            if (param0->unk_AC != 0) {
                param0->unk_AC--;
                PlaySE(1500);
            }
        } else if (gSystem.newAndRepeatedKeys & (0x0080)) {
            if (param0->unk_AC + 6 < param0->unk_B4) {
                param0->unk_AC++;
                PlaySE(1500);
            }

            if (param0->unk_AC + 6 == param0->unk_B4) {
                ov75_02247C70(param0);
                param0->unk_A8++;
                PlaySE(1500);
            }
        } else if (gSystem.newKeys & 0x0002) {
            PlaySE(1500);
            param0->unk_B8 = 2;
            param0->unk_A8 = 0xFF;
        }

        if (v1 != param0->unk_AC) {
            ov75_02248034(param0);
        }

        break;
    case 2:
        if (gSystem.newKeys & 0x0010 || gSystem.newKeys & 0x0020) {
            param0->unk_B0 ^= 1;
            PlaySE(1500);
            ov75_02247C70(param0);
        } else if (gSystem.newKeys & 0x0001) {
            param0->unk_B8 = 2 - param0->unk_B0;
            PlaySE(1500);
            param0->unk_A8++;
        } else if (gSystem.newKeys & 0x0002) {
            param0->unk_B8 = 2;
            PlaySE(1500);
            param0->unk_A8++;
        } else if (gSystem.newKeys & 0x0040) {
            ov75_02247D0C(param0);
            PlaySE(1500);
            param0->unk_A8--;
            param0->unk_AC--;
            ov75_02248034(param0);
        }

        break;
    default:
        ClearWindowPixelBuffer(&param0->unk_C4, 0);
        ClearWindowTilemapAndCopyToVram(&param0->unk_C4);
        RemoveWindow(&param0->unk_C4);

        ClearWindowPixelBuffer(&param0->unk_D4, 0);
        ClearWindowTilemapAndCopyToVram(&param0->unk_D4);
        RemoveWindow(&param0->unk_D4);

        DestroyListMenuCursorObj(param0->unk_E4);

        BgClearTilemapBufferAndCommit(param0->unk_04, BG_LAYER_MAIN_3);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, 0);

        param0->unk_A8 = 0;
        return param0->unk_B8;
    }

    ov75_02247D24(param0);
    return 0;
}
