#include "global.h"
#include "gf_gfx_planes.h"
#include "sprite_resources_header.h"

#include "constants/heap.h"
#include "overlay_90_unk_struct_ov114_0225c76c.h"
#include "overlay_90_unk_struct_ov114_0225c9a8.h"
#include "overlay_90_unk_struct_ov114_0225e0f8.h"
#include "overlay_90_unk_struct_ov114_0225cef0.h"
#include "overlay_90_unk_struct_ov114_02260284.h"
#include "overlay_90_unk_struct_ov114_0225_d084.h"
#include "overlay_90_unk_struct_ov114_0225d338.h"
#include "overlay_90_unk_struct_ov114_0225cfcc.h"
#include "overlay_90_unk_struct_ov114_0225ccd0.h"
#include "overlay_90_unk_struct_ov114_0225cdb4.h"
#include "overlay_90_unk_struct_ov114_0225cbf0.h"
#include "overlay_90_unk_struct_ov114_0225f270.h"
#include "overlay_90_unk_struct_ov114_0225f578.h"
#include "overlay_90_unk_struct_ov114_0225dac0.h"
#include "overlay_90_unk_struct_ov114_0225dffc.h"
#include "overlay_91_unk_struct_ov66_02230dbc.h"
#include "overlay_45_unk_struct_ov66_0222e294.h"
#include "overlay_90_unk_struct_ov114_0225_e854_t.h"
#include "sprite.h"
#include "overlay_07_bg_scroll_context.h"
#include "overlay_41_sprite_resource_collection.h"
#include "struct_defs/resdat_narc_structs.h"

typedef enum HeapID HeapID;

struct BgConfig;

struct GraphicsModes;

struct MessageBank;

struct MessageBankEntry;

struct SaveBlockInfo;

struct SaveData;

struct SaveDataBody;

struct SaveDataState;

struct SavePageInfo;

struct String;

struct MessageFormat;

struct StringTemplateArg;

struct StringTemplateArgHeader;

struct SysTask;

struct SysTaskManager;

struct TouchScreenHitTableActionStateInternal;

struct UnkStruct_ov114_0225E854_t;

struct YesNoButtonAnimData;

struct YesNoButtonSprite;

enum SaveBlockID {
    SAVE_BLOCK_ID_NORMAL = 0,
    SAVE_BLOCK_ID_BOXES,

    SAVE_BLOCK_ID_MAX
};

typedef struct SaveBlockInfo {
    u8 saveBlockID;
    u8 sectorStartPos;
    u8 sectorsInUse;
    u32 offset;
    u32 size;
} SaveBlockInfo;

typedef struct SavePageInfo {
    int pageID;
    u32 size;
    u32 location;
    u16 checksum;
    u16 blockID;
} SavePageInfo;

typedef struct SaveDataBody {
    u8 data[(0x1000) * 32];
} SaveDataBody;

typedef struct SaveDataState {
    BOOL fullSaveMode;
    int startBlock;
    int currentBlock;
    int endBlock;
    s32 lockID;
    int mainSequence;
    u32 globalCounterBackup;
    u32 blockCounterBackup[SAVE_BLOCK_ID_MAX];
    volatile BOOL locked;
} SaveDataState;

typedef struct UnkStruct_ov114_0225E854_t UnkStruct_ov114_0225E854;

extern u32 ov90_0225888C(const UnkStruct_ov114_0225C76C *param0, u32 param1);

BOOL ov90_022588A4(const UnkStruct_ov114_0225C76C *param0, u32 param1);

extern void ov00_021E69A8(enum HeapID heapID);

enum PlazaMinigame {
    UnkEnum_ov66_022324D0_00,
    UnkEnum_ov66_022324D0_01,
    UnkEnum_ov66_022324D0_02,
    UnkEnum_ov66_022324D0_03,
    UnkEnum_ov66_022324D0_04,
    UnkEnum_ov66_022324D0_05,
    UnkEnum_ov66_022324D0_06,
    UnkEnum_ov66_022324D0_07,
    UnkEnum_ov66_022324D0_08,
    UnkEnum_ov66_022324D0_09,
    UnkEnum_ov66_022324D0_10,
    UnkEnum_ov66_022324D0_11,
    UnkEnum_ov66_022324D0_12 = UnkEnum_ov66_022324D0_02 + 1,
};

extern void ov90_0225C15C(UnkStruct_ov114_0225E854 *param0);

typedef struct MessageBankEntry {
    u32 offset;
    u32 length;
} MessageBankEntry;

typedef struct MessageBank {
    u16 count;
    u16 seed;

    MessageBankEntry entries[];
} MessageBank;

typedef struct SysTaskManager SysTaskManager;

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

SysTask * SysTask_CreateOnMainQueue(SysTaskFunc func, void *data, u32 priority);

SysTask * SysTask_CreateOnVWaitQueue(SysTaskFunc func, void *data, u32 priority);

typedef struct StringTemplateArgHeader {
    u8 unused_00;
    u8 unused_01;
    u8 unused_02;
    u8 unused_03;
} StringTemplateArgHeader;

typedef struct StringTemplateArg {
    StringTemplateArgHeader header;
    String *string;
} StringTemplateArg;

typedef struct MessageFormat {
    u32 maxArgs;
    enum HeapID heapID;
    StringTemplateArg *args;
    String *templateBuf;
} MessageFormat;

void GF_CreateVramTransferManager(u32 capacity, enum HeapID heapID);

typedef struct TouchScreenHitTableActionStateInternal TouchScreenHitTableActionStateInternal;

