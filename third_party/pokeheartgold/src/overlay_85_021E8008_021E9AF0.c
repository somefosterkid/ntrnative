#include "global.h"
#include "sprite_resources_header.h"
#include "field_system_def.h"

#include "bg_window.h"
#include "options.h"
#include "overlay_85.h"
#include "pm_string.h"
#include "save.h"
#include "sys_task.h"
#include "text.h"
#include "unk_0200A090.h"
#include "overlay_85_ov109_021D5140.h"
#include "overlay_41_sprite_resource_collection.h"
#include "player_data.h"
#include "msgdata.h"
#include "game_stats.h"
#include "overlay_85_ov109_021D0F70.h"
#include "unk_struct_0209_c194.h"

s32 ov85_021E943C(void *);

s32 ov85_021EA0EC(void *, s32, s32);

s32 ov85_021EA17C(s32);

s32 ov85_021E9AF0(UnkStruct_ov109_021D5140 *arg0, s32 arg1) {
    if (ov85_021EA17C(arg0->unk_5C) != 0) {
        ov85_021EA0EC(arg0, 9, 0);
    }
    arg0->unk_3B8 = 0x12;
    ov85_021E943C(arg0);
    return arg1;
}

s32 ov85_021E9B1C(UnkStruct_ov109_021D5140 *arg0, s32 arg1) {
    if (ov85_021EA17C(arg0->unk_5C) != 0) {
        arg0->unk_3B8 = 0xA;
    }
    ov85_021E943C(arg0);
    return arg1;
}

struct GameStats;

struct JournalEntry;

struct ListMenu2D;

struct MsgData;

struct SpriteList;

struct SpriteResourceCollection;

struct MessageFormat;

struct PlayerProfile;

struct UnkStruct_0205C22C_t;

struct UnkStruct_ov109_021D0F70_t;

typedef struct UnkStruct_0205C22C_t UnkStruct_0205C22C;

typedef struct JournalEntry JournalEntry;

typedef struct ListMenu2D ListMenu2D;

int sub_02037454(void);

u16 sub_0203769C(void);

void ov85_021EA39C(UnkStruct_ov109_021D5140 *param0, int param1);

int ov85_021E9B40(UnkStruct_ov109_021D5140 *param0, int param1);

int ov85_021E9B40(UnkStruct_ov109_021D5140 *param0, int param1) {
    if ((param0->unk_5C != 0xff) && (ov85_021EA17C(param0->unk_5C) == 0)) {
        RemoveTextPrinter(param0->unk_5C);
    }

    ov85_021EA0EC(param0, 15, 1);
    param0->unk_3B8 = 20;

    if (sub_0203769C() == 0) {
        ov85_021EA39C(param0, 0);
    }

    param0->unk_4ABC = sub_02037454();
    param0->unk_4AB8 = (10 * 30);

    ov85_021E943C(param0);

    return param1;
}

int ov85_021E9BA0(UnkStruct_ov109_021D5140 *param0, int param1);

int ov85_021E9BA0(UnkStruct_ov109_021D5140 *param0, int param1) {
    if ((param0->unk_4ABC != 0) && (sub_02037454() != param0->unk_4ABC)) {
        param0->unk_4ABC = 0;
    }

    param0->unk_3B8 = 21;
    param0->unk_3C4 = 0;

    ov85_021E943C(param0);
    return param1;
}

int ov85_021E9BDC(UnkStruct_ov109_021D5140 *param0, int param1);

int ov85_021E9BDC(UnkStruct_ov109_021D5140 *param0, int param1) {
    if ((param0->unk_4ABC != 0) && (sub_02037454() != param0->unk_4ABC)) {
        param0->unk_4ABC = 0;
    }

    param0->unk_4AB8--;

    if (param0->unk_4AB8 < 0) {
        param0->unk_4ABC = 0;
    }

    if ((++param0->unk_3C4 > 60) && (param0->unk_4ABC == 0)) {
        param0->unk_3B8 = 0;

        if (sub_0203769C() == 0) {
            ov85_021EA39C(param0, 1);
        }
    }

    ov85_021E943C(param0);
    return param1;
}
