#include "global.h"

#include "text.h"
#include "overlay_75_unk_struct_ov98_02246_e88.h"
#include "overlay_manager.h"
#include "overlay_70_gtsapplication_state.h"
#include "msgdata.h"
#include "overlay_73_wifi_battle_tower_app_state.h"
#include "list_menu_items.h"
#include "list_menu.h"
#include "render_window.h"

typedef struct UnkStruct_ov75_022494A4 {
    u8 unk_0[0x4];
    void *unk_4;
    s32 unk_8;
    s32 unk_C;
    u8 unk_10[0x34];
    s32 unk_44;
    u8 unk_48[0x40];
    void *unk_88;
} UnkStruct_ov75_022494A4;

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

extern void DWC_CleanupInet(void);

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

void ov70_02237F2C(void);

void ov75_02246BD8(UnkStruct_ov98_02246E88 *param0);

void sub_0203957C(void);

SysTask *SysTask_ExecuteOnVBlank(SysTaskFunc callback, void *param, u32 priority);

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *);

static inline void inline_ov61_0222C3B0_sub(SysTask *, void *);

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

typedef struct {
    UnkStruct_ov98_02246E88 *unk_00;
    BgConfig *unk_04;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
    int unk_18;
    int unk_1C;
    MessageFormat *unk_20;
    MsgData *unk_24;
    MsgData *unk_28;
    MsgData *unk_2C;
    MsgData *unk_30;
    MsgData *unk_34;
    String *unk_38;
    String *unk_3C;
    String *unk_40;
    int unk_44;
    Window unk_48;
    Window unk_58;
    Window unk_68;
    Window unk_78;
    Menu *unk_88;
    void *unk_8C;
    int unk_90;
    int unk_94;
    int unk_98;
    int unk_9C;
    ListMenuItem *unk_A0;
    ListMenu *unk_A4;
    int unk_A8;
    int unk_AC;
    int unk_B0;
    int unk_B4;
    int unk_B8;
    int unk_BC;
    int unk_C0;
    Window unk_C4;
    Window unk_D4;
    ColoredArrow *unk_E4;
    s32 unk_E8;
    UnkStruct_ov61_0222C3B0 unk_EC;
} UnkStruct_ov98_02247704;

void ov75_022494CC(UnkStruct_ov98_02247704 *param0, MsgData *param1, int param2, int param3, u16 param4);

int ov75_02249534(int param0);

int ov75_0224937C(UnkStruct_ov98_02247704 *param0);

int ov75_0224937C(UnkStruct_ov98_02247704 *param0) {
    switch (param0->unk_94) {
    case 0:
        ov75_022494CC(param0, param0->unk_34, 15, 1, 0xf0f);
        param0->unk_94++;
        break;
    case 1:
        if (ov75_02249534(param0->unk_44) == 0) {
            sub_0203957C();

            DWC_CleanupInet();

            sub_0203A914();
            Sys_ClearSleepDisableFlag(4);

            if (param0->unk_00->unk_11C == 1) {
                ov70_02237F2C();
                param0->unk_00->unk_11C = 0;
            }

            param0->unk_94++;
        }
        break;
    case 2:
        ov75_022494CC(param0, param0->unk_34, 16, 1, 0xf0f);
        param0->unk_94++;
        break;
    case 3:
        if (ov75_02249534(param0->unk_44) == 0) {
            param0->unk_94++;
        }
        break;
    default:
        param0->unk_90++;

        if (param0->unk_90 > 30) {
            ov75_02246BD8(param0->unk_00);
            param0->unk_08 = 29;
        }
        break;
    }

    return 0;
}

s32 ov75_02249460(void *arg0);

s32 ov75_02249460(void *arg0) {
    if (ov75_02249534(*(s32 *)((u8 *)arg0 + 0x44)) == 0) {
        *(s32 *)((u8 *)arg0 + 8) = (s32) * (s32 *)((u8 *)arg0 + 0xC);
    }
    return 0;
}

int ov75_02249478(UnkStruct_ov98_02247704 *param0);

int ov75_02249478(UnkStruct_ov98_02247704 *param0) {
    if (ov75_02249534(param0->unk_44) == 0) {
        if (param0->unk_90 > 30) {
            param0->unk_08 = param0->unk_0C;
        }

        param0->unk_90++;
    }

    return 0;
}

void *ov75_02247890(void *, s32, s32);

s32 ov75_022494A4(UnkStruct_ov75_022494A4 *arg0);

s32 ov75_022494A4(UnkStruct_ov75_022494A4 *arg0) {
    if (ov75_02249534(arg0->unk_44) == 0) {
        arg0->unk_88 = ov75_02247890(arg0->unk_4, 0x234, 0);
        arg0->unk_8 = (s32) arg0->unk_C;
    }
    return 0;
}

struct TextPrinterTemplate;



typedef BOOL (*TextPrinterCallback)(TextPrinterTemplate *, u16);


void ov75_022494CC(UnkStruct_ov98_02247704 *param0, MsgData *param1, int param2, int param3, u16 param4) {
    String *v0 = NewString_ReadMsgData(param1, param2);

    StringExpandPlaceholders(param0->unk_20, param0->unk_38, v0);
    String_Delete(v0);
    FillWindowPixelBuffer(&param0->unk_48, 0xf0f);
    DrawFrameAndWindow2(&param0->unk_48, 0, 1, 10);

    param0->unk_44 = AddTextPrinterParameterized(&param0->unk_48, FONT_MESSAGE, param0->unk_38, 0, 0, param3, NULL);
    param0->unk_90 = 0;

    if ((param3 == 0xff) || (param3 == 0)) {
        param0->unk_44 = 0xff;
    }
}

int ov75_02249534(int param0) {
    if ((param0 == 0xff) || (TextPrinterCheckActive(param0) == 0)) {
        return 0;
    }

    return 1;
}

int ov75_02246CF8(UnkStruct_ov98_02246E88 *param0);

int ov75_02249550(UnkStruct_ov98_02247704 *param0);

int ov75_02249550(UnkStruct_ov98_02247704 *param0) {
    if (ov75_02249534(param0->unk_44) == 1) {
        return 0;
    }

    switch (param0->unk_94) {
    case 0:
        ov75_022494CC(param0, param0->unk_34, ov75_02246CF8(param0->unk_00) == 2 ? 0x26 : 0x27, 1, 0x0F0F);
        param0->unk_94++;
        break;
    case 1:
        param0->unk_08 = 5;
        break;
    }

    return 0;
}
