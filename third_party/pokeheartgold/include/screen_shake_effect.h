#ifndef POKEHEARTGOLD_SCREEN_SHAKE_EFFECT_H
#define POKEHEARTGOLD_SCREEN_SHAKE_EFFECT_H

#include "global.h"
#include "sys_task.h"
#include "screen_scroll_manager.h"

typedef struct ScreenShakeEffect {
    ScreenScrollManager *screenScrollMgr;
    SysTask *dmaTransferTask;
    u32 dmaCounter;
} ScreenShakeEffect;

// sizeof(ScreenShakeEffect) == 0xc

#endif // POKEHEARTGOLD_SCREEN_SHAKE_EFFECT_H
