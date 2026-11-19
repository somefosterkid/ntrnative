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

typedef struct {
    u8 unk_00;
    u8 unk_01;
    u8 unk_02;
    u8 unk_03;
} UnkStruct_02096274;

BOOL sub_02037030(int cmd, const void *data, int size);

int sub_02037454(UnkStruct_020961E8 *, int);

u16 sub_0203769C(void);

void ov73_021E66F0(UnkStruct_020961E8 *unused);

int ov73_021E746C(void);

int ov73_021E69E8(UnkStruct_020961E8 *param0, int param1);

int ov73_021E69E8(UnkStruct_020961E8 *param0, int param1) {
    if ((param0->unk_4AC0 != sub_02037454(param0, param1)) || (param0->unk_4AC0 != ov73_021E746C())) {
        param0->unk_4AC2 = 0;
        param0->unk_3A8 = 8;
        ov73_021E66F0(param0);
        return param1;
    }

    param0->unk_4AC2++;

    if (param0->unk_4AC2 > 30) {
        UnkStruct_02096274 v0;

        MI_CpuClear8(&v0, sizeof(UnkStruct_02096274));

        v0.unk_02 = 1;
        v0.unk_00 = sub_0203769C();

        sub_02037030(112, &v0, sizeof(UnkStruct_02096274));

        param0->unk_4AC2 = 0;
        param0->unk_3A8 = 9;
    }

    ov73_021E66F0(param0);
    return param1;
}
