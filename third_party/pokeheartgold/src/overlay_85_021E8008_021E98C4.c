#include "global.h"
#include "sprite_resources_header.h"
#include "field_system_def.h"

#include "bg_window.h"
#include "options.h"
#include "overlay_85.h"
#include "pm_string.h"
#include "save.h"
#include "sys_task.h"
#include "unk_0200A090.h"
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

int ov85_021E98C4(UnkStruct_ov109_021D5140 *param0, int param1);

int ov85_021E98C4(UnkStruct_ov109_021D5140 *param0, int param1) {
    if (param0->unk_3C4++ > 60) {
        param0->unk_3B8 = 15;
    }

    return param1;
}

s32 ov85_021E943C(s32);

s32 ov85_021E9458(s32, s32);

s32 ov85_021EA0EC(s32, s32, s32);

s32 ov85_021E98E0(s32 arg0, s32 arg1) {
    ov85_021EA0EC(arg0, 5, 0);
    ov85_021E9458(arg0, 0xA);
    ov85_021E943C(arg0);
    return arg1;
}
