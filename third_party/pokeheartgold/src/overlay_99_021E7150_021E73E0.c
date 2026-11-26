#include "global.h"

#include "bg_window.h"
#include "sprite_system.h"

s32 ov99_021E7198(BgConfig **, s32);

#if defined(SDK_PORT) || defined(NONMATCHING)
s32 ov99_021E738C(BgConfig **, u8, u8);
#else
s32 ov99_021E738C(BgConfig **, s16, s16);
#endif

void ov99_021E73E0(BgConfig **arg0, s32 arg1, s32 arg2);

void ov99_021E73E0(BgConfig **arg0, s32 arg1, s32 arg2) {
    if (ov99_021E7198(arg0, arg2) != 0) {
        ov99_021E738C(arg0, (s16)(((arg1 % 6) * 4) + 6), (s16)(((arg1 / 6) * 3) + 4));
        BgCommitTilemapBufferToVram(*arg0, 3U);
    }
}

void ov99_021E7428(u8 *arg0, s32 arg1);

void ov99_021E7428(u8 *arg0, s32 flag) {
    s32 v0;

    v0 = 0;
    do {
        ManagedSprite_SetDrawFlag(*(ManagedSprite **)((u8 *)(arg0 + ((v0 + 0x14) * 4)) + 0x408), flag);
        v0 += 1;
    } while (v0 < 0x1E);
}
