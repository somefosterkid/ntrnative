#include "global.h"
#include "overlay_73_unk_struct_020961_e8_t.h"

#include "overlay_manager.h"
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







typedef u32 TextColor;

typedef struct Menu Menu;

BOOL sub_02037030(int cmd, const void *data, int size);

u16 sub_0203769C(void);

BOOL IsPaletteFadeFinished(void);

u16 sub_02033250(void);

BOOL ov73_021E735C(Window *param0, int unused, u32 param2, UnkStruct_020961E8 *param3);

int ov73_021E7340(void);

void ov73_021E762C(UnkStruct_020961E8 *param0);

int ov73_021E7870(UnkStruct_020961E8 *param0, int param1);

int ov59_021D17F8(UnkStruct_020961E8 *param0, int param1);

int ov59_021D1838(UnkStruct_020961E8 *param0, int param1);

int ov73_021E6B6C(UnkStruct_020961E8 *param0, int param1);

int ov73_021E6BC8(UnkStruct_020961E8 *param0, int param1);

int ov59_021D1DA0(UnkStruct_020961E8 *param0, int param1);

int ov59_021D1C64(UnkStruct_020961E8 *param0, int param1);

int ov59_021D1C34(UnkStruct_020961E8 *param0, int param1);

int ov59_021D1A14(UnkStruct_020961E8 *param0, int param1);

int ov73_021E68AC(UnkStruct_020961E8 *param0, int param1);

int ov59_021D1B74(UnkStruct_020961E8 *param0, int param1);

int ov73_021E69E8(UnkStruct_020961E8 *param0, int param1);

int ov59_021D1C14(UnkStruct_020961E8 *param0, int param1);

int ov59_021D1E98(UnkStruct_020961E8 *param0, int param1);

int ov73_021E6CFC(UnkStruct_020961E8 *param0, int param1);

int ov59_021D1EF4(UnkStruct_020961E8 *param0, int param1);

int ov73_021E6D54(UnkStruct_020961E8 *param0, int param1);

int ov73_021E6DF8(UnkStruct_020961E8 *param0, int param1);

int ov59_021D2044(UnkStruct_020961E8 *param0, int param1);

int ov73_021E6E8C(UnkStruct_020961E8 *param0, int param1);

int ov59_021D20A4(UnkStruct_020961E8 *param0, int param1);

int ov59_021D20D0(UnkStruct_020961E8 *param0, int param1);

int ov59_021D19C0(UnkStruct_020961E8 *param0, int param1);

int ov59_021D20F4(UnkStruct_020961E8 *param0, int param1);

int ov73_021E6FE8(UnkStruct_020961E8 *param0, int param1);

int ov73_021E6FAC(UnkStruct_020961E8 *param0, int param1);

int ov59_021D1998(UnkStruct_020961E8 *param0, int param1);

int ov73_021E6778(UnkStruct_020961E8 *param0, int param1);

int ov59_021D1DC8(UnkStruct_020961E8 *param0, int param1);

int ov73_021E6C50(UnkStruct_020961E8 *param0, int param1);

int ov73_021E6CC0(UnkStruct_020961E8 *param0, int param1);

int ov73_021E6E48(UnkStruct_020961E8 *param0, int param1);

extern int (*_021EA7C0[31])(UnkStruct_020961E8 *, int);

int ov73_021E746C(void);

int ov73_021E5AB8(OverlayManager *appMan, int *param1);

int ov73_021E5AB8(OverlayManager *appMan, int *param1) {
    UnkStruct_020961E8 *v0 = OverlayManager_GetData(appMan);

    if ((sub_0203769C() == 0) && (v0->unk_4AB4 != 0)) {
        v0->unk_4AB4 &= sub_02033250();
    }

    switch (*param1) {
    case 0:
        if (IsPaletteFadeFinished()) {
            *param1 = 1;

            if (sub_0203769C() != 0) {
                if (ov73_021E746C() > 2) {
                    sub_02037030(114, 0L, 0);
                }
            }
        }
        break;
    case 1:
        if (_021EA7C0[v0->unk_3A8] != 0L) {
            *param1 = (*_021EA7C0[v0->unk_3A8])(v0, *param1);
        }

        if (v0->unk_4AA4 == 0) {
            ov73_021E735C(v0->unk_2FC, 0, ((TextColor)(((1 & 0xFF) << 16) | ((3 & 0xFF) << 8) | ((0 & 0xFF) << 0))), v0);
        }

        ov73_021E762C(v0);

        if (sub_0203769C() == 0) {
            int v1 = ov73_021E7870(v0, 1);

            if (*param1 == 1) {
                *param1 = v1;
            }
        }
        break;
    case 2:
        if (_021EA7C0[v0->unk_3A8] != 0L) {
            *param1 = (*_021EA7C0[v0->unk_3A8])(v0, *param1);
        }
        break;
    case 3:
        if (IsPaletteFadeFinished()) {
            return 1;
        }
        break;
    }

    SpriteList_RenderAndAnimateSprites(v0->unk_50);

    return 0;
}
