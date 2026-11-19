#include "global.h"

#include "overlay_07_battle_anim_system.h"
#include "sprite_system.h"
#include "sys_task_api.h"

void ov07_0221DAD0(SysTask *task, void *param);

void ov07_0221DAD0(SysTask *task, void *param) {
    BattleAnimPokemonDrawContext *drawContext = param;
    if (drawContext->active == 0) {
        SysTask_Destroy(task);
        return;
    }
    if (drawContext->battleAnimSys->pokemonSpriteManager != NULL) {
        SpriteSystem_DrawSprites(drawContext->spriteManager);
    }
}
