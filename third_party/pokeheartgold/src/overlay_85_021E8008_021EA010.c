#include "global.h"
#include "sprite_resources_header.h"
#include "field_system_def.h"

#include "bg_window.h"
#include "options.h"
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

PlayerProfile *sub_02034818(int netId);

BOOL ov85_021EA010(UnkStruct_ov109_021D5140 *param0);

BOOL ov85_021EA010(UnkStruct_ov109_021D5140 *param0) {
    int v0, v1 = 0;

    for (v0 = 0; v0 < 5; v0++) {
        param0->unk_3D8[v0][1] = param0->unk_3D8[v0][0];
        param0->unk_3D8[v0][0] = sub_02034818(v0);
        param0->unk_4AC4[v0][1] = param0->unk_4AC4[v0][0];

        if (param0->unk_3D8[v0][0] != NULL) {
            param0->unk_4AC4[v0][0].val2.unk_00 = PlayerProfile_GetTrainerID(param0->unk_3D8[v0][0]);
            param0->unk_4AC4[v0][0].val2.unk_04 = 1;
        } else {
            param0->unk_4AC4[v0][0].val1 = 0;
        }
    }

    for (v0 = 0; v0 < 5; v0++) {
        if (param0->unk_3D8[v0][1] != param0->unk_3D8[v0][0]) {
            v1 = 1;

            if (param0->unk_3D8[v0][0] == NULL) {
                param0->unk_400[v0] = 3;
            } else {
                param0->unk_400[v0] = 1;
            }
        } else {
            if (param0->unk_4AC4[v0][0].val1 != param0->unk_4AC4[v0][1].val1) {
                v1 = 1;

                if (param0->unk_4AC4[v0][0].val2.unk_04 == 0) {
                    param0->unk_400[v0] = 3;
                } else {
                    param0->unk_400[v0] = 1;
                }
            }
        }
    }

    return v1;
}
