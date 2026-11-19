#include "global.h"

#include "pokemon.h"
#include "sprite.h"
#include "sprite_system.h"

void ov112_021E7594(u8 *, u8 *, u8 *, u8 *);

void ov112_021E7CC8(u8 *);

void ov112_021E7DA4(u8 *arg0);

s32 ov112_021E9290(BoxPokemon *, void *, void *, s32);

void ov112_021EA5A4(u8 *, u32);

void ov112_021EA670(u8 *, u32);

void ov112_021ED25C(u8 *);

u32 ov112_021EDA4C(u8 *param_1);

u32 ov112_021EDA4C(u8 *param_1) {
    ov112_021E7DA4(param_1);
    ov112_021E7594(param_1 + 0xc910, param_1 + 0xf1d0, param_1 + 0x9d70, param_1 + 0xaabc);
    ov112_021E7CC8(param_1);
    if (*(Pokemon **)(param_1 + 0x1e42c) != NULL) {
        ov112_021E9290(&(*(Pokemon **)(param_1 + 0x1e42c))->box, param_1 + 0x1d7ac, param_1 + 0x1d79c, 0);
    } else {
        ov112_021E9290(*(BoxPokemon **)(param_1 + 0x1e430), param_1 + 0x1d7ac, param_1 + 0x1d79c, 0);
    }
    ManagedSprite_SetOamMode(*(ManagedSprite **)(param_1 + 0x1e550), GX_OAM_MODE_XLU);
    ov112_021EA670(param_1, 8);
    ManagedSprite_SetAnim(*(ManagedSprite **)(param_1 + 0x1e550), 0x19);
    ManagedSprite_SetAnimationFrame(*(ManagedSprite **)(param_1 + 0x1e550), 0);
    *(u16 *)(param_1 + 0x1f2d4) = 0;
    ov112_021EA5A4(param_1, 2);
    ov112_021ED25C(param_1);
    return 0x1f;
}
