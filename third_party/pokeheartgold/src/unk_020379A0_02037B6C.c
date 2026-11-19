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
#include "unk_struct_0209_bdf8.h"
#include "overlay_85_ov109_021D5140.h"
#include "simple3danimation.h"
#include "overlay_85_ov109_021D0F70.h"
#include "unk_struct_0209_c194.h"
#include "player_data.h"
#include "game_stats.h"
#include "msgdata.h"
#include "unk_struct_0209_c194.h"
#include "comm_tool.h"


void sub_02037B6C(s32 param_1, u32 param_2, u8 *param_3);

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
BOOL sub_020376E0(int cmd, const void *data);
int sub_02037B88(UnkStruct_ov109_021D0F70 *param0);
void sub_02037B8C(u8 param0, u8 param1);

void sub_02037B6C(s32 param_1, u32 param_2, u8 *param_3) {
    _021D414C->unk_00[param_1].unk_00 = *param_3;
    _021D414C->unk_00[param_1].unk_01 = param_3[1];
    return;
}

int sub_02037B88(UnkStruct_ov109_021D0F70 *param0) {
    return 2;
}

void sub_02037B8C(u8 param0, u8 param1) {
    UnkStruct_02036574 v0;

    v0.unk_00 = param0;
    v0.unk_01 = param1;

    sub_020376E0(19, &v0);
}
