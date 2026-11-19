#include "global.h"

#include "constants/heap.h"

#include "overlay_07_battle_anim_funcs.h"
#include "struct_defs/resdat_narc_structs.h"

#include "heap.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_anim_common.h"

struct BattleAnimSystem;

enum HeapID ov07_0221BFD0(BattleAnimSystem *system);

ManagedSpriteTemplate ov07_0221F9E8(BattleAnimSystem *system);

void ov07_02231E08(BattleAnimSystem *system, int spriteAlpha, int bgAlpha);

typedef struct HelpingHandSpriteContext {
    u8 state;
    u8 unused0[6];
    u8 sLotoId;
    u8 handCount;
    u8 spriteAlpha;
    u8 bgAlpha;
    BattleAnimSystem *battleAnimSys;
    SpriteSystem *spriteSys;
    SpriteManager *spriteMan;
    ManagedSprite *handSprites[6];
    int currentPair;
    ManagedSprite *hand1;
    ManagedSprite *hand2;
    XYTransformContext hand1Pos;
    XYTransformContext hand2Pos;
} HelpingHandSpriteContext;

enum HelpingHandSpriteState {
    HELPING_HAND_SPRITE_STATE_FADE_IN = 0,
    HELPING_HAND_SPRITE_STATE_SETUP_MOVE,
    HELPING_HAND_SPRITE_STATE_MOVE,
    HELPING_HAND_SPRITE_STATE_FADE_OUT,
};

extern const Point2D ov07_02236796[6];

void ov07_02229FC4(SysTask *task, void *param);

void ov07_0222A1F0(BattleAnimSystem *system, SpriteSystem *spriteSys, SpriteManager *spriteMan, ManagedSprite *sprite);

void ov07_0222A1F0(BattleAnimSystem *system, SpriteSystem *spriteSys, SpriteManager *spriteMan, ManagedSprite *sprite) {
    int i;
    ManagedSpriteTemplate template;

    HelpingHandSpriteContext *ctx = Heap_Alloc(ov07_0221BFD0(system), sizeof(HelpingHandSpriteContext));

    ((ctx != 0L) ? (void)0 : GF_AssertFail());

    ctx->sLotoId = 0;
    ctx->state = HELPING_HAND_SPRITE_STATE_FADE_IN;
    ctx->spriteSys = spriteSys;
    ctx->spriteMan = spriteMan;
    ctx->currentPair = 0;
    ctx->battleAnimSys = system;

    template = ov07_0221F9E8(system);
    ov07_02231E08(ctx->battleAnimSys, -1, -1);

    ctx->spriteAlpha = 0;
    ctx->bgAlpha = 15;

    G2_ChangeBlendAlpha(ctx->spriteAlpha, ctx->bgAlpha);

    ctx->handCount = ov07_0221C4A8(ctx->battleAnimSys, 0);
    ctx->handSprites[0] = sprite;

    for (i = 1; i < ctx->handCount; i++) {
        ctx->handSprites[i] = SpriteSystem_NewSprite(ctx->spriteSys, ctx->spriteMan, &template);
    }

    ManagedSprite_SetAnim(ctx->handSprites[0], 0);
    ManagedSprite_SetAnim(ctx->handSprites[1], 0);
    ManagedSprite_SetAnim(ctx->handSprites[2], 1);
    ManagedSprite_SetAnim(ctx->handSprites[3], 1);
    ManagedSprite_SetAnim(ctx->handSprites[4], 2);
    ManagedSprite_SetAnim(ctx->handSprites[5], 3);
    ManagedSprite_SetFlipMode(ctx->handSprites[0], 1);
    ManagedSprite_SetFlipMode(ctx->handSprites[3], 1);

    for (i = 0; i < ctx->handCount; i++) {
        ManagedSprite_SetAnimateFlag(ctx->handSprites[i], 1);
        ManagedSprite_SetPositionXY(ctx->handSprites[i], ov07_02236796[i].x, ov07_02236796[i].y);
        ManagedSprite_SetOamMode(ctx->handSprites[i], GX_OAM_MODE_XLU);
    }

    ManagedSprite_SetDrawFlag(ctx->handSprites[1], 0);
    ManagedSprite_SetDrawFlag(ctx->handSprites[2], 0);
    ManagedSprite_SetDrawFlag(ctx->handSprites[4], 0);
    ManagedSprite_SetDrawFlag(ctx->handSprites[5], 0);

    ov07_0221C3F4(system, ov07_02229FC4, ctx, 1100);
}
