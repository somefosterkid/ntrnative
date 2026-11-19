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
#include "system.h"
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

extern void DWC_CleanupInet(void);

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

void ov70_02237F2C(void);

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

int ov75_02248714(UnkStruct_ov98_02247704 *param0);

int ov75_02248714(UnkStruct_ov98_02247704 *param0) {
    sub_0203957C();

    DWC_CleanupInet();

    sub_0203A914();
    Sys_ClearSleepDisableFlag(4);

    if (param0->unk_00->unk_11C == 1) {
        ov70_02237F2C();
        param0->unk_00->unk_11C = 0;
    }

    param0->unk_08 = 11;

    return 0;
}

void ov75_02247854(UnkStruct_ov98_02247704 *param0, int param1, int param2);

void ov75_022494CC(UnkStruct_ov98_02247704 *param0, MessageLoader *param1, int param2, int param3, u16 param4);

int ov75_0224874C(UnkStruct_ov98_02247704 *param0);

int ov75_0224874C(UnkStruct_ov98_02247704 *param0) {
    ov75_022494CC(param0, param0->unk_30, 27, 1, 0xf0f);
    ov75_02247854(param0, 35, 29);

    return 0;
}

extern void DWC_InitInetEx(DWCInetControl *inetCntl, u16 dmaNo, u16 powerMode, u16 ssl_priority);

extern void DWC_ConnectInetAsync(void);

void sub_0203A880(void);

void ov75_0224785C(UnkStruct_ov98_02247704 *param0);

int ov75_02249534(int param0);

int ov75_02248774(UnkStruct_ov98_02247704 *param0);

int ov75_02248774(UnkStruct_ov98_02247704 *param0) {
    switch (param0->unk_94) {
    case 0:
        sub_02039528(param0->unk_00->saveData);
        sub_0203A880();
        ov75_022494CC(param0, param0->unk_2C, 1, 1, 0xf0f);
        ov75_0224785C(param0);
        param0->unk_94++;
        break;
    case 1:
        if (ov75_02249534(param0->unk_44) == 0) {
            param0->unk_94++;
        }
        break;
    case 2:
        Sys_SetSleepDisableFlag(4);

        DWC_InitInetEx(&param0->unk_00->unk_14, 2, 1, 20);
        DWC_SetAuthServer(DWC_CONNECTINET_AUTH_RELEASE);
        DWC_ConnectInetAsync();
        param0->unk_08 = 13;
        break;
    }

    return 0;
}
