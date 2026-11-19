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


PlayerProfile *sub_02034818(int netId, int, u8);

BOOL sub_02037030(int cmd, const void *data, int size);

int sub_02037454(void);

u16 sub_0203769C(void);

void ov73_021E781C(UnkStruct_020961E8 *param0, int param1);

void ov73_021E7120(UnkStruct_020961E8 *param0, int param1, u8 param2);

void ov73_021E7120(UnkStruct_020961E8 *param0, int param1, u8 param2) {
    if (param0->unk_3A8 == 3) {
        switch (param1) {
        case 1:
            BufferPlayersName(param0->unk_24, 0, sub_02034818(param2, param1, param2));
            ov73_021E781C(param0, 1);

            param0->unk_3A8 = param1;
            param0->unk_4AAC = sub_02037454();
            param0->unk_4AB4 = 0;

            if (sub_0203769C() == 0) {
                int v0 = 1;
                sub_02037030(117, &v0, 1);
            }
            break;
        case 19:
            BufferPlayersName(param0->unk_24, 0, sub_02034818(param2, param1, param2));

            if (param2 == sub_0203769C()) {
                return;
            }

            if (sub_0203769C() == 0) {
                param0->unk_4AB4 &= 0xffff ^ param2;
            }

            param0->unk_3A8 = param1;
            break;
        default:
            GF_ASSERT(FALSE);
        }
    } else if (param1 == 1) {
        param0->unk_4AB4 = 0;
        param0->unk_4AAC = sub_02037454();
    }
}
