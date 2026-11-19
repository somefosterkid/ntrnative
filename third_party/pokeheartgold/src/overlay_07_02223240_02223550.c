#include "global.h"
#include "overlay_07_battle_anim_funcs.h"
#include "sys_task.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_anim_common.h"
#include "constants/heap.h"
#include "pokepic.h"
#include "sprite.h"
#include "gf_gfx_planes.h"
#include "filesystem_files_def.h"
#include "palette.h"
#include "sprite_system.h"

struct BattleAnimSystem;

struct BgConfig;

struct Pokepic;

struct PokepicDrawParam;

struct SysTask;

enum BgOffsetUpdateOp {
    BG_OFFSET_UPDATE_SET_X = 0,
    BG_OFFSET_UPDATE_ADD_X,
    BG_OFFSET_UPDATE_SUB_X,
    BG_OFFSET_UPDATE_SET_Y,
    BG_OFFSET_UPDATE_ADD_Y,
    BG_OFFSET_UPDATE_SUB_Y,
};

typedef struct BgConfig BgConfig;

void ToggleBgLayer(u8 bgLayer, u8 enable);

void BgSetPosTextAndCommit(BgConfig *bgConfig, u8 bgLayer, u8 bgOffsetUpdateOp, int val);

void BgClearTilemapBufferAndCommit(BgConfig *bgConfig, u8 bgLayer);

typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);

enum BgNarcMemberType {
    BG_NARC_MEMBER_NCGR = 0,
    BG_NARC_MEMBER_NCLR,
    BG_NARC_MEMBER_NSCR1,
    BG_NARC_MEMBER_NSCR2,
    BG_NARC_MEMBER_NSCR3,

    BG_NARC_MEMBER_COUNT,
};

SysTask *ov07_0221C410(BattleAnimSystem *system, SysTaskFunc func, void *param);

u16 ov07_0221C468(BattleAnimSystem *system);

s32 ov07_0221C4A8(BattleAnimSystem *system, int index);

int ov07_0221FB7C(int bgID, enum BgNarcMemberType type);

typedef struct SpriteShakeInfo {
    s16 extentX;
    s16 extentY;
    s16 interval;
    s16 amount;
    int targets;
} SpriteShakeInfo;

enum Battler {
    BTLSCR_ALL_BATTLERS = 0,
    BTLSCR_ATTACKER = 1,
    BTLSCR_DEFENDER = 2,
    BTLSCR_PLAYER = 3,
    BTLSCR_ENEMY = 4,
    BTLSCR_FAINTED_MON = 5,
    BTLSCR_SWITCHED_MON = 6,
    BTLSCR_SIDE_EFFECT_MON = 7,
    BTLSCR_ABILITY_MON = 8,
    BTLSCR_PLAYER_SLOT_1 = 9,
    BTLSCR_ENEMY_SLOT_1 = 10,
    BTLSCR_PLAYER_SLOT_2 = 11,
    BTLSCR_ENEMY_SLOT_2 = 12,
    BTLSCR_UNUSED_13 = 13,
    BTLSCR_MSG_ATTACKER = 14,
    BTLSCR_MSG_DEFENDER = 15,
    BTLSCR_ATTACKER_PARTNER = 16,
    BTLSCR_DEFENDER_PARTNER = 17,
    BTLSCR_FORCED_OUT = 18,
    BTLSCR_ATTACKER_ENEMY = 19,
    BTLSCR_DEFENDER_ENEMY = 20,
    BTLSCR_MSG_BATTLER_TEMP = 21,
    BTLSCR_SWITCHED_MON_AFTER = 22,
};

enum Battler ov07_0223192C(BattleAnimSystem *system, int battler);

u32 GfGfxLoader_LoadCharData(enum NarcId narcID, u32 narcMemberIdx, BgConfig *bgConfig, u32 bgLayer, u32 offset, u32 size, BOOL compressed, u32 heapID);

void GfGfxLoader_LoadScrnData(enum NarcId narcID, u32 narcMemberIdx, BgConfig *bgConfig, u32 bgLayer, u32 offset, u32 size, BOOL compressed, enum HeapID heapID);

