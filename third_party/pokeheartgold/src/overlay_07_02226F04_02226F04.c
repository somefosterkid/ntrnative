#include "global.h"

#include "heap.h"
#include "overlay_07_battle_anim_funcs.h"
#include "sprite_system.h"
#include "overlay_07_set_pokemon_sprite_priority_context.h"
#include "sys_task.h"

s32 ov07_02226CB0(void *);

void ov07_02226F04(SysTask *arg0, void *ptr);

void ov07_02226F04(SysTask *arg0, void *ptr) {
    if (*((s32 *)&((SetPokemonSpritePriorityContext *)ptr)->mode) != 0) {
        ov07_02226CB0(ptr);
    }
    *(u8 *)ptr = (u8)(*(u8 *)ptr + 1);
    if ((s32) * (u8 *)ptr >= (s32) *((s32 *)&((SetPokemonSpritePriorityContext *)ptr)->maxFrames)) {
        reg_GX_DISPCNT = (s32)(0xFFFF1FFF & reg_GX_DISPCNT);
        reg_G2_WININ = (u16)(reg_G2_WININ & ~0x3F);
        reg_G2_WINOUT = (u16)(reg_G2_WINOUT & ~0x3F);
        reg_G2_WIN0H = 0;
        reg_G2_WIN0V = 0;
        ManagedSprite_SetDrawFlag(((SetPokemonSpritePriorityContext *)ptr)->sprite, 0);
        SpriteSystem_DrawSprites(((SetPokemonSpritePriorityContext *)ptr)->common.pokemonSpriteManager);
        ov07_0221C448(*(void **)((u8 *)ptr + 4), arg0);
        Heap_Free(ptr);
        return;
    }
    ManagedSprite_TickFrame(((SetPokemonSpritePriorityContext *)ptr)->sprite);
    SpriteSystem_DrawSprites(((SetPokemonSpritePriorityContext *)ptr)->common.pokemonSpriteManager);
}
