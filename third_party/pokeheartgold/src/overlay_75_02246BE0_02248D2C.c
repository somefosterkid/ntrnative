#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "list_menu.h"
#include "options.h"
#include "overlay_manager.h"
#include "palette.h"
#include "pm_string.h"
#include "save.h"
#include "sys_task.h"
#include "unk_02037C94.h"

struct ApplicationManager;

struct ColoredArrow;

struct Menu;

struct MessageLoader;

struct StringList;

struct StringTemplate;

typedef struct ApplicationManager ApplicationManager;

struct ApplicationManager {
    OverlayManagerTemplate template;
    int execState;
    int procState;
    void *args;
    void *data;
    ApplicationManager *parent;
    ApplicationManager *child;
};

typedef struct WorldExchangeTrainer {
    u8 gameCode;
    u8 language;
    u8 country;
    u8 unk_03;
    u32 trainerId;
    u16 trainerName[7 + 1];
    u32 unk_10;
    u8 macAddress[6];
    u8 unk_1A[2];
    char email[56];
    u32 emailInitialised;
    u16 rngValue;
    u16 unk_5A;
} WorldExchangeTrainer;

typedef struct WorldExchangeTrainerError {
    u32 validationError;
    u32 systemError;
} WorldExchangeTrainerError;

typedef struct MessageLoader MessageLoader;

typedef struct StringTemplate StringTemplate;

typedef struct {
    SysTask *unk_00;
    BOOL unk_04;
    u16 unk_08[4][16];
    u16 unk_88[21][16];
    s16 unk_328;
    s8 unk_32A;
    u8 unk_32B;
} UnkStruct_ov61_0222C3B0;

typedef struct ColoredArrow ColoredArrow;

typedef struct StringList StringList;

typedef struct Menu Menu;

BOOL ov70_02237F38(void);

s32 ov70_02237F58(void);

void ov70_02238304(const WorldExchangeTrainer *trainer, WorldExchangeTrainerError *err);

typedef struct {
    ApplicationManager *appMan;
    SaveData *saveData;
    Options *options;
    void *unk_0C;
    NNSFndHeapHandle unk_10;
    DWCInetControl unk_14;
    u16 unk_7C;
    u8 unk_7E;
    u8 padding_7F;
    int unk_80;
    int unk_84;
    u8 padding_88[4];
    int unk_8C;
    int unk_90;
    u8 padding_94[4];
    void *unk_98;
    WorldExchangeTrainer unk_9C;
    WorldExchangeTrainerError unk_100;
    u32 unk_108;
    u32 unk_10C;
    u8 unk_110;
    u8 unk_111[3];
    String *unk_114;
    String *unk_118;
    BOOL unk_11C;
} UnkStruct_ov98_02246E88;

void ov75_02246CCC(UnkStruct_ov98_02246E88 *param0);

u32 ov75_02246CE0(UnkStruct_ov98_02246E88 *param0);

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

