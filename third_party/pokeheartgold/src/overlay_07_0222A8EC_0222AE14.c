#include "global.h"

#include "overlay_07_battle_anim_funcs.h"
#include "sprite_system.h"
#include "sys_task.h"

// the layout of MimicContext (overlay_07_0222A8EC_0222AEA0_0222AEA0.c)
typedef struct UnkStruct_ov07_0222AE14_arg1 {
    BattleAnimScriptFuncCommon common;
    BattleAnimSpriteInfo defenderSprite;
    BattleAnimSpriteInfo opponentSprites[2];
    s16 defenderY;
    s16 defenderHeight;
    s16 unused;
    XYTransformContext scale;
} UnkStruct_ov07_0222AE14_arg1;

s32 ov07_0222260C(void *);

s32 ov07_02222644(void *, f32 *, f32 *);

s32 ov07_022226FC(ManagedSprite *, s16, s16, s32, s32);

s32 ov07_02232508(void *);

void ov07_0222AE14(SysTask *arg0, void *param);

void ov07_0222AE14(SysTask *arg0, void *param) {
    UnkStruct_ov07_0222AE14_arg1 *arg1 = param;
    f32 x;
    f32 y;

    if (arg1->common.state == 0) {
        if (ov07_0222260C((void *)&arg1->scale) == 0) {
            ManagedSprite_SetDrawFlag(arg1->defenderSprite.hwSprite, 0);
            ManagedSprite_SetDrawFlag(arg1->opponentSprites[0].hwSprite, 0);
            ManagedSprite_SetDrawFlag(arg1->opponentSprites[0].hwSprite, 0);
            ov07_0221C448(arg1->common.battleAnimSys, arg0);
            ov07_02232508(arg1);
            return;
        }
        ov07_02222644((void *)&arg1->scale, &x, &y);
        ManagedSprite_SetAffineScale(arg1->defenderSprite.hwSprite, x, y);
        ov07_022226FC(arg1->defenderSprite.hwSprite, arg1->defenderY, arg1->defenderHeight, arg1->scale.data[4], 0);
        ManagedSprite_TickFrame(arg1->defenderSprite.hwSprite);
        ManagedSprite_TickFrame(arg1->opponentSprites[0].hwSprite);
        ManagedSprite_TickFrame(arg1->opponentSprites[1].hwSprite);
        SpriteSystem_DrawSprites(arg1->common.pokemonSpriteManager);
    }
}
