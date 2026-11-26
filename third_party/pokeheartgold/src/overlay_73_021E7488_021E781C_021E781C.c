#include "global.h"
#include "sprite_resources_header.h"
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

#if defined(SDK_PORT) || defined(NONMATCHING)
void sub_02038C1C(s8 maxNumConnection);
#else
void sub_02038C1C(int maxNumConnection);
#endif

int sub_02037454(void);

u16 sub_0203769C(UnkStruct_020961E8 *, int);

void ov73_021E781C(UnkStruct_020961E8 *param0, int param1);

void ov73_021E781C(UnkStruct_020961E8 *param0, int param1) {
    if (sub_0203769C(param0, param1) == 0) {
        if (param1 == -1) {
            sub_02038C1C(1);
        } else {
            int v0 = sub_02037454() + param1;

            if (v0 > 5) {
                v0 = 5;
            }

            sub_02038C1C(v0);
        }

        if (param1 == -1) {
            param0->unk_4AB9 = 2;
        } else if (param1 == 0) {
            param0->unk_4AB9 = 1;
        } else {
            param0->unk_4AB9 = 0;
        }
    }
}
