#include "global.h"
#include "field_system_def.h"
#include "simple3dmodel.h"

#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "options.h"
#include "palette.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "save.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "unk_02005D10.h"
#include "unk_struct_0209_bdf8.h"
#include "overlay_85_ov109_021D5140.h"
#include "simple3danimation.h"
#include "overlay_85_ov109_021D0F70.h"
#include "player_data.h"
#include "game_stats.h"
#include "msgdata.h"
#include "unk_struct_0209_c194.h"

struct GameStats;

struct JournalEntry;

struct MsgData;

struct MessageFormat;

struct PlayerProfile;

struct UnkStruct_0205C22C_t;

struct UnkStruct_ov109_021D0F70_t;

struct UnkStruct_ov109_021D5140_t;

typedef struct UnkStruct_0205C22C_t UnkStruct_0205C22C;

typedef struct JournalEntry JournalEntry;

int ov85_021E652C(UnkStruct_ov109_021D0F70 *param0);

int ov85_021E652C(UnkStruct_ov109_021D0F70 *param0) {
    if (param0->unk_0C < (30 * 4)) {
        param0->unk_0C++;
    } else if (IsFanfarePlaying() == 0) {
        param0->unk_0C = 0;
        param0->unk_00 = 43;
    }

    return 0;
}

void ov85_021E7194(UnkStruct_ov109_021D0F70 *param0, u32 param1);

int ov85_021E6550(UnkStruct_ov109_021D0F70 *param0);

int ov85_021E6550(UnkStruct_ov109_021D0F70 *param0) {
    if (param0->unk_0C < (30 * 4)) {
        param0->unk_0C++;
    } else if (IsFanfarePlaying() == 0) {
        param0->unk_0C = 0;
        param0->unk_00 = 40;
        ov85_021E7194(param0, 4);
    }

    return 0;
}
