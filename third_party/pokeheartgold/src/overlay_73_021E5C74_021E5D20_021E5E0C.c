#include "global.h"
#include "overlay_73_unk_struct_020961_e8_t.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "field_types_def.h"
#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "msgdata.h"
#include "options.h"
#include "overworld_anim_manager.h"
#include "pm_string.h"
#include "save.h"
#include "sprite.h"
#include "sys_task.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_0205B3DC.h"
#include "wild_encounter.h"
#include "struct_defs/resdat_narc_structs.h"
#include "list_menu_items.h"
#include "list_menu_2d.h"
#include "overlay_41_sprite_resource_collection.h"
#include "trainer_case.h"
#include "union_room_manager.h"
#include "game_stats.h"
#include "save_palpad.h"

typedef struct MessageBankEntry MessageBankEntry;

typedef struct MessageBank MessageBank;

typedef struct ListMenuCursor ListMenuCursor;

typedef struct StringTemplateArgHeader StringTemplateArgHeader;

typedef struct StringTemplateArg StringTemplateArg;

typedef struct JournalEntryTitle JournalEntryTitle;

typedef struct JournalEntryMon JournalEntryMon;

typedef struct JournalEntryTrainer JournalEntryTrainer;

typedef struct JournalEntry JournalEntry;

typedef struct EncodingSeed EncodingSeed;

struct MessageBankEntry {
    u32 offset;
    u32 length;
};

struct MessageBank {
    u16 count;
    u16 seed;

    MessageBankEntry entries[];
};

typedef u32 TextColor;

struct ListMenuCursor {
    TextColor color;
    String *string;
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

struct JournalEntryTitle {
    u32 year : 7;
    u32 month : 4;
    u32 week : 3;
    u32 day : 5;
    u32 mapID : 13;
};

struct JournalEntryMon {
    u8 battleResult;
    u8 stringVariant : 2;
    u8 timeOfDay : 4;
    u8 gender : 2;
    u16 species;
};

struct JournalEntryTrainer {
    u16 standard : 1;
    u16 trainerID : 15;
    u16 mapID;
};

struct JournalEntry {
    JournalEntryTitle title;
    u32 locationEvents[4];
    JournalEntryMon mon;
    JournalEntryTrainer trainer;
    u8 onlineEvents[2][42];
};

struct EncodingSeed {
    u16 byteSum;
    u16 modifier;
};

s32 ov73_021E7740(UnkStruct_020961E8 *, NARC *);

void ov73_021E5E0C(UnkStruct_020961E8 *param0, NARC *param1);

void ov73_021E5E0C(UnkStruct_020961E8 *param0, NARC *param1) {
    s32 i;

    for (i = 0; i < 5; i++) {
        param0->unk_2C[i] = String_New(8U, HEAP_ID_50);
        param0->unk_3C8[i][0] = NULL;
        param0->unk_3C8[i][1] = NULL;
        param0->unk_4AC8[i][0].val1 = 0;
        param0->unk_4AC8[i][1].val1 = 0;
        param0->unk_3F0[i] = 0;
    }
    param0->unk_44 = String_New(0xB4U, HEAP_ID_50);
    param0->unk_48 = String_New(0x28U, HEAP_ID_50);
    param0->unk_3A8 = 0;
    ReadMsgDataIntoString(param0->unk_28, 0x11, param0->unk_48);
    ov73_021E7740(param0, param1);
    param0->unk_40C = sub_0205B4A4(HEAP_ID_50);
    param0->unk_0C.unk_00 = 0;
    param0->unk_0C.unk_08 = 0;
    param0->unk_0C.unk_04 = 0;
    param0->unk_0C.unk_0C = GfGfxLoader_GetPlttDataFromOpenNarc(param1, 7, &param0->unk_0C.unk_10, HEAP_ID_50);
    param0->unk_4AA8 = 0;
    param0->unk_4AAC = 2;
    param0->unk_4A15 = 0;
}
