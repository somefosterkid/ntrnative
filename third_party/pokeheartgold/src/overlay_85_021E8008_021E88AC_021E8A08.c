#include "global.h"
#include "sprite_resources_header.h"
#include "field_system_def.h"
#include "overlay_85_ov109_021D5140.h"
#include "overlay_manager.h"
#include "save.h"
#include "options.h"
#include "overlay_41_sprite_resource_collection.h"
#include "player_data.h"
#include "msgdata.h"
#include "game_stats.h"
#include "overlay_85_ov109_021D0F70.h"
#include "unk_struct_0209_c194.h"

struct BgConfig;

struct G2dRenderer;

struct GameStats;

struct JournalEntry;

struct ListMenu2D;

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

struct UnkStruct_ov109_021D0F70_t;

struct Window;

typedef struct UnkStruct_0205C22C_t UnkStruct_0205C22C;

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

typedef struct JournalEntry JournalEntry;

typedef u32 TextColor;

typedef struct ListMenu2D ListMenu2D;

u16 sub_0203769C(void);

BOOL IsPaletteFadeFinished(void);

BOOL sub_02096D4C(UnkStruct_0209BDF8 *param0, u32 param1, const void *param2, u32 param3);

u16 sub_02033250(void);

BOOL ov85_021E9EC0(Window *param0, int param1, TextColor param2, UnkStruct_ov109_021D5140 *param3);

int ov109_021D548C(void);

void ov85_021EA1AC(UnkStruct_ov109_021D5140 *param0);

int ov85_021EA3F0(UnkStruct_ov109_021D5140 *param0, int param1);

int ov109_021D474C(UnkStruct_ov109_021D5140 *param0, int param1);

int ov109_021D478C(UnkStruct_ov109_021D5140 *param0, int param1);

int ov85_021E97B8(UnkStruct_ov109_021D5140 *param0, int param1);

int ov85_021E97E8(UnkStruct_ov109_021D5140 *param0, int param1);

int ov85_021E9814(UnkStruct_ov109_021D5140 *param0, int param1);

int ov85_021E9700(UnkStruct_ov109_021D5140 *param0, int param1);

int ov85_021E96D0(UnkStruct_ov109_021D5140 *param0, int param1);

int ov85_021E94BC(UnkStruct_ov109_021D5140 *param0, int param1);

void *ov85_021E94EC(UnkStruct_ov109_021D5140 *param0, void *param1);

int ov109_021D4AA8(UnkStruct_ov109_021D5140 *param0, int param1);

int ov85_021E962C(UnkStruct_ov109_021D5140 *param0, int param1);

int ov109_021D4B44(UnkStruct_ov109_021D5140 *param0, int param1);

int ov109_021D4D9C(UnkStruct_ov109_021D5140 *param0, int param1);

int ov85_021E9900(UnkStruct_ov109_021D5140 *param0, int param1);

int ov85_021E993C(UnkStruct_ov109_021D5140 *param0, int param1);

int ov85_021E996C(UnkStruct_ov109_021D5140 *param0, int param1);

int ov85_021E9A1C(UnkStruct_ov109_021D5140 *param0, int param1);

int ov85_021E9A90(UnkStruct_ov109_021D5140 *param0, int param1);

int ov85_021E9AB0(UnkStruct_ov109_021D5140 *param0, int param1);

int ov109_021D4FAC(UnkStruct_ov109_021D5140 *param0, int param1);

int ov109_021D4FD8(UnkStruct_ov109_021D5140 *param0, int param1);

int ov85_021E9468(UnkStruct_ov109_021D5140 *param0, int param1);

int ov85_021E9B40(UnkStruct_ov109_021D5140 *param0, int param1);

int ov85_021E9BDC(UnkStruct_ov109_021D5140 *param0, int param1);

int ov85_021E9BA0(UnkStruct_ov109_021D5140 *param0, int param1);

int ov109_021D48D4(UnkStruct_ov109_021D5140 *param0, int param1);

int ov85_021E948C(UnkStruct_ov109_021D5140 *param0, int param1);

int ov109_021D4CC8(UnkStruct_ov109_021D5140 *param0, int param1);

int ov85_021E9864(UnkStruct_ov109_021D5140 *param0, int param1);

int ov85_021E98C4(UnkStruct_ov109_021D5140 *param0, int param1);

int ov85_021E9A6C(UnkStruct_ov109_021D5140 *param0, int param1);

int ov85_021E9C50(UnkStruct_ov109_021D5140 *param0, int param1);

extern int (*const ov85_021EA9E0[32])(UnkStruct_ov109_021D5140 *, int);

int ov85_021E8A08(OverlayManager *appMan, int *param1);

BOOL System_GetTouchNew(void);

int ov85_021E9FD0(void);

int ov85_021E8A08(OverlayManager *appMan, int *param1) {
    UnkStruct_ov109_021D5140 *v0 = OverlayManager_GetData(appMan);
    if (System_GetTouchNew()) {
        ((u32 *)&gSystem)[0x17] = 1;
    }

    if ((sub_0203769C() == 0) && (v0->unk_10->unk_30 != 0)) {
        v0->unk_10->unk_30 &= sub_02033250();
    }

    switch (v0->unk_00) {
    case 0:
        if (IsPaletteFadeFinished()) {
            v0->unk_00 = 1;

            if (sub_0203769C() != 0) {
                if (ov85_021E9FD0() > 2) {
                    sub_02096D4C(v0->unk_10, 4, 0L, 0);
                }
            }
        }

        break;
    case 1:

        if (ov85_021EA9E0[v0->unk_3B8] != 0L) {
            v0->unk_00 = (*ov85_021EA9E0[v0->unk_3B8])(v0, v0->unk_00);
        }

        if (v0->unk_10->unk_24 == 0) {
            ov85_021E9EC0(
                v0->unk_30C, 0, ((TextColor)(((1 & 0xFF) << 16) | ((3 & 0xFF) << 8) | ((0 & 0xFF) << 0))), v0);
        }

        ov85_021EA1AC(v0);

        if (sub_0203769C() == 0) {
            int v1 = ov85_021EA3F0(v0, 1);

            if (v0->unk_00 == 1) {
                v0->unk_00 = v1;
            }
        }

        break;
    case 2:
        if (ov85_021EA9E0[v0->unk_3B8] != 0L) {
            v0->unk_00 = (*ov85_021EA9E0[v0->unk_3B8])(v0, v0->unk_00);
        }

        break;
    case 3:
        if (IsPaletteFadeFinished()) {
            return 1;
        }

        break;
    }

    SpriteList_RenderAndAnimateSprites(v0->unk_60);
    return 0;
}
