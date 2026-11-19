#include "global.h"

#include "constants/heap.h"
#include "sprite_resources_header.h"
#include "player_data.h"
#include "overlay_45_unk_struct_ov66_0222e294.h"
#include "overlay_91_unk_struct_ov66_02230dbc.h"
#include "overlay_90_unk_struct_ov114_0225c76c.h"
#include "overlay_90_unk_struct_ov114_0225c9a8.h"
#include "sprite.h"
#include "bg_window.h"
#include "overlay_07_bg_scroll_context.h"
#include "overlay_41_sprite_resource_collection.h"
#include "message_format.h"
#include "overlay_90_unk_struct_ov114_0225cbf0.h"
#include "overlay_90_unk_struct_ov114_0225cdb4.h"
#include "overlay_90_unk_struct_ov114_0225ccd0.h"
#include "overlay_90_unk_struct_ov114_0225cfcc.h"
#include "overlay_90_unk_struct_ov114_02260284.h"
#include "overlay_90_unk_struct_ov114_0225cef0.h"
#include "overlay_90_unk_struct_ov114_0225_d084.h"
#include "overlay_90_unk_struct_ov114_0225d338.h"
#include "overlay_90_unk_struct_ov114_0225dac0.h"
#include "overlay_90_unk_struct_ov114_0225e0f8.h"
#include "overlay_90_unk_struct_ov114_0225dffc.h"
#include "overlay_90_unk_struct_ov114_0225f578.h"
#include "overlay_90_unk_struct_ov114_0225f270.h"
#include "overlay_90_unk_struct_ov114_0225_e854_t.h"
#include "gf_gfx_planes.h"
#include "struct_defs/resdat_narc_structs.h"

struct BgConfig;

struct GraphicsModes;

struct MessageBank;

struct MessageBankEntry;

struct SaveBlockInfo;

struct SaveDataBody;

struct SaveDataState;

struct SavePageInfo;

struct String;

struct StringTemplateArg;

struct StringTemplateArgHeader;

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

void ov45_0222ACB8(WiFiPlazaSystem *param0, enum PlazaMinigame param1, u32 param2, u32 param3, u32 param4, u32 param5, u32 param6);

typedef struct MessageBankEntry {
    u32 offset;
    u32 length;
} MessageBankEntry;

typedef struct MessageBank {
    u16 count;
    u16 seed;

    MessageBankEntry entries[];
} MessageBank;

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

u32 ov90_0225BA14(const UnkStruct_ov114_0225E854 *param0);

void ov114_0225CEF0(UnkStruct_ov114_0225CEF0 *param0, const GraphicsModes *param1, const UnkStruct_ov114_02260284 *param2, u32 param3, enum HeapID heapID);

void ov90_0225BA38(UnkStruct_ov114_0225E854 *param0);

void ov90_0225BA38(UnkStruct_ov114_0225E854 *param0) {
    int v0;
    u32 v1[4] = { 0 };
    u32 v2;
    u32 v3;

    v2 = 0;
    v3 = ov90_0225BA14(param0);

    for (v0 = 0; v0 < param0->unk_0C.unk_08; v0++) {
        if ((param0->unk_1C.unk_10[v0] == 0) && (param0->unk_1C.unk_00[v0] != v3)) {
            v1[v2] = param0->unk_0C.unk_0C->unk_04.unk_00[v0];
            v2++;
        }
    }

    if (v2 > 0) {
        ov45_0222ACB8(param0->unk_0C.unk_0C->unk_30, (enum PlazaMinigame)(param0->unk_07), v2, v1[0], v1[1], v1[2], v1[3]);
    }
}
