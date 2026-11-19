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

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov85_021E943C(void);
#else
s32 ov85_021E943C();
#endif

s32 ov85_021E9614(UnkStruct_ov109_021D5140 *arg0, s32 arg1) {
    arg0->unk_4AC0 = 0;
    ov85_021E943C();
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

typedef struct {
    u8 unk_00;
    u8 unk_01;
    u8 unk_02;
    u8 unk_03;
} UnkStruct_0209BF64;

typedef struct UnkStruct_0205C22C_t UnkStruct_0205C22C;

typedef struct JournalEntry JournalEntry;

typedef struct ListMenu2D ListMenu2D;

int sub_02037454(void);

u16 sub_0203769C(void);

BOOL sub_02096D4C(UnkStruct_0209BDF8 *param0, u32 param1, const void *param2, u32 param3);

int ov85_021E9FD0(void);

int ov85_021E962C(UnkStruct_ov109_021D5140 *param0, int param1);

int ov85_021E962C(UnkStruct_ov109_021D5140 *param0, int param1) {
    if ((param0->unk_10->unk_38 != sub_02037454()) || (param0->unk_10->unk_38 != ov85_021E9FD0())) {
        param0->unk_4AC0 = 0;
        param0->unk_3B8 = 8;
#if defined(SDK_PORT) || defined(NONMATCHING)
        ov85_021E943C();
#else
        ov85_021E943C(param0);
#endif
        return param1;
    }

    param0->unk_4AC0++;

    if (param0->unk_4AC0 > 30) {
        UnkStruct_0209BF64 v0;

        MI_CpuClear8(&v0, sizeof(UnkStruct_0209BF64));
        v0.unk_02 = 1;
        v0.unk_00 = sub_0203769C();

        sub_02096D4C(param0->unk_10, 2, &v0, sizeof(UnkStruct_0209BF64));

        param0->unk_4AC0 = 0;
        param0->unk_3B8 = 9;
    }

#if defined(SDK_PORT) || defined(NONMATCHING)
    ov85_021E943C();
#else
    ov85_021E943C(param0);
#endif
    return param1;
}
