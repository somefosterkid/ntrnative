#include "global.h"
#include "sprite_resources_header.h"
#include "field_system_def.h"

#include "options.h"
#include "pm_string.h"
#include "save.h"
#include "sys_task.h"
#include "unk_02005D10.h"
#include "unk_0200A090.h"
#include "overlay_85_ov109_021D5140.h"
#include "overlay_41_sprite_resource_collection.h"
#include "player_data.h"
#include "msgdata.h"
#include "screen_fade_internal.h"
#include "game_stats.h"
#include "overlay_85_ov109_021D0F70.h"
#include "unk_struct_0209_c194.h"

struct BgConfig;

struct GameStats;

struct JournalEntry;

struct ListMenu2D;

struct MsgData;

struct SpriteList;

struct SpriteResourceCollection;

struct MessageFormat;

struct SysTaskQueue;

struct PlayerProfile;

struct UnkStruct_0205C22C_t;

struct UnkStruct_ov109_021D0F70_t;

typedef struct UnkStruct_0205C22C_t UnkStruct_0205C22C;

typedef struct JournalEntry JournalEntry;

typedef struct ListMenu2D ListMenu2D;

int sub_02037454(void);

u16 sub_0203769C(void);

typedef void (*UnkCallback)(void);

enum ButtonMode {
    BUTTON_MODE_NORMAL = 0,
    BUTTON_MODE_START_IS_X = 1,
    BUTTON_MODE_SWAP_XY = 2,
    BUTTON_MODE_L_IS_A = 3,
};

BOOL sub_02096D4C(UnkStruct_0209BDF8 *param0, u32 param1, const void *param2, u32 param3);

void ov85_021EA0EC(UnkStruct_ov109_021D5140 *param0, int param1, int param2);

int ov85_021E9FD0(void);

void ov85_021E9458(UnkStruct_ov109_021D5140 *param0, int param1);

void ov85_021EA39C(UnkStruct_ov109_021D5140 *param0, int param1);

void ov85_021E9324(UnkStruct_ov109_021D5140 *param0);

void ov85_021E943C(void);

void ov85_021E9324(UnkStruct_ov109_021D5140 *param0) {
    if (gSystem.newKeys & 0x0001) {
        if (sub_0203769C() == 0) {
            if ((ov85_021E9FD0() == param0->unk_10->unk_2C) && (param0->unk_10->unk_30 == 0)) {
                u8 v0 = 1;

                ov85_021EA0EC(param0, 3, 0);
                ov85_021E9458(param0, 22);
                sub_02096D4C(param0->unk_10, 7, &v0, 1);
                ov85_021EA39C(param0, 0);
            } else {
                PlaySE(1522);
            }
        }
    } else if (gSystem.newKeys & 0x0002) {
        if (sub_0203769C()) {
            if (param0->unk_10->unk_28 == 0) {
                ov85_021EA0EC(param0, 4, 0);
                ov85_021E9458(param0, 4);
            } else {
                PlaySE(1522);
            }
        } else {
            if ((sub_02037454() == param0->unk_10->unk_2C) && (param0->unk_10->unk_30 == 0)) {
                u8 v1 = 1;

                ov85_021EA0EC(param0, 4, 0);
                ov85_021E9458(param0, 4);
                sub_02096D4C(param0->unk_10, 7, &v1, 1);
                ov85_021EA39C(param0, 0);
            } else {
                PlaySE(1522);
            }
        }
    } else {
        if (param0->unk_4AAC == 0) {
            if ((sub_0203769C() == 0) && (sub_02037454() == param0->unk_10->unk_2C)) {
                u8 v2 = 0;

                sub_02096D4C(param0->unk_10, 7, &v2, 1);
            }
        }
    }
}

void ov85_021E943C(void) {
}
