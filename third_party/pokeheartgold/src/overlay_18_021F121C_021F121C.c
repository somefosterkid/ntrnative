#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "sprite.h"
#include "sprite_system.h"

void ov18_021F121C(PokedexAppData *pokedexApp, s32 param_2, s16 param_3, u32 param_4, s32 param_5);

void ov18_021F121C(PokedexAppData *pokedexApp, s32 param_2, s16 param_3, u32 param_4, s32 param_5) {
    s16 uStack_18[2];
    if (param_5 == 0) {
        ManagedSprite_GetPositionXY(pokedexApp->unk_0670[param_2], &uStack_18[1], &uStack_18[0]);
        ManagedSprite_SetPositionXY(pokedexApp->unk_0670[param_2], uStack_18[1] + param_3, (s16)(((u32)((uStack_18[0] + param_4) * 0x10000)) >> 0x10));
        return;
    }
    ManagedSprite_GetPositionXYWithSubscreenOffset(pokedexApp->unk_0670[param_2], &uStack_18[1], &uStack_18[0], (fx32)((512 > 0) ? ((512 * (1 << 12)) + 0.5f) : ((512 * (1 << 12)) - 0.5f)));
    ManagedSprite_SetPositionXYWithSubscreenOffset(pokedexApp->unk_0670[param_2], uStack_18[1] + param_3, (s16)(((u32)((uStack_18[0] + param_4) * 0x10000)) >> 0x10), (fx32)((512 > 0) ? ((512 * (1 << 12)) + 0.5f) : ((512 * (1 << 12)) - 0.5f)));
    return;
}
