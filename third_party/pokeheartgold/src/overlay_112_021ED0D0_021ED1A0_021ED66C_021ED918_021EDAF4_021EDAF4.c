#include "global.h"

#include "sprite_system.h"

void ov112_021EDAF4(void *arg0, s32 arg1);

void ov112_021EDAF4(void *arg0, s32 arg1) {
    if (arg1 == 0) {
        ManagedSprite_SetAnimationFrame(*(ManagedSprite **)((u8 *)arg0 + 0x1E550), 0);
    } else if (arg1 > 30) {
        ManagedSprite_SetAnimationFrame(*(ManagedSprite **)((u8 *)arg0 + 0x1E550), (arg1 - 30) / 4);
    }
}
