#include "global.h"
#include "heap.h"
#include "overlay_75_unk_struct_ov98_02246_e88.h"
#include "overlay_manager.h"
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

void Heap_Free(void *ptr);

void BlendPalette(const u16 *src, u16 *dest, u16 size, u8 fraction, u16 target);

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};


void AddWindowParameterized(BgConfig *bgConfig, Window *window, u8 bgLayer, u8 tilemapLeft, u8 tilemapTop, u8 width, u8 height, u8 palette, u16 baseTile);

void RemoveWindow(Window *window);

void CopyWindowToVram(Window *window);

void ClearWindowTilemapAndCopyToVram(Window *window);

void *GfGfxLoader_GetPlttDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, NNSG2dPaletteData **outPaletteData, enum HeapID heapID);

typedef struct ColoredArrow ColoredArrow;


void ListMenuItems_Delete(ListMenuItem *list);


typedef void (*CursorCallback)(ListMenu *menu, u32 index, u8 onInit);

typedef void (*PrintCallback)(ListMenu *menu, u32 index, u8 yOffset);



void DestroyListMenu(ListMenu *menu, u16 *outListPos, u16 *outCursorPos);

typedef struct Menu Menu;

void ov75_02246BCC(UnkStruct_ov98_02246E88 *param0, int param1, int param2);

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

void DrawFrameAndWindow1(Window *window, u8 skipTransfer, u16 tile, u8 palette);

void ClearWindowPixelBuffer(Window *window, u8 skipTransfer);

void ClearFrameAndWindow2(Window *window, u8 skipTransfer);

BOOL PlaySE(u16 seqID);

BOOL sub_02031744(SaveData *saveData, s32);

typedef struct {
    u32 unk_00;
    u32 unk_04;
} UnkStruct_ov98_02249BDC;

void ov69_021E68D8(ListMenu *param0, u32 param1, u8 param2);

void ov98_022499B4(ListMenu *param0, u32 param1, u8 param2);

extern const UnkStruct_ov98_02249BDC ov75_022499BC[4];

extern const ListMenuTemplate ov75_022499DC;

extern const u8 ov75_02249974[4];

extern const UnkStruct_ov98_02249BDC ov75_0224998C[2];

extern const ListMenuTemplate ov75_0224999C;

extern const u8 ov75_02249978[4];

int ov75_022478E0(UnkStruct_ov98_02247704 *param0, s32 arg1);

int ov75_022478E0(UnkStruct_ov98_02247704 *param0, s32 arg1) {
    u32 v0;

    switch (param0->unk_94) {
    case 0: {
        int v1;
        ListMenuTemplate v2;
        const u8 *v3;
        const UnkStruct_ov98_02249BDC *v4;
        int v5;

        if (sub_02031744(param0->unk_00->saveData, arg1) == 1) {
            v3 = ov75_02249974;
            v4 = ov75_022499BC;
            v5 = (sizeof(ov75_022499BC) / sizeof(*(ov75_022499BC)));
            v2 = ov75_022499DC;
        } else {
            v3 = ov75_02249978;
            v4 = ov75_0224998C;
            v5 = (sizeof(ov75_0224998C) / sizeof(*(ov75_0224998C)));
            v2 = ov75_0224999C;
        }

        AddWindowParameterized(param0->unk_04, &param0->unk_78, 0, v3[0], v3[1], v3[2], v3[3], 13, ((1 + (18 + 12)) + 9) + 27 * 4);

        param0->unk_A0 = ListMenuItems_New(v5, HEAP_ID_116);

        for (v1 = 0; v1 < v5; v1++) {
            ListMenuItems_AppendFromMsgData(param0->unk_A0, param0->unk_34, v4[v1].unk_00, v4[v1].unk_04);
        }

        v2.window = &param0->unk_78;
        v2.items = param0->unk_A0;
        param0->unk_A4 = ListMenuInit(&v2, 0, 0, HEAP_ID_116);

        DrawFrameAndWindow1(&param0->unk_78, 1, 1 + (18 + 12), 11);
        ClearFrameAndWindow2(&param0->unk_48, 1);
        CopyWindowToVram(&param0->unk_78);
    }

        param0->unk_94++;
        break;
    case 1:
        v0 = ListMenu_ProcessInput(param0->unk_A4);

        switch (v0) {
        case 0xffffffff:
            break;
        case 0xfffffffe:
            PlaySE(1500);
            ov75_02246BCC(param0->unk_00, 6, 0);
            param0->unk_9C = 29;
            param0->unk_94++;
            break;
        default:
            PlaySE(1500);
            param0->unk_9C = v0;
            param0->unk_94++;
            break;
        }
        break;
    default:
        ListMenuItems_Delete(param0->unk_A0);
        DestroyListMenu(param0->unk_A4, 0L, 0L);
        ClearWindowPixelBuffer(&param0->unk_78, 1);
        ClearWindowTilemapAndCopyToVram(&param0->unk_78);
        RemoveWindow(&param0->unk_78);
        param0->unk_08 = param0->unk_9C;
        break;
    }

    return 0;
}
