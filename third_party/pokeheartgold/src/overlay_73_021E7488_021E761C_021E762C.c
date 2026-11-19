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


void Sprite_SetDrawFlag(Sprite *sprite, BOOL draw);





typedef struct JournalEntry JournalEntry;







typedef struct Menu Menu;

u16 sub_0203769C(void);

BOOL PlaySE(u16 seqID);

void ov73_021E77A4(NNSG2dCharacterData *param0[2], NNSG2dPaletteData *param1[2], int param2, int param3, int param4);

void ov73_021E762C(UnkStruct_020961E8 *param0);

void ov73_021E762C(UnkStruct_020961E8 *param0) {
    int v0, v1 = 0;

    for (v0 = 0; v0 < 5; v0++) {
        switch (param0->unk_3F0[v0]) {
        case 0:
            break;
        case 1: {
            int v2, v3;

            ((param0->unk_3C8[v0][0] != 0L) ? (void)0 : GF_AssertFail());

            v3 = PlayerProfile_GetTrainerGender(param0->unk_3C8[v0][0]);
            v2 = PlayerProfile_GetAvatar(param0->unk_3C8[v0][0]);

            if (sub_0203769C() == v0) {
                Sprite_SetAnimCtrlSeq(param0->unk_28C[v0 + 1], 38 + v3 * 2);
            } else {
                ov73_021E77A4(param0->unk_390, param0->unk_3A0, v0, v2, v3);
                Sprite_SetAnimCtrlSeq(param0->unk_28C[v0 + 1], 27 + v0 * 2);
            }
        }
            Sprite_SetDrawFlag(param0->unk_28C[v0 + 1], 1);
            param0->unk_3F0[v0] = 2;
            v1 = 1;
            break;
        case 2:
            break;
        case 3:
            if (sub_0203769C() == v0) {
                int v4 = PlayerProfile_GetTrainerGender(param0->unk_3C8[v0][0]);
                Sprite_SetAnimCtrlSeq(param0->unk_28C[v0 + 1], 38 + v4 * 2 + 1);
            } else {
                Sprite_SetAnimCtrlSeq(param0->unk_28C[v0 + 1], 27 + v0 * 2 + 1);
            }

            param0->unk_3F0[v0] = 0;
            break;
        }
    }

    if (v1) {
        PlaySE(1614);
    }
}
