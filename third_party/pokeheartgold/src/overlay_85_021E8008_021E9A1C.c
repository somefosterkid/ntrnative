#include "global.h"
#include "sprite_resources_header.h"
#include "field_system_def.h"

#include "bg_window.h"
#include "options.h"
#include "pm_string.h"
#include "save.h"
#include "sys_task.h"
#include "unk_0200A090.h"
#include "unk_020379A0.h"
#include "overlay_85_ov109_021D5140.h"
#include "overlay_41_sprite_resource_collection.h"
#include "player_data.h"
#include "msgdata.h"
#include "game_stats.h"
#include "overlay_85_ov109_021D0F70.h"
#include "unk_struct_0209_c194.h"

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

PlayerProfile *sub_02034818(int netId);

u16 sub_0203769C(void);

void ov85_021E943C(UnkStruct_ov109_021D5140 *param0);

void ov85_021EA0EC(UnkStruct_ov109_021D5140 *param0, int param1, int param2);

void ov85_021E9458(UnkStruct_ov109_021D5140 *param0, int param1);

int ov85_021E9A1C(UnkStruct_ov109_021D5140 *param0, int param1);

int ov85_021E9A1C(UnkStruct_ov109_021D5140 *param0, int param1) {
    if (sub_0203769C() == 0) {
        ov85_021EA0EC(param0, 5, 0);
    } else {
        BufferPlayersName(param0->unk_34, 0, sub_02034818(0));
        ov85_021EA0EC(param0, 16, 0);
    }

    ov85_021E9458(param0, 14);
    param0->unk_3C4 = 0;
    ov85_021E943C(param0);

    return param1;
}

int ov85_021E9A6C(UnkStruct_ov109_021D5140 *param0, int param1);

int ov85_021E9A6C(UnkStruct_ov109_021D5140 *param0, int param1) {
    param0->unk_3C4++;

    if (param0->unk_3C4 > 45) {
        param0->unk_3B8 = 15;
    }

    ov85_021E943C(param0);
    return param1;
}

int ov85_021E9A90(UnkStruct_ov109_021D5140 *param0, int param1);

int ov85_021E9A90(UnkStruct_ov109_021D5140 *param0, int param1) {
    sub_02037AC0(202);
    param0->unk_3B8 = 16;

    ov85_021E943C(param0);
    return param1;
}
