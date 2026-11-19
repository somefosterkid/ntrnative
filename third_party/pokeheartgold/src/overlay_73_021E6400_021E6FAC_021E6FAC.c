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

int sub_02037454(void);

void ov73_021E66F0(UnkStruct_020961E8 *unused);

int ov73_021E6FAC(UnkStruct_020961E8 *param0, int param1);

int ov73_021E6FAC(UnkStruct_020961E8 *param0, int param1) {
    if ((param0->unk_4ABC != 0) && (sub_02037454() != param0->unk_4ABC)) {
        param0->unk_4ABC = 0;
    }

    param0->unk_3A8 = 21;
    param0->unk_3B4 = 0;

    ov73_021E66F0(param0);
    return param1;
}

u16 sub_0203769C(void);

void ov73_021E781C(UnkStruct_020961E8 *param0, int param1);

int ov73_021E6FE8(UnkStruct_020961E8 *param0, int param1);

int ov73_021E6FE8(UnkStruct_020961E8 *param0, int param1) {
    if ((param0->unk_4ABC != 0) && (sub_02037454() != param0->unk_4ABC)) {
        param0->unk_4ABC = 0;
    }

    param0->unk_4AC4--;

    if (param0->unk_4AC4 < 0) {
        param0->unk_4ABC = 0;
    }

    if ((++param0->unk_3B4 > 60) && (param0->unk_4ABC == 0)) {
        param0->unk_3A8 = 0;

        if (sub_0203769C() == 0) {
            ov73_021E781C(param0, 1);
        }
    }

    ov73_021E66F0(param0);
    return param1;
}
