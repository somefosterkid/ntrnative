#include "global.h"

#include "application/pokedex/pokedex_internal.h"
#include "sprite.h"
#include "sprite_system.h"
#include "unk_02005D10.h"





u32 ov18_021F5BEC(PokedexAppData_UnkSub0868_State77_Sub4 *param_1);

u32 ov18_021F5BEC(PokedexAppData_UnkSub0868_State77_Sub4 *param_1) {
    u16 uVar1;
    s32 iVar2;

    switch (param_1->state) {
    case 0:
        PlaySE(0x8ed);
        ov18_021F5180(param_1, 0x700, 0);
        param_1->state = param_1->state + 1;
    case 1:
        ManagedSprite_OffsetAffineZRotation(param_1->pokedexApp->unk_0670[7], param_1->rotation);
        uVar1 = ManagedSprite_GetRotation(param_1->pokedexApp->unk_0670[7]);
        ov18_021F5000(param_1->pokedexApp, uVar1);
        param_1->rotation = param_1->rotation + param_1->rotationStep;
        if (param_1->direction == 0) {
            if (0xf600 < uVar1) {
                break;
            }
            ManagedSprite_SetAffineZRotation(param_1->pokedexApp->unk_0670[7], 0xf600);
            ov18_021F5000(param_1->pokedexApp, 0xf600);
            param_1->state = param_1->state + 1;
        } else {
            if (uVar1 < 0xa00) {
                break;
            }
            ManagedSprite_SetAffineZRotation(param_1->pokedexApp->unk_0670[7], 0xa00);
            ov18_021F5000(param_1->pokedexApp, 0xa00);
            param_1->state = param_1->state + 1;
        }
    case 2:
        ov18_021F54C0(param_1, 0xffe8, 1);
        param_1->state = param_1->state + 1;
    case 3:
        iVar2 = ov18_021F55D8(param_1);
        if (iVar2 == 0) {
            return 0;
        }
    default:
        break;
    }
    return 1;
}
