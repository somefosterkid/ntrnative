#include "global.h"

#include "overlay_07_battle_anim_funcs.h"
#include "pokepic.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_grudge_sprite_context.h"

ManagedSpriteTemplate ov07_0221F9E8(BattleAnimSystem *system);

void ov07_02231378(SysTask *task, void *param);

void ov07_02231468(BattleAnimSystem *arg0, SpriteSystem *arg1, SpriteManager *arg2, ManagedSprite *arg3);

void ov07_02231468(BattleAnimSystem *arg0, SpriteSystem *arg1, SpriteManager *arg2, ManagedSprite *arg3) {
    ManagedSpriteTemplate unkTemplate;
    SpriteManager *spriteManager;
    Pokepic *pokepic;
    s32 v1;
    GrudgeSpriteContext *v0;
    s32 v2;

    spriteManager = arg2;
    v0 = ov07_022324D8(arg0, sizeof(GrudgeSpriteContext));
    v0->battleAnimSys = arg0;
    v0->spriteSys = arg1;
    v0->spriteMan = spriteManager;
    pokepic = ov07_0221FA48(v0->battleAnimSys, ov07_0221C468(arg0));
    v0->attackerX = (s16)Pokepic_GetAttr(pokepic, 0);
    v0->attackerY = (s16)Pokepic_GetAttr(pokepic, 1);
    unkTemplate = ov07_0221F9E8(arg0);
    v1 = 0;
    v2 = 0;
    do {
        if (v1 == 0) {
            v0->flameSprites[v2] = arg3;
        } else {
            v0->flameSprites[v2] = SpriteSystem_NewSprite(v0->spriteSys, v0->spriteMan, &unkTemplate);
        }
        ManagedSprite_SetDrawPriority(v0->flameSprites[v2], 0x64U);
        ManagedSprite_SetOamMode(v0->flameSprites[v2], GX_OAM_MODE_XLU);
        ManagedSprite_SetAnimateFlag(v0->flameSprites[v2], 1);
        v1 += 1;
        v2 += 1;
    } while (v1 < 6);
    v0->dir = ov07_02222004(v0->battleAnimSys, ov07_0221C468(v0->battleAnimSys));
    ov07_0221C410(v0->battleAnimSys, ov07_02231378, v0);
}

void ov07_02231514(void *arg0, s16 arg1, s16 arg2);

void ov07_02231514(void *arg0, s16 arg1, s16 arg2) {
    *(s16 *)((u8 *)arg0 + 0xC) = arg1;
    *(s16 *)((u8 *)arg0 + 0xE) = arg2;
}

void ov07_0223151C(void *arg0, s16 arg1, s16 arg2);

void ov07_0223151C(void *arg0, s16 arg1, s16 arg2) {
    *(s16 *)((u8 *)arg0 + 0x14) = arg1;
    *(s16 *)((u8 *)arg0 + 0x16) = arg2;
}
