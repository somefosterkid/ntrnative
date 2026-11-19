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
#include "system.h"
#include "touchscreen.h"
#include "unk_02005D10.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_02035900.h"
#include "wild_encounter.h"
#include "struct_defs/resdat_narc_structs.h"
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

int ov73_021E746C(void);

void ov73_021E781C(UnkStruct_020961E8 *param0, int param1);

s32 ov73_021E64B0(UnkStruct_020961E8 *, s32);

s32 ov73_021E670C(UnkStruct_020961E8 *, s32);

s32 ov73_021E71E4(UnkStruct_020961E8 *, s32, s8);

s32 ov73_021E756C(UnkStruct_020961E8 *, s32, s32);

extern TouchscreenHitbox ov73_021EA534;

void ov73_021E6508(UnkStruct_020961E8 *arg0, s32 arg1);

void ov73_021E6508(UnkStruct_020961E8 *arg0, s32 arg1) {
    u8 sp4[8];
    s32 temp_r6;
    u8 var_r4;

    var_r4 = 0;
    ov73_021E64B0(arg0, arg1);
    temp_r6 = TouchscreenHitbox_FindRectAtTouchNew(&ov73_021EA534);
    *(u16 *)&sp4[4] = 0xFFD5;
    if ((temp_r6 != -1) && (DoesPixelAtScreenXYMatchPtrVal(arg0->unk_00, 1U, gSystem.touchX, gSystem.touchY, (u16 *)&sp4[4]) != 0)) {
        var_r4 = temp_r6 + 1;
    }
    if ((1 & gSystem.newKeys) || (var_r4 == 1)) {
        if (sub_0203769C() == 0) {
            if ((arg0->unk_4AAC == ov73_021E746C()) && (arg0->unk_4AB4 == 0)) {
                sp4[2] = 1;
                ov73_021E756C(arg0, 3, 0);
                ov73_021E670C(arg0, 0x16);
                sub_02037030(0x75, &sp4[2], 1);
                ov73_021E781C(arg0, 0);
                PlaySE(0x5DCU);
                ov73_021E71E4(arg0, 0, 1);
                ScheduleBgTilemapBufferTransfer(arg0->unk_00, 1U);
            } else {
                PlaySE(0x5F2U);
            }
        }
    } else if ((2 & gSystem.newKeys) || (var_r4 == 2)) {
        if (sub_0203769C() != 0) {
            if (arg0->unk_4AA0 == 0) {
                ov73_021E756C(arg0, 4, 0);
                ov73_021E670C(arg0, 4);
                PlaySE(0x940U);
            } else if (arg0->unk_4A38 < 0) {
                arg0->unk_4A38 = 2;
                PlaySE(0x5F2U);
            }
        } else if ((arg0->unk_4AAC == sub_02037454()) && (arg0->unk_4AB4 == 0)) {
            sp4[1] = 1;
            ov73_021E756C(arg0, 4, 0);
            ov73_021E670C(arg0, 4);
            sub_02037030(0x75, &sp4[1], 1);
            ov73_021E781C(arg0, 0);
            PlaySE(0x940U);
            ov73_021E71E4(arg0, 1, 1);
            ScheduleBgTilemapBufferTransfer(arg0->unk_00, 1U);
        } else {
            PlaySE(0x5F2U);
        }
    } else if ((arg0->unk_4AB0 == 0) && (sub_0203769C() == 0) && (arg0->unk_4AAC == sub_02037454())) {
        sp4[0] = 0;
        sub_02037030(0x75, sp4, 1);
    }
    if (arg0->unk_4A38 >= 0) {
        arg0->unk_4A38--;
    }
    if (arg0->unk_4A38 == 0) {
        ov73_021E71E4(arg0, 1, 0);
        ScheduleBgTilemapBufferTransfer(arg0->unk_00, 1U);
    }
}
