#include "global.h"

#include "sprite.h"
#include "sprite_system.h"
#include "overlay_112_pokewalker_app.h"

void ov112_021EA570(void *arg0);

void ov112_021EA584(Ov112PokewalkerApp *arg0);

void ov112_021EA5A4(u8 *param_1, s32 param_2);

void ov112_021EA570(void *arg0) {
    ManagedSprite_SetDrawFlag(*(ManagedSprite **)((u8 *)arg0 + 0x1E530), 0);
}

void ov112_021EA584(Ov112PokewalkerApp *arg0) {
    ManagedSprite_SetAnim(arg0->sprites[0], ManagedSprite_GetActiveAnim(arg0->sprites[0]) + 3);
}

void ov112_021EA5A4(u8 *param_1, s32 param_2) {
    ManagedSprite_SetAnim(*(ManagedSprite **)(param_1 + 0x1e534), 0);
    ManagedSprite_SetDrawFlag(*(ManagedSprite **)(param_1 + 0x1e534), 1);
    if (param_2 == 0) {
        ManagedSprite_SetDrawFlag(*(ManagedSprite **)(param_1 + 0x1e538), 0);
        return;
    }
    if (0 < param_2) {
        ManagedSprite_SetDrawPriority(*(ManagedSprite **)(param_1 + 0x1e534), 2);
        ManagedSprite_SetDrawPriority(*(ManagedSprite **)(param_1 + 0x1e538), 1);
        ManagedSprite_SetDrawFlag(*(ManagedSprite **)(param_1 + 0x1e538), 1);
        ManagedSprite_SetAnim(*(ManagedSprite **)(param_1 + 0x1e538), param_2);
        ManagedSprite_SetAnimateFlag(*(ManagedSprite **)(param_1 + 0x1e538), 1);
    }
    return;
}
