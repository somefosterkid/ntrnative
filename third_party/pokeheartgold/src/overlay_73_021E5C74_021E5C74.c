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

void ov59_021D23B0(u16 *param0);

extern const u8 ov73_021EA52A[5][2];

void ov73_021E72F4(u16 *param0);

void ov73_021E5C74(SysTask *sysTask, void *param1);

void ov73_021E5C74(SysTask *sysTask, void *param1) {
    UnkStruct_020961E8 *v0 = (UnkStruct_020961E8 *)param1;
    UnkStruct_ov59_021D109C *v1 = &v0->unk_0C;

    if (v1->unk_00) {
        if (v1->unk_08 > ov73_021EA52A[v1->unk_04][0]) {
            u16 *v2;

            v1->unk_08 = 0;
            v1->unk_04++;

            if (ov73_021EA52A[v1->unk_04][1] == 0xff) {
                v1->unk_04 = 0;
            }

            v2 = (u16 *)v1->unk_10->pRawData;
            GX_LoadOBJPltt(&v2[16 * ov73_021EA52A[v1->unk_04][1]], 0, 32);
        } else {
            v0->unk_0C.unk_08++;
        }

        ov73_021E72F4(&v0->unk_408);
    }
}
