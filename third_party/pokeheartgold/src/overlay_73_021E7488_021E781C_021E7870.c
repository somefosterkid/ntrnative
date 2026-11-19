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

void ov73_021E6830(UnkStruct_020961E8 *param0);

BOOL sub_02037030(int cmd, const void *data, int size);

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

u16 sub_02033250(void);

int ov73_021E746C(void);

void ov73_021E781C(UnkStruct_020961E8 *param0, int param1);

int ov73_021E7870(UnkStruct_020961E8 *param0, int param1);

int ov73_021E7870(UnkStruct_020961E8 *param0, int param1) {
    int v0 = ov73_021E746C();
    if (v0 > param0->unk_4AAC) {
        u8 v1 = 1;
        sub_02037030(117, &v1, 1);
        param0->unk_4AB0 = 1;
    } else {
        param0->unk_4AB0 = 0;
    }
    if (v0 == param0->unk_4AA8) {
        return 1;
    }
    switch (v0) {
    case 1:
        if ((sub_02037454() > 1) || (sub_02033250() > 1)) {
            return 1;
        }
        param0->unk_3A8 = 17;
        ov73_021E781C(param0, -1);
        ov73_021E6830(param0);
        return 2;

    case 2:
    case 3:
    case 4:
        sub_0205A904(UR_STATUS_MIXING_RECORDS);
        if (v0 < param0->unk_4AA8) {
            switch (param0->unk_4AB9) {
            case 0:
                ov73_021E781C(param0, param1);
                break;

            case 1:
                ov73_021E781C(param0, 0);
                break;
            }

            param0->unk_4AAC = sub_02037454();
        }
        break;

    case 5:
        sub_0205A904(UR_STATUS_UNK_09);
        ov73_021E781C(param0, param1);
    }

    param0->unk_4AA8 = ov73_021E746C();
    return 1;
}
