#include "global.h"
struct PokedexAppData;

#include "filesystem.h"
#include "sprite_system.h"
#include "application/pokedex/pokedex_internal.h"

void ov18_021F1294(PokedexAppData *arg0, s32 arg1, s16 arg2, s16 arg3, s32 arg4);

void ov18_021F1294(PokedexAppData *arg0, s32 arg1, s16 x, s16 y, s32 arg4) {
    if (arg4 == 0) {
        ManagedSprite_SetPositionXY(arg0->unk_0670[arg1], x, y);
        return;
    }
    ManagedSprite_SetPositionXYWithSubscreenOffset(arg0->unk_0670[arg1], x, y, 0x200000);
}

void ov18_021F12C8(PokedexAppData *arg0, s32 arg1, s16 *arg2, s16 *arg3, s32 arg4);

void ov18_021F12C8(PokedexAppData *arg0, s32 arg1, s16 *x, s16 *y, s32 arg4) {
    if (arg4 == 0) {
        ManagedSprite_GetPositionXY(arg0->unk_0670[arg1], x, y);
        return;
    }
    ManagedSprite_GetPositionXYWithSubscreenOffset(arg0->unk_0670[arg1], x, y, 0x200000);
}

void ov18_021F12FC(struct PokedexAppData *arg0);

void ov18_021F12FC(PokedexAppData *arg0) {
    arg0->unk_0858 = NARC_New(NARC_poketool_icongra_poke_icon, HEAP_ID_POKEDEX_APP);
}
