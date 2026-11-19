#include "global.h"
#include "overlay_73_unk_struct_020961_e8_t.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "field_types_def.h"
#include "filesystem.h"
#include "options.h"
#include "overworld_anim_manager.h"
#include "pm_string.h"
#include "save.h"
#include "sprite.h"
#include "sys_task.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_02035900.h"
#include "wild_encounter.h"
#include "msgdata.h"
#include "game_stats.h"
#include "unk_02005D10.h"
#include "unk_02037C94.h"
#include "struct_defs/resdat_narc_structs.h"
#include "overlay_41_sprite_resource_collection.h"
#include "trainer_case.h"
#include "union_room_manager.h"
#include "save_palpad.h"

typedef struct UnkStruct_ov73_021E6C20 {
    u8 unk_0[0x8];
    void *unk_8;
    u8 unk_C[0x30C];
    s32 unk_318;
    u8 unk_31C[0x58];
    u8 unk_374;
} UnkStruct_ov73_021E6C20;

typedef struct UnkStruct_ov73_021E6C50 {
    u8 unk_0[0x8];
    SaveData **unk_8;
    s32 unk_C;
    u8 unk_10[0x218];
    Sprite *unk_228;
    u8 unk_22C[0xF8];
    s32 unk_324;
    u8 unk_328[0x4C];
    u8 unk_374;
    u8 unk_375[0x469F];
    s8 unk_4A14;
} UnkStruct_ov73_021E6C50;

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

struct GameStats {
    u32 recordsU32[71];
    u16 recordsU16[77];
    u16 padding;
    EncodingSeed seed;
};

s32 ov73_021E6B6C(UnkStruct_020961E8 *param0, s32 param1);

s32 ov73_021E63BC(void *, String *, s32);
void ov73_021E6B98(void *arg0);
s32 ov73_021E6BC8(void *arg0, s32 arg1);
s32 ov73_021EA3D0(SaveData *, void *);
s32 ov73_021E6BF8(void *arg0, s32 arg1);

s32 ov73_021E6C20(UnkStruct_ov73_021E6C20 *arg0, s32 arg1);
s32 ov73_021E670C(void *, s32);
s32 ov73_021E756C(void *, s32, s32);
s32 ov73_021E6C50(UnkStruct_ov73_021E6C50 *arg0, s32 arg1);

s32 ov73_021E6B6C(UnkStruct_020961E8 *param0, s32 param1) {
    if ((param0->unk_4ABB == 0) && (sub_02037030(0x73, NULL, 0) == 1)) {
        param0->unk_4ABB = 1;
    }
    return param1;
}

void ov73_021E6B98(void *arg0) {
    ReadMsgDataIntoString(*(MsgData **)((u8 *)arg0 + 0x28), 0x12, *(String **)((u8 *)arg0 + 0x48));
    ov73_021E63BC(arg0 + 0x2D8, *(String **)((u8 *)arg0 + 0x48), 0);
    Sprite_SetAnimCtrlSeq(*(Sprite **)((u8 *)arg0 + 0x228), 0x25);
    *(s32 *)((u8 *)arg0 + 0xC) = 1;
}

s32 ov73_021E6BC8(void *arg0, s32 arg1) {
    if (sub_0203769C() == 0) {
        sub_02037454();
    }
    if (*(s32 *)((u8 *)arg0 + 0x4A08) == sub_02037454()) {
        *(s32 *)((u8 *)arg0 + 0x318) = 0x1A;
    }
    return arg1;
}

s32 ov73_021E6BF8(void *arg0, s32 arg1) {
    ov73_021EA3D0(*(void **)((u8 *)arg0 + 0x4A0C), arg0 + 0xF48);
    *(s32 *)((u8 *)arg0 + 0x318) = 0x1B;
    return arg1;
}

s32 ov73_021E6C20(UnkStruct_ov73_021E6C20 *arg0, s32 arg1) {
    gSystem.softResetDisabled = 1;
    GameStats_AddScore(*(GameStats **)((u8 *)(arg0->unk_8) + 0xC), 0x13);
    sub_02039EAC((void *)&arg0->unk_374);
    arg0->unk_318 = 0x1C;
    return arg1;
}

s32 ov73_021E6C50(UnkStruct_ov73_021E6C50 *arg0, s32 arg1) {
    if (sub_0203769C() == 0) {
        sub_02037454();
    }
    if (sub_02039EB4(*arg0->unk_8, 2, (void *)&arg0->unk_374) != 0) {
        StopSE(0x657U, 8);
        ov73_021E756C(arg0, 0xD, 0);
        ov73_021E670C(arg0, 0x1D);
        Sprite_SetAnimCtrlSeq(arg0->unk_228, 0);
        arg0->unk_C = 0;
        arg0->unk_324 = 0;
        gSystem.softResetDisabled = 0;
        arg0->unk_4A14 = 0;
    }
    return arg1;
}