typedef struct ScrollCustomBgContext {
    BOOL reverse;
    BOOL scrollFinished;
    u16 targetBgAlpha;
    s16 x;
    s16 y;
    s16 stepX;
    s16 stepY;
    u16 slowDownTime;
    u16 timer;
    u16 bgAlpha;
    u16 otherAlpha;
    BattleAnimScriptFuncCommon common;
    SpriteShakeInfo unused0;
    BattleAnimSpriteInfo sLotoId[4];
    XYTransformContext unused2;
} ScrollCustomBgContext;

void ov07_022233D8(SysTask *task, void *param);

void ov07_02223550(BattleAnimSystem *system, s32 arg1, s32 arg2);

void ov07_02231FE4(BattleAnimSystem *system, BattleAnimScriptFuncCommon *common);

enum HeapID ov07_0221BFD0(BattleAnimSystem *system);

BOOL ov07_0221BFC0(BattleAnimSystem *system);

void ov07_02223550(BattleAnimSystem *system, s32 arg1, s32 arg2) {
    ScrollCustomBgContext *ctx = ov07_022324D8(system, sizeof(ScrollCustomBgContext));

    ov07_02231FE4(system, &ctx->common);

    ctx->x = ov07_0221C4A8(system, 1);
    ctx->y = ov07_0221C4A8(system, 2);
    ctx->stepX = ov07_0221C4A8(system, 3);
    ctx->stepY = ov07_0221C4A8(system, 4);
    ctx->reverse = ov07_0221C4A8(system, 5);
    ctx->targetBgAlpha = ov07_0221C4A8(system, 6);
    ctx->slowDownTime = ov07_0221C4A8(system, 7);
    ctx->timer = 0;

    if (ctx->reverse && ov07_0223192C(system, ov07_0221C468(system)) == BTLSCR_ENEMY) {
        ctx->x *= -1;
        ctx->y *= -1;
        ctx->stepX *= -1;
        ctx->stepY *= -1;
        ctx->y -= ((128 / 3) * 2);
    } else {
        ctx->y += ((128 / 3) * 2);
    }

    ctx->bgAlpha = 0;
    ctx->otherAlpha = 31;
    ctx->scrollFinished = 0;

    ToggleBgLayer(GF_BG_LYR_MAIN_2, 0);

    int bgID = ov07_0221C4A8(system, 0);

    GfGfxLoader_LoadCharData(
        NARC_a_0_0_7,
        ov07_0221FB7C(bgID, BG_NARC_MEMBER_NCGR),
        ctx->common.bgConfig,
        GF_BG_LYR_MAIN_2,
        0,
        0,
        1,
        ov07_0221BFD0(system));
    PaletteData_LoadNarc(
        ctx->common.paletteData,
        NARC_a_0_0_7,
        ov07_0221FB7C(bgID, BG_NARC_MEMBER_NCLR),
        ov07_0221BFD0(system),
        PLTTBUF_MAIN_BG,
        ((16) * sizeof(u16)),
        ((9) * (16)));
    BgClearTilemapBufferAndCommit(ctx->common.bgConfig, GF_BG_LYR_MAIN_2);

    enum BgNarcMemberType member = BG_NARC_MEMBER_NSCR1;

    if (ov07_0221BFC0(system) == 1) {
        member = BG_NARC_MEMBER_NSCR3;
    } else if (ov07_0223192C(system, ov07_0221C468(system)) == BTLSCR_ENEMY) {
        member = BG_NARC_MEMBER_NSCR2;
    }

    GfGfxLoader_LoadScrnData(
        NARC_a_0_0_7,
        ov07_0221FB7C(bgID, member),
        ctx->common.bgConfig,
        GF_BG_LYR_MAIN_2,
        0,
        0,
        1,
        ov07_0221BFD0(system));

    BgSetPosTextAndCommit(ctx->common.bgConfig, GF_BG_LYR_MAIN_2, BG_OFFSET_UPDATE_SET_X, ctx->x);
    BgSetPosTextAndCommit(ctx->common.bgConfig, GF_BG_LYR_MAIN_2, BG_OFFSET_UPDATE_SET_Y, ctx->y);

    ov07_0221C410(ctx->common.battleAnimSys, ov07_022233D8, ctx);
}
