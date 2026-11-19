#include "global.h"
#include "constants/heap.h"

struct OverlayManager;

struct OverlayManagerTemplate;

struct BgConfig;

struct ListMenuCursor;

struct DWCstInetControl;

struct ListMenu;

struct ListMenuTemplate;

struct ListMenu2D;

struct MsgData;

struct NARC;

struct Options;

struct SaveData;

struct String;

struct ListMenuItem;

struct MessageFormat;

struct SysTask;

struct SysTaskManager;

struct Window;

struct WorldExchangeTrainer;

struct WorldExchangeTrainerError;

struct tagDWCWDSData;

typedef struct OverlayManager OverlayManager;

typedef BOOL (*OverlayFunc)(OverlayManager *appMan, int *state);

typedef struct OverlayManagerTemplate {
    OverlayFunc init;
    OverlayFunc main;
    OverlayFunc exit;
    const FSOverlayID overlayID;
} OverlayManagerTemplate;

struct OverlayManager {
    OverlayManagerTemplate template;
    int execState;
    int procState;
    void *args;
    void *data;
    OverlayManager *parent;
    OverlayManager *child;
};

void *OverlayManager_GetData(OverlayManager *appMan);

typedef enum {
    DWC_WDS_STATE_IDLE = 0,
    DWC_WDS_STATE_PROCESS = 1,
    DWC_WDS_STATE_COMPLETED = 2,
    DWC_WDS_STATE_FAILED = 3,
    DWC_WDS_STATE_ERROR = 4
} DWCWDSState;

typedef struct tagDWCWDSData {
    u8 ssid[32];
    u8 wep[32];
    int wepMode;
    char apnum[10];
} DWCWDSData;

typedef struct DWCstInetControl {
    volatile int ac_state;
    u16 state;
    u16 online;
    u16 dmaNo;
    u16 powerMode;
    u8 wontExecuteWDS;
    DWCWDSState processWDSstate;
    DWCWDSData nspotInfo;

} DWCInetControl;

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

typedef struct SaveData SaveData;

typedef struct Options Options;

void Heap_Free(void *ptr);

typedef struct String String;

typedef struct NARC {
    FSFile file;
    u32 fatbStart;
    u32 fimgStart;
    u16 numFiles;
} NARC;

void BlendPalette(const u16 *src, u16 *dest, u16 size, u8 fraction, u16 target);

typedef struct SysTask SysTask;

typedef struct SysTaskManager SysTaskManager;

typedef void (*SysTaskFunc)(SysTask *, void *);

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

struct SysTask {
    SysTaskManager *manager;
    SysTask *prevTask;
    SysTask *nextTask;
    u32 priority;
    void *param;
    SysTaskFunc callback;
    enum TaskState state;
};

struct SysTaskManager {
    u16 maxTasks;
    u16 stackPointer;
    SysTask sentinelTask;
    SysTask **taskStack;
    SysTask *tasks;
    BOOL locked;
    SysTask *currentTask;
    SysTask *nextTask;
};

typedef struct BgConfig BgConfig;

typedef struct Window {
    BgConfig *bgConfig;
    u8 bgLayer;
    u8 tilemapLeft;
    u8 tilemapTop;
    u8 width;
    u8 height;
    u8 palette;
    u16 baseTile : 15;
    u16 colorMode : 1;
    void *pixels;
} Window;

typedef struct MsgData MsgData;

typedef struct MessageFormat MessageFormat;

void *GfGfxLoader_GetPlttDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, NNSG2dPaletteData **outPaletteData, enum HeapID heapID);

typedef struct {
    SysTask *unk_00;
    BOOL unk_04;
    u16 unk_08[4][16];
    u16 unk_88[21][16];
    s16 unk_328;
    s8 unk_32A;
    u8 unk_32B;
} UnkStruct_ov61_0222C3B0;

typedef struct ListMenuCursor ListMenuCursor;

typedef struct ListMenuItem ListMenuItem;

typedef struct ListMenu ListMenu;

typedef void (*CursorCallback)(ListMenu *menu, u32 index, u8 onInit);

typedef void (*PrintCallback)(ListMenu *menu, u32 index, u8 yOffset);

typedef struct ListMenuTemplate {
    const ListMenuItem *choices;
    CursorCallback cursorCallback;
    PrintCallback printCallback;
    Window *window;
    u16 count;
    u16 maxDisplay;
    u8 headerXOffset;
    u8 textXOffset;
    u8 cursorXOffset;
    u8 yOffset : 4;
    u8 textColorFg : 4;
    u8 textColorBg : 4;
    u8 textColorShadow : 4;
    u16 letterSpacing : 3;
    u16 lineSpacing : 4;
    u16 pagerMode : 2;
    u16 fontID : 6;
    u16 cursorType : 1;
    void *parent;
} ListMenuTemplate;

