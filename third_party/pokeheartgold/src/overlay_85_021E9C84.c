#include "global.h"
#include "sprite_resources_header.h"
#include "field_system_def.h"

#include "bg_window.h"
#include "options.h"
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

PlayerProfile *sub_02034818(int netId);

u16 sub_0203769C(void);

void ov85_021EA0EC(UnkStruct_ov109_021D5140 *param0, int param1, int param2);

int ov85_021EA17C(int param0);

void ov85_021EA19C(UnkStruct_ov109_021D5140 *param0);

void ov85_021E9C84(UnkStruct_ov109_021D5140 *param0, int param1, u8 param2);

int sub_02037454(void);
BOOL sub_02096D4C(UnkStruct_0209BDF8 *param0, u32 param1, const void *param2, u32 param3);
void ov85_021EA39C(UnkStruct_ov109_021D5140 *param0, int param1);
void ov85_021E9D9C(UnkStruct_ov109_021D5140 *param0, int param1, u8 param2);

void ov85_021E9C84(UnkStruct_ov109_021D5140 *param0, int param1, u8 param2) {
    switch (param1) {
    case 2:
        ov85_021EA19C(param0);
        break;
    case 13:
        if (param0->unk_394 != 0L) {
            Clear2dMenuWindowAndDelete(param0->unk_394, HEAP_ID_102);
            param0->unk_394 = 0L;
        }

        break;
    case 25:
        if (ov85_021EA17C(param0->unk_5C) == 0) {
            RemoveTextPrinter(param0->unk_5C);
        }

        ov85_021EA0EC(param0, 12, 0);

        if (param0->unk_394 != 0L) {
            Clear2dMenuWindowAndDelete(param0->unk_394, HEAP_ID_102);
            param0->unk_394 = 0L;
        }

        break;
    case 19:
        if (param0->unk_4AB4 == 1) {
            return;
        }

        BufferPlayersName(param0->unk_34, 0, sub_02034818(param2));

        if (param2 == sub_0203769C()) {
            return;
        }

        if (param0->unk_394 != 0L) {
            Clear2dMenuWindowAndDelete(param0->unk_394, HEAP_ID_102);
            param0->unk_394 = 0L;
        }

        if (sub_0203769C() == 0) {
            param0->unk_10->unk_30 &= 0xffff ^ param2;
        }
        break;
    case 7:
        break;
    case 8:
        break;
    case 31:
        if (ov85_021EA17C(param0->unk_5C) == 0) {
            RemoveTextPrinter(param0->unk_5C);
        }

        if (param0->unk_394 != 0L) {
            Clear2dMenuWindowAndDelete(param0->unk_394, HEAP_ID_102);
            param0->unk_394 = 0L;
        }

        break;
    default:
        (("FALSE") ? (void)0 : GF_AssertFail());
        return;
    }

    param0->unk_3B8 = param1;
}

void ov85_021E9D9C(UnkStruct_ov109_021D5140 *param0, int param1, u8 param2) {
    if (param0->unk_3B8 == 3) {
        switch (param1) {
        case 1:
            BufferPlayersName(param0->unk_34, 0, sub_02034818(param2));
            ov85_021EA39C(param0, 1);

            param0->unk_3B8 = param1;
            param0->unk_10->unk_2C = sub_02037454();
            param0->unk_10->unk_30 = 0;

            if (sub_0203769C() == 0) {
                int v0 = 1;
                sub_02096D4C(param0->unk_10, 7, &v0, 1);
            }
            break;
        case 19:
            BufferPlayersName(param0->unk_34, 0, sub_02034818(param2));

            if (param2 == sub_0203769C()) {
                return;
            }

            if (sub_0203769C() == 0) {
                param0->unk_10->unk_30 &= 0xffff ^ param2;
            }

            param0->unk_3B8 = param1;
            break;
        default:
            GF_ASSERT(FALSE);
            return;
        }
    } else if (param1 == 1) {
        param0->unk_10->unk_30 = 0;
        param0->unk_10->unk_2C = sub_02037454();
    }
}
