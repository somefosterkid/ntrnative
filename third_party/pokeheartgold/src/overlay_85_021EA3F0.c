#include "global.h"
#include "sprite_resources_header.h"
#include "field_system_def.h"

#include "bg_window.h"
#include "options.h"
#include "pm_string.h"
#include "save.h"
#include "sys_task.h"
#include "unk_0200A090.h"
#include "unk_02032844.h"
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

void Clear2dMenuWindowAndDelete(ListMenu2D *menu, u32 heapID);

int sub_02037454(void);

enum UnionRoomStatus {
    UR_STATUS_NONE = 0,
    UR_STATUS_DRAWING,
    UR_STATUS_MIXING_RECORDS,
    UR_STATUS_UNK_03,
    UR_STATUS_BUSY,
    UR_STATUS_SHOWING_TRAINER_CASE,
    UR_STATUS_BATTLING,
    UR_STATUS_TRADING,
    UR_STATUS_UNK_08,
    UR_STATUS_UNK_09,
    UR_STATUS_UNK_10,
    UR_STATUS_UNK_11,
    UR_STATUS_UNK_12,
    UR_STATUS_SPIN_TRADING,
};

void sub_0205A904(enum UnionRoomStatus status);

BOOL sub_02096D4C(UnkStruct_0209BDF8 *param0, u32 param1, const void *param2, u32 param3);

int ov85_021E9FD0(void);

void ov85_021EA39C(UnkStruct_ov109_021D5140 *param0, int param1);

int ov85_021EA3F0(UnkStruct_ov109_021D5140 *param0, int param1);

void ov85_021EA4EC(void);

int ov85_021EA3F0(UnkStruct_ov109_021D5140 *param0, int param1) {
    int v0 = ov85_021E9FD0();

    if (v0 > param0->unk_10->unk_2C) {
        u8 v1 = 1;

        sub_02096D4C(param0->unk_10, 7, &v1, 1);
        param0->unk_4AAC = 1;
    } else {
        param0->unk_4AAC = 0;
    }

    if (v0 == param0->unk_4AA8) {
        return 1;
    }

    switch (v0) {
    case 1:
        if ((sub_02037454() > 1) || (sub_02033250() > 1)) {
            return 1;
        }

        param0->unk_3B8 = 17;
        ov85_021EA39C(param0, -1);

        if (param0->unk_394 != 0L) {
            Clear2dMenuWindowAndDelete(param0->unk_394, HEAP_ID_102);
            param0->unk_394 = 0L;
        }
        return 2;
        break;
    case 2:
    case 3:
    case 4:
        sub_0205A904(UR_STATUS_SPIN_TRADING);

        if (v0 < param0->unk_4AA8) {
            switch (param0->unk_4AB5) {
            case 0:
                ov85_021EA39C(param0, param1);
                break;
            case 1:
                ov85_021EA39C(param0, 0);
                break;
            }

            param0->unk_10->unk_2C = sub_02037454();
        }
        break;
    case 5:
        sub_0205A904(UR_STATUS_UNK_12);
        ov85_021EA39C(param0, param1);
        break;
    }

    param0->unk_4AA8 = ov85_021E9FD0();

    return 1;
}

void ov85_021EA4EC(void) {
}