struct ListMenu {
    ListMenuTemplate template;
    u8 cursorPal : 4;
    u8 fillValue : 4;
    u8 cursorShadowPal : 4;
    u8 unk_21_4 : 4;
    u8 lettersSpacing : 6;
    u8 unk_23;
    u8 fontId : 7;
    u8 overrideEnabled : 1;
    struct ListMenuCursor *cursor;
    u16 cursorPos;
    u16 itemsAbove;
    u8 unk_30;
    u8 unk_31;
    u8 taskId;
    u8 unk_33;
    u8 heapID;
};

typedef struct ListMenu2D ListMenu2D;

typedef struct {
    OverlayManager *appMan;
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

static inline void inline_ov61_0222C0F8(UnkStruct_ov61_0222C3B0 *param0) {
    return;
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

BOOL IsPaletteFadeFinished(void);

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
    ListMenu2D *unk_88;
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
    ListMenuCursor *unk_E4;
    s32 unk_E8;
    UnkStruct_ov61_0222C3B0 unk_EC;
} UnkStruct_ov98_02247704;

int ov75_022478E0(UnkStruct_ov98_02247704 *param0, s32 arg1);

int ov98_02247D30(UnkStruct_ov98_02247704 *param0);

int ov98_02247D50(UnkStruct_ov98_02247704 *param0);

int ov98_02247E38(UnkStruct_ov98_02247704 *param0);

int ov98_02248684(UnkStruct_ov98_02247704 *param0);

int ov98_022487AC(UnkStruct_ov98_02247704 *param0);

int ov75_0224854C(UnkStruct_ov98_02247704 *param0);

int ov75_02248584(UnkStruct_ov98_02247704 *param0);

int ov98_022488F8(UnkStruct_ov98_02247704 *param0);

int ov75_022486EC(UnkStruct_ov98_02247704 *param0);

int ov75_02248714(UnkStruct_ov98_02247704 *param0);

int ov75_0224874C(UnkStruct_ov98_02247704 *param0);

int ov75_02248774(UnkStruct_ov98_02247704 *param0);

int ov75_02248800(UnkStruct_ov98_02247704 *param0);

int ov75_022488AC(UnkStruct_ov98_02247704 *param0);

int ov75_022488BC(UnkStruct_ov98_02247704 *param0);

int ov75_02248994(UnkStruct_ov98_02247704 *param0);

int ov75_022489F8(UnkStruct_ov98_02247704 *param0);

int ov75_02248A20(UnkStruct_ov98_02247704 *param0);

int ov98_02248DF4(UnkStruct_ov98_02247704 *param0);

int ov98_02248EC0(UnkStruct_ov98_02247704 *param0);

int ov98_02248EE0(UnkStruct_ov98_02247704 *param0);

int ov75_02248D2C(UnkStruct_ov98_02247704 *param0);

int ov75_02248F18(UnkStruct_ov98_02247704 *param0);

int ov75_02248F7C(UnkStruct_ov98_02247704 *param0);

int ov75_02248FE8(UnkStruct_ov98_02247704 *param0);

int ov75_022490D8(UnkStruct_ov98_02247704 *param0);

int ov75_022491CC(UnkStruct_ov98_02247704 *param0);

int ov75_022491F0(UnkStruct_ov98_02247704 *param0);

int ov75_0224921C(UnkStruct_ov98_02247704 *param0);

int ov98_022494A0(UnkStruct_ov98_02247704 *param0);

int ov75_02249278(UnkStruct_ov98_02247704 *param0);

int ov75_0224937C(UnkStruct_ov98_02247704 *param0);

int ov98_022496A8(UnkStruct_ov98_02247704 *param0);

int ov75_02249478(UnkStruct_ov98_02247704 *param0);

int ov98_022496EC(UnkStruct_ov98_02247704 *param0);

int ov75_02249550(UnkStruct_ov98_02247704 *param0);

s32 ov75_022495B0(void *arg0, s32 arg1, s32 arg2);

extern int (*ov39_0222AA20[14])(UnkStruct_ov98_02247704 *);

int ov39_02228308(OverlayManager *appMan, int *param1);

int ov39_02228308(OverlayManager *appMan, int *param1) {
    UnkStruct_ov98_02247704 *v0 = OverlayManager_GetData(appMan);
    int v1, v2;

    switch (*param1) {
    case 0:
        if (IsPaletteFadeFinished() == TRUE) {
            *param1 = 1;
        }
        break;
    case 1:
        v2 = v0->unk_08;
        v1 = (*ov39_0222AA20[v0->unk_08])(v0);

        if (v2 != v0->unk_08) {
            v0->unk_94 = 0;
            v0->unk_9C = 0;
            v0->unk_98 = 0;
        }

        if (v1 == 1) {
            *param1 = 2;
        }
        break;
    case 2:
        if (IsPaletteFadeFinished() == TRUE) {
            return 1;
        }
        break;
    }

    inline_ov61_0222C0F8(&v0->unk_EC);

    return 0;
}
