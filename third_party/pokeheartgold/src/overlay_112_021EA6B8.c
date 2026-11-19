#include "global.h"

#include "overlay_112_pokewalker_app.h"
#include "sprite.h"
#include "sprite_system.h"

void ov112_021EA6B8(Ov112PokewalkerApp *app, s32 idx, u16 x, u16 y);

void ov112_021EA6D8(Ov112PokewalkerApp *app, s32 idx, s32 anim, u16 x, s32 y);

void ov112_021EA6B8(Ov112PokewalkerApp *app, s32 idx, u16 x, u16 y) {
    ManagedSprite_SetPositionXYWithSubscreenOffset(app->sprites[idx], x, y, 0x100000);
    return;
}

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov112_021EA6D8(Ov112PokewalkerApp *app, s32 idx, s32 anim, u16 x, s32 y) {
    if (y < 0) {
        ManagedSprite_SetPositionXYWithSubscreenOffset(app->sprites[idx + 0x75], x, (s16)(y + 0xD0), FX32_CONST(256));
        ManagedSprite_SetAnim(app->sprites[idx + 0x75], anim + 4);
        ManagedSprite_SetDrawFlag(app->sprites[idx + 0x75], 1);
        ManagedSprite_SetDrawFlag(app->sprites[idx + 0x11], 0);
        return;
    }
    ManagedSprite_SetPositionXY(app->sprites[idx + 0x11], x, (s16)y);
    ManagedSprite_SetAnim(app->sprites[idx + 0x11], anim + 0x13);
    ManagedSprite_SetDrawFlag(app->sprites[idx + 0x75], 0);
    ManagedSprite_SetDrawFlag(app->sprites[idx + 0x11], 1);
}
#else
// DS form: the sprites[] base and the byte offset of the slot stay in two registers (ldr [r6, r7]); the indexed form re-shifts.
void ov112_021EA6D8(Ov112PokewalkerApp *app, s32 idx, s32 anim, u16 x, s32 y) {
    u8 *sprites;
    s32 offs;
    u32 new_var;
    if (y < 0) {
        new_var = 0xd0;
        sprites = (u8 *)app->sprites;
        offs = (idx + 0x75) * 4;
        ManagedSprite_SetPositionXYWithSubscreenOffset(*((ManagedSprite **)(sprites + offs)), x, (s16)(((u32)((y + new_var) * 0x10000)) >> 0x10), FX32_CONST(256));
        ManagedSprite_SetAnim(*((ManagedSprite **)(sprites + offs)), anim + 4);
        ManagedSprite_SetDrawFlag(*((ManagedSprite **)(sprites + offs)), 1);
        ManagedSprite_SetDrawFlag(*((ManagedSprite **)(((u8 *)app + ((idx + 0x11) * 4)) + offsetof(Ov112PokewalkerApp, sprites))), 0);
        return;
    }
    sprites = (u8 *)app->sprites;
    offs = (idx + 0x11) * 4;
    ManagedSprite_SetPositionXY(*((ManagedSprite **)(sprites + offs)), x, (s16)y);
    ManagedSprite_SetAnim(*((ManagedSprite **)(sprites + offs)), anim + 0x13);
    ManagedSprite_SetDrawFlag(*((ManagedSprite **)(((u8 *)app + ((idx + 0x75) * 4)) + offsetof(Ov112PokewalkerApp, sprites))), 0);
    ManagedSprite_SetDrawFlag(*((ManagedSprite **)(sprites + offs)), 1);
    return;
}
#endif
