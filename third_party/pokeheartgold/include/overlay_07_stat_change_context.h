#ifndef POKEHEARTGOLD_OVERLAY_07_STAT_CHANGE_CONTEXT_H
#define POKEHEARTGOLD_OVERLAY_07_STAT_CHANGE_CONTEXT_H

#include "global.h"
#include "filesystem_files_def.h"
#include "gf_gfx_planes.h"
#include "sprite_system.h"
#include "overlay_07_battle_anim_common.h"

// sizeof(StatChangeConfig) == 0x18
typedef struct StatChangeConfig {
    NarcId narcID;
    int memberTableIndex;
    int mode;
    s16 stepX;
    s16 stepY;
    s16 offsetX;
    s16 offsetY;
    u16 overlayAlpha;
    u16 otherAlpha;
} StatChangeConfig;

// sizeof(StatChangeContext) == 0x40 on the DS; common.battleAnimSys is read at +0x1C, bgConfig at +0x2C
typedef struct StatChangeContext {
    StatChangeConfig config;
    BattleAnimScriptFuncCommon common;
    ManagedSprite *attackerSprite;
    ManagedSprite *attackerSprite2;
    ManagedSprite *attackerPartnerSprite;
    u8 timer;
} StatChangeContext;

void ov07_02231524(StatChangeContext *ctx, GFBgLayer bgLayer);

#endif // POKEHEARTGOLD_OVERLAY_07_STAT_CHANGE_CONTEXT_H
