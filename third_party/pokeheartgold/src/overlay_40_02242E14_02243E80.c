#include "global.h"

#include "sprite_system.h"

void ov40_02243E80(u8 *arg0, s32 arg1, s32 arg2);

void ov40_02243E80(u8 *arg0, s32 arg1, s32 arg2) {
    if (arg2 == 1) {
        ManagedSprite_SetDrawFlag(*(ManagedSprite **)((u8 *)(arg0 + (0x1C * arg1)) + 0x194), 1);
        return;
    }
    ManagedSprite_SetDrawFlag(*(ManagedSprite **)((u8 *)(arg0 + (0x1C * arg1)) + 0x194), 0);
}
