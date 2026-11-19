#include "global.h"

#include "application/pokedex/pokedex_internal.h"
#include "sprite_system.h"
#include "unk_02005D10.h"

s32 ov18_021F5638(PokedexAppData_UnkSub0868_State77_Sub4 *arg0);

s32 ov18_021F5638(PokedexAppData_UnkSub0868_State77_Sub4 *arg0) {
    u16 v1;
    u8 v0;

    v0 = arg0->state;
    switch (v0) {
    case 0:
        PlaySE(0x8EBU);
        ov18_021F5180(arg0, 0x300, 0);
        arg0->state = (u8)(arg0->state + 1);
    case 1:
        ManagedSprite_OffsetAffineZRotation(arg0->pokedexApp->unk_0670[7], arg0->rotation);
        v1 = ManagedSprite_GetRotation(arg0->pokedexApp->unk_0670[7]);
        ov18_021F5000(arg0->pokedexApp, v1);
        arg0->rotation = (u32)(arg0->rotation + arg0->rotationStep);
        if (arg0->direction == 0) {
            if ((u32)v1 <= 0xF600U) {
                ManagedSprite_SetAffineZRotation(arg0->pokedexApp->unk_0670[7], 0xF600U);
                ov18_021F5000(arg0->pokedexApp, 0xF600U);
                return 0;
            }
            break;
        }
        if ((u32)v1 >= 0xA00U) {
            ManagedSprite_SetAffineZRotation(arg0->pokedexApp->unk_0670[7], 0xA00U);
            ov18_021F5000(arg0->pokedexApp, 0xA00U);
            return 0;
        }
    default:
        break;
    }
    return 1;
}
