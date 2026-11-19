#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "options.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "sys_task.h"
#include "unk_02009D48.h"
#include "overlay_70_gtsapplication_state.h"
#include "overlay_71_trade_sequence_data.h"
#include "msgdata.h"

typedef struct MessageBankEntry MessageBankEntry;

typedef struct MessageBank MessageBank;


typedef struct StringTemplateArgHeader StringTemplateArgHeader;

typedef struct StringTemplateArg StringTemplateArg;

typedef u16 charcode_t;

struct MessageBankEntry {
    u32 offset;
    u32 length;
};

struct MessageBank {
    u16 count;
    u16 seed;

    MessageBankEntry entries[];
};


struct StringTemplateArgHeader {
    u8 unused_00;
    u8 unused_01;
    u8 unused_02;
    u8 unused_03;
};

struct StringTemplateArg {
    StringTemplateArgHeader header;
    String *string;
};

enum TradeType {
    TRADE_TYPE_NORMAL = 1,
    TRADE_TYPE_SEND_ONLY,
    TRADE_TYPE_RECEIVE_ONLY = 4,
};

enum TradeBackground {
    TRADE_BACKGROUND_DAY = 0,
    TRADE_BACKGROUND_EVENING,
    TRADE_BACKGROUND_NIGHT,
    TRADE_BACKGROUND_WIFI,
};

void ov71_02247230(TradeSequenceData *sequenceData, s32 pokemonIndex, u32 bgLayer, u32 paletteIdx, u32 tilemapX, u32 tilemapY);

void ov71_02247230(TradeSequenceData *sequenceData, s32 pokemonIndex, u32 bgLayer, u32 paletteIdx, u32 tilemapX, u32 tilemapY) {
    NNSG2dScreenData *sp1C;
    s32 var_r1;
    void *temp_r0;

    if (pokemonIndex == 0) {
        var_r1 = 0x10A;
    } else if (sequenceData->flipSprite != 0) {
        var_r1 = 0x10B;
    } else {
        var_r1 = 0x10A;
    }
    temp_r0 = GfGfxLoader_GetScrnData(NARC_a_0_0_7, var_r1, 0, &sp1C, HEAP_ID_56);
    if (temp_r0 != NULL) {
        CopyToBgTilemapRect(sequenceData->bgConfig, bgLayer, tilemapX, tilemapY, 0xAU, 0xAU, sp1C->rawData, 0U, 0U, 0x20U, 0x20U);
        BgTilemapRectChangePalette(sequenceData->bgConfig, bgLayer, tilemapX, tilemapY, 0xAU, 0xAU, (s32)paletteIdx);
        Heap_Free(temp_r0);
    }
}
