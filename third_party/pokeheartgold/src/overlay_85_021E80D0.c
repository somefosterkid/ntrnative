#include "global.h"
#include "field_system_def.h"
#include "simple3dmodel.h"

#include "bg_window.h"
#include "camera.h"
#include "error_handling.h"
#include "filesystem.h"
#include "options.h"
#include "overlay_85.h"
#include "palette.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "save.h"
#include "sprite_system.h"
#include "sys_task_api.h"
#include "sys_task.h"
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

void ov85_021E8008(UnkStruct_ov109_021D0F70 *param0, UnkStruct_ov109_021D34A8 *param1);

void ov85_021E8128(UnkStruct_ov109_021D0F70 *param0);
BOOL ov85_021E8144(UnkStruct_ov109_021D0F70 *param0);

BOOL ov85_021E8150(UnkStruct_ov109_021D0F70 *param0);

void ov85_021E80D0(SysTask *param0, void *param1) {
    UnkStruct_ov109_021D0F70 *v0 = param1;
    UnkStruct_ov109_021D34A8 *v1 = &v0->unk_C30;

    ov85_021E8008(v0, v1);
}

void ov85_021E80E0(UnkStruct_ov109_021D0F70 *param0);

void ov85_021E80E0(UnkStruct_ov109_021D0F70 *param0) {
    UnkStruct_ov109_021D34A8 *v0 = &param0->unk_C30;
    memset(v0, 0, sizeof(UnkStruct_ov109_021D34A8));
    param0->unk_DBC = SysTask_CreateOnMainQueue(ov85_021E80D0, param0, 255);

    GF_ASSERT(param0->unk_DBC);
}

void ov85_021E8118(UnkStruct_ov109_021D0F70 *param0);

void ov85_021E8118(UnkStruct_ov109_021D0F70 *param0) {
    SysTask_Destroy(param0->unk_DBC);
}

void ov85_021E8128(UnkStruct_ov109_021D0F70 *param0) {
    UnkStruct_ov109_021D34A8 *v0 = &param0->unk_C30;
    GF_ASSERT(v0->unk_04 == 0);
    v0->unk_08 = 1;
    v0->unk_00 = 1;
}

BOOL ov85_021E8144(UnkStruct_ov109_021D0F70 *param0) {
    UnkStruct_ov109_021D34A8 *v0 = &param0->unk_C30;
    return v0->unk_04;
}

BOOL ov85_021E8150(UnkStruct_ov109_021D0F70 *param0) {
    UnkStruct_ov109_021D34A8 *v0 = &param0->unk_C30;
    return v0->unk_08;
}