typedef struct {
    UnkStruct_ov98_02246E88 *unk_00;
    BgConfig *unk_04;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
    int unk_18;
    int unk_1C;
    StringTemplate *unk_20;
    MessageLoader *unk_24;
    MessageLoader *unk_28;
    MessageLoader *unk_2C;
    MessageLoader *unk_30;
    MessageLoader *unk_34;
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
    StringList *unk_A0;
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

void ov75_02247854(UnkStruct_ov98_02247704 *param0, int param1, int param2);

void ov75_0224785C(UnkStruct_ov98_02247704 *param0);

void ov75_02247878(UnkStruct_ov98_02247704 *param0);

void ov75_022494CC(UnkStruct_ov98_02247704 *param0, MessageLoader *param1, int param2, int param3, u16 param4);

int ov75_02249534(int param0);

int ov75_02248D2C(UnkStruct_ov98_02247704 *param0);

int ov75_02248D2C(UnkStruct_ov98_02247704 *param0) {
    if (ov75_02246CE0(param0->unk_00) == 0xffffffff) {
        param0->unk_08 = 21;
        return 0;
    }

    switch (param0->unk_94) {
    case 0:

        ov75_022494CC(param0, param0->unk_34, 18, 0, 0xf0f);
        ov75_0224785C(param0);
        ov75_02246CCC(param0->unk_00);
        ov70_02238304(&param0->unk_00->unk_9C, &param0->unk_00->unk_100);
        param0->unk_94++;
        break;
    case 1:
        if (ov70_02237F38()) {
            s32 v0 = ov70_02237F58();

            param0->unk_E8 = 0;
            ov75_02247878(param0);

            switch (v0) {
            case 0:
                switch (param0->unk_00->unk_100.validationError) {
                case 0:
                    switch (param0->unk_00->unk_100.systemError) {
                    case 0:
                        param0->unk_94++;
                        break;
                    case 3:
                        ov75_022494CC(param0, param0->unk_34, 19, 1, 0xf0f);
                        ov75_02247854(param0, 34, 21);
                        break;
                    case 1:
                        param0->unk_10 = -5006;
                        param0->unk_08 = 32;
                        break;
                    case 2:
                        param0->unk_10 = -5007;
                        param0->unk_08 = 32;
                        break;
                    default:
                        sub_020399EC();
                        break;
                    }
                    break;
                case 1:
                    param0->unk_10 = -5010;
                    param0->unk_08 = 32;
                    break;
                case 2:
                    param0->unk_10 = -5011;
                    param0->unk_08 = 32;
                    break;
                default:
                    ((0) ? (void)0 : GF_AssertFail());
                    sub_020399EC();
                    break;
                }
                break;
            case 1:
                param0->unk_10 = v0;
                param0->unk_08 = 32;
                break;
            case 2:
            case -1:
                param0->unk_10 = v0;
                param0->unk_08 = 32;
                break;
            case -12:
            case -15:
                param0->unk_10 = v0;
                param0->unk_08 = 32;
                break;
            case -2:
            case -14:
                param0->unk_10 = v0;
                param0->unk_08 = 32;
                break;
            case -13:
            default:
                sub_020399EC();
                break;
            }
        } else {
            param0->unk_E8++;

            if (param0->unk_E8 == (30 * 60 * 2)) {
                sub_020399EC();
            }
        }
        break;
    case 2:
        ov75_022494CC(param0, param0->unk_34, 20, 1, 0xf0f);
        param0->unk_94++;
        break;
    case 3:
        if (ov75_02249534(param0->unk_44) == 0) {
            ov75_02247854(param0, 26, 23);
        }
        break;
    }

    return 0;
}

void ov75_02246BCC(UnkStruct_ov98_02246E88 *param0, int param1, int param2);

void ov75_02246BE0(UnkStruct_ov98_02246E88 *param0, s8 param1);

int ov75_02248F18(UnkStruct_ov98_02247704 *param0);

int ov75_02248F18(UnkStruct_ov98_02247704 *param0) {
    if (ov75_02249534(param0->unk_44) == 1) {
        return 0;
    }

    switch (param0->unk_94) {
    case 0:
        ov75_022494CC(param0, param0->unk_34, 21, 1, 0xf0f);
        param0->unk_94++;
        break;
    case 1:
        ov75_02246BCC(param0->unk_00, 3, 2);
        ov75_02246BE0(param0->unk_00, 24);
        param0->unk_08 = 30;
        break;
    }

    return 0;
}

void ov75_02246C3C(UnkStruct_ov98_02246E88 *param0);

void ov75_02246C68(UnkStruct_ov98_02246E88 *param0);

void ov75_02246C8C(UnkStruct_ov98_02246E88 *param0);

int ov75_02248F7C(UnkStruct_ov98_02247704 *param0);

int ov75_02248F7C(UnkStruct_ov98_02247704 *param0) {
    if (ov75_02249534(param0->unk_44) == 1) {
        return 0;
    }

    switch (param0->unk_94) {
    case 0:
        ov75_022494CC(param0, param0->unk_34, 24, 1, 0xf0f);
        param0->unk_94++;
        break;
    case 1:
        ov75_02246C3C(param0->unk_00);
        ov75_02246C68(param0->unk_00);
        ov75_02246C8C(param0->unk_00);
        ov75_02247854(param0, 25, 0);
        break;
    }

    return 0;
}
