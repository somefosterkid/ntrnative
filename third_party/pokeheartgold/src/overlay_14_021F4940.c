#include "global.h"

#include "sprite.h"
#include "sprite_system.h"
#include "overlay_14_box_funcs.h"

void ov14_021F4940(void *param_1, s32 param_2, s16 *param_3, s16 *param_4) {
    ManagedSprite_GetPositionXY(*(ManagedSprite **)(param_1 + param_2 * 4 + 0x338), param_3, param_4);
    return;
}
