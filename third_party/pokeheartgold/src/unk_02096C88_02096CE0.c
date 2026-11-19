#include "global.h"
#include "simple3dmodel.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "camera.h"
#include "field_types_def.h"
#include "filesystem.h"
#include "options.h"
#include "overworld_anim_manager.h"
#include "palette.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "save.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_02033AE0.h"
#include "wild_encounter.h"
#include "error_handling.h"
#include "unk_02035900.h"
#include "unk_02037C94.h"
#include "unk_0205A44C.h"
#include "unk_02096C88.h"
#include "unk_struct_0209_bdf8.h"
#include "overlay_85_ov109_021D5140.h"
#include "struct_defs/resdat_narc_structs.h"
#include "overlay_41_sprite_resource_collection.h"
#include "trainer_case.h"
#include "union_room_manager.h"
#include "game_stats.h"
#include "simple3danimation.h"
#include "save_palpad.h"
#include "overlay_85_ov109_021D0F70.h"
#include "unk_struct_0209_c194.h"

typedef struct MessageBankEntry MessageBankEntry;

typedef struct MessageBank MessageBank;

typedef struct ListMenuCursor ListMenuCursor;

typedef struct StringTemplateArgHeader StringTemplateArgHeader;

typedef struct StringTemplateArg StringTemplateArg;

typedef struct UnkStruct_0205C95C_t UnkStruct_0205C95C;

typedef struct UnkStruct_0205C22C_t UnkStruct_0205C22C;

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

typedef struct {
    u8 unk_00;
    u8 unk_01;
    u8 unk_02;
    u8 unk_03;
    u8 unk_04;
    u8 padding_05;
    u16 unk_06;
    u8 unk_08;
    u8 unk_09;
    u8 padding_0A[2];
    u32 unk_0C;
    OverworldAnimManager *unk_10;
    OverworldAnimManager *unk_14;
} UnkStruct_0205C680;

struct UnkStruct_0205C22C_t {
    UnionRoomManager *unionRoomMan;
    SysTask *unk_04;
    PlayerAvatar *playerAvatar;
    UnkStruct_0205C680 unk_0C[50 + 1];
    FieldSystem *fieldSystem;
    SavePalPad *unk_474;
    UnkStruct_0205C95C *unk_478;
    int unk_47C;
    int unk_480;
};

typedef struct {
    String *unk_00;
    String *unk_04;
    String *unk_08;
    u32 unk_0C;
    int unk_10;
    MailMessage unk_14;
} UnkStruct_0205C924;

struct UnkStruct_0205C95C_t {
    UnkStruct_0205C924 unk_00[30];
    int unk_348;
    int unk_34C;
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

struct GameStats {
    u32 recordsU32[71];
    u16 recordsU16[77];
    u16 padding;
    EncodingSeed seed;
};

extern s32 _0210884C;

void sub_02096CE0(UnkStruct_0209BDF8 *param0);

void sub_0205AD24(UnkStruct_0205C22C *);
struct GameStats;
struct JournalEntry;
struct UnkStruct_0205C22C_t;
struct UnkStruct_ov109_021D0F70_t;
typedef struct {
    u32 unk_00;
    u8 unk_04[20];
} UnkStruct_0209BE84;
BOOL sub_02096D14(UnkStruct_0209BDF8 *param0, u32 param1, const void *param2, u32 param3);

void sub_02096CE0(UnkStruct_0209BDF8 *param0) {
    sub_0203410C(&_0210884C, 0x87, param0);
}

void sub_02096CF4(UnkStruct_0209BDF8 *param_1) {
    sub_02038C1C(2);
    sub_02037FF0();
    sub_0205AD24(param_1->unk_00->unk_14.unk_0C);
    sub_0205A904(0);
    return;
}

BOOL sub_02096D14(UnkStruct_0209BDF8 *param0, u32 param1, const void *param2, u32 param3) {
    GF_ASSERT(param1 < 18);
    GF_ASSERT(param3 + 4 <= 24);

    {
        BOOL v0;
        UnkStruct_0209BE84 *v1 = (void *)param0->unk_04;

        v1->unk_00 = param1;
        memcpy(v1->unk_04, param2, param3);
        v0 = sub_02037030(130, v1, 24);

        return v0;
    }
}
