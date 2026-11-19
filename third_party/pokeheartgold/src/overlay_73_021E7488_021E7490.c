#include "global.h"
#include "overlay_73_unk_struct_020961_e8_t.h"
#include "save.h"
#include "options.h"
#include "pm_string.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "player_data.h"
#include "msgdata.h"
#include "game_stats.h"

struct BgConfig;

struct G2dRenderer;

struct GameStats;

struct JournalEntry;

struct Menu;

struct MsgData;

struct Options;

struct SaveData;

struct Sprite;

struct SpriteList;

struct SpriteResource;

struct SpriteResourceCollection;

struct SpriteResourcesHeader;

struct String;

struct MessageFormat;

struct SysTask;

struct SysTaskQueue;

struct PlayerProfile;

struct UnkStruct_0205C22C_t;

struct UnkStruct_020961E8_t;

struct Window;


typedef struct UnkStruct_0205C22C_t UnkStruct_0205C22C;

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};


typedef struct JournalEntry JournalEntry;







typedef struct Menu Menu;

PlayerProfile *sub_02034818(int netId);

BOOL ov73_021E7490(UnkStruct_020961E8 *param0);

BOOL ov73_021E7490(UnkStruct_020961E8 *param0) {
    int v0, v1 = 0;

    for (v0 = 0; v0 < 5; v0++) {
        param0->unk_3C8[v0][1] = param0->unk_3C8[v0][0];
        param0->unk_3C8[v0][0] = sub_02034818(v0);
        param0->unk_4AC8[v0][1] = param0->unk_4AC8[v0][0];

        if (param0->unk_3C8[v0][0] != NULL) {
            param0->unk_4AC8[v0][0].val2.unk_00 = PlayerProfile_GetTrainerID(param0->unk_3C8[v0][0]);
            param0->unk_4AC8[v0][0].val2.unk_04 = 1;
        } else {
            param0->unk_4AC8[v0][0].val1 = 0;
        }
    }

    for (v0 = 0; v0 < 5; v0++) {
        if (param0->unk_3C8[v0][1] != param0->unk_3C8[v0][0]) {
            v1 = 1;

            if (param0->unk_3C8[v0][0] == NULL) {
                param0->unk_3F0[v0] = 3;
            } else {
                param0->unk_3F0[v0] = 1;
            }
        } else {
            if (param0->unk_4AC8[v0][0].val1 != param0->unk_4AC8[v0][1].val1) {
                v1 = 1;

                if (param0->unk_4AC8[v0][0].val2.unk_04 == 0) {
                    param0->unk_3F0[v0] = 3;
                } else {
                    param0->unk_3F0[v0] = 1;
                }
            }
        }
    }

    return v1;
}
