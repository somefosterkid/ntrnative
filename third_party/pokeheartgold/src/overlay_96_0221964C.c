#include "global.h"

#include "constants/heap.h"

#include "sprite.h"
#include "sprite_system.h"
#include "overlay_96_ov96_digit_work.h"

void ov96_0221964C(Ov96DigitWork *work);

void ov96_0221964C(Ov96DigitWork *work) {
    s32 iVar1;
#if defined(SDK_PORT) || defined(NONMATCHING)
    iVar1 = 0;
    do {
        ManagedSprite_SetAnimateFlag(work->sprites[16 + iVar1], 0);
        iVar1 = iVar1 + 1;
    } while (iVar1 < 2);
#else
    void *param_1 = work;

    iVar1 = 0;
    do {
        ManagedSprite_SetAnimateFlag(*(ManagedSprite **)(param_1 + 0xa0), 0);
        iVar1 = iVar1 + 1;
        param_1 = param_1 + 4;
    } while (iVar1 < 2);
#endif
    return;
}