typedef struct YesNoButtonAnimData {
    const fx32 *frameThresholds;
    u32 frameCount;
    fx32 frameProgress;
} YesNoButtonAnimData;

typedef struct YesNoButtonSprite {
    BgConfig *bgConfig;
    enum GFBgLayer bgLayer;
    void *nscrBuffers[4];
    NNSG2dScreenData *screenData[4];
    YesNoButtonAnimData animData;
    u8 tilemapLeft;
    u8 tilemapTop;
    u8 animFrame;
} YesNoButtonSprite;

extern void ov90_02258DD0(UnkStruct_ov114_0225CCD0 *param0, u32 param1, u32 heapID);

void ov90_022590CC(UnkStruct_ov114_0225CFCC *param0, u32 param1, u32 param2, u32 param3, u32 param4);

extern void ov90_02259184(UnkStruct_ov114_0225D084 *param0, u32 heapID);

extern void ov90_0225938C(UnkStruct_ov114_0225D338 *param0, UnkStruct_ov114_0225CEF0 *param1, SaveData *saveData, BOOL param3, u32 heapID);

extern void ov90_0225A204(UnkStruct_ov114_0225E0F8 *param0, const UnkStruct_ov114_0225C76C *param1);

BOOL ov90_0225A258(const UnkStruct_ov114_0225C76C *param0, const UnkStruct_ov114_0225E0F8 *param1);

extern void ov90_0225A850(struct UnkStruct_ov114_0225E854_t *param0, u32 heapID);

extern void ov90_0225A980(SysTask *param0, void *param1);

extern void ov90_0225AE4C(SysTask *param0, void *param1);

extern void ov90_0225B230(SysTask *param0, void *param1);

extern void ov90_0225B340(UnkStruct_ov114_0225F270 *param0, UnkStruct_ov114_0225CEF0 *param1, u32 param2, u32 heapID);

extern const GraphicsBanks ov90_0225C2F4;

extern const GraphicsModes ov90_0225C264;

extern const UnkStruct_ov114_02260284 ov90_0225C43C[6];

extern void ov90_02258FF0(UnkStruct_ov114_0225CEF0 *param0, const GraphicsModes *param1, const UnkStruct_ov114_02260284 *param2, u32 param3, enum HeapID heapID);

UnkStruct_ov114_0225E854 *ov90_0225A6B4(const UnkStruct_ov114_0225C76C *param0, const UnkStruct_ov114_0225C9A8 *param1, u32 param2, enum HeapID heapID);

UnkStruct_ov114_0225E854 *ov90_0225A6B4(const UnkStruct_ov114_0225C76C *param0, const UnkStruct_ov114_0225C9A8 *param1, u32 param2, enum HeapID heapID) {
    UnkStruct_ov114_0225E854 *v0;

    v0 = Heap_Alloc(heapID, sizeof(UnkStruct_ov114_0225E854));
    memset(v0, 0, sizeof(UnkStruct_ov114_0225E854));
    memcpy(&v0->unk_1C, param1, sizeof(UnkStruct_ov114_0225C9A8));
    memcpy(&v0->unk_0C, param0, sizeof(UnkStruct_ov114_0225C76C));

    v0->unk_07 = param2;

    ov90_0225C15C(v0);

    G2_BlendNone();
    G2S_BlendNone();
    GX_SetVisibleWnd(0);
    GXS_SetVisibleWnd(0);

    GfGfx_SetBanks(&ov90_0225C2F4);

    v0->heapID = heapID;

    ov90_0225A204(&v0->unk_34, &v0->unk_0C);
    GF_CreateVramTransferManager((HeapID)(16), (HeapID)(heapID));

    ov90_02258FF0(&v0->unk_4C, &ov90_0225C264, ov90_0225C43C, 6, (HeapID)(heapID));
    ov90_022590CC(&v0->unk_A0, 32, 2, 2, heapID);
    ov90_02258DD0(&v0->unk_230, 2, heapID);
    ov90_02259184(&v0->unk_58, heapID);
    ov90_0225A850(v0, heapID);

    {
        BOOL v2;
        u32 v3;

        v3 = ov90_0225888C(&v0->unk_0C, v0->unk_0C.unk_09);
        v2 = ov90_022588A4(&v0->unk_0C, v3);
        ov90_0225938C(&v0->unk_68, &v0->unk_4C, v0->unk_0C.saveData, v2, heapID);
    }

    ov90_0225B340(&v0->unk_24C, &v0->unk_4C, v0->unk_0C.unk_0A, heapID);

    {

        v0->unk_05 = ov90_0225A258(&v0->unk_0C, &v0->unk_34);
    }

    switch (v0->unk_07) {
    case UnkEnum_ov66_022324D0_00:
    case UnkEnum_ov66_022324D0_01:
        v0->unk_6A8 = SysTask_CreateOnMainQueue(ov90_0225A980, v0, 0);
        break;
    case UnkEnum_ov66_022324D0_02:
        v0->unk_6A8 = SysTask_CreateOnMainQueue(ov90_0225AE4C, v0, 0);
        break;
    default:
        ((0) ? (void)0 : GF_AssertFail());
        v0->unk_6A8 = SysTask_CreateOnMainQueue(ov90_0225A980, v0, 0);
        break;
    }

    v0->unk_6AC = SysTask_CreateOnVWaitQueue(ov90_0225B230, v0, 0);

    if (v0->unk_0C.unk_0A) {
        ov00_021E69A8((enum HeapID)(heapID));
    }

    return v0;
}
