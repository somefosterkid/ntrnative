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

typedef struct {
    u8 unk_00;
    u8 unk_01;
    u8 unk_02;
    u8 unk_03;
} UnkStruct_02096274;


void ov73_021E705C(UnkStruct_020961E8 *param0, int param1, u32 param2);

int ov73_021E7488(void);

void sub_02038C1C(int maxNumConnection);

BOOL sub_02037030(int cmd, const void *data, int size);

int sub_02037454(void);

u16 sub_0203769C(void);

u16 sub_02033250(void);

void sub_0208FBF0(int param0, int param1, void *param2, void *param3);

void sub_0208FBF0(int param0, int param1, void *param2, void *param3) {
    UnkStruct_020961E8 *v0 = (UnkStruct_020961E8 *)param3;
    UnkStruct_02096274 v1;
    UnkStruct_02096274 *v2 = param2;

    if (param0 != 0) {
        if (sub_0203769C() == 0) {
            v1 = *v2;
            v1.unk_00 = param0;
            v1.unk_01 = v0->unk_4AAC;

            switch (v2->unk_02) {
            case 0:
                if ((v0->unk_4AAC != sub_02037454()) || (v0->unk_4AAC != ov73_021E7488()) || (v0->unk_4AAC != MATH_CountPopulation(sub_02033250()))) {
                    v1.unk_03 = 0;
                } else {
                    v0->unk_4AB4 |= 1 << param0;
                    v1.unk_03 = 1;

                    sub_02038C1C(sub_02037454());
                }
                break;
            case 1:
                break;
            }

            sub_02037030(112, &v1, sizeof(UnkStruct_02096274));
        }
    } else {
        switch (v2->unk_02) {
        case 0:
            if (v2->unk_00 == sub_0203769C()) {
                if (v2->unk_03 == 0) {
                    ov73_021E705C(v0, 8, v2->unk_00);
                } else {
                    v0->unk_4AC0 = v2->unk_01;
                    ov73_021E705C(v0, 7, v2->unk_00);
                }
            }
            break;
        case 1:
            ov73_021E705C(v0, 19, v2->unk_00);
            break;
        }
    }
}
