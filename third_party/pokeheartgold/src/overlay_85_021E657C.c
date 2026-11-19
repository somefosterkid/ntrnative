#include "global.h"
#include "field_system_def.h"
#include "simple3dmodel.h"

#include "camera.h"
#include "game_stats.h"
#include "options.h"
#include "overlay_85.h"
#include "palette.h"
#include "pm_string.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "unk_020379A0.h"
#include "unk_struct_0209_bdf8.h"
#include "overlay_85_ov109_021D5140.h"
#include "simple3danimation.h"
#include "overlay_85_ov109_021D0F70.h"
#include "player_data.h"
#include "msgdata.h"
#include "unk_struct_0209_c194.h"

struct BgConfig;

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

int ov85_021E657C(UnkStruct_ov109_021D0F70 *param0);

int ov85_021E657C(UnkStruct_ov109_021D0F70 *param0) {
    if (param0->unk_0C < (30 * 4)) {
        param0->unk_0C++;
    } else {
        param0->unk_0C = 0;
        param0->unk_00 = 43;
    }

    return 0;
}

void ov85_021E7194(u32 *, u32);

u32 ov85_021E6594(u32 *param_1) {
    GameStats_Inc(*(GameStats **)(param_1[0x33] + 0x28), 0x78);
    GameStats_AddScore(*(GameStats **)(param_1[0x33] + 0x28), 0x22);
    ov85_021E7194(param_1, 0xb);
    sub_02037AC0(0xca);
    ((u8 *)&gSystem)[0x6c] = 1;
    *param_1 = 0x2c;
    return 0;
}
